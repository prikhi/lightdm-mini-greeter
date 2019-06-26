/* Functions related to the Configuration */
#include <stdlib.h>
#include <string.h>

#include <gdk/gdk.h>
#include <glib.h>

#include "config.h"
#include "utils.h"

static gboolean is_rtl_keymap_layout(void);
static void load_config_from_file(Config *config, const char *path);
static void set_default_values(Config *config);

Config *initialize_config(void)
{
    Config *config = malloc(sizeof(Config));
    if (config == NULL) {
        g_error("Could not allocate memory for Config");
    }

    // First load default config from /etc
    load_config_from_file(config, CONFIG_FILE);

    // Try to find a local config file
    char path[1024];
    sprintf(path, "/home/%s/.config/lightdm-mini-greeter.conf",
            config->login_user);
    if (access(path, R_OK) != -1) {
        load_config_from_file(config, path);
    }

    set_default_values(config);

    return config;
}

void set_default_values(Config *config)
{
    if (strcmp(config->login_user, "CHANGE_ME") == 0) {
        g_message("User configuration value is unchanged.");
    }
    if (config->password_label_text == NULL) {
        config->password_label_text = (gchar *) "Password:";
    }
    if (config->invalid_password_text == NULL) {
        config->invalid_password_text = (gchar *) "Invalid Password";
    }
    if (config->background_image == NULL ||
        strcmp(config->background_image, "") == 0) {
        config->background_image = (gchar *) "\"\"";
    }
}

void read_string(gchar **val, GKeyFile *file,
                 const gchar *group, const gchar *key)
{
    gchar *read = g_key_file_get_string(file, group, key, NULL);
    if (read != NULL) {
        *val = read;
    }
}

void read_bool(gboolean *val, GKeyFile *file,
               const gchar *group, const gchar *key)
{
    GError *err = NULL;
    gboolean read = g_key_file_get_boolean(file, group, key, &err);
    if (err == NULL) {
        *val = read;
    } else {
        g_error_free(err);
    }

}

void read_color(GdkRGBA **val, GKeyFile *file, const char *key_name)
{
    gchar *color_string = g_key_file_get_string(
        file, "greeter-theme", key_name, NULL);

    if (color_string == NULL) return;

    if (strstr(color_string, "#") != NULL) {
        // Remove quotations from hex color strings
        remove_char(color_string, '"');
        remove_char(color_string, '\'');
    }

    GdkRGBA color;

    gboolean color_was_parsed = gdk_rgba_parse(&color, color_string);
    if (!color_was_parsed) {
        g_critical("Could not parse the '%s' setting: %s",
                   key_name, color_string);
    }

    *val = gdk_rgba_copy(&color);
}

void read_alignment(gboolean *val, GKeyFile *keyfile) {
    gchar *password_alignment_text = g_key_file_get_string(
        keyfile, "greeter", "password-alignment", NULL);

    if (password_alignment_text == NULL) return;

    // alignment is false for left and true for right
    gboolean alignment = !(strcmp(password_alignment_text, "left") == 0);

    free(password_alignment_text);

    // The left/right values are switched for RTL layouts.
    if (is_rtl_keymap_layout()) alignment = ! alignment;

    *val = alignment;
}

/* Parse a greeter-hotkeys key into the GDKkeyval of it's first character */
void read_hotkey(guint *val, GKeyFile *keyfile, const char *key_name)
{
    gchar *key = g_key_file_get_string(
        keyfile, "greeter-hotkeys", key_name, NULL);

    if (key == NULL) return;

    if (strcmp(key, "") == 0) {
        g_error("Configuration contains empty key for '%s'\n", key_name);
    }

    *val = gdk_unicode_to_keyval((guint) key[0]);
}

void read_int(guint *val, GKeyFile *file, const char *key_name) {
    GError *err = NULL;
    gint read = g_key_file_get_integer(
        file, "greeter-theme", key_name, &err);
    if (err != NULL) return;
    if (read < 0) {
        *val = (guint) (-1 * read);
    } else {
        *val = (guint) read;
    }
}



void load_config_from_file(Config *config, const char *path)
{

    // Load the key-value file
    GKeyFile *keyfile = g_key_file_new();
    gboolean keyfile_loaded = g_key_file_load_from_file(
        keyfile, path, G_KEY_FILE_NONE, NULL);
    if (!keyfile_loaded) {
        g_error("Could not load configuration file.");
    }

    read_string(&config->login_user, keyfile, "greeter", "user");
    read_string(&config->password_label_text, keyfile,
                "greeter", "password-label-text");
    read_string(&config->invalid_password_text, keyfile,
                "greeter", "invalid-password-text");
    read_bool(&config->show_password_label, keyfile,
              "greeter", "show-password-label");
    read_bool(&config->show_input_cursor, keyfile,
              "greeter", "show-input-cursor");

    read_alignment(&config->password_alignment, keyfile);

    // Parse Hotkey Settings
    read_hotkey(&config->suspend_key, keyfile, "suspend-key");
    read_hotkey(&config->hibernate_key, keyfile, "hibernate-key");
    read_hotkey(&config->restart_key, keyfile, "restart-key");
    read_hotkey(&config->shutdown_key, keyfile, "shutdown-key");

    gchar *mod_key = g_key_file_get_string(
        keyfile, "greeter-hotkeys", "mod-key", NULL);
    if (mod_key != NULL) {
        if (strcmp(mod_key, "control") == 0) {
            config->mod_bit = GDK_CONTROL_MASK;
        } else if (strcmp(mod_key, "alt") == 0) {
            config->mod_bit = GDK_MOD1_MASK;
        } else if (strcmp(mod_key, "meta") == 0) {
            config->mod_bit = GDK_SUPER_MASK;
        }
    }

    // Parse Theme Settings
    read_string(&config->font, keyfile, "greeter-theme", "font");
    read_string(&config->font_size, keyfile, "greeter-theme", "font-size");
    read_string(&config->background_image, keyfile,
                "greeter-theme", "background-image");
    read_string(&config->border_width, keyfile,
                "greeter-theme", "border-width");

    read_color(&config->text_color, keyfile, "text-color");
    read_color(&config->error_color, keyfile, "error-color");
    read_color(&config->background_color, keyfile, "background-color");
    read_color(&config->window_color, keyfile, "window-color");
    read_color(&config->border_color, keyfile, "border-color");
    read_color(&config->password_color, keyfile, "password-color");
    read_color(&config->password_background_color, keyfile,
               "password-background-color");

    read_int(&config->layout_spacing, keyfile, "layout-space");

    g_key_file_free(keyfile);
}

/* Cleanup any memory allocated for the Config */
void destroy_config(Config *config)
{
    free(config->login_user);
    free(config->font);
    free(config->font_size);
    free(config->text_color);
    free(config->error_color);
    free(config->background_image);
    free(config->background_color);
    free(config->window_color);
    free(config->border_color);
    free(config->border_width);
    free(config->password_label_text);
    free(config->invalid_password_text);
    free(config->password_color);
    free(config->password_background_color);
    free(config);
}

static gboolean is_rtl_keymap_layout(void) {
    GdkDisplay *display = gdk_display_get_default();
    if (display == NULL) {
        return FALSE;
    }
    GdkKeymap *keymap = gdk_keymap_get_for_display(display);
    PangoDirection text_direction = gdk_keymap_get_direction(keymap);
    return text_direction == PANGO_DIRECTION_RTL;
}
