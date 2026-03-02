CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -pedantic
TARGET  = task_manager
SRC     = task_manager.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) tasks.txt

.PHONY: all clean