TOOLCHAIN_PREFIX = aarch64-linux-gnu-
CC = $(TOOLCHAIN_PREFIX)gcc
LD = $(TOOLCHAIN_PREFIX)ld
OBJCPY = $(TOOLCHAIN_PREFIX)objcopy

BUILD_DIR = build
SRC_DIR = src

SRCS_C = $(wildcard $(SRC_DIR)/*.c)
OBJS_C = $(SRCS_C:$(SRC_DIR)/%.c=$(BUILD_DIR)/c/%.o)
SRCS_ASM = $(wildcard $(SRC_DIR)/*.S)
OBJS_ASM = $(SRCS_ASM:$(SRC_DIR)/%.S=$(BUILD_DIR)/asm/%.o)

CFLAGS = -Wall -g -c

.PHONY: all clean

all: clean build_dir atomic


clean:
	rm -rf $(BUILD_DIR)
	

$(BUILD_DIR)/c/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/asm/%.o: $(SRC_DIR)/%.S
	$(CC) $(CFLAGS) $< -o $@

build_dir: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/c
	mkdir -p $(BUILD_DIR)/asm


atomic: $(OBJS_C) $(OBJS_ASM)
	$(CC) -g $(OBJS_C) $(OBJS_ASM) $(OBJS_LIB) -o atomic
	

run: all
	qemu-aarch64 -L /usr/aarch64-linux-gnu ./atomic

debug: all
	qemu-aarch64 -g 1234 -L /usr/aarch64-linux-gnu ./atomic 