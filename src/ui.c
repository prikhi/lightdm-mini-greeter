/* Functions related to the GUI. */
#include <stdlib.h>

#include <gtk/gtk.h>
#include <lightdm.h>

#include "callbacks.h"
#include "ui.h"
#include "utils.h"


static UI *new_ui(void);
static void setup_background_window(Config *config, UI *ui);
static void set_window_to_screen_size(GtkWindow *window);
static void setup_main_window(UI *ui);
static void create_and_attach_layout_container(UI *ui);
static void create_and_attach_password_field(UI *ui);


/* Initialize the Main Window & it's Children */
UI *initialize_ui(Config *config)
{
    UI *ui = new_ui();

    setup_background_window(config, ui);
    setup_main_window(ui);
    create_and_attach_layout_container(ui);
    create_and_attach_password_field(ui);

    return ui;
}


/* Create a new UI with all values initialized to NULL */
static UI *new_ui(void)
{
    UI *ui = malloc(sizeof(UI));
    if (ui == NULL) {
        g_error("Could not allocate memory for UI");
    }
    ui->background_window = NULL;
    ui->main_window = NULL;
    ui->layout_box = NULL;
    ui->password_label = NULL;
    ui->password_input = NULL;

    return ui;
}


/* Create & Configure the Background Window */
static void setup_background_window(Config *config, UI *ui)
{
    GtkWindow *background_window = GTK_WINDOW(gtk_window_new(
        GTK_WINDOW_TOPLEVEL));
    gtk_window_set_type_hint(background_window, GDK_WINDOW_TYPE_HINT_DESKTOP);
    gtk_window_set_keep_below(background_window, TRUE);
    gtk_widget_set_app_paintable(GTK_WIDGET(background_window), TRUE);

    // Set Window Size to Screen Size
    set_window_to_screen_size(background_window);

    // Set Background Color
    gtk_widget_override_background_color(
        GTK_WIDGET(background_window), GTK_STATE_FLAG_NORMAL,
        config->background_color
    );

    g_signal_connect(background_window, "destroy", G_CALLBACK(gtk_main_quit),
                     NULL);

    ui->background_window = background_window;
}


/* Set the Window's Minimum Size to the Default Screen's Size */
static void set_window_to_screen_size(GtkWindow *window)
{
    GdkScreen *screen = gdk_screen_get_default();
    gtk_widget_set_size_request(
        GTK_WIDGET(window),
        gdk_screen_get_width(screen),
        gdk_screen_get_height(screen)
    );
    gtk_window_move(window, 0, 0);
    gtk_window_set_resizable(window, FALSE);
}


/* Create & Configure the Main Window */
static void setup_main_window(UI *ui)
{
    GtkWindow *main_window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

    gtk_window_set_default_size(main_window, 250, 150);
    gtk_window_set_position(main_window, GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    ui->main_window = main_window;
}


/* Add a Layout Container for All Displayed Widgets */
static void create_and_attach_layout_container(UI *ui)
{
    ui->layout_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_container_add(GTK_CONTAINER(ui->main_window), ui->layout_box);
}


/* Add a label & entry field for the user's password */
static void create_and_attach_password_field(UI *ui)
{
    // Label
    ui->password_label = gtk_label_new("Password:");
    gtk_label_set_justify(GTK_LABEL(ui->password_label), GTK_JUSTIFY_RIGHT);
    gtk_box_pack_start(GTK_BOX(ui->layout_box), ui->password_label, TRUE, FALSE, 0);

    // Entry
    ui->password_input = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(ui->password_input), FALSE);
    gtk_entry_set_alignment(GTK_ENTRY(ui->password_input), 1);
    gtk_box_pack_start(GTK_BOX(ui->layout_box), ui->password_input, TRUE, FALSE, 0);
}
