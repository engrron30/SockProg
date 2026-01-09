# Commands
GCC = gcc
TEST = test
MKDIR = mkdir

CFLAGS += -DSERVER_REUSE_ADDR

# Output Files
OUT_DIR ?= $(CURDIR)/../../Output
SERVER_BIN = server
CLIENT_BIN = client

.PHONY: all prep clean
all: prep_build build

build:
	$(GCC) $(CFLAGS) server.c -o $(OUT_DIR)/$(SERVER_BIN)
#	$(GCC) client.c -o $(OUT_DIR)/$(CLIENT_BIN)

prep_build:
	$(TEST) -d $(OUT_DIR) || $(MKDIR) -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)
