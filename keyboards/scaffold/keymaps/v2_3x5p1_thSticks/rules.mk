OLED_DRIVER_ENABLE = yes  # Enables the use of OLED displays
OLED_ENABLE = yes  # Enables the use of OLED displays

CONSOLE_ENABLE = yes
COMBO_ENABLE = yes
UNICODE_ENABLE = no
JOYSTICK_SCAFFOLD_ENABLE  = yes  # Enables analog joystick code


ifeq ($(strip $(JOYSTICK_SCAFFOLD_ENABLE)), yes)
    POINTING_DEVICE_ENABLE = yes
    OPT_DEFS += -DJOYSTICK_SCAFFOLD_ENABLE
        SRC += analog.c
        SRC += joystick_scaffold.c
endif

