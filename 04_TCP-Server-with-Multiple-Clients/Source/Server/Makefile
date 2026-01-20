# Commands
GCC = gcc
TEST = test
MKDIR = mkdir

# Output Files
OUT_DIR ?= $(CURDIR)/../../Output
INC_DIR ?= $(CURDIR)/../../Include
COMMON_HEADERS_DIR ?= $(INC_DIR)/Common
SERVER_BIN = server
CLIENT_BIN = client

CFLAGS += -DSERVER_REUSE_ADDR

# Include Headers
CFLAGS += -I$(COMMON_HEADERS_DIR)

.PHONY: all prep clean
all: prep_build build

build:
	$(GCC) $(CFLAGS) server.c -o $(OUT_DIR)/$(SERVER_BIN)

prep_build:
	$(TEST) -d $(OUT_DIR) || $(MKDIR) -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)
