/* lightdm-mini-greeter - A minimal GTK LightDM Greeter */
#include <glib.h>
#include <gtk/gtk.h>
#include <lightdm.h>

#include "callbacks.c"
#include "utils.h"


int main(int argc, char **argv)
{
    // GLib, GTK & LightDM Initialization
    g_log_set_always_fatal(G_LOG_LEVEL_CRITICAL);
    gtk_init(&argc, &argv);
    LightDMGreeter *greeter = lightdm_greeter_new();

    // Creaate Main Window & Set Attributes
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 250, 150);
    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    // Layout Box
    GtkWidget *layout_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_container_add(GTK_CONTAINER(main_window), layout_box);

    // Password Label
    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_label_set_justify(GTK_LABEL(password_label), GTK_JUSTIFY_RIGHT);
    gtk_box_pack_start(GTK_BOX(layout_box), password_label, TRUE, FALSE, 0);
    // Password Entry
    GtkWidget *password_input = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_input), FALSE);
    gtk_entry_set_alignment(GTK_ENTRY(password_input), 1);
    gtk_box_pack_start(GTK_BOX(layout_box), password_input, TRUE, FALSE, 0);


    // Connect Signals
    g_signal_connect(GTK_ENTRY(password_input), "activate",
                     G_CALLBACK(handle_password), greeter);
    g_signal_connect(G_OBJECT(greeter), "authentication-complete",
                     G_CALLBACK(authentication_complete_cb), NULL);


    connect_to_lightdm_daemon(greeter);

    begin_authentication_as_default_user(greeter);

    gtk_widget_show_all(main_window);
    gtk_main();
}
