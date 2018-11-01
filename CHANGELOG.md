# CHANGELOG

## master

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
