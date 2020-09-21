TARGET = test

SRC_DIR = ./src
BUILD_DIR = ./build
BIN_DIR = ./bin

SRC_LIST = $(wildcard $(SRC_DIR)/*.c)
OBJ_LIST = $(SRC_LIST:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

CFLAGS += -Wall -Iinclude
LDLIBS += -lm

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ_LIST)
	$(CC) $^ $(LDLIBS) -o $(BIN_DIR)/$@


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BUILD_DIR)/* $(BIN_DIR)/*
