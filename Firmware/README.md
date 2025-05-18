# Dezentrale Badge Firmware

> **Note:** This project is a work in progress.

## Button actions:

| Press times | Action                                    |
| ----------- | ----------------------------------------- |
| 1x          | Depends on animation                      |
| 2x          | Depends on animation                      |
| 3x          | Next animation                            |
| 4x          | Go to sleep (wake by pressing the button) |
| 5x          | Enter the USB bootloader                  |

## Setup

### 1. Toolchain

Find instructions to install the required dependencies and toolchain here: [ch32fun Installation Guide](https://github.com/cnlohr/ch32fun/wiki/Installation)

### 2. SDK

Clone the SDK [ch32fun](https://github.com/cnlohr/ch32fun) alongside this repository as shown below:

```
.
├── DezentraleBadge  # this repo
│   ├── Firmware
│   └── Hardware
└── ch32fun          # sdk repo
    └── ...
```

## Build & Flash

Navigate to the `Firmware` directory and run `make` to build and flash the firmware.

The provided `Makefile` includes a custom command for flashing the badge using an Ardulink programmer. If you need to change the serial port or use a different programmer, please update the Makefile accordingly. For the official WCH-LinkE, you can remove the custom flashing line.

### Flash using the USB bootloader

If your badge has the USB bootloader installed, you can flash it by connecting via USB. To enter the bootloader, follow these steps:

#### Option A: From firmware

You can enter the bootloader from firmware by pressing the button 5 times, the badge then starts to blink rapidly. To enter the
bootloader you now have to press the button again. By pressing the button 3x (animation change) you can abort entering the bootloader.

#### Option B: Pressing the button while power on

1. Remove the battery.
2. Insert the USB cable (if not already connected).
3. Press and hold the button.
4. Re-insert the battery while holding the button.
