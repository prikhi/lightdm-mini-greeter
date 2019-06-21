#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>


typedef struct UI_ {
    GtkWindow   **background_windows;
    int         monitor_count;
    GtkWindow   *main_window;
    GtkGrid     *layout_container;
    GtkWidget   *password_label;
    GtkWidget   *password_input;
    GtkWidget   *session_label;
    GtkWidget   *feedback_label;
} UI;


UI *initialize_ui(Config *config);

#endif
