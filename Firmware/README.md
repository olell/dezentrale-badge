# Dezentrale Badge Firmware

! This is very much work in progress !

## Setup

### 1. Toolchain
Instructions to install the dependencies/toolchain can be found here: https://github.com/cnlohr/ch32fun/wiki/Installation

### 2. SDK
Clone the SDK (ch32fun) parallel to the directory of this repository like this:
```
.
├── DezentraleBadge  # this repo
│   ├── Firmware
│   └── Hardware
└── ch32fun          # sdk repo
    └── ...
```

## Build & Flash
Navigate to the Firmware directory and run `make` to build and flash the firmware.
Please notice that the Makefile contains a custom command to flash the badge using an ardulink programmer. If you want
to change the serial port or use another programmer, please change that. If you're using the official wch-linkE, you
can probably just remove that line.
