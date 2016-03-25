CC        = clang
LD        = $(CC)

CFLAGS    = -std=c99 -Wall -Wextra -Werror -Wno-unused-parameter -pedantic -D_GNU_SOURCE -Iinclude
LDFLAGS   = -flto

BUILD_DIR = build
SRC_DIR   = src

TARGETS   = $(BUILD_DIR)
TARGETS  += $(BUILD_DIR)/vector_test
TARGETS  += $(BUILD_DIR)/bip_buffer_test

VECTOR_TEST_OBJECTS  = $(BUILD_DIR)/vector_test.o

BIP_BUFFER_TEST_OBJECTS  = $(BUILD_DIR)/bip_buffer_test.o
BIP_BUFFER_TEST_OBJECTS += $(BUILD_DIR)/bip_buffer.o

.PHONY: clean

all: $(TARGETS)

$(BUILD_DIR)/vector_test: $(VECTOR_TEST_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $<

$(BUILD_DIR)/bip_buffer_test: $(BIP_BUFFER_TEST_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<
