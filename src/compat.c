/* Backwards-Compatible Functions for LightDM */
#include <lightdm.h>

#include "compat.h"

gboolean compat_greeter_authenticate(LightDMGreeter *greeter, const gchar *username, GError **error)
{
#ifdef LIGHTDM_1_19_1_LOWER
    lightdm_greeter_authenticate(greeter, username);
    return TRUE;
#else
    return lightdm_greeter_authenticate(greeter, username, error);
#endif
}

gboolean compat_greeter_respond(LightDMGreeter *greeter, const gchar *response, GError **error)
{
#ifdef LIGHTDM_1_19_1_LOWER
    lightdm_greeter_respond(greeter, response);
    return TRUE;
#else
    return lightdm_greeter_respond(greeter, response, error);
#endif

}
