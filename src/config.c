/* Functions related to the Configuration */
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <gdk/gdk.h>
#include <glib.h>

#include "config.h"
#include "utils.h"


static gchar *parse_greeter_string(GKeyFile *keyfile, const char *group_name,
                                   const char *key_name, const gchar *fallback);
static gint parse_greeter_integer(GKeyFile *keyfile, const char *group_name,
                                  const char *key_name, const gint fallback);
static gboolean parse_greeter_boolean(GKeyFile *keyfile, const char *group_name,
                                      const char *key_name, const gboolean fallback);
static GdkRGBA *parse_greeter_color_key(GKeyFile *keyfile, const char *key_name, const char *default_str);
static guint parse_greeter_hotkey_keyval(GKeyFile *keyfile, const char *key_name, const char default_char);
static gunichar *parse_greeter_password_char(GKeyFile *keyfile);
static gfloat parse_greeter_password_alignment(GKeyFile *keyfile);
static gboolean is_rtl_keymap_layout(void);
gboolean input_string_equals(gchar *input_str, const gchar * const fixed_str);

/* Initialize the configuration, sourcing the greeter's configuration file */
Config *initialize_config(void)
{
    Config *config = malloc(sizeof(Config));
    if (config == NULL) {
        g_error("Could not allocate memory for Config");
    }

    // Load the key-value file
    GKeyFile *keyfile = g_key_file_new();
    GError *keyerror = NULL;
    gboolean keyfile_loaded = g_key_file_load_from_file(
        keyfile, CONFIG_FILE, G_KEY_FILE_NONE, &keyerror);
    if (!keyfile_loaded) {
        if (keyerror != NULL) {
            g_error("Could not load configuration file: %s", keyerror->message);
            free(keyerror);
        } else {
            g_error("Could not load configuration file.");
        }
    }

    // Parse values from the keyfile into a Config.
    config->login_user =
        g_strchomp(g_key_file_get_string(keyfile, "greeter", "user", NULL));
    if (strcmp(config->login_user, "CHANGE_ME") == 0) {
        g_message("User configuration value is unchanged.");
    }
    config->show_password_label =
        parse_greeter_boolean(keyfile, "greeter", "show-password-label", TRUE);
    config->password_label_text = parse_greeter_string(
        keyfile, "greeter", "password-label-text", "Password:");
    config->invalid_password_text = parse_greeter_string(
        keyfile, "greeter", "invalid-password-text", "Invalid Password");
    config->show_input_cursor =
        parse_greeter_boolean(keyfile, "greeter", "show-input-cursor", TRUE);
    config->password_alignment = parse_greeter_password_alignment(keyfile);
    config->password_input_width = parse_greeter_integer(
        keyfile, "greeter", "password-input-width", -1);
    config->show_image_on_all_monitors = parse_greeter_boolean(
        keyfile, "greeter", "show-image-on-all-monitors", FALSE);
    config->show_sys_info = parse_greeter_boolean(
        keyfile, "greeter", "show-sys-info", FALSE);

    // Parse Hotkey Settings
    config->suspend_key = parse_greeter_hotkey_keyval(keyfile, "suspend-key", 'u');
    config->hibernate_key = parse_greeter_hotkey_keyval(keyfile, "hibernate-key", 'h');
    config->restart_key = parse_greeter_hotkey_keyval(keyfile, "restart-key", 'r');
    config->shutdown_key = parse_greeter_hotkey_keyval(keyfile, "shutdown-key", 's');
    config->session_key = parse_greeter_hotkey_keyval(keyfile, "session-key", 'e');
    gchar *mod_key =
        g_key_file_get_string(keyfile, "greeter-hotkeys", "mod-key", NULL);
    if (mod_key == NULL) {
        config->mod_bit = GDK_SUPER_MASK;
    } else {
        g_strchomp(mod_key);
        if (strcmp(mod_key, "control") == 0) {
            config->mod_bit = GDK_CONTROL_MASK;
        } else if (strcmp(mod_key, "alt") == 0) {
            config->mod_bit = GDK_MOD1_MASK;
        } else if (strcmp(mod_key, "meta") == 0) {
            config->mod_bit = GDK_SUPER_MASK;
        } else {
            g_error("Invalid mod-key configuration value: '%s'\n", mod_key);
        }
    }

    // Parse Theme Settings
    // Font
    config->font =
        parse_greeter_string(keyfile, "greeter-theme", "font", "Sans");
    config->font_size =
        parse_greeter_string(keyfile, "greeter-theme", "font-size", "1em");
    config->font_weight =
        parse_greeter_string(keyfile, "greeter-theme", "font-weight", "bold");
    config->font_style =
        parse_greeter_string(keyfile, "greeter-theme", "font-style", "normal");
    config->text_color =
        parse_greeter_color_key(keyfile, "text-color", "#080800");
    config->error_color =
        parse_greeter_color_key(keyfile, "error-color", "#F8F8F0");
    // Background
    config->background_image =
        g_key_file_get_string(keyfile, "greeter-theme", "background-image", NULL);
    if (config->background_image == NULL || strcmp(config->background_image, "") == 0) {
        config->background_image = (gchar *) "\"\"";
    } else {
        size_t path_size = strlen(config->background_image);
        if (config->background_image[path_size - 2] == '/'){
            // removes starting and trailing quotes for use with opendir
            config->background_image[path_size - 1] = '\0';
            char * raw_path = config->background_image + 1;

            DIR *dir_stream = opendir(raw_path);
            if (dir_stream == NULL) {
                g_error("Could not open background image folder: %s", config->background_image);
                config->background_image = (gchar *) "\"\"";
            } else {
                unsigned int images_found = 0;
                struct dirent * de[100];
                //goes through all the files in the directory
                while ((de[images_found] = readdir(dir_stream)) != NULL) {
                    char * end_pointer = de[images_found]->d_name + strlen(de[images_found]->d_name);
                    //if a file contains an image extension saves the entity with its filename
                    if (strcmp(end_pointer - 5, ".jpeg") == 0 ||
                        strcmp(end_pointer - 4, ".jpg")  == 0 ||
                        strcmp(end_pointer - 4, ".png")  == 0) {
                        images_found++;
                    }
                }
                //if any images were found
                if (images_found > 0) {
                    //randomly select one
                    srand( (unsigned int) time(NULL));
                    unsigned int selected_image = ((unsigned int) rand()) % images_found;
                    size_t filename_size = strlen(de[selected_image]->d_name);
                    //resize the path variable to contain the folder and file
                    //the path_size already contains the space for the two quotes
                    //one added for string termination 
                    config->background_image = (char * ) g_realloc(config->background_image, path_size + filename_size + 1);
                    //append filename to the path, overwriting the trailing quote of the path
                    memcpy(config->background_image + path_size - 1, de[selected_image]->d_name, filename_size);
                    //adds final trailing quote and string termination char
                    config->background_image[path_size + filename_size - 1] = '\"';
                    config->background_image[path_size + filename_size]     = '\0';
                } else {
                    g_error("No images were found in the background image folder: %s", config->background_image);
                    config->background_image = (gchar *) "\"\"";
                }
            }
            closedir(dir_stream);
        }
    }
    config->background_color =
        parse_greeter_color_key(keyfile, "background-color", "#1B1D1E");
    config->background_image_size =
        parse_greeter_string(keyfile, "greeter-theme", "background-image-size", "auto");
    // Window
    config->window_color =
        parse_greeter_color_key(keyfile, "window-color", "#F92672");
    config->border_color =
        parse_greeter_color_key(keyfile, "border-color", "#080800");
    config->border_width = parse_greeter_string(
        keyfile, "greeter-theme", "border-width", "2px");
    // Password
    config->password_char =
        parse_greeter_password_char(keyfile);
    config->password_color =
        parse_greeter_color_key(keyfile, "password-color", "#F8F8F0");
    config->password_background_color =
        parse_greeter_color_key(keyfile, "password-background-color", "#1B1D1E");
    gchar *temp_password_border_color = g_key_file_get_string(
        keyfile, "greeter-theme", "password-border-color", NULL);
    if (temp_password_border_color == NULL) {
        config->password_border_color = config->border_color;
    } else {
        free(temp_password_border_color);
        config->password_border_color =
            parse_greeter_color_key(keyfile, "password-border-color", "#080800");
    }
    config->password_border_width = parse_greeter_string(
        keyfile, "greeter-theme", "password-border-width", config->border_width);
    config->password_border_radius = parse_greeter_string(
        keyfile, "greeter-theme", "password-border-radius", "0.341125em");
    // System Info
    gchar *temp_sys_info_color = g_key_file_get_string(
        keyfile, "greeter-theme", "sys-info-color", NULL);
    if (temp_sys_info_color == NULL) {
        config->sys_info_color = config->text_color;
    } else {
        config->sys_info_color = parse_greeter_color_key(
            keyfile, "sys-info-color", "#080800");
    }
    config->sys_info_font = parse_greeter_string(keyfile, "greeter-theme", "sys-info-font", config->font);
    config->sys_info_font_size =
        parse_greeter_string(keyfile, "greeter-theme", "sys-info-font-size", config->font_size);
    config->sys_info_margin =
        parse_greeter_string(keyfile, "greeter-theme", "sys-info-margin", "-5px -5px -5px");


    gint layout_spacing =
        parse_greeter_integer(keyfile, "greeter-theme", "layout-space", 15);
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
    free(config->font_weight);
    free(config->font_style);
    free(config->text_color);
    free(config->error_color);
    free(config->background_image);
    free(config->background_color);
    free(config->background_image_size);
    free(config->window_color);
    free(config->border_color);
    free(config->border_width);
    free(config->password_label_text);
    free(config->invalid_password_text);
    free(config->password_char);
    free(config->password_color);
    free(config->password_background_color);
    free(config->password_border_color);
    free(config->password_border_width);
    free(config->password_border_radius);
    free(config->sys_info_color);
    free(config->sys_info_font_size);
    free(config->sys_info_margin);
    free(config);
}


