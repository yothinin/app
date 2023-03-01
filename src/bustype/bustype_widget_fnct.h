/*
 * header for station_widget_fnct.c
 */
 
#ifndef BUSTYPE_WIDGET_FNCT_H
#define BUSTYPE_WIDGET_FNCT_H
#include "bustype_struct.h"

gboolean entBusCode_release (GtkWidget *widget, GdkEventKey *event, gpointer userdata);
gboolean entBusName_release (GtkWidget *widget, GdkEventKey *event, gpointer userdata);
void entBusCode_focus (GtkWidget *widget, gpointer userdata);
void entBusName_focus (GtkWidget *widget, gpointer userdata);
void busExit_click (GtkWidget *widget, gpointer userdata);
void busNew_click (GtkWidget *widget, gpointer userdata);
void busSave_click (GtkWidget *widget, gpointer user_data);
void busDelete_click (GtkWidget *widget, gpointer user_data);
void bus_row_change (GtkWidget *treeView, gpointer userdata);
void bus_update_tree_view(BustypeWidgets *bustypeObj, Bustype bustype, gboolean result);
void bus_update_tree_store(BustypeWidgets *bustypeObj, Bustype bustype, gboolean result);
void insertBustypeToListStore (BustypeWidgets *bustypeObj);
void entBusCode_grab_focus (GtkWidget *widget, gpointer user_data);
void entBusName_grab_focus (GtkWidget *widget, gpointer user_data);

#endif // PROVINCE_WIDGET_FNCT_H
