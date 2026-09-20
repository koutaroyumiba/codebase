CC ?= cc
CPPFLAGS ?= -Isrc
CFLAGS ?= -std=c23 -Wall -Wextra -Wpedantic

SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build
TARGET := $(BUILD_DIR)/codebase
TEST_TARGET := $(BUILD_DIR)/codebase_tests

SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
TEST_SOURCES := $(shell find $(TEST_DIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/$(TEST_DIR)/%.o,$(TEST_SOURCES))
LIB_OBJECTS := $(filter-out $(BUILD_DIR)/main.o, $(OBJECTS))
DEPS := $(OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)

.PHONY: all run test clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

$(TEST_TARGET): $(TEST_OBJECTS) $(LIB_OBJECTS) | $(BUILD_DIR)
	$(CC) $(TEST_OBJECTS) $(LIB_OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
