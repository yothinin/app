/*
 * bustype_struct.h
 * purpose to store a struct for the module
 * name format: [module]_struct.h
 */
#include <gtk/gtk.h>
 
#ifndef BUSTYPE_STRUCT_H
#define BUSTYPE_STRUCT_H

// BustypeWidgets is the struct for keep widgets to send to another function
// store value in function name: activate
//
// Bustype is struct for store busCode and busName to passing
// to functin in mysql_fnct.c

typedef struct {
  gchar *busCode;
  gchar *busName;
} Bustype;

typedef struct {
  GtkBuilder *builder;
  GtkWidget *window;
  GtkWidget *treeView ;
  GtkTreeModel *model;
  GtkTreeStore *treeStore; 
  GtkTreeIter iter;
  GtkTreeSelection *selected;
  GtkWidget *entBusCode; 
  GtkWidget *entBusName;
  GtkWidget *btnNew;
  GtkWidget *btnSave;
  GtkWidget *btnDelete;
  GtkWidget *btnExit;
  gint edit;
  gchar *message;
  gboolean *is_running_ptr;
} BustypeWidgets;

#endif // BUSTYPE_STRUCT_H
