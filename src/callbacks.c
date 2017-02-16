/* Callback Functions for LightDM & GTK */
#include <gtk/gtk.h>
#include <lightdm.h>

#include "app.h"
#include "utils.h"
#include "callbacks.h"


/* LightDM Callbacks */

/* Start the Default Session Once Fully Authenticated */
void authentication_complete_cb(LightDMGreeter *greeter, App *app)
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
        if (!gtk_widget_get_visible(APP_FEEDBACK_LABEL(app))) {
            gtk_widget_show(APP_FEEDBACK_LABEL(app));
        }
        gtk_label_set_text(GTK_LABEL(APP_FEEDBACK_LABEL(app)),
                           "Invalid Password");
        begin_authentication_as_default_user(app);
    }
}



/* GUI Callbacks */

/* Attempt to Authenticate When a Password is Entered */
void handle_password(GtkWidget *password_input, App *app)
{
    if (!lightdm_greeter_get_is_authenticated(app->greeter)) {
        if (!lightdm_greeter_get_in_authentication(app->greeter)) {
            begin_authentication_as_default_user(app);
        }
        g_message("Using entered password to authenticate");
        const gchar *password_text =
            gtk_entry_get_text(GTK_ENTRY(password_input));
        lightdm_greeter_respond(app->greeter, password_text, NULL);
    } else {
        g_message("Password entered while already authenticated");
    }
    gtk_entry_set_text(GTK_ENTRY(password_input), "");
}


/* Select the Password input if the Tab Key is Pressed */
gboolean handle_tab_key(GtkWidget *widget, GdkEvent *event, App *app)
{
    (void) widget;  // Window accessible through app.

    GdkEventKey *key_event = (GdkEventKey *) event;
    if (event->type == GDK_KEY_PRESS && key_event->keyval == GDK_KEY_Tab) {
        g_message("Handling Tab Key Press");
        gtk_window_present(GTK_WINDOW(APP_MAIN_WINDOW(app)));
        gtk_widget_grab_focus(GTK_WIDGET(APP_PASSWORD_INPUT(app)));
        return FALSE;
    }
    return TRUE;
}

/* Shutdown, Restart, Hibernate or Suspend if the correct keys are pressed */
gboolean handle_power_management_keys(GtkWidget *widget, GdkEventKey *event,
                                      Config *config)
{
    (void) widget;

    if (event->state & config->mod_bit) {
        if (event->keyval == config->suspend_key && lightdm_get_can_suspend()) {
            lightdm_suspend(NULL);
        } else if (event->keyval == config->hibernate_key &&
                   lightdm_get_can_hibernate()) {
            lightdm_hibernate(NULL);
        } else if (event->keyval == config->restart_key &&
                   lightdm_get_can_restart()) {
            lightdm_restart(NULL);
        } else if (event->keyval == config->shutdown_key &&
                   lightdm_get_can_shutdown()) {
            lightdm_shutdown(NULL);
        } else {
            return FALSE;
        }
        return TRUE;
    }

    return FALSE;
}
