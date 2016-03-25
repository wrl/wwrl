CC        = clang
LD        = $(CC)

CFLAGS    = -std=c99 -Wall -Wextra -Werror -Wno-unused-parameter -pedantic -D_GNU_SOURCE -Iinclude -ggdb
LDFLAGS   = -flto -ggdb

BUILD_DIR = build
TEST_DIR  = test
SRC_DIR   = src

TARGETS   = $(BUILD_DIR)
TARGETS  += $(BUILD_DIR)/test/vector
TARGETS  += $(BUILD_DIR)/test/bip_buffer

VECTOR_TEST_OBJECTS  = $(BUILD_DIR)/test/vector.o

BIP_BUFFER_TEST_OBJECTS  = $(BUILD_DIR)/test/bip_buffer.o
BIP_BUFFER_TEST_OBJECTS += $(BUILD_DIR)/bip_buffer.o

.PHONY: clean

all: $(TARGETS)

$(BUILD_DIR)/test/vector: $(VECTOR_TEST_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/test/bip_buffer: $(BIP_BUFFER_TEST_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@
	@mkdir -p $@/test

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<
