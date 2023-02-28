#include <gtk/gtk.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "route.h"
#include "station.h"
//#include "route_struct.h"

GtkApplication *app;
gboolean is_running_route = FALSE;
gboolean is_running_station = FALSE;
GtkWidget *vUserBox;
GtkWidget *vUserInfo;
GtkWidget *icon_view;

static void on_icon_view_item_activated(GtkIconView *icon_view, GtkTreePath *path, gpointer user_data) {
    GtkTreeModel *model;
    GtkTreeIter iter;

    //RouteWidgets *mobj = (RouteWidgets*) user_data; // pointer to struct
    //gboolean *is_running_ptr = (gboolean *) user_data;
    //*is_running_ptr = FALSE;
    
    // Get the model and iter of the selected item
    model = gtk_icon_view_get_model(icon_view);
    gtk_tree_model_get_iter(model, &iter, path);

    // Get the text of the selected item
    gchar *text;
    gtk_tree_model_get(model, &iter, 1, &text, -1);

    if (strcmp(text, "เส้นทาง") == 0) {
      if (!is_running_route) {
        GtkWidget *r = do_route (app, &is_running_route);
        gtk_widget_show_all (r);    
      }else{
        g_print ("โปรแกรมข้อมูลเส้นทางเดินรถถูกเปิดไว้แล้ว ไม่สามารถเรียกซ้ำได้\n");
      }
    } else if (strcmp(text, "สถานี") == 0) {
      if (!is_running_station){
        GtkWidget *s = do_station (app, &is_running_station);
        gtk_widget_show_all (s);
      }else {
        g_print ("โปรแกรมข้อมูลสถานีถูกเปิดไว้แล้ว ไม่สามารถเรียกซ้ำได้\n");
      }
    }
    g_free(text);
}

static void on_window_closed(GtkWidget *widget, gpointer user_data) {

}

static void btnLogin_click(GtkWidget *widget, gpointer user_data){
  gtk_widget_set_sensitive (icon_view, TRUE);
  gtk_widget_hide (vUserBox);
  GtkWidget *lblLoginName = gtk_label_new ("โยธิน อินบรรเลง");
  GtkWidget *btnLogout = gtk_button_new_with_label ("Logout");
  gtk_box_pack_start (GTK_BOX (vUserInfo), lblLoginName, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vUserInfo), btnLogout, FALSE, FALSE, 0);
  gtk_widget_show_all (vUserInfo);
}

