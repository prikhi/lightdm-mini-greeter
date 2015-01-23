/* Application Initialization Things */
#include <stdlib.h>

#include <gtk/gtk.h>
#include <lightdm.h>

#include "app.h"
#include "callbacks.h"


/* Initialize the Greeter & UI */
App *initialize_app(int argc, char **argv)
{
    g_log_set_always_fatal(G_LOG_LEVEL_CRITICAL);
    gtk_init(&argc, &argv);

    App *app = malloc(sizeof(App));

    app->greeter = lightdm_greeter_new();
    app->ui = initialize_ui();

    g_signal_connect(G_OBJECT(app->greeter), "authentication-complete",
                     G_CALLBACK(authentication_complete_cb), NULL);
    g_signal_connect(GTK_ENTRY(app->ui->password_input), "activate",
                     G_CALLBACK(handle_password), app);


    return app;
}


/* Free any dynamically allocated memory */
void destroy_app(App *app)
{
    free(app->ui);
    free(app);
}
