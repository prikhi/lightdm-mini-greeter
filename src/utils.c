/* General Utility Functions */
#include <lightdm.h>

#include "app.h"
#include "compat.h"
#include "lightdm/session.h"
#include "utils.h"
#include "focus_ring.h"

static gchar *get_session_key(gconstpointer data);


/* Connect to the LightDM daemon or exit with an error */
void connect_to_lightdm_daemon(LightDMGreeter *greeter)
{
    if (!lightdm_greeter_connect_sync(greeter, NULL)) {
        g_critical("Could not connect to the LightDM daemon");
    }
}


/* Begin authentication as the default user, or exit with an error */
void begin_authentication_as_default_user(App *app)
{
    const gchar *default_user = APP_LOGIN_USER(app);
    if (g_strcmp0(default_user, NULL) == 0) {
        g_critical("A default user has not been not set");
    } else {
        g_message("Beginning authentication as the default user: %s",
                  default_user);
        compat_greeter_authenticate(app->greeter, default_user, NULL);
    }
}


/* Remove every occurence of a character from a string */
void remove_char(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}


/* Get Sessions & Build the Focus Ring */
void make_session_focus_ring(App *app)
{
    const gchar *default_session =
            lightdm_greeter_get_default_session_hint(app->greeter);
    const GList *sessions = lightdm_get_sessions();
    FocusRing *session_ring = initialize_focus_ring(sessions, &get_session_key, "sessions");

    if (default_session != NULL) {
        focus_ring_scroll_to_value(session_ring, default_session);
    }
    g_message("Initial session set to: %s", focus_ring_get_value(session_ring));

    app->session_ring = session_ring;
}
/* Retrieves the `key` field of a session, used to pull current session out of
 * a FocusRing.
 */
static gchar *get_session_key(gconstpointer data)
{
    LightDMSession *session = (LightDMSession *) data;
    return (gchar *) lightdm_session_get_key(session);
}
