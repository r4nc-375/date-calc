CC = gcc
CFLAGS = -O0 -g3 -std=c11 -I./src $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = $(shell pkg-config --libs gtk+-3.0)

SOURCES = main.c
OBJECTS = $(SOURCES:%.c=%.o)
TARGET = date-calc

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -rf $(OBJECTS) $(TARGET)

.PHONY: all clean
