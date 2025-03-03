TOOLCHAIN_PREFIX = aarch64-linux-gnu-
CC = $(TOOLCHAIN_PREFIX)gcc
LD = $(TOOLCHAIN_PREFIX)ld
OBJCPY = $(TOOLCHAIN_PREFIX)objcopy

BUILD_DIR = build
SRC_DIR = src

LINKER_FILE = $(SRC_DIR)/linker.ld
SRCS_C = $(wildcard $(SRC_DIR)/*.c)
OBJS_C = $(SRCS_C:$(SRC_DIR)/%.c=$(BUILD_DIR)/c/%.o)
SRCS_ASM = $(wildcard $(SRC_DIR)/*.S)
OBJS_ASM = $(SRCS_ASM:$(SRC_DIR)/%.S=$(BUILD_DIR)/asm/%.o)

CFLAGS = -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles -fpie -g -Iinclude -c
#CFLAGS = -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles -g -Iinclude -c

.PHONY: all clean

all: clean build_dir kernel8.img


clean:
	rm -rf $(BUILD_DIR)
	rm -f *.elf *.img	

$(BUILD_DIR)/c/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/asm/%.o: $(SRC_DIR)/%.S
	$(CC) $(CFLAGS) $< -o $@

build_dir: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/c
	mkdir -p $(BUILD_DIR)/asm


kernel8.img: $(OBJS_C) $(OBJS_ASM)
	$(LD) $(OBJS_C) $(OBJS_ASM) $(OBJS_LIB) -T $(LINKER_FILE) -pie -nostdlib -o kernel8.elf
	#$(LD) $(OBJS_C) $(OBJS_ASM) $(OBJS_LIB) -T $(LINKER_FILE) -nostdlib -o kernel8.elf
	$(OBJCPY) -O binary kernel8.elf kernel8.img

mini_uart: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio

run: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio

debug: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -serial stdio -S -s

asm: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -d in_asm

tty: all
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial pty
