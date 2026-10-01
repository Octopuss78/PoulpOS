ARMGNU  ?= aarch64-linux-gnu
CC      = $(ARMGNU)-gcc
LD      = $(ARMGNU)-ld
OBJCOPY = $(ARMGNU)-objcopy

BUILD   = build
SRCDIRS = boot arch drivers lib kernel
INCDIRS = $(addprefix -I,$(SRCDIRS))

CFLAGS  = -Wall -Wextra -ffreestanding -nostdinc -nostdlib -nostartfiles \
          -mgeneral-regs-only -MMD -MP $(INCDIRS)
ASFLAGS = -MMD -MP $(INCDIRS)

CSRC = $(foreach d,$(SRCDIRS),$(wildcard $(d)/*.c))
SSRC = $(foreach d,$(SRCDIRS),$(wildcard $(d)/*.S))

# boot.o en premier dans la liste d'objets
OBJS = $(BUILD)/boot/boot.o \
       $(patsubst %.S,$(BUILD)/%.o,$(filter-out boot/boot.S,$(SSRC))) \
       $(patsubst %.c,$(BUILD)/%.o,$(CSRC))

all: $(BUILD)/kernel8.img

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD)/kernel8.img: $(OBJS) link.ld
	$(LD) -nostdlib -T link.ld $(OBJS) -o $(BUILD)/kernel8.elf
	$(OBJCOPY) -O binary $(BUILD)/kernel8.elf $@

clean:
	rm -rf $(BUILD)

-include $(OBJS:.o=.d)

.PHONY: all clean
