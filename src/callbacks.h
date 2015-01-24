#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <lightdm.h>

#include "app.h"


void authentication_complete_cb(LightDMGreeter *greeter, App *app);
void handle_password(GtkWidget *password_input, App *app);

#endif
