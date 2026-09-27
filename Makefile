CC = gcc
CFLAGS = -Wall -Wextra -Ibackend/include -O2
LIBS = 

SRC_DIR = backend/src
OBJ_DIR = obj

SRCS = backend/main.c \
       $(SRC_DIR)/server.c \
       $(SRC_DIR)/handlers.c \
       $(SRC_DIR)/mongoose.c \
       $(SRC_DIR)/db.c

OBJS = main.o \
       $(OBJ_DIR)/server.o \
       $(OBJ_DIR)/handlers.o \
       $(OBJ_DIR)/mongoose.o \
       $(OBJ_DIR)/db.o


TARGET = yggdrasil_server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

main.o: backend/main.c
	$(CC) $(CFLAGS) -c backend/main.c -o main.o

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf main.o $(OBJ_DIR) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
