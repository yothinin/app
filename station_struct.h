/*
 * station_struct.h
 * purpose to store a struct for the module
 * name format: [module]_struct.h
 */
#include <gtk/gtk.h>
 
#ifndef STATION_STRUCT_H
#define STATION_STRUCT_H

// MyObjects is the struct for keep widgets to send to another function
// store value in function name: activate
//
// Station is struct for store staCode and staName to passing
// to functin in mysql_fnct.c

typedef struct {
  gchar *staCode;
  gchar *staName;
} Station;

typedef struct _MyObjects{
  GtkBuilder *builder;
  GtkWidget *window;
  GtkWidget *treeview ;
  GtkTreeModel *model;
  GtkListStore *liststore; 
  GtkTreeIter iter;
  GtkTreeSelection *selected;
  GtkWidget *entStaCode; 
  GtkWidget *entStaName;
  GtkWidget *btnSave;
  GtkWidget *btnDelete;
  GtkWidget *btnExit;
  GtkWidget *btnNew;
  GtkWidget *btnDemo;
  gint edit;
  gchar *message;
  gboolean *is_running_ptr;
}MyObjects;

#endif // STATION_STRUCT_H
