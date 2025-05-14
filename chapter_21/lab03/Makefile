TOOLCHAIN_PREFIX = aarch64-linux-gnu-
CC = $(TOOLCHAIN_PREFIX)gcc
LD = $(TOOLCHAIN_PREFIX)ld
OBJCPY = $(TOOLCHAIN_PREFIX)objcopy
QEMU = qemu-aarch64

BUILD_DIR = build
SRC_DIR = src

LINKER_FILE = $(SRC_DIR)/linker.ld
SRCS_C = $(wildcard $(SRC_DIR)/*.c)
OBJS_C = $(SRCS_C:$(SRC_DIR)/%.c=$(BUILD_DIR)/c/%.o)
OFFSET_C = $(wildcard $(SRC_DIR)/offsets/*.c)
OBJS_OFFSET_C = $(OFFSET_C:$(SRC_DIR)/offsets/%.c=$(BUILD_DIR)/c/offsets/%.o)
SRCS_ASM = $(wildcard $(SRC_DIR)/*.S)
OBJS_ASM = $(SRCS_ASM:$(SRC_DIR)/%.S=$(BUILD_DIR)/asm/%.o)

OFFSET_GEN = asm-offsets
HEADER = asm-offsets.h

CFLAGS = -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles -g -Iinclude -c

OFFSET_CFLAGS = -Wall -O2 -g -Iinclude -Iinclude/offsets -D_BUILD_ASM_OFFSET_ -c

.PHONY: all clean

all: clean build_dir kernel8.img

clean:
	rm -f $(OFFSET_GEN)
	rm -f include/$(HEADER)
	rm -rf $(BUILD_DIR)
	rm -f *.elf *.img	

$(BUILD_DIR)/c/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/c/offsets/%.o: $(SRC_DIR)/offsets/%.c
	$(CC) $(OFFSET_CFLAGS) $< -o $@

$(BUILD_DIR)/asm/%.o: $(SRC_DIR)/%.S
	$(CC) $(CFLAGS) $< -o $@

build_dir: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/c
	mkdir -p $(BUILD_DIR)/c/offsets
	mkdir -p $(BUILD_DIR)/asm

$(HEADER): $(OFFSET_GEN)
	$(QEMU) ./$(OFFSET_GEN) > include/$(HEADER)

$(OFFSET_GEN): $(OBJS_OFFSET_C)
	$(CC) -Iinclude -Iinclude/offset  -static -o $(OFFSET_GEN) $(OBJS_OFFSET_C)

$(OBJS_C): $(HEADER)

kernel8.img: $(OBJS_C) $(OBJS_ASM)
	$(LD) $(OBJS_C) $(OBJS_ASM) $(OBJS_LIB) -T $(LINKER_FILE) -nostdlib -o kernel8.elf
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
