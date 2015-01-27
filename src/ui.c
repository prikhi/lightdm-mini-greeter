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
static void create_and_attach_feedback_label(UI *ui);


/* Initialize the Main Window & it's Children */
UI *initialize_ui(Config *config)
{
    UI *ui = new_ui();

    setup_background_window(config, ui);
    setup_main_window(ui);
    create_and_attach_layout_container(ui);
    create_and_attach_password_field(ui);
    create_and_attach_feedback_label(ui);

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
    ui->layout_container = NULL;
    ui->password_label = NULL;
    ui->password_input = NULL;
    ui->feedback_label = NULL;

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

    gtk_window_set_default_size(main_window, 1, 1);
    gtk_window_set_position(main_window, GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    ui->main_window = main_window;
}


/* Add a Layout Container for All Displayed Widgets */
static void create_and_attach_layout_container(UI *ui)
{
    ui->layout_container = GTK_GRID(gtk_grid_new());
    gtk_grid_set_column_spacing(ui->layout_container, 5);

    gtk_container_add(GTK_CONTAINER(ui->main_window),
                      GTK_WIDGET(ui->layout_container));
}


/* Add a label & entry field for the user's password */
static void create_and_attach_password_field(UI *ui)
{
    // Label
    ui->password_label = gtk_label_new("Password:");
    gtk_label_set_justify(GTK_LABEL(ui->password_label), GTK_JUSTIFY_RIGHT);
    gtk_grid_attach(ui->layout_container, ui->password_label, 0, 0, 1, 1);

    // Entry
    ui->password_input = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(ui->password_input), FALSE);
    gtk_entry_set_alignment(GTK_ENTRY(ui->password_input), 1);
    gtk_grid_attach_next_to(ui->layout_container, ui->password_input,
                            ui->password_label, GTK_POS_RIGHT, 1, 1);
}


/* Add a label for feedback to the user */
static void create_and_attach_feedback_label(UI *ui)
{
    ui->feedback_label = gtk_label_new("");
    gtk_label_set_justify(GTK_LABEL(ui->feedback_label), GTK_JUSTIFY_CENTER);
    gtk_widget_set_no_show_all(ui->feedback_label, TRUE);

    gtk_grid_attach_next_to(ui->layout_container, ui->feedback_label,
                            ui->password_label, GTK_POS_BOTTOM, 2, 1);
}
