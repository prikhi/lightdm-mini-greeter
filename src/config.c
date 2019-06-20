/* Functions related to the Configuration */
#include <stdlib.h>
#include <string.h>

#include <gdk/gdk.h>
#include <glib.h>

#include "config.h"
#include "utils.h"


static GdkRGBA *parse_greeter_color_key(GKeyFile *keyfile, const char *key_name);
static guint parse_greeter_hotkey_keyval(GKeyFile *keyfile, const char *key_name);
static gboolean parse_greeter_password_alignment(GKeyFile *keyfile);
static gboolean is_rtl_keymap_layout(void);

/* Initialize the configuration, sourcing the greeter's configuration file */
Config *initialize_config(void)
{
    Config *config = malloc(sizeof(Config));
    if (config == NULL) {
        g_error("Could not allocate memory for Config");
    }

    // Load the key-value file
    GKeyFile *keyfile = g_key_file_new();
    gboolean keyfile_loaded = g_key_file_load_from_file(
        keyfile, CONFIG_FILE, G_KEY_FILE_NONE, NULL);
    if (!keyfile_loaded) {
        g_error("Could not load configuration file.");
    }

    // Parse values from the keyfile into a Config.
    config->login_user = g_key_file_get_string(
        keyfile, "greeter", "user", NULL);
    if (strcmp(config->login_user, "CHANGE_ME") == 0) {
        g_message("User configuration value is unchanged.");
    }
    config->show_password_label = g_key_file_get_boolean(
        keyfile, "greeter", "show-password-label", NULL);
    config->password_label_text = g_key_file_get_string(
        keyfile, "greeter", "password-label-text", NULL);
    if (config->password_label_text == NULL) {
        config->password_label_text = (gchar *) "Password:";
    }
    config->invalid_password_text = g_key_file_get_string(
        keyfile, "greeter", "invalid-password-text", NULL);
    if (config->invalid_password_text == NULL) {
        config->invalid_password_text = (gchar *) "Invalid Password";
    }
    config->show_input_cursor = g_key_file_get_boolean(
        keyfile, "greeter", "show-input-cursor", NULL);
    config->password_alignment = parse_greeter_password_alignment(keyfile);

    // Parse Hotkey Settings
    config->suspend_key = parse_greeter_hotkey_keyval(keyfile, "suspend-key");
    config->hibernate_key = parse_greeter_hotkey_keyval(keyfile, "hibernate-key");
    config->restart_key = parse_greeter_hotkey_keyval(keyfile, "restart-key");
    config->shutdown_key = parse_greeter_hotkey_keyval(keyfile, "shutdown-key");
    gchar *mod_key = g_key_file_get_string(
        keyfile, "greeter-hotkeys", "mod-key", NULL);
    if (strcmp(mod_key, "control") == 0) {
        config->mod_bit = GDK_CONTROL_MASK;
    } else if (strcmp(mod_key, "alt") == 0) {
        config->mod_bit = GDK_MOD1_MASK;
    } else if (strcmp(mod_key, "meta") == 0) {
        config->mod_bit = GDK_SUPER_MASK;
    } else {
        g_error("Invalid mod-key configuration value: '%s'\n", mod_key);
    }

    // Parse Theme Settings
    config->font = g_key_file_get_string(
        keyfile, "greeter-theme", "font", NULL);
    config->font_size = g_key_file_get_string(
        keyfile, "greeter-theme", "font-size", NULL);
    config->text_color =
        parse_greeter_color_key(keyfile, "text-color");
    config->error_color =
        parse_greeter_color_key(keyfile, "error-color");
    config->background_image = g_key_file_get_string(
        keyfile, "greeter-theme", "background-image", NULL);
    if (config->background_image == NULL || strcmp(config->background_image, "") == 0) {
        config->background_image = (gchar *) "\"\"";
    }
    config->background_color =
        parse_greeter_color_key(keyfile, "background-color");
    config->window_color =
        parse_greeter_color_key(keyfile, "window-color");
    config->border_color =
        parse_greeter_color_key(keyfile, "border-color");
    config->border_width = g_key_file_get_string(
        keyfile, "greeter-theme", "border-width", NULL);
    config->password_color =
        parse_greeter_color_key(keyfile, "password-color");
    config->password_background_color =
        parse_greeter_color_key(keyfile, "password-background-color");
    gchar *temp_password_background_color = g_key_file_get_string(
        keyfile, "greeter-theme", "password-border-color", NULL);
    if (temp_password_background_color == NULL) {
        config->password_border_color = config->border_color;
    } else {
        config->password_border_color =
            parse_greeter_color_key(keyfile, "password-border-color");
    }
    config->password_border_width = g_key_file_get_string(
        keyfile, "greeter-theme", "password-border-width", NULL);
    if (config->password_border_width == NULL) {
        config->password_border_width = config->border_width;
    }

    gint layout_spacing = g_key_file_get_integer(
        keyfile, "greeter-theme", "layout-space", NULL);
    if (layout_spacing < 0) {
        config->layout_spacing = (guint) (-1 * layout_spacing);
    } else {
        config->layout_spacing = (guint) layout_spacing;
    }


    g_key_file_free(keyfile);

    return config;
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
    free(config->password_border_width);
    free(config);
}


