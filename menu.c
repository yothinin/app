#include <gtk/gtk.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "province.h"
#include "route.h"
#include "station.h"
//#include "route_struct.h"

GtkApplication *app;
gboolean is_running_route = FALSE;
gboolean is_running_station = FALSE;
gboolean is_running_province = FALSE;

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
        GtkWidget *route = do_route (app, &is_running_route);
        gtk_widget_show_all (route);    
      }else{
        g_print ("โปรแกรมข้อมูลเส้นทางเดินรถถูกเปิดไว้แล้ว ไม่สามารถเรียกซ้ำได้\n");
      }
    }else if (strcmp(text, "สถานี") == 0) {
      if (!is_running_station){
        GtkWidget *station = do_station (app, &is_running_station);
        gtk_widget_show_all (station);
      }else {
        g_print ("โปรแกรมข้อมูลสถานีถูกเปิดไว้แล้ว ไม่สามารถเรียกซ้ำได้\n");
      }
    }else if (strcmp (text, "จังหวัด") == 0) {
      if (!is_running_province) {
        GtkWidget *prv = do_province (app, &is_running_province);
        gtk_widget_show_all (prv);
      }else {
        g_print ("โปรแกรมข้อมูลจังหวัดถูกเปิดไว้แล้ว ไม่สามารถเรียกซ้ำได้\n");
      }
    }
    g_free(text);
}

static void on_window_closed(GtkWidget *widget, gpointer user_data) {

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
  gtk_window_set_title(GTK_WINDOW(window), "ระบบงานจำหน่ายตั๋วโดยสาร");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_default_size (GTK_WINDOW (window), 480, 480);

  // Create three pixbufs from image files
  GdkPixbuf *province_pixbuf = gdk_pixbuf_new_from_file ("icons/province.png", NULL);
  GdkPixbuf *employee_pixbuf = gdk_pixbuf_new_from_file ("icons/employee.png", NULL);
  GdkPixbuf *user_pixbuf = gdk_pixbuf_new_from_file ("icons/user.png", NULL);
  GdkPixbuf *route_pixbuf = gdk_pixbuf_new_from_file ("icons/route.png", NULL);
  GdkPixbuf *station_pixbuf = gdk_pixbuf_new_from_file ("icons/station.png", NULL);
  GdkPixbuf *bus_pixbuf = gdk_pixbuf_new_from_file ("icons/bus.png", NULL);
  GdkPixbuf *schedule_pixbuf = gdk_pixbuf_new_from_file ("icons/schedule.png", NULL);
  GdkPixbuf *ticket_pixbuf = gdk_pixbuf_new_from_file ("icons/ticket.png", NULL);
  GdkPixbuf *printer_pixbuf = gdk_pixbuf_new_from_file ("icons/printer.png", NULL);
  GdkPixbuf *tool_pixbuf = gdk_pixbuf_new_from_file ("icons/tool.png", NULL);

  // Create a tree store to hold the icon data
  GtkTreeStore *store = gtk_tree_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
  GtkTreeIter iter;

  // Add the Province icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, province_pixbuf, 1, "จังหวัด", -1);
  
  // Add the Employee icon
  gtk_tree_store_append (store, &iter, NULL);
  gtk_tree_store_set (store, &iter, 0, employee_pixbuf, 1, "พนักงาน", -1);
  
  // Add the User Login icon
  gtk_tree_store_append (store, &iter, NULL);
  gtk_tree_store_set (store, &iter, 0, user_pixbuf, 1, "ผู้ใช้งาน", -1);

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
  
  // Add the Config icon
  gtk_tree_store_append (store, &iter, NULL);
  gtk_tree_store_set (store, &iter, 0, tool_pixbuf, 1, "จัดการ", -1);

  // Create a new icon view widget
  GtkWidget *icon_view = gtk_icon_view_new();
  gtk_container_add(GTK_CONTAINER(window), icon_view);

  // Set the icon view's model to the tree store
  gtk_icon_view_set_model(GTK_ICON_VIEW(icon_view), GTK_TREE_MODEL(store));
  g_object_unref(store);

  // Set the icon view's properties
  gtk_icon_view_set_text_column(GTK_ICON_VIEW(icon_view), 1);
  gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(icon_view), 0);

  // Connect the signals
  g_signal_connect(icon_view, "item-activated", G_CALLBACK(on_icon_view_item_activated), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), NULL);

  g_object_unref (province_pixbuf);
  g_object_unref (employee_pixbuf);
  g_object_unref (user_pixbuf);
  g_object_unref (route_pixbuf);
  g_object_unref (station_pixbuf);
  g_object_unref (bus_pixbuf);
  g_object_unref (schedule_pixbuf);
  g_object_unref (ticket_pixbuf);
  g_object_unref (printer_pixbuf);
  g_object_unref (tool_pixbuf);

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
  
  app = gtk_application_new ("com.pimpanya", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

