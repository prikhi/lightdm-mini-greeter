/* Application Initialization Things */
#include <stdlib.h>

#include <gtk/gtk.h>
#include <lightdm.h>

#include "app.h"


/* Initialize the Greeter & UI */
App *initialize_app(int argc, char **argv)
{
    g_log_set_always_fatal(G_LOG_LEVEL_CRITICAL);
    gtk_init(&argc, &argv);

    LightDMGreeter *greeter = lightdm_greeter_new();
    UI *ui = initialize_ui(greeter);

    App *app = malloc(sizeof(App));
    app->ui = ui;
    app->greeter = greeter;

    return app;
}


/* Free any dynamically allocated memory */
void destroy_app(App *app)
{
    free(app->ui);
    free(app);
}
