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
void load_lightdm_config(App *app);

/* Config Member Accessors */
#define APP_LOGIN_USER(app)             (app)->config->login_user

/* UI Member Accessors */
#define APP_BACKGROUND_WINDOWS(app)     (app)->ui->background_windows
#define APP_MONITOR_COUNT(app)          (app)->ui->monitor_count
#define APP_MAIN_WINDOW(app)            (app)->ui->main_window
#define APP_PASSWORD_INPUT(app)         (app)->ui->password_input
#define APP_SESSION_LABEL(app)          (app)->ui->session_label
#define APP_FEEDBACK_LABEL(app)         (app)->ui->feedback_label

#endif
