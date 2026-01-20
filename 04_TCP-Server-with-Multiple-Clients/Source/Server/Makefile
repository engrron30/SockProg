# Commands
GCC = gcc
TEST = test
MKDIR = mkdir

# Output Files
SRC_DIR ?= $(CURDIR)/../../Source
INC_DIR ?= $(CURDIR)/../../Include
OUT_DIR ?= $(CURDIR)/../../Output
COM_DIR ?= $(SRC_DIR)/Common
SERVER_BIN = server

CFILES += $(COM_DIR)/config_socket.c
CFLAGS += -DSERVER_REUSE_ADDR

# Include Headers
CFLAGS += -I$(INC_DIR)

.PHONY: all prep clean
all: prep_build build

build:
	$(GCC) $(CFLAGS) $(CFILES) server.c -o $(OUT_DIR)/$(SERVER_BIN)

prep_build:
	$(TEST) -d $(OUT_DIR) || $(MKDIR) -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)
