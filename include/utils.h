#ifndef UTILS_H
#define UTILS_H

#include <lightdm.h>

#include "app.h"


void connect_to_lightdm_daemon(LightDMGreeter *greeter);
void begin_authentication_as_default_user(App *app);
void remove_char(char *str, char garbage);

#endif
