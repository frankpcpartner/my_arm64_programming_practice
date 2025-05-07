CC = gcc
LD = ld

BUILD_DIR = build
SRC_DIR = src

SRCS_C = $(wildcard $(SRC_DIR)/*.c)
OBJS_C = $(SRCS_C:$(SRC_DIR)/%.c=$(BUILD_DIR)/c/%.o)

CFLAGS = -pthread -Wall -g -Iinclude -c

.PHONY: all clean

all: clean build_dir test_false_sharing 


clean:
	rm -rf $(BUILD_DIR)
	rm -f test_false_sharing

$(BUILD_DIR)/c/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

build_dir: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/c
	


test_false_sharing: $(OBJS_C)
	$(CC) $(OBJS_C) -o test_false_sharing	

