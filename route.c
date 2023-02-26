#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <glib/gprintf.h>
#include "functions.h"
#include "route_struct.h"
#include "route_widget_fnct.h"

gboolean on_main_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer userdata) {
  RouteWidgets *mobj = (RouteWidgets*) userdata;
  GtkWidget *dialog;
  gint result;

  dialog = gtk_message_dialog_new_with_markup(GTK_WINDOW(mobj->window),
                                              GTK_DIALOG_MODAL,
                                              GTK_MESSAGE_QUESTION,
                                              GTK_BUTTONS_NONE,
                                              "<span size='large' weight='bold'>กรุณายืนยันออกจากโปรแกรม</span>");

  gtk_dialog_add_buttons(GTK_DIALOG(dialog),
                         "Yes",
                         GTK_RESPONSE_YES,
                         "No",
                         GTK_RESPONSE_NO,
                         NULL);

  result = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  if (result == GTK_RESPONSE_YES) {
    g_print("Main window close button clicked\nExit: %s\n", mobj->message);
    mobj->is_running_route = FALSE;
    //gtk_main_quit();
    return FALSE;
  } else {
    return TRUE;
  }
}

void btnExit_click(GtkWidget *widget, gpointer userdata) {
  RouteWidgets *mobj = (RouteWidgets*) userdata;
  GtkWidget *dialog;
  gint result;

  dialog = gtk_message_dialog_new(GTK_WINDOW(mobj->window),
                                  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_QUESTION,
                                  GTK_BUTTONS_YES_NO,
                                  "กรุณายืนยันออกจากโปรแกรม");

  result = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  if (result == GTK_RESPONSE_YES) {
    g_print("btnExit.Clicked\nExit: %s\n", mobj->message);
    mobj->is_running_route = FALSE;
    gtk_widget_destroy(mobj->window);
  }
  // If the user clicked "No" or closed the dialog box, do nothing and return to the program.
}

GtkWidget *
do_route (GtkApplication *app, gpointer user_data){
    
  RouteWidgets *mobj = (RouteWidgets*) user_data; // pointer to struct
  mobj->is_running_route = TRUE;
  mobj->message = "โปรแกรมเส้นทางเดินรถ";
  mobj->builder = gtk_builder_new_from_file("glade/tms_route.glade");
  
  mobj->window = (GtkWidget*)gtk_builder_get_object(mobj->builder, "window");
  gtk_window_set_position (GTK_WINDOW(mobj->window), GTK_ALIGN_CENTER);
  
  mobj->edit = 0;
  g_print ("%s\n", mobj->message);
  
  mobj->treeview = (GtkWidget*) gtk_builder_get_object (mobj->builder, "treeviewRoute");
  mobj->lblThaiDate = (GtkWidget*) gtk_builder_get_object (mobj->builder, "lblThaiDate");
  mobj->entRoute = (GtkWidget*) gtk_builder_get_object (mobj->builder, "entRoute");
  mobj->cmbDirection = (GtkWidget*) gtk_builder_get_object (mobj->builder, "cmbDirection");
  mobj->cmbFrom = (GtkWidget*) gtk_builder_get_object (mobj->builder, "cmbFrom");
  mobj->cmbDest = (GtkWidget*) gtk_builder_get_object (mobj->builder, "cmbDest");
  mobj->btnNew = (GtkWidget*) gtk_builder_get_object (mobj->builder, "btnNew");
  mobj->btnSave = (GtkWidget*) gtk_builder_get_object (mobj->builder, "btnSave");
  mobj->btnExit = (GtkWidget*) gtk_builder_get_object (mobj->builder, "btnExit");
  mobj->btnDelete = (GtkWidget*) gtk_builder_get_object (mobj->builder, "btnDelete");
  mobj->treeListStore = (GtkListStore*) gtk_builder_get_object (mobj->builder, "liststore1");
  
  insertDataToTreeListStore (mobj);
  insertDataToCmbListStore (mobj);
  btnNew_click (NULL, mobj);

  g_print ("Test function...\n");
  long double price = 2147483647.99;
  char *thaitext = thai_baht_conversion(price);
  printf("%.2Lf is %s\n", price, thaitext);
  free(thaitext);
  
  time_t raw_time;
  char *formatted_date;
  char lang[] = "th";
  char type[] = "1";
  time(&raw_time);
  formatted_date = dateFormat(lang, raw_time, type);
  printf("Formatted date: %s\n\n", formatted_date);
  gtk_label_set_text (GTK_LABEL (mobj->lblThaiDate), formatted_date);
  free(formatted_date);

  g_signal_connect (mobj->treeview, "cursor-changed", G_CALLBACK(row_change), mobj);
  g_signal_connect (mobj->entRoute, "focus-in-event", G_CALLBACK (entRoute_focus), mobj);
  g_signal_connect (mobj->entRoute, "key-release-event", G_CALLBACK (entRoute_release), mobj);
  g_signal_connect (mobj->cmbDirection, "changed", G_CALLBACK (cmbDirection_change), mobj);
  g_signal_connect (mobj->cmbDirection, "key-press-event", G_CALLBACK(cmbDirection_key_press), mobj);
  g_signal_connect (mobj->cmbFrom, "changed", G_CALLBACK (cmbFrom_change), mobj);
  g_signal_connect (mobj->cmbFrom, "key-press-event", G_CALLBACK(cmbFrom_key_press), mobj);
  g_signal_connect (mobj->cmbDest, "changed", G_CALLBACK (cmbDest_change), mobj);
  g_signal_connect (mobj->cmbDest, "key-press-event", G_CALLBACK(cmbDest_key_press), mobj);
  g_signal_connect (mobj->btnNew, "clicked", G_CALLBACK (btnNew_click), mobj);
  g_signal_connect (mobj->btnSave, "clicked", G_CALLBACK (btnSave_click), mobj);
  g_signal_connect (mobj->btnDelete, "clicked", G_CALLBACK (btnDelete_click), mobj);
  g_signal_connect (mobj->btnExit, "clicked", G_CALLBACK (btnExit_click), mobj);
  g_signal_connect (mobj->window, "delete-event", G_CALLBACK(on_main_window_delete_event), mobj);

  gtk_widget_grab_focus (mobj->btnNew);
  //gtk_widget_show_all (mobj->window);
  
  //g_object_unref (G_OBJECT(mobj->builder));

  return (mobj->window);
}