static GtkWidget *do_menu (GtkApplication* app,
          gpointer        user_data) 
{

  //RouteWidgets *mobj = (RouteWidgets*) user_data; // pointer to struct
  //gboolean *is_running_ptr = (gboolean *) user_data;

  GtkWidget *window = NULL;
  window = gtk_application_window_new (app);
  // Create a new window
  //GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "TMS 1.0");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_default_size (GTK_WINDOW (window), 180, 480);

  // Create three pixbufs from image files
  GdkPixbuf *route_pixbuf = gdk_pixbuf_new_from_file("icons/route.png", NULL);
  GdkPixbuf *station_pixbuf = gdk_pixbuf_new_from_file("icons/station.png", NULL);
  GdkPixbuf *bus_pixbuf = gdk_pixbuf_new_from_file("icons/bus.png", NULL);
  GdkPixbuf *schedule_pixbuf = gdk_pixbuf_new_from_file("icons/schedule.png", NULL);
  GdkPixbuf *ticket_pixbuf = gdk_pixbuf_new_from_file("icons/ticket.png", NULL);
  GdkPixbuf *printer_pixbuf = gdk_pixbuf_new_from_file("icons/printer.png", NULL);

  // Create a tree store to hold the icon data
  GtkTreeStore *store = gtk_tree_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
  GtkTreeIter iter;

  // Add the Route icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, route_pixbuf, 1, "เส้นทาง", -1);

  // Add the Station icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, station_pixbuf, 1, "สถานี", -1);

  // Add the Schedule icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, bus_pixbuf, 1, "ประเภทรถ", -1);

  // Add the Schedule icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, schedule_pixbuf, 1, "ตารางเดินรถ", -1);

  // Add the Schedule icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, ticket_pixbuf, 1, "จำหน่ายตั๋ว", -1);

  // Add the Schedule icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, printer_pixbuf, 1, "รายงาน", -1);

  // Create a new icon view widget
  icon_view = gtk_icon_view_new();
  GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  vUserBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget *hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
  GtkWidget *lblUser = gtk_label_new ("รหัสผู้ใช้งาน");
  gtk_label_set_width_chars (GTK_LABEL (lblUser), 10);
  gtk_label_set_xalign (GTK_LABEL (lblUser), 0.0);
  GtkWidget *lblPass = gtk_label_new ("รหัสผ่าน");
  gtk_label_set_width_chars (GTK_LABEL (lblPass), 10);
  gtk_label_set_xalign (GTK_LABEL (lblPass), 0.0);
  GtkWidget *entUser = gtk_entry_new ();
  GtkWidget *entPass = gtk_entry_new ();
  GtkWidget *lblEmpty = gtk_label_new ("");
  gtk_label_set_width_chars (GTK_LABEL (lblEmpty), 10);
  GtkWidget *btnLogin = gtk_button_new_with_label ("Login");

  gtk_box_pack_start (GTK_BOX (hbox), lblUser, FALSE, FALSE, 1);
  gtk_box_pack_start (GTK_BOX (hbox), entUser, FALSE, FALSE, 1);
  gtk_box_pack_start (GTK_BOX (vUserBox), hbox, FALSE, FALSE, 1);
  
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_box_pack_start (GTK_BOX (hbox), lblPass, FALSE, FALSE, 1);
  gtk_box_pack_start (GTK_BOX (hbox), entPass, FALSE, FALSE, 1);
  gtk_box_pack_start (GTK_BOX (vUserBox), hbox, FALSE, FALSE, 1);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_box_pack_start (GTK_BOX (hbox), lblEmpty, FALSE, FALSE, 1);
  gtk_box_pack_start (GTK_BOX (hbox), btnLogin, FALSE, FALSE, 1);
  gtk_box_pack_start (GTK_BOX (vUserBox), hbox, FALSE, FALSE, 1);
  
  gtk_box_pack_start (GTK_BOX (vbox), vUserBox, FALSE, FALSE, 0);
  
  gtk_box_pack_start (GTK_BOX (vbox), icon_view, FALSE, FALSE, 0);
  
  vUserInfo = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
  gtk_box_pack_end (GTK_BOX (vbox), vUserInfo, FALSE, FALSE, 0);
    
  gtk_container_add (GTK_CONTAINER(window), vbox);

  g_signal_connect (btnLogin, "clicked", G_CALLBACK (btnLogin_click), NULL);

  // Set the icon view's model to the tree store
  gtk_icon_view_set_model(GTK_ICON_VIEW(icon_view), GTK_TREE_MODEL(store));
  g_object_unref(store);

  // Set the icon view's properties
  gtk_icon_view_set_text_column(GTK_ICON_VIEW(icon_view), 1);
  gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(icon_view), 0);

  gtk_widget_set_sensitive (icon_view, FALSE);

  // Connect the signals
  g_signal_connect(icon_view, "item-activated", G_CALLBACK(on_icon_view_item_activated), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), NULL);

  g_object_unref (route_pixbuf);
  g_object_unref (station_pixbuf);
  g_object_unref (bus_pixbuf);
  g_object_unref (schedule_pixbuf);
  g_object_unref (ticket_pixbuf);
  g_object_unref (printer_pixbuf);

  return (window);
}

static void
activate (GtkApplication* app, gpointer user_data){
  //RouteWidgets *mobj = (RouteWidgets*) user_data;
  //gboolean *is_running_ptr = (gboolean *) user_data;
  GtkWidget *main_window = do_menu (app, NULL);
  gtk_widget_show_all (main_window);
}

int
main (int argc, char **argv){
  int status;
  //RouteWidgets mobj;

  //mobj.is_running_route = FALSE;
  
  app = gtk_application_new ("com.pimpanya", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

