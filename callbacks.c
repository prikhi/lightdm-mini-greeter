/* Callback Functions for LightDM & GTK */
#include <gtk/gtk.h>
#include <lightdm.h>

#include "utils.h"


/* Start the Default Session Once Fully Authenticated */
void authentication_complete_cb(LightDMGreeter *greeter)
{
    // Attempt to Start the User Session
    const gchar *default_session = lightdm_greeter_get_default_session_hint(
            greeter);

    g_message("Attempting to start session: %s", default_session);
    if (!lightdm_greeter_get_is_authenticated(greeter) ||
            !lightdm_greeter_start_session_sync(greeter, default_session, NULL)) {
        g_message("Received invalid password");
        begin_authentication_as_default_user(greeter);
    }
}


/* If Authenticating, Respond with the Entered Password */
void handle_password(GtkWidget *password_input, LightDMGreeter *greeter)
{
    gboolean in_authentication = lightdm_greeter_get_in_authentication(greeter);

    if (in_authentication) {
        g_message("Using entered password to authenticate");
        const gchar *password_text = gtk_entry_get_text(
            GTK_ENTRY(password_input));
        lightdm_greeter_respond(greeter, password_text);
    } else {
        g_critical("Given password while not in authentication");
    }
}
