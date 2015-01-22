/* Callback Functions for LightDM & GTK */

#include <stdio.h>

#include <gtk/gtk.h>
#include <lightdm.h>


/* Start the Default Session Once Fully Authenticated */
void authentication_complete_cb(LightDMGreeter *greeter)
{
    // Attempt to Start the User Session
    const gchar *default_session = lightdm_greeter_get_default_session_hint(
            greeter);

    fprintf(stderr, "[Debug] Attempting to start session %s", default_session);
    if (!lightdm_greeter_get_is_authenticated(greeter) ||
            !lightdm_greeter_start_session_sync(greeter, default_session, NULL)) {
        // Authenticaton Failed, Retry
        lightdm_greeter_authenticate(greeter, NULL);
    }
}


/* If Authenticating, Respond with the Entered Password */
void handle_password(GtkWidget *password_input, LightDMGreeter *greeter)
{
    gboolean in_authentication = lightdm_greeter_get_in_authentication(greeter);

    if (in_authentication) {
        fprintf(stderr, "[Debug] Attempting authentication\n");
        const gchar *password_text = gtk_entry_get_text(
            GTK_ENTRY(password_input));
        lightdm_greeter_respond(greeter, password_text);
    } else {
        fprintf(stderr, "[Error] Given password while not in authentication\n");
    }
}
