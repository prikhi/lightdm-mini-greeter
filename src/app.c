/* Application Initialization Things */
#include <stdlib.h>

#include <gtk/gtk.h>
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

    return app;
}


/* Free any dynamically allocated memory */
void destroy_app(App *app)
{
    destroy_config(app->config);
    free(app->ui);
    free(app);
}
