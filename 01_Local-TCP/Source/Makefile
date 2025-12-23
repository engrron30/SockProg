GCC = gcc

# Output Files
OUT_DIR ?= $(CURDIR)/../Output
SERVER_BIN = server
CLIENT_BIN = client

.PHONY: all prep clean
all: clean prep_build build

build:
	$(GCC) server.c -o $(OUT_DIR)/$(SERVER_BIN)
	$(GCC) client.c -o $(OUT_DIR)/$(CLIENT_BIN)

prep_build:
	test -d $(OUT_DIR) || mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)
