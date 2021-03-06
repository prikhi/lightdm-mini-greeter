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
    gchar    *title_label_text;
    gboolean  show_input_cursor;
    gfloat    password_alignment;
    gint      password_input_width;
    gboolean  show_image_on_all_monitors;

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
