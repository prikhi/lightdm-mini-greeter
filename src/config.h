#ifndef CONFIG_H
#define CONFIG_H

#include <gdk/gdk.h>
#include <glib.h>

#ifndef CONFIG_FILE
#define CONFIG_FILE "/etc/lightdm/lightdm-minigreeter.conf"
#endif


typedef struct Config_ {
    gchar *login_user;
    GdkRGBA *background_color;
} Config;


Config *initialize_config(void);
void destroy_config(Config *config);

#endif
