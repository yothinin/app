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
  MyObjects *mobj = (MyObjects*) userdata;
  g_print ("exit: %s\n", mobj->message);
  change_keyb ("us");
  gtk_widget_set_sensitive (mobj->btnExit, FALSE);
  //gtk_main_quit();
  *(mobj->is_running_ptr) = FALSE;
  gtk_widget_destroy (mobj->window);
}

GtkWidget *
do_station (GtkApplication *app, gpointer user_data){
  //MyObjects *mobj = (MyObjects*) user_data; // pointer to struct
  MyObjects *mobj = g_new(MyObjects, 1);

  gboolean *is_running_ptr = (gboolean *) user_data;
  *is_running_ptr = TRUE;
  
  mobj->is_running_ptr = is_running_ptr;
  mobj->message = "โปรแกรมข้อมูลสถานี";
  mobj->builder = gtk_builder_new_from_file("glade/tms_station.glade");
  //gtk_builder_connect_signals(builder, NULL);

  mobj->window = (GtkWidget*)gtk_builder_get_object(mobj->builder, "window");
  gtk_window_set_position (GTK_WINDOW(mobj->window), GTK_ALIGN_CENTER);

  mobj->edit = 0;
  g_print ("%s\n", mobj->message);

  mobj->treeview = (GtkWidget*) gtk_builder_get_object (mobj->builder, "treeView");
  mobj->entStaCode = (GtkWidget*) gtk_builder_get_object (mobj->builder, "entStaCode");
  mobj->entStaName = (GtkWidget*) gtk_builder_get_object (mobj->builder, "entStaName");
  mobj->btnSave = (GtkWidget*) gtk_builder_get_object (mobj->builder, "btnSave");
  mobj->btnDelete = (GtkWidget*) gtk_builder_get_object (mobj->builder, "btnDelete");
  mobj->btnExit = (GtkWidget*) gtk_builder_get_object (mobj->builder, "btnExit");
  mobj->btnNew = (GtkWidget*) gtk_builder_get_object (mobj->builder, "btnNew");
  mobj->liststore = (GtkListStore*) gtk_builder_get_object (mobj->builder, "mainStore");
  mobj->btnDemo = (GtkWidget*) gtk_builder_get_object (mobj->builder, "btnDemo");
  
  insertDataToListStore(mobj); // Insert data to GtkListStore at the first run.

  // don't use & before mobj.
  g_signal_connect (mobj->btnNew, "clicked", G_CALLBACK (stationNew_click), mobj);
  g_signal_connect (mobj->entStaCode, "focus-in-event", G_CALLBACK (entStaCode_focus), mobj);
  g_signal_connect (mobj->entStaName, "focus-in-event", G_CALLBACK (entStaName_focus), mobj);
  g_signal_connect (mobj->entStaCode, "key-release-event", G_CALLBACK (entStaCode_release), mobj);
  g_signal_connect (mobj->entStaName, "key-release-event", G_CALLBACK (entStaName_release), mobj);
  g_signal_connect (mobj->btnSave, "clicked", G_CALLBACK (stationSave_click), mobj);
  g_signal_connect (mobj->btnDelete, "clicked", G_CALLBACK (stationDelete_click), mobj);
  g_signal_connect (mobj->treeview, "cursor-changed", G_CALLBACK (station_row_change), mobj);
  g_signal_connect (mobj->btnExit, "clicked", G_CALLBACK (stationExit_click), mobj);
  g_signal_connect (mobj->window, "destroy", G_CALLBACK (stationExit_click), mobj);
  g_signal_connect (mobj->btnDemo, "clicked", G_CALLBACK (btnDemo_click), mobj);
  
  gtk_widget_set_sensitive (mobj->entStaCode, FALSE);
  gtk_widget_set_sensitive (mobj->entStaName, FALSE);
  gtk_widget_set_sensitive (mobj->btnSave, FALSE);
  gtk_widget_set_sensitive (mobj->btnDelete, FALSE);

  gtk_widget_grab_focus (mobj->btnNew);
  //gtk_widget_show_all (window);
  
  g_object_unref (G_OBJECT(mobj->builder));
  
  return (mobj->window);
}
