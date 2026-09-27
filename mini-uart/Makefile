ARMGNU    = aarch64-linux-gnu
CFLAGS    = -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles
BUILD_DIR = build

CFILES = $(wildcard *.c)
OFILES = $(CFILES:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/kernel8.img

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: boot.S | $(BUILD_DIR)
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel8.img: $(BUILD_DIR)/boot.o $(OFILES) link.ld
	$(ARMGNU)-ld -nostdlib $(BUILD_DIR)/boot.o $(OFILES) -T link.ld -o $(BUILD_DIR)/kernel8.elf
	$(ARMGNU)-objcopy -O binary $(BUILD_DIR)/kernel8.elf $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
