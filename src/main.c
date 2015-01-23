/* lightdm-mini-greeter - A minimal GTK LightDM Greeter */
#include <sys/mman.h>

#include <gtk/gtk.h>
#include <lightdm.h>

#include "ui.h"
#include "utils.h"


int main(int argc, char **argv)
{
    // Keep our data out of any swap devices
    mlockall(MCL_CURRENT | MCL_FUTURE);

    // GLib, GTK & LightDM Initialization
    g_log_set_always_fatal(G_LOG_LEVEL_CRITICAL);
    gtk_init(&argc, &argv);
    LightDMGreeter *greeter = lightdm_greeter_new();

    UI *ui = initialize_ui(greeter);

    connect_to_lightdm_daemon(greeter);

    begin_authentication_as_default_user(greeter);

    gtk_widget_show_all(ui->main_window);
    gtk_main();
}