/* Parse a greeter-colors group key into a newly-allocated GdkRGBA value */
static GdkRGBA *parse_greeter_color_key(GKeyFile *keyfile, const char *key_name)
{
    gchar *color_string = g_key_file_get_string(
        keyfile, "greeter-theme", key_name, NULL);
    if (strstr(color_string, "#") != NULL) {
        // Remove quotations from hex color strings
        remove_char(color_string, '"');
        remove_char(color_string, '\'');
    }

    GdkRGBA *color = malloc(sizeof(GdkRGBA));

    gboolean color_was_parsed = gdk_rgba_parse(color, color_string);
    if (!color_was_parsed) {
        g_critical("Could not parse the '%s' setting: %s",
                   key_name, color_string);
    }

    return color;
}

/* Parse a greeter-hotkeys key into the GDKkeyval of it's first character */
static guint parse_greeter_hotkey_keyval(GKeyFile *keyfile, const char *key_name)
{
    gchar *key = g_key_file_get_string(
        keyfile, "greeter-hotkeys", key_name, NULL);

    if (strcmp(key, "") == 0) {
        g_error("Configuration contains empty key for '%s'\n", key_name);
    }

    return gdk_unicode_to_keyval((guint) key[0]);
}

/* Parse the password input alignment, properly handling RTL layouts.
 *
 * Note that the gboolean returned by this function is meant to be used with
 * the `gtk_entry_set_alignment` function.
 */
static gboolean parse_greeter_password_alignment(GKeyFile *keyfile) {
    gboolean initial_alignment;

    gchar *password_alignment_text = g_key_file_get_string(
        keyfile, "greeter", "password-alignment", NULL);
    if (password_alignment_text == NULL) {
        initial_alignment = 1;
    } else {
        if (strcmp(password_alignment_text, "left") == 0) {
            initial_alignment = 0;
        } else {
            initial_alignment = 1;
        }
        free(password_alignment_text);
    }

    // The left/right values are switched for RTL layouts.
    if (is_rtl_keymap_layout()) {
        if (initial_alignment == 0) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return initial_alignment;
    }
}

/* Determine if the default Display's Keymap is in the Right-to-Left direction
 */
static gboolean is_rtl_keymap_layout(void) {
    GdkDisplay *display = gdk_display_get_default();
    if (display == NULL) {
        return FALSE;
    }
    GdkKeymap *keymap = gdk_keymap_get_for_display(display);
    PangoDirection text_direction = gdk_keymap_get_direction(keymap);
    return text_direction == PANGO_DIRECTION_RTL;
}
