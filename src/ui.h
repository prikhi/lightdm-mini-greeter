#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>


typedef struct UI_ {
    GtkWindow *background_window;
    GtkWindow *main_window;
    GtkWidget *layout_box;
    GtkWidget *password_label;
    GtkWidget *password_input;
} UI;


UI *initialize_ui(void);

#endif
