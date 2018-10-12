#ifndef COMPAT_H
#define COMPAT_H

#include <lightdm.h>

#include "defines.h"

// v1.19.2 of LightDM introduced GError arguments but Debian jessie & stretch are not updated yet
gboolean compat_greeter_authenticate(LightDMGreeter *greeter, const gchar *username, GError **error);
gboolean compat_greeter_respond(LightDMGreeter *greeter, const gchar *response, GError **error);

#endif
