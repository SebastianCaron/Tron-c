COMPILER := gcc
SRC_DIR ?= src
BIN_DIR ?= bin

DEBUG ?= 1
ifeq '$(DEBUG)' '1'
CFLAGS ?= -Wall -MMD -g
else
CFLAGS ?= -Wall -MMD -03 -DNDEBUG
endif

LDFLAGS = -lncurses $$(sdl2-config --cflags --libs) -lSDL2_ttf

OBJS := $(shell find $(SRC_DIR) -name "*.c" | sed 's/.c$$/.o/g' | sed 's/$(SRC_DIR)/$(BIN_DIR)/g')
DEPS := $(OBJS:.o=.d)

TARGET ?= tron

all: $(TARGET)
	@echo "BUILD OK"

$(TARGET): $(OBJS)
	$(COMPILER) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) -c $< -o $@

.PHONY: clean mrpropre
clean:
	rm -rf $(BIN_DIR)

mrpropre: clean
	clear
	rm -f $(TARGET)

-include $(DEPS)


