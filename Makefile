COMPILER := gcc
SRC_DIR ?= src
BIN_DIR ?= bin

DEBUG ?= 1
ifeq '$(DEBUG)' '1'
CFLAGS ?= -Wall -MMD -g
else
CFLAGS ?= -Wall -MMD -03 -DNDEBUG
endif

LDFLAGS = -lncursesw $$(sdl2-config --cflags --libs) -lSDL2_ttf -lm

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

.PHONY: clean mrpropre fr0 debug network
clean:
	rm -rf $(BIN_DIR)

mrpropre: clean
	clear
	rm -f $(TARGET)
	rm -rf val.txt
	rm -f q.bot


fr0: mrpropre all
	clear
	./tron

debug: all
	clear
	valgrind --leak-check=full --log-file=val.txt ./tron -sdl

network: all
	./tron -sdl -h &
	./tron -sdl -ip127.0.0.1 &

bot: CFLAGS += -DBOT_TRAINING=1
bot: mrpropre all
	@echo "BUILD QLEARNING OK"

training:
	@total_victories=0; \
	count=0; \
	for i in $(shell seq 1 $(n)); do \
		echo "Training run $$i of $(n)"; \
		output=$$(./tron -train -ep100000); \
		echo "$$output"; \
		victory_percent=$$(echo "$$output" | tail -n1 | cut -d' ' -f6); \
		total_victories=$$(echo "$$total_victories + $$victory_percent" | bc); \
		count=$$((count + 1)); \
	done; \
	average=$$(echo "scale=2; $$total_victories / $$count" | bc); \
	echo "Average : $$average %"

-include $(DEPS)

