#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>


typedef struct UI_ {
    GtkWindow   *background_window;
    GtkWindow   *main_window;
    GtkGrid     *layout_container;
    GtkWidget   *password_label;
    GtkWidget   *password_input;
    GtkWidget   *feedback_label;
} UI;


UI *initialize_ui(Config *config);

#endif
