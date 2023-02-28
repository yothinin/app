/*
 * station_struct.h
 * purpose to store a struct for the module
 * name format: [module]_struct.h
 */
#include <gtk/gtk.h>
 
#ifndef PROVINCE_STRUCT_H
#define PROVINCE_STRUCT_H

// MyObjects is the struct for keep widgets to send to another function
// store value in function name: activate
//
// Station is struct for store staCode and staName to passing
// to functin in mysql_fnct.c

typedef struct {
  gchar *prvCode;
  gchar *prvName;
} Province;

typedef struct {
  GtkBuilder *builder;
  GtkWidget *window;
  GtkWidget *treeView ;
  GtkTreeModel *model;
  GtkTreeStore *treeStore; 
  GtkTreeIter iter;
  GtkTreeSelection *selected;
  GtkWidget *entPrvCode; 
  GtkWidget *entPrvName;
  GtkWidget *btnNew;
  GtkWidget *btnSave;
  GtkWidget *btnDelete;
  GtkWidget *btnExit;
  gint edit;
  gchar *message;
  gboolean *is_running_ptr;
} ProvinceWidgets;

#endif // PROVINCE_STRUCT_H
