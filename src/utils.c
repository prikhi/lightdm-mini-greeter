/* General Utility Functions */
#include <lightdm.h>

#include "app.h"
#include "compat.h"
#include "utils.h"


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
