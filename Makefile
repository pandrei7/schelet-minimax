# Sources used.
SRC_DIR := .
OUT_DIR := .

CC_SRC := $(wildcard $(SRC_DIR)/*.cc)
CC_EXECS := $(CC_SRC:$(SRC_DIR)/%.cc=$(OUT_DIR)/%)

TARGETS := $(CC_EXECS)

# Compilation flags.
CCFLAGS := -std=c++17 -Wall -Wextra


build: $(TARGETS)

$(CC_EXECS): $(OUT_DIR)/%: $(SRC_DIR)/%.cc
	g++ -o $@ $^ $(CCFLAGS)


.PHONY: clean
clean:
	rm -f $(TARGETS)
