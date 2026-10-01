#!/bin/sh
set -e
qemu-system-aarch64 -M raspi3b -kernel build/kernel8.img -display none -serial null -serial stdio
