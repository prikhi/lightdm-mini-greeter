#ifndef CONFIG_H
#define CONFIG_H

#include <gdk/gdk.h>
#include <glib.h>

#ifndef CONFIG_FILE
#define CONFIG_FILE "/etc/lightdm/lightdm-mini-greeter.conf"
#endif


// Represents the System's Greeter Configuration. Parsed from `CONFIG_FILE`.
typedef struct Config_ {
    gchar    *login_user;
    gboolean  show_password_label;
    gchar    *password_label_text;
    gchar    *invalid_password_text;
    gboolean  show_input_cursor;
    gfloat    password_alignment;
    gint      password_input_width;
    gboolean  show_image_on_all_monitors;
    gboolean  show_sys_info;

    /* Theme Configuration */
    gchar    *font;
    gchar    *font_size;
    gchar    *font_weight;
    gchar    *font_style;
    GdkRGBA  *text_color;
    GdkRGBA  *error_color;
    // Windows
    gchar    *background_image;
    GdkRGBA  *background_color;
    gchar    *background_image_size;
    GdkRGBA  *window_color;
    GdkRGBA  *border_color;
    gchar    *border_width;
    guint     layout_spacing;
    // Password Input
    gunichar *password_char;
    GdkRGBA  *password_color;
    GdkRGBA  *password_background_color;
    GdkRGBA  *password_border_color;
    gchar    *password_border_width;
    gchar    *password_border_radius;
    // System Info
    gchar    *sys_info_font;
    gchar    *sys_info_font_size;
    GdkRGBA  *sys_info_color;
    gchar    *sys_info_margin;

    /* Hotkeys */
    guint     mod_bit;
    guint     shutdown_key;
    guint     restart_key;
    guint     hibernate_key;
    guint     suspend_key;
    guint     session_key;
    guint     help_key;
} Config;


Config *initialize_config(void);
void destroy_config(Config *config);

#endif
