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
#include "province_mysql_fnct.h"
#include "province_widget_fnct.h"
#include "province_struct.h"

void prvExit_click (GtkWidget *widget, gpointer userdata){
  ProvinceWidgets *prvObj = (ProvinceWidgets*) userdata;
  g_print ("exit: %s\n", prvObj->message);
  change_keyb ("us");
  gtk_widget_set_sensitive (prvObj->btnExit, FALSE);
  //gtk_main_quit();
  *(prvObj->is_running_ptr) = FALSE;
  gtk_widget_destroy (prvObj->window);
}

GtkWidget *
do_province (GtkApplication *app, gpointer user_data){
  ProvinceWidgets *prvObj = g_new(ProvinceWidgets, 1);

  gboolean *is_running_ptr = (gboolean *) user_data;
  *is_running_ptr = TRUE;
  
  prvObj->is_running_ptr = is_running_ptr;
  prvObj->message = "โปรแกรมข้อมูลสถานี";
  prvObj->builder = gtk_builder_new_from_file("glade/tms_province.glade");
  //gtk_builder_connect_signals(builder, NULL);

  prvObj->window = (GtkWidget*)gtk_builder_get_object(prvObj->builder, "window");
  gtk_window_set_position (GTK_WINDOW(prvObj->window), GTK_ALIGN_CENTER);

  prvObj->edit = 0;
  g_print ("%s\n", prvObj->message);

  prvObj->treeView = (GtkWidget*) gtk_builder_get_object (prvObj->builder, "treeView");
  prvObj->entPrvCode = (GtkWidget*) gtk_builder_get_object (prvObj->builder, "entPrvCode");
  prvObj->entPrvName = (GtkWidget*) gtk_builder_get_object (prvObj->builder, "entPrvName");
  prvObj->btnSave = (GtkWidget*) gtk_builder_get_object (prvObj->builder, "btnSave");
  prvObj->btnDelete = (GtkWidget*) gtk_builder_get_object (prvObj->builder, "btnDelete");
  prvObj->btnExit = (GtkWidget*) gtk_builder_get_object (prvObj->builder, "btnExit");
  prvObj->btnNew = (GtkWidget*) gtk_builder_get_object (prvObj->builder, "btnNew");
  prvObj->treeStore = (GtkTreeStore*) gtk_builder_get_object (prvObj->builder, "treeStore");
  
  insertProvinceToListStore (prvObj);
  //insertProvinceToListStore (prvObj); // Insert data to GtkListStore at the first run.

  // don't use & before prvObj.
  //g_signal_connect (prvObj->btnNew, "clicked", G_CALLBACK (stationNew_click), prvObj);
  //g_signal_connect (prvObj->entStaCode, "focus-in-event", G_CALLBACK (entStaCode_focus), prvObj);
  //g_signal_connect (prvObj->entStaName, "focus-in-event", G_CALLBACK (entStaName_focus), prvObj);
  //g_signal_connect (prvObj->entStaCode, "key-release-event", G_CALLBACK (entStaCode_release), prvObj);
  //g_signal_connect (prvObj->entStaName, "key-release-event", G_CALLBACK (entStaName_release), prvObj);
  //g_signal_connect (prvObj->btnSave, "clicked", G_CALLBACK (stationSave_click), prvObj);
  //g_signal_connect (prvObj->btnDelete, "clicked", G_CALLBACK (stationDelete_click), prvObj);
  //g_signal_connect (prvObj->treeview, "cursor-changed", G_CALLBACK (station_row_change), prvObj);
  g_signal_connect (prvObj->btnExit, "clicked", G_CALLBACK (prvExit_click), prvObj);
  g_signal_connect (prvObj->window, "destroy", G_CALLBACK (prvExit_click), prvObj);
  
  gtk_widget_set_sensitive (prvObj->entPrvCode, FALSE);
  gtk_widget_set_sensitive (prvObj->entPrvName, FALSE);
  gtk_widget_set_sensitive (prvObj->btnSave, FALSE);
  gtk_widget_set_sensitive (prvObj->btnDelete, FALSE);

  gtk_widget_grab_focus (prvObj->btnNew);
  
  g_object_unref (G_OBJECT(prvObj->builder));
  
  return (prvObj->window);
}
