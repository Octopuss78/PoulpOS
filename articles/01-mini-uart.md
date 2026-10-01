# mini-uart

First step of PoulpOS: bring up the BCM2837's mini UART and print "Hello world" over serial, on QEMU and on a real Raspberry Pi 3B+.

Full write-up: [Setting up Mini UART Bare Metal on a Raspberry Pi 3B+](<ARTICLE_LINK>)

## Requirements
- `aarch64-linux-gnu-gcc` (cross-compiler)
- `qemu-system-aarch64` (optional, to run without hardware)
- A **3.3 V** USB-to-serial adapter (e.g. FT232) for the real board

## Build
    make

Output: `build/kernel8.img`

## Run on QEMU
    qemu-system-aarch64 -M raspi3b -kernel build/kernel8.img -serial null -serial stdio -display none

## Run on a Raspberry Pi 3B+
1. Wire the adapter to the Pi's header (physical pins):
   - adapter TX → pin 10 (GPIO15, RXD)
   - adapter RX → pin 8 (GPIO14, TXD)
   - GND → pin 6
2. Copy the firmware and the kernel to the SD card's FAT32 boot partition:

       cp firmware/* build/kernel8.img <mountpoint>/

3. Open a serial session, then power the Pi:

       picocom -b 115200 /dev/ttyUSB0

## Credits
- `link.ld` and `Makefile` adapted from [sypstraw/rpi4-osdev](https://github.com/sypstraw/rpi4-osdev)
- `firmware/` files from [raspberrypi/firmware](https://github.com/raspberrypi/firmware) (<VERSION_OR_COMMIT>), distributed under `firmware/LICENCE.broadcom`
