/* Callback Functions for LightDM & GTK */
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>
#include <lightdm.h>
#include <string.h>

#include "app.h"
#include "utils.h"
#include "callbacks.h"
#include "compat.h"


/* LightDM Callbacks */

/* Start the Default Session Once Fully Authenticated */
void authentication_complete_cb(LightDMGreeter *greeter, App *app)
{
    if (lightdm_greeter_get_is_authenticated(greeter)) {
        const gchar *default_session =
            lightdm_greeter_get_default_session_hint(greeter);

        g_message("Attempting to start session: %s", default_session);

        if (g_strcmp0(default_session, "default") == 0) {
            default_session = NULL;
        }

        gboolean session_started_successfully =
            !lightdm_greeter_start_session_sync(greeter, default_session, NULL);

        if (!session_started_successfully) {
            g_message("Unable to start session");
        }
    } else {
        if (strlen(app->config->invalid_password_text) > 0) {
           if (!gtk_widget_get_visible(APP_FEEDBACK_LABEL(app))) {
                gtk_widget_show(APP_FEEDBACK_LABEL(app));
            }
            gtk_label_set_text(GTK_LABEL(APP_FEEDBACK_LABEL(app)),
                               app->config->invalid_password_text);
        }
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
        compat_greeter_respond(app->greeter, password_text, NULL);
    } else {
        g_message("Password entered while already authenticated");
    }
    gtk_entry_set_text(GTK_ENTRY(password_input), "");
}

/* X events handler to monitor activity */
GdkFilterReturn window_event_activity(GdkXEvent *xevent, GdkEvent *event, gpointer user_data) {
    App *app = user_data;
    gboolean check_switch = FALSE;
    XEvent *e = (XEvent *) xevent;

    if (e->type == VisibilityNotify) {
        GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(APP_MAIN_WINDOW(app)));
        if (window != NULL && gdk_x11_window_get_xid(window) == e->xvisibility.window) {
            g_message("Window visibility changed");
            app->window_visible = e->xvisibility.state != VisibilityFullyObscured;
            check_switch = TRUE;
        }
    } else if (app->xss_event_base >= 0 && e->type == app->xss_event_base + ScreenSaverNotify) {
        XScreenSaverNotifyEvent *xssne = (XScreenSaverNotifyEvent *) e;
        if (xssne->state == ScreenSaverOff || xssne->state == ScreenSaverDisabled) {
            g_message("Screen saver is inactive");
            app->xss_active = FALSE;
            check_switch = TRUE;
        } else if (xssne->state == ScreenSaverOn) {
            g_message("Screen saver is active");
            app->xss_active = TRUE;
            check_switch = TRUE;
        }
    }

    if (check_switch && app_is_active_switch(app)) {
        if (app_is_active(app)) {
            begin_authentication_as_default_user(app);
            g_message("Authentication process resumed");
        } else {
            compat_greeter_cancel_authentication(app->greeter, NULL);
            g_message("Authentication process cancelled");
        }
    }
    return GDK_FILTER_CONTINUE;
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
