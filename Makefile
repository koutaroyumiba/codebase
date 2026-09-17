CC ?= cc
CPPFLAGS ?= -Isrc
CFLAGS ?= -std=c23 -Wall -Wextra -Wpedantic

SRC_DIR := src
BUILD_DIR := build
TARGET := $(BUILD_DIR)/codebase
TEST_TARGET := $(BUILD_DIR)/codebase_tests

SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d) $(TEST_TARGET).d

.PHONY: all run test clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

$(TEST_TARGET): tests/test_main.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -MF $@.d -MT $@ $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
