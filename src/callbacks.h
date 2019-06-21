#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <lightdm.h>

#include "app.h"


void authentication_complete_cb(LightDMGreeter *greeter, App *app);
void handle_password(GtkWidget *password_input, App *app);
gboolean handle_tab_key(GtkWidget *widget, GdkEvent *event, App *app);
gboolean handle_key_presses(GtkWidget *widget, GdkEventKey *event, App *app);

#endif
