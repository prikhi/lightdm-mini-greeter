#ifndef CONFIG_H
#define CONFIG_H

#include <gdk/gdk.h>
#include <glib.h>

#ifndef CONFIG_FILE
#define CONFIG_FILE "/etc/lightdm/lightdm-minigreeter.conf"
#endif


// Represents the System's Greeter Configuration. Parsed from `CONFIG_FILE`.
typedef struct Config_ {
    gchar    *login_user;
    gboolean  show_password_label;

    /* Theme Configuration */
    gchar    *font;
    GdkRGBA  *text_color;
    GdkRGBA  *error_color;
    // Windows
    GdkRGBA  *background_color;
    GdkRGBA  *window_color;
    GdkRGBA  *border_color;
    gchar    *border_width;
    guint     layout_spacing;
    // Password Input
    GdkRGBA  *password_color;
    GdkRGBA  *password_background_color;

    /* Hotkeys */
    guint     mod_bit;
    guint     shutdown_key;
    guint     restart_key;
    guint     hibernate_key;
    guint     suspend_key;
} Config;


Config *initialize_config(void);
void destroy_config(Config *config);

#endif
