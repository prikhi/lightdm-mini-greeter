/* Callback Functions for LightDM & GTK */
#include <gtk/gtk.h>
#include <lightdm.h>

#include "utils.h"


/* Start the Default Session Once Fully Authenticated */
void authentication_complete_cb(LightDMGreeter *greeter)
{
    if (lightdm_greeter_get_is_authenticated(greeter)) {
        const gchar *default_session =
            lightdm_greeter_get_default_session_hint(greeter);

        g_message("Attempting to start session: %s", default_session);

        gboolean session_started_successfully =
            !lightdm_greeter_start_session_sync(greeter, default_session, NULL);

        if (!session_started_successfully) {
            g_message("Unable to start session");
        }
    } else {
        g_message("Authenticaton unsuccessful");
        begin_authentication_as_default_user(greeter);
    }
}


/* If Authenticating, Respond with the Entered Password */
void handle_password(GtkWidget *password_input, LightDMGreeter *greeter)
{
    if (!lightdm_greeter_get_is_authenticated(greeter)) {
        if (!lightdm_greeter_get_in_authentication(greeter)) {
            begin_authentication_as_default_user(greeter);
        }
        g_message("Using entered password to authenticate");
        const gchar *password_text =
            gtk_entry_get_text(GTK_ENTRY(password_input));
        lightdm_greeter_respond(greeter, password_text);
    } else {
        g_message("Password entered while already authenticated");
    }
    gtk_entry_set_text(GTK_ENTRY(password_input), "");
}
