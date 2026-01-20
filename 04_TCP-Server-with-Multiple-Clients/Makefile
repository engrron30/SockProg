# Default Directories
SRC_DIR = $(CURDIR)/Source
INC_DIR = $(CURDIR)/Include
OUT_DIR = $(CURDIR)/Output
SERVER_DIR = $(SRC_DIR)/Server
CLIENT_DIR = $(SRC_DIR)/Client

# Commands
MKDIR = mkdir
MAKE = make

all:
	$(MKDIR) -p $(OUT_DIR)
	$(MAKE) -C $(SERVER_DIR) INC_DIR=$(INC_DIR) OUT_DIR=$(OUT_DIR)
	$(MAKE) -C $(CLIENT_DIR) INC_DIR=$(INC_DIR) OUT_DIR=$(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)
