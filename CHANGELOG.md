# CHANGELOG

## master

* Allow setting the `password-alignment` option to `center`, in addition to
  `left` or `right`.
* Add a `password-border-radius` configuration option to set the border radius
  of the password input.
* Add a `password-character` configuration option to set the character used to
  mask the entered password. `-1` specifies the default character, `0` hides
  any entered text, & the first character is used for any other value.
* Document how to make configuration file a symlink into home directories.
* Improve error message when configuration file is unreadable.
* Disable the password input while authenticating. This fixes a bug causing
  lightdm to crash if the greeter tried authenticating before a previous
  authentication attempt had completed.
* Add a `password-input-width` configuration option to set the number of
  characters that should fit in the password input. This is just a suggested
  width - GTK may render a narrower input.
* Ensure the mouse cursor is always hidden.
* Add a `show-image-on-all-monitors` configuration option to display the
  background image on every monitor instead of just the primary monitor.
* Ignore any trailing whitespace in the `user`, `mod-key`, &
  `password-alignment` configuration options.

## v0.4.0

* Add `password-border-color` & `password-border-width` options for customizing
  the password input's border independently of the main window's border.
* Fix default path to config file if not set by autotools.
* Add `font-weight` & `font-style` options to customize the bolding &
  italicizing of all text.
* Use the alternatives system for Debian-based builds.
* Add a `password-alignment` option to customize the alignment of the text in the
  password input.
* Add an `invalid-password-text` option to customize the wrong password error
  text.

## v0.3.4

* Fix flashing of password input on start.
* Fix background color on primary monitor when background image is not set.

## v0.3.3

* Fix background image aligment in multi-head configurations.
* Fix builds with liblightdm v1.19.1 and below.
* Fix customization of label colors.

## v0.3.2

* Fix linker flag build failures.
* Fix silent failures when using old configuration files that are missing newly
  added options.
* Fix positioning main window in the center of the primary monitor for various
  multi-head configurations.

## v0.3.1

* Fix white background when the `background-image` option is blank or not
  present in the config file.

## v0.3.0

* Add a `password-label-text` option to customize the label text.
* Add a `show-input-cursor` option to hide the input's cursor.
* Add a `background-image` option to set a centered background image.
* Change the default value of the `user` option to `CHANGE_ME`.

## v0.2.0

* Add a `font-size` option to the configuration file.
* Fix start failure when no `user-session` is specified.
* Fix color of password input's cursor.

## v0.1.3

* Fix a build issue with some compilers.

## v0.1.2

* Fix background color when using multiple monitors.

## v0.1.1

* Fix various warnings caused by the generated CSS.

## v0.1.0

* Initial release.
