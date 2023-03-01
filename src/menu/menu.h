#ifndef MENU_H
#define MENU_H

#include <gtk/gtk.h>

GtkWidget *
do_route (GtkApplication *app, gpointer user_data);
GtkWidget *
do_station (GtkApplication *app, gpointer user_data);
GtkWidget *
do_province (GtkApplication *app, gpointer user_data);
GtkWidget *
do_bustype (GtkApplication *app, gpointer user_data);

#endif /* MENU_H */
