Scaffold
========
the [Scaffold](https://github.com/choubbikeyboards/scaffold) is a split keyboard with analog sticks used as a pointing device and a scroll wheel. It is inspired by many sources, like the Absolem, the Kyria, and the Ferris.
![Scaffold picture](https://github.com/choubbikeyboards/scaffold/blob/main/front.jpg)

## First Time Setup

Download or clone the `qmk_firmware` repo and navigate to its top level directory. Once your build environment is setup, you'll be able to generate the default .hex using:

Depending on your Layout chose one of the follwing commands:

```
$ make scaffold:YOUR_KEYMAP_NAME
```

example:
```
$ make scaffold:default
```

If everything worked correctly you will see a file:

```
scaffold_YOUR_BOARD_YOUR_KEYMAP_NAME.hex
```

For more information on customizing keymaps, take a look at the primary documentation for [Customizing Your Keymap](/docs/faq_keymap.md) in the main readme.md.


## Keymaps

#### Default
Simple QWERTY or Colemak-DH layout with tri-layer system.
Edit the keymap.c file to adjust a few things first:
* to choose between a QWERTY or Colemak DH base layout, comment the one you don't want and uncomment the one you want between `#define COLEMAK_DH_SCAFFOLD` and `//#define QWERTY_SCAFFOLD`. By default it is set to Colemak DH.
* If you don't use OLED displays, skip this step. Otherwise, if you didn't install the bottom keys (to keep the traditional 3x5 layout for example), comment or remove the line `#define HAS_ADD_BOTTOM_ROW` to avoid displaying the layout for these keys that you can't use. For the same reason, if you kept the additional index columns free for joysticks, comment or remove the line `#define HAS_ADD_INDEX_COL`.

## Notes on Software Configuration

The default keymap is usable for the full 42-key keyboard, although it is targeted towards 3x5 and 4x4-1 layouts, and has redundant keys on the full board.

## Flashing

To flash your firmware take a look at: [Flashing Instructions and Bootloader Information](https://docs.qmk.fm/#/flashing)
I recommend using `EE_hands` (configured by default) to let each half know if which one it is no matter which half you plugged in: `make scaffold:default:avrdude-split-left` or `make scaffold:default:avrdude-split-right` to flash each half.

