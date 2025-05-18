# Dezentrale Badge Firmware

> **Note:** This project is a work in progress.

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
1. Remove the battery.
2. Insert the USB cable (if not already connected).
3. Press and hold the button.
4. Re-insert the battery while holding the button.

After flashing, power-cycle the device by removing and reinserting the battery to start the new code.
