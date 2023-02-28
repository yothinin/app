CC=gcc
CFLAGS=-g -Wall
LIBS=`pkg-config --libs --cflags gtk+-3.0` -lmariadbclient -I/usr/include/glib-2.0 -lm -lX11 -lxkbfile -rdynamic
MENU_SOURCES=menu.c province.c route.c station.c functions.c mysql_fnct.c route_widget_fnct.c route_mysql_fnct.c station_widget_fnct.c station_mysql_fnct.c
MENU_OBJECTS=$(MENU_SOURCES:.c=.o)
MENU_EXECUTABLE=menu

.PHONY: all menu clean

all: $(MENU_EXECUTABLE)

menu: $(MENU_EXECUTABLE)

$(MENU_EXECUTABLE): $(MENU_OBJECTS)
	$(CC) $(MENU_OBJECTS) $(LIBS) -o $(MENU_EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

clean:
	rm -f $(MENU_OBJECTS) $(MENU_EXECUTABLE)
