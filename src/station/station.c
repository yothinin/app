/*
 * All, these code are example and prototype of my new project 
 * in the simple code and some where have a problem or not good
 * if you (all) who found something wrong or the best way to coding
 * please tell me more to correct it.
 * 
 * yothinin@pimpanya.com
 * 
*/

#ifndef GTK_GTK_H
#define GTK_GTK_H
#include <gtk/gtk.h>
#endif //GTK_GTK_H

#include "functions.h"
#include "station_mysql_fnct.h"
#include "station_widget_fnct.h"
#include "station_struct.h"

void stationExit_click (GtkWidget *widget, gpointer userdata){
  StationWidgets *stationObj = (StationWidgets*) userdata;
  g_print ("exit: %s\n", stationObj->message);
  change_keyb ("us");
  gtk_widget_set_sensitive (stationObj->btnExit, FALSE);
  //gtk_main_quit();
  *(stationObj->is_running_ptr) = FALSE;
  gtk_widget_destroy (stationObj->window);
}

GtkWidget *
do_station (GtkApplication *app, gpointer user_data){
  //StationWidgets *stationObj = (StationWidgets*) user_data; // pointer to struct
  StationWidgets *stationObj = g_new(StationWidgets, 1);

  gboolean *is_running_ptr = (gboolean *) user_data;
  *is_running_ptr = TRUE;
  
  stationObj->is_running_ptr = is_running_ptr;
  stationObj->message = "โปรแกรมข้อมูลสถานี";
  stationObj->builder = gtk_builder_new_from_file("glade/tms_station.glade");
  //gtk_builder_connect_signals(builder, NULL);

  stationObj->window = (GtkWidget*)gtk_builder_get_object(stationObj->builder, "window");
  gtk_window_set_position (GTK_WINDOW(stationObj->window), GTK_ALIGN_CENTER);

  stationObj->edit = 0;
  g_print ("%s\n", stationObj->message);

  stationObj->treeview = (GtkWidget*) gtk_builder_get_object (stationObj->builder, "treeView");
  stationObj->entStaCode = (GtkWidget*) gtk_builder_get_object (stationObj->builder, "entStaCode");
  stationObj->entStaName = (GtkWidget*) gtk_builder_get_object (stationObj->builder, "entStaName");
  stationObj->btnSave = (GtkWidget*) gtk_builder_get_object (stationObj->builder, "btnSave");
  stationObj->btnDelete = (GtkWidget*) gtk_builder_get_object (stationObj->builder, "btnDelete");
  stationObj->btnExit = (GtkWidget*) gtk_builder_get_object (stationObj->builder, "btnExit");
  stationObj->btnNew = (GtkWidget*) gtk_builder_get_object (stationObj->builder, "btnNew");
  stationObj->liststore = (GtkListStore*) gtk_builder_get_object (stationObj->builder, "mainStore");
  stationObj->btnDemo = (GtkWidget*) gtk_builder_get_object (stationObj->builder, "btnDemo");
  
  insertDataToListStore(stationObj); // Insert data to GtkListStore at the first run.

  // don't use & before stationObj.
  g_signal_connect (stationObj->btnNew, "clicked", G_CALLBACK (stationNew_click), stationObj);
  g_signal_connect (stationObj->entStaCode, "focus-in-event", G_CALLBACK (entStaCode_focus), stationObj);
  g_signal_connect (stationObj->entStaName, "focus-in-event", G_CALLBACK (entStaName_focus), stationObj);
  g_signal_connect (stationObj->entStaCode, "key-release-event", G_CALLBACK (entStaCode_release), stationObj);
  g_signal_connect (stationObj->entStaName, "key-release-event", G_CALLBACK (entStaName_release), stationObj);
  g_signal_connect (stationObj->btnSave, "clicked", G_CALLBACK (stationSave_click), stationObj);
  g_signal_connect (stationObj->btnDelete, "clicked", G_CALLBACK (stationDelete_click), stationObj);
  g_signal_connect (stationObj->treeview, "cursor-changed", G_CALLBACK (station_row_change), stationObj);
  g_signal_connect (stationObj->btnExit, "clicked", G_CALLBACK (stationExit_click), stationObj);
  g_signal_connect (stationObj->window, "destroy", G_CALLBACK (stationExit_click), stationObj);
  g_signal_connect (stationObj->btnDemo, "clicked", G_CALLBACK (btnDemo_click), stationObj);
  
  gtk_widget_set_sensitive (stationObj->entStaCode, FALSE);
  gtk_widget_set_sensitive (stationObj->entStaName, FALSE);
  gtk_widget_set_sensitive (stationObj->btnSave, FALSE);
  gtk_widget_set_sensitive (stationObj->btnDelete, FALSE);

  gtk_widget_grab_focus (stationObj->btnNew);
  //gtk_widget_show_all (window);
  
  g_object_unref (G_OBJECT(stationObj->builder));
  
  return (stationObj->window);
}
