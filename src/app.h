#ifndef APP_H
#define APP_H

#include <lightdm.h>

#include "config.h"
#include "ui.h"


typedef struct App_ {
    Config *config;
    LightDMGreeter *greeter;
    UI *ui;
} App;


App *initialize_app(int argc, char **argv);
void destroy_app(App *app);

#endif
