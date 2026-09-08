CC = gcc
CFLAGS = -Wall -Wextra -O0 -g3 -std=c11 -I./src
LDFLAGS = 

# Директории для объектных файлов
GUI_OBJ_DIR = obj/gui
CLI_OBJ_DIR = obj/cli

# GUI версия
GUI_SOURCES = main.c
GUI_OBJECTS = $(GUI_SOURCES:%.c=$(GUI_OBJ_DIR)/%.o)
GUI_TARGET = date-calc
GUI_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GUI_LDFLAGS = $(shell pkg-config --libs gtk+-3.0)

# CLI версия
CLI_SOURCES = main.c args.c
CLI_OBJECTS = $(CLI_SOURCES:%.c=$(CLI_OBJ_DIR)/%.o)
CLI_TARGET = date-calc-cli
CLI_CFLAGS = -DCLI

# Создание директорий
$(shell mkdir -p $(GUI_OBJ_DIR) $(CLI_OBJ_DIR))

all: $(GUI_TARGET) $(CLI_TARGET)

gui: $(GUI_TARGET)

cli: $(CLI_TARGET)

# GUI компиляция
$(GUI_OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(GUI_CFLAGS) -c $< -o $@

# CLI компиляция
$(CLI_OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CLI_CFLAGS) -c $< -o $@

$(GUI_TARGET): $(GUI_OBJECTS)
	$(CC) $(GUI_OBJECTS) $(LDFLAGS) $(GUI_LDFLAGS) -o $@

$(CLI_TARGET): $(CLI_OBJECTS)
	$(CC) $(CLI_OBJECTS) $(LDFLAGS) -o $@

clean:
	rm -rf obj $(GUI_TARGET) $(CLI_TARGET)

install: all
	install -Dm 755 $(GUI_TARGET) /usr/bin/
	install -Dm 644 $(GUI_TARGET).desktop /usr/share/applications/
	install -Dm 755 $(CLI_TARGET) /usr/bin/

.PHONY: all clean install gui cli