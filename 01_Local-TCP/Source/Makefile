GCC = gcc

# Output Files
OUT_DIR ?= ../Output
SERVER_BIN = server
CLIENT_BIN = client

.PHONY: all clean
all:
	$(GCC) server.c -o $(OUT_DIR)/$(SERVER_BIN)
	$(GCC) client.c -o $(OUT_DIR)/$(CLIENT_BIN)

clean:
	rm -rf $(OUT_DIR)
