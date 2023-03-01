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
#include "bustype_mysql_fnct.h"
#include "bustype_widget_fnct.h"
#include "bustype_struct.h"

void busExit_click (GtkWidget *widget, gpointer userdata){
  BustypeWidgets *busObj = (BustypeWidgets*) userdata;
  g_print ("exit: %s\n", busObj->message);
  change_keyb ("us");
  gtk_widget_set_sensitive (busObj->btnExit, FALSE);
  //gtk_main_quit();
  *(busObj->is_running_ptr) = FALSE;
  gtk_widget_destroy (busObj->window);
}

GtkWidget *
do_bustype (GtkApplication *app, gpointer user_data){
  BustypeWidgets *busObj = g_new(BustypeWidgets, 1);

  gboolean *is_running_ptr = (gboolean *) user_data;
  *is_running_ptr = TRUE;
  
  busObj->is_running_ptr = is_running_ptr;
  busObj->message = "โปรแกรมข้อมูลสถานี";
  busObj->builder = gtk_builder_new_from_file("glade/tms_bus.glade");
  //gtk_builder_connect_signals(builder, NULL);

  busObj->window = (GtkWidget*)gtk_builder_get_object(busObj->builder, "window");
  gtk_window_set_position (GTK_WINDOW(busObj->window), GTK_ALIGN_CENTER);

  busObj->edit = 0;
  g_print ("%s\n", busObj->message);
  
  busObj->treeView = (GtkWidget*) gtk_builder_get_object (busObj->builder, "treeView");
  busObj->entBusCode = (GtkWidget*) gtk_builder_get_object (busObj->builder, "entBusCode");
  busObj->entBusName = (GtkWidget*) gtk_builder_get_object (busObj->builder, "entBusName");
  busObj->btnSave = (GtkWidget*) gtk_builder_get_object (busObj->builder, "btnSave");
  busObj->btnDelete = (GtkWidget*) gtk_builder_get_object (busObj->builder, "btnDelete");
  busObj->btnExit = (GtkWidget*) gtk_builder_get_object (busObj->builder, "btnExit");
  busObj->btnNew = (GtkWidget*) gtk_builder_get_object (busObj->builder, "btnNew");
  busObj->treeStore = (GtkTreeStore*) gtk_builder_get_object (busObj->builder, "treeStore"); 

  
  insertBustypeToListStore (busObj);
  
  //insertProvinceToListStore (busObj); // Insert data to GtkListStore at the first run.

  // don't use & before busObj.
  //g_signal_connect (busObj->btnNew, "clicked", G_CALLBACK (prvNew_click), busObj);
  //g_signal_connect (busObj->entPrvCode, "focus-in-event", G_CALLBACK (entPrvCode_focus), busObj);
  //g_signal_connect (busObj->entPrvName, "focus-in-event", G_CALLBACK (entPrvName_focus), busObj);
  //g_signal_connect (busObj->entPrvCode, "key-release-event", G_CALLBACK (entPrvCode_release), busObj);
  //g_signal_connect (busObj->entPrvName, "key-release-event", G_CALLBACK (entPrvName_release), busObj);
  //g_signal_connect (busObj->btnSave, "clicked", G_CALLBACK (prvSave_click), busObj);
  //g_signal_connect (busObj->btnDelete, "clicked", G_CALLBACK (prvDelete_click), busObj);
  //g_signal_connect (busObj->treeView, "cursor-changed", G_CALLBACK (prv_row_change), busObj);
  g_signal_connect (busObj->btnExit, "clicked", G_CALLBACK (busExit_click), busObj);
  g_signal_connect (busObj->window, "destroy", G_CALLBACK (busExit_click), busObj);
  
  gtk_widget_set_sensitive (busObj->entBusCode, FALSE);
  gtk_widget_set_sensitive (busObj->entBusName, FALSE);
  gtk_widget_set_sensitive (busObj->btnSave, FALSE);
  gtk_widget_set_sensitive (busObj->btnDelete, FALSE);

  gtk_widget_grab_focus (busObj->btnNew);
  
  g_object_unref (G_OBJECT(busObj->builder));
  
  return (busObj->window);
}
