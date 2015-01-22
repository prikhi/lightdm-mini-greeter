/* General Utility Functions */
#include <lightdm.h>


/* Connect to the LightDM daemon or exit with an error */
void connect_to_lightdm_daemon(LightDMGreeter *greeter)
{
    if (!lightdm_greeter_connect_sync(greeter, NULL)) {
        g_critical("Could not connect to the LightDM daemon");
    }
}


/* Begin authentication as the default user, or exit with an error */
void begin_authentication_as_default_user(LightDMGreeter *greeter)
{
    // const gchar *default_user = lightdm_greeter_get_select_user_hint(greeter);
    const gchar *default_user = "prikhi";
    if (g_strcmp0(default_user, NULL) == 0) {
        g_critical("A default user has not been not set");
    } else {
        g_message("Beginning authentication as the default user: %s",
                  default_user);
        lightdm_greeter_authenticate(greeter, default_user);
    }
}
