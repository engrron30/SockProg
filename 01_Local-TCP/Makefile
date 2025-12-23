# Default Directories
SRC_DIR = $(CURDIR)/Source
OUT_DIR = $(CURDIR)/Output

# Commands
MKDIR = mkdir
MAKE = make

all:
	$(MKDIR) -p $(OUT_DIR)
	$(MAKE) -C $(SRC_DIR) OUT_DIR=$(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)