/* Parse a string from the config file, returning a copy of the fallback value
 * if the key is not present in the group.
 */
static gchar *parse_greeter_string(GKeyFile *keyfile, const char *group_name,
                                   const char *key_name, const gchar *fallback)
{
    gchar *parsed_string = g_key_file_get_string(keyfile, group_name, key_name, NULL);
    if (parsed_string == NULL) {
        g_warning("Could not find value for %s.%s - falling back to '%s'",
                  group_name, key_name, fallback);
        return g_strdup(fallback);
    } else {
        return parsed_string;
    }
}

/* Parse an integer from the config file, returning the fallback value if the
 * key is not present in the group, or if the value is not an integer.
 */
static gint parse_greeter_integer(GKeyFile *keyfile, const char *group_name,
                                  const char *key_name, const gint fallback)
{
    GError *parse_error = NULL;
    gint parse_result = g_key_file_get_integer(
        keyfile, group_name, key_name, &parse_error);
    if (parse_error != NULL) {
        if (parse_error->code == G_KEY_FILE_ERROR_INVALID_VALUE) {
            // Read the value as a string so we can log it
            gchar *value = g_key_file_get_string(
                keyfile, group_name, key_name, NULL);
            g_warning("Invalid integer for %s.%s: `%s`",
                      group_name, key_name, value);
            free(value);
        }
        g_error_free(parse_error);
        return fallback;
    }
    return parse_result;
}

