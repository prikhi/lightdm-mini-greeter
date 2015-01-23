#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <lightdm.h>


void authentication_complete_cb(LightDMGreeter *greeter);
void handle_password(GtkWidget *password_input, LightDMGreeter *greeter);

#endif
