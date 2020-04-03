/* Application Initialization Things */
#include <stdlib.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <X11/extensions/scrnsaver.h>
#include <lightdm.h>

#include "app.h"
#include "callbacks.h"
#include "config.h"


/* Initialize the Greeter & UI */
App *initialize_app(int argc, char **argv)
{
    g_log_set_always_fatal(G_LOG_LEVEL_CRITICAL);
    gtk_init(&argc, &argv);

    // Allocate & Initialize
    App *app = malloc(sizeof(App));
    if (app == NULL) {
        g_error("Could not allocate memory for App");
    }

    app->config = initialize_config();
    app->greeter = lightdm_greeter_new();
    app->ui = initialize_ui(app->config);

    app->window_visible = TRUE;
    app->xss_active = FALSE;
    app->xss_event_base = -1;
    app->last_active = TRUE;

    // Connect Greeter & UI Signals
    g_signal_connect(app->greeter, "authentication-complete",
                     G_CALLBACK(authentication_complete_cb), app);
    g_signal_connect(GTK_ENTRY(APP_PASSWORD_INPUT(app)), "activate",
                     G_CALLBACK(handle_password), app);
    // This was added to fix a bug where the background window would be focused
    // instead of the main window, preventing users from entering their password.
    // It's undocument & probably not necessary any more. Investigate & remove.
    for (int m = 0; m < APP_MONITOR_COUNT(app); m++) {
        g_signal_connect(GTK_WIDGET(APP_BACKGROUND_WINDOWS(app)[m]),
                         "key-press-event",
                         G_CALLBACK(handle_tab_key), app);
    }
    g_signal_connect(GTK_WIDGET(APP_MAIN_WINDOW(app)), "key-press-event",
                     G_CALLBACK(handle_power_management_keys), app->config);

    for (int m = 0; m < APP_MONITOR_COUNT(app); m++) {
        gtk_widget_show_all(GTK_WIDGET(APP_BACKGROUND_WINDOWS(app)[m]));
    }
    gtk_widget_show_all(GTK_WIDGET(APP_MAIN_WINDOW(app)));
    gtk_window_present(APP_MAIN_WINDOW(app));

    if (app->config->stop_on_inactivity) {
        gdk_window_add_filter(NULL, window_event_activity, app);

        GdkDisplay *display = gtk_widget_get_display(GTK_WIDGET(APP_MAIN_WINDOW(app)));
        gdk_x11_display_error_trap_push(display);
        int event_base, error_base;
        if (XScreenSaverQueryExtension(gdk_x11_display_get_xdisplay(display),
            &event_base, &error_base)) {
            GdkScreen *screen = gdk_display_get_default_screen(display);
            GdkWindow *root = gdk_screen_get_root_window(screen);
            XScreenSaverSelectInput(gdk_x11_display_get_xdisplay(display),
                gdk_x11_window_get_xid(root), ScreenSaverNotifyMask);
            app->xss_event_base = event_base;
        }
        gdk_x11_display_error_trap_pop_ignored(display);
    }

    return app;
}


/* Free any dynamically allocated memory */
void destroy_app(App *app)
{
    destroy_config(app->config);
    free(app->ui);
    free(app);
}

/* Check app is active */
gboolean app_is_active(App *app)
{
    if (app != NULL) {
        gboolean active = app->window_visible && !app->xss_active;
        app->last_active = active;
        return active;
    }
    return FALSE;
}

/* Check app activity changed since last activity check */
gboolean app_is_active_switch(App *app)
{
    if (app != NULL) {
        gboolean last_active = app->last_active;
        gboolean active = app_is_active(app);
        return active != last_active;
    }
    return FALSE;
}
