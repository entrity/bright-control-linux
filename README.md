# Bright Controls (Linux)

## Installation

```
make build install
```

A `.desktop` file is included. With it installed to your `~/.local/share/applications` directory, you get a Unity launcher item.

A c source file is included (rather than using a script) so that it can be given to the root user and have its user bit set.

## GUI Usage

Run the desktop file, and you get a panel of buttons for altering redshift (gamma) and bright (backlight). You need to supply images for the buttons yourself (not included for copyright reasons). See the `IMG_*` variables at the top of the python script.

## CLI Usage

```
bright    # read the current bright value
bright +  # double the current bright value
bright -  # halve the current bright value
bright +X # increment the current bright value by X
bright -X # decrement the current bright value by X
```
