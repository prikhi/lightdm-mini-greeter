#ifndef CONFIG_H
#define CONFIG_H

#include <gdk/gdk.h>
#include <glib.h>

#ifndef CONFIG_FILE
#define CONFIG_FILE "/etc/lightdm/lightdm-minigreeter.conf"
#endif


// Represents the System's Greeter Configuration. Parsed from `CONFIG_FILE`.
typedef struct Config_ {
    gchar *login_user;
    gboolean show_password_label;

    // Theme Configuration
    GdkRGBA *background_color;
} Config;


Config *initialize_config(void);
void destroy_config(Config *config);

#endif
