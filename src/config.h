#ifndef CONFIG_H
#define CONFIG_H

#include <glib.h>

#define CONFIG_FILE "/etc/lightdm/lightdm-minigreeter.conf"


typedef struct Config_ {
    gchar *login_user;
} Config;


Config *initialize_config(void);
void destroy_config(Config *config);

#endif