/* Parse a boolean from the config file, returning the fallback value if the
 * key is not present in the group or cannot be parsed as a boolean.
 */
static gboolean parse_greeter_boolean(GKeyFile *keyfile, const char *group_name,
                                      const char *key_name, const gboolean fallback)
{
    GError *parse_error = NULL;
    gboolean parse_result = g_key_file_get_boolean(
        keyfile, group_name, key_name, &parse_error);
    if (!parse_result && parse_error != NULL) {
        if (parse_error->code == G_KEY_FILE_ERROR_INVALID_VALUE) {
            // Read the value as a string so we can log it
            gchar *value = g_key_file_get_string(
                keyfile, group_name, key_name, NULL);
            g_warning("Invalid boolean for %s.%s: `%s`\n",
                      group_name, key_name, value);
            g_free(value);
        }
        g_error_free(parse_error);
        return fallback;
    }
    return parse_result;
}

/* Parse a greeter-colors group key into a newly-allocated GdkRGBA value */
static GdkRGBA *parse_greeter_color_key(GKeyFile *keyfile, const char *key_name, const char *default_str)
{
    gchar *color_string = g_key_file_get_string(
        keyfile, "greeter-theme", key_name, NULL);
    if (color_string == NULL) {
        g_warning("Could not find value for %s.%s - falling back to '%s'",
                  "greeter-theme", key_name, default_str);
    }

    GdkRGBA *default_color = malloc(sizeof(GdkRGBA));
    gboolean default_was_parsed = gdk_rgba_parse(default_color, default_str);
    if (!default_was_parsed) {
        g_critical("Could not parse the default '%s' setting: %s", key_name, default_str);
    }

    if (color_string != NULL) {
        if (strstr(color_string, "#") != NULL) {
            // Remove quotations from hex color strings
            remove_char(color_string, '"');
            remove_char(color_string, '\'');
        }

        GdkRGBA *color = malloc(sizeof(GdkRGBA));
        gboolean color_was_parsed = gdk_rgba_parse(color, color_string);
        if (color_was_parsed) {
            free(default_color);
            return color;
        }
        g_warning("Could not parse the '%s' setting: %s - falling back to default of %s",
                    key_name, color_string, default_str);
        free(color);
    }
    return default_color;
}

