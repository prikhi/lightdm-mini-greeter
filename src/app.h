#ifndef APP_H
#define APP_H

#include <lightdm.h>

#include "ui.h"


typedef struct App_ {
    UI *ui;
    LightDMGreeter *greeter;
} App;


App *initialize_app(int argc, char **argv);
void destroy_app(App *app);

#endif
