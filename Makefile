# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -pthread -O2
DEBUG_FLAGS = -g -DDEBUG

# Project name
PROG = traffic_sim

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Test files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_PROG = $(BIN_DIR)/run_tests

# Default target
.PHONY: all
all: dirs $(BIN_DIR)/$(PROG)

# Create necessary directories
.PHONY: dirs
dirs:
	@mkdir -p $(SRC_DIR) $(OBJ_DIR) $(BIN_DIR) $(TEST_DIR)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link the program
$(BIN_DIR)/$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS)

# Debug build
.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Run the program
.PHONY: run
run: all
	./$(BIN_DIR)/$(PROG)

# Compile tests
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build and run tests
.PHONY: test
test: dirs $(TEST_PROG)
	./$(TEST_PROG)

$(TEST_PROG): $(TEST_OBJS) $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	$(CC) $^ -o $@ $(CFLAGS)

# Install the program
.PHONY: install
install: all
	install -m 755 $(BIN_DIR)/$(PROG) /usr/local/bin/$(PROG)

# Uninstall the program
.PHONY: uninstall
uninstall:
	rm -f /usr/local/bin/$(PROG)

# Clean build files
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Generate documentation using doxygen
.PHONY: docs
docs:
	doxygen Doxyfile

# Format source code using clang-format
.PHONY: format
format:
	find . -name '*.c' -o -name '*.h' | xargs clang-format -i

# Check for memory leaks using valgrind
.PHONY: memcheck
memcheck: debug
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN_DIR)/$(PROG)

# Static code analysis using cppcheck
.PHONY: analyze
analyze:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SRC_DIR)

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all       - Build the program (default)"
	@echo "  debug     - Build with debug symbols"
	@echo "  run       - Build and run the program"
	@echo "  test      - Build and run tests"
	@echo "  clean     - Remove build files"
	@echo "  install   - Install the program"
	@echo "  uninstall - Uninstall the program"
	@echo "  docs      - Generate documentation"
	@echo "  format    - Format source code"
	@echo "  memcheck  - Check for memory leaks"
	@echo "  analyze   - Run static code analysis"
	@echo "  help      - Show this help message"