/* Parse a greeter-hotkeys key into the GDKkeyval of it's first character */
static guint parse_greeter_hotkey_keyval(GKeyFile *keyfile, const char *key_name, const char default_char)
{
    gchar *key = g_key_file_get_string(
        keyfile, "greeter-hotkeys", key_name, NULL);

    guint32 key_code;
    if (key == NULL) {
        key_code = (guint32) default_char;
    } else if (strcmp(key, "") == 0) {
        g_warning("Configuration contains empty key for '%s' - falling back to default of %c\n", key_name, default_char);
        key_code = (guint32) default_char;
    } else {
        key_code = (guint32) key[0];
    }

    return gdk_unicode_to_keyval(key_code);
}

/* Parse the password masking character that should be displayed when typing
 * into the password input.
 *
 * We first attempt to parse a literal -1 or 0, where -1 means to use the
 * default character & 0 means to display no characters when typing a password.
 *
 * If that parsing fails, we attempt to parse the field as a string & use the
 * first character of the string. If the string is empty or parsing fails, we
 * fall back to the default characer.
 *
 * Since the related gtk_entry function takes a unsigned int, we use pointers,
 * where NULL means "use the default" & all other values indicate an argument
 * to the `gtk_entry_set_invisible_char` function.
 *
 */
static gunichar *parse_greeter_password_char(GKeyFile *keyfile)
{
    const char *const group_name = "greeter-theme";
    const char *const key_name = "password-character";
    GError *parse_error = NULL;

    // Attempt the int parsing

    const gint int_result = g_key_file_get_integer(
        keyfile, group_name, key_name, &parse_error);
    // Matches -1
    if (int_result == -1) {
        return NULL;
    }

    gunichar *result = malloc(sizeof(gunichar));
    // Matches 0
    if (int_result == 0 && parse_error == NULL) {
        *result = 0;
        return result;
    } else if (parse_error != NULL) {
        g_error_free(parse_error);
    }

    // Atempt the string parsing

    gchar *str_result = g_key_file_get_string(
        keyfile, group_name, key_name, NULL);

    // Invalid or 0-length string
    if (str_result == NULL || strlen(str_result) == 0) {
        if (str_result != NULL) {
            free(str_result);
        }
        free(result);
        return NULL;
    }

    // Convert to unicode code points
    gunichar *unicode_str = g_utf8_to_ucs4(str_result, -1, NULL, NULL, NULL);
    free(str_result);

    if (unicode_str == NULL) {
        free(result);
        return NULL;
    }

    *result = unicode_str[0];
    g_free(unicode_str);
    return result;
}

/* Parse the password input alignment, properly handling RTL layouts.
 *
 * Note that the gboolean returned by this function is meant to be used with
 * the `gtk_entry_set_alignment` function.
 */
static gfloat parse_greeter_password_alignment(GKeyFile *keyfile)
{
    gfloat alignment;

    gchar *password_alignment_text = parse_greeter_string(
        keyfile, "greeter", "password-alignment", "right");
    gboolean is_rtl = is_rtl_keymap_layout();

    if (input_string_equals(password_alignment_text, "left")) {
        alignment = is_rtl ? 1 : 0;
    } else if (input_string_equals(password_alignment_text, "center")) {
        alignment = 0.5;
    } else {
        alignment = is_rtl ? 0 : 1;
    }
    free(password_alignment_text);
    return alignment;
}

/* Determine if the default Display's Keymap is in the Right-to-Left direction
 */
static gboolean is_rtl_keymap_layout(void)
{
    GdkDisplay *display = gdk_display_get_default();
    if (display == NULL) {
        return FALSE;
    }
    GdkKeymap *keymap = gdk_keymap_get_for_display(display);
    PangoDirection text_direction = gdk_keymap_get_direction(keymap);
    return text_direction == PANGO_DIRECTION_RTL;
}

/* Take a string from the config file, trim any whitespace & check it's
 * equality with a fixed string.
 */
gboolean input_string_equals(gchar *input_str, const gchar * const fixed_str) {
    return strcmp(g_strchomp(input_str), fixed_str) == 0;
}
