# Mini-Greeter

[![AUR package](https://repology.org/badge/version-for-repo/aur/lightdm-mini-greeter.svg)](https://aur.archlinux.org/packages/lightdm-mini-greeter) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/a1c58074072542be8ea60d1bf14863fc)](https://www.codacy.com/app/pavan-rikhi-agit/lightdm-mini-greeter?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=prikhi/lightdm-mini-greeter&amp;utm_campaign=Badge_Grade)

A minimal but highly configurable single-user GTK3 greeter for LightDM.

Inspired by the SLiM Display Manager & LightDM GTK3 Greeter.

## Goals

Eventually this is will present a more customizable interface:

* Randomized Background Wallpapers
* Configurable language/session info? (lightdm provides this already?)

[Open Feature Requests](http://bugs.sleepanarchy.com/projects/mini-greeter/issues/)

## Current Status

Right now you can:

* log in
* hide the `Password:` label & customize the text
* hide the password input's cursor
* set the size of the login window, the font & every color.
* set a background image.
* use modifiable hotkeys to trigger a shutdown, restart, hibernate or suspend.

![A screen with a dark background and a single password input box in the center](http://bugs.sleepanarchy.com/projects/mini-greeter/repository/revisions/master/entry/screenshot.png "Mini Greeter Screenshot")

## Install

Arch Linux users can just install the [lightdm-mini-greeter
package][aur-package] from the Arch User Repository:

```sh
packer -S lightdm-mini-greeter
```

Otherwise, grab the source, build the greeter, & install it manually:

```sh
./autogen.sh
./configure --datadir /usr/share --bindir /usr/bin --sysconfdir /etc
make
sudo make install
```

Build Dependencies:
* automake
* pkg-config
* GTK+

Debian/Ubuntu:
* liblightdm-gobject-1-dev
* libgtk-3-dev

Dependency install on Debian/Ubuntu:
```sh
sudo apt install automake pkg-config liblightdm-gobject-1-dev libgtk-3-dev
```

You can then specify `lightdm-mini-greeter` as your `greeter-session` in
`/etc/lightdm/lightdm.conf`. If you have multiple Desktop Environments or
Window Mangers installed, you can specify the one to start by changing the
`user-session` option as well(look in `/usr/share/xsession` for possible
values).

Modify `/etc/lightdm/lightdm-mini-greeter.conf` to customize the greeter. At
the very least, you will need to set the `user`.

You can test it out using LightDM's `test-mode`:

    lightdm --test-mode -d

Or with `dm-tool`:

    dm-tool add-nested-seat

Uninstall with:

    sudo make uninstall


## Contribute

You can submit feature requests, bug reports, pull requests or patches on
either [github](http://github.com/prikhi/lightdm-mini-greeter) or
[redmine](http://bugs.sleepanarchy.com/projects/mini-greeter/).

If you like Mini-Greeter, please consider packaging it for your distribution.


### Style

* Use indentation and braces, 4 spaces - no tabs, no trailing whitespace.
* Declare pointers like this: `char *p1, *p2;`, avoid: `char* p1;`.
* Function braces should be on their own line.
* If/else/while/do should always use braces and indentation.
* Use `g_critical` for irrecoverable user errors, `g_error` for programming
  errors.

When in doubt, check surrounding code.


## License

GPL-3


[aur-package]: https://aur.archlinux.org/packages/lightdm-mini-greeter/
