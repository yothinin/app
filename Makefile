CC=gcc
CFLAGS=-g -Wall
LIBS=`pkg-config --libs --cflags gtk+-3.0` -lmariadbclient -I/usr/include/glib-2.0 -lm -lX11 -lxkbfile -rdynamic -lgtk-3
MENU_SOURCES=src/menu/menu.c \
              src/province/province.c \
              src/bustype/bustype.c \
              src/route/route.c \
              src/station/station.c \
              src/common/functions.c \
              src/common/mysql_fnct.c \
              src/province/province_widget_fnct.c \
              src/province/province_mysql_fnct.c \
              src/bustype/bustype_mysql_fnct.c \
              src/bustype/bustype_widget_fnct.c \
              src/route/route_widget_fnct.c \
              src/route/route_mysql_fnct.c \
              src/station/station_widget_fnct.c \
              src/station/station_mysql_fnct.c

MENU_OBJECTS=$(patsubst src/%.c,objects/%.o,$(MENU_SOURCES))
$(info MENU_OBJECTS=$(MENU_OBJECTS))

MENU_EXECUTABLE=bin/menu

.PHONY: all menu clean

all: $(MENU_EXECUTABLE)

menu: $(MENU_EXECUTABLE)

$(MENU_EXECUTABLE): $(MENU_OBJECTS)
	mkdir -p $(@D)
	$(CC) $(MENU_OBJECTS) $(LIBS) -o $(MENU_EXECUTABLE)

objects/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LIBS) -Isrc/common -c $< -o $@

clean:
	rm -rf objects/* bin/menu
