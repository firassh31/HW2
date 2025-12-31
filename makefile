CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c11 -pedantic
TARGET  := my_copy
SRC     := my_copy.c
OBJ     := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
