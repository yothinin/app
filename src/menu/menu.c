#include <gtk/gtk.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
//#include "route_struct.h"

GtkApplication *app;
GtkWidget *vUserBox;
GtkWidget *vUserInfo;
GtkWidget *icon_view;
gboolean is_running_route = FALSE;
gboolean is_running_station = FALSE;
gboolean is_running_province = FALSE;
gboolean is_running_bustype = FALSE;

static void btnLogout_click (GtkWidget *widget, gpointer user_data) {
  gtk_widget_set_sensitive (icon_view, FALSE);
  gtk_icon_view_unselect_all (GTK_ICON_VIEW (icon_view));

  GList *children, *iter;
  children = gtk_container_get_children(GTK_CONTAINER(vUserInfo));
  for (iter = children; iter != NULL; iter = g_list_next(iter)) {
    g_print ("Widget name: %s\n", gtk_widget_get_name (GTK_WIDGET (iter->data)));
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  }
  g_list_free(children);

  // hide vUserInfo box
  gtk_widget_hide (GTK_WIDGET (vUserInfo));

  gtk_widget_show_all (vUserBox);
}

static void btnLogin_click(GtkWidget *widget, gpointer user_data){
  gtk_widget_set_sensitive (icon_view, TRUE);
  gtk_widget_hide (vUserBox);

  GtkWidget *lblLoginName = gtk_label_new ("โยธิน อินบรรเลง");
  GtkWidget *btnLogout = gtk_button_new_with_label ("Logout");
  gtk_button_set_relief(GTK_BUTTON(btnLogout), GTK_RELIEF_NONE); // remove button border

  gtk_widget_set_name (lblLoginName, "my-label");
  gtk_widget_set_name (btnLogout, "my-button");

  GtkCssProvider *provider = gtk_css_provider_new();

  char *style_path = g_build_filename (g_path_get_dirname (__FILE__), "..", "..", "css", "style.css", NULL);

  gtk_css_provider_load_from_path(provider, style_path, NULL);
  //gtk_css_provider_load_from_data(provider, ".my-class { background-color: #00ff00; }", -1, NULL);

  GtkStyleContext *context = gtk_widget_get_style_context(btnLogout);
  gtk_style_context_add_class (context, "my-class");
  gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  g_object_unref(provider);

  g_signal_connect (btnLogout, "clicked", G_CALLBACK (btnLogout_click), NULL);
  gtk_box_pack_start (GTK_BOX (vUserInfo), lblLoginName, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (vUserInfo), btnLogout, FALSE, FALSE, 2);

  g_free (style_path);
  gtk_widget_show_all (vUserInfo);
}

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

    GtkWidget *running = NULL;
    if (strcmp (text, "จังหวัด") == 0){
      if (!is_running_province)
        running = do_province (app, &is_running_province);
      else
        g_print ("โปรแกรมข้อมูลจังหวัดถูกเปิดไว้แล้ว ไม่สามารถเรียกซ้ำได้\n");
    }

    if (strcmp (text, "ประเภทรถ") == 0) {
      if (!is_running_bustype)
        running = do_bustype (app, &is_running_bustype);
      else
        g_print ("โปรแกรมข้อมูลประเภทรถถูกเปิดไว้แล้ว ไม่สามารถเรียกซ้ำได้\n");

    }

    if (strcmp (text, "เส้นทาง") == 0) {
      if (!is_running_route) {
        running = do_route (app, &is_running_route);
      }else{
        g_print ("โปรแกรมข้อมูลเส้นทางเดินรถถูกเปิดไว้แล้ว ไม่สามารถเรียกซ้ำได้\n");
      }
    } else if (strcmp(text, "สถานี") == 0) {
      if (!is_running_station){
        running = do_station (app, &is_running_station);
      }else {
        g_print ("โปรแกรมข้อมูลสถานีถูกเปิดไว้แล้ว ไม่สามารถเรียกซ้ำได้\n");
      }
    }
    if (running != NULL)
      gtk_widget_show_all (running);

    g_free(text);
}

static void on_window_closed(GtkWidget *widget, gpointer user_data) {

}

static GtkWidget *do_menu (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window = NULL;
  window = gtk_application_window_new (app);

  GError *error = NULL;
  char *transport_path = g_build_filename (g_path_get_dirname (__FILE__), "..", "..", "icons", "transport.png", NULL);
  GdkPixbuf *icon = gdk_pixbuf_new_from_file(transport_path, &error);
  g_free (transport_path);

  if (error == NULL){
    gtk_window_set_icon (GTK_WINDOW (window), icon);
    g_object_unref (icon);
  }else {
    g_warning("Unable to load icon: %s", error->message);
    g_error_free(error);
  }

  // Create a new window
  //GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "TMS 1.0");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_default_size (GTK_WINDOW (window), 460, 640);
  GdkGeometry hints = { .min_width = 460, .min_height = 640 };
  gtk_window_set_geometry_hints(GTK_WINDOW(window), NULL, &hints, GDK_HINT_MIN_SIZE);


  // Create three pixbufs from image files
  //GdkPixbuf *province_pixbuf = gdk_pixbuf_new_from_file("../../icons/thailand.ico", NULL);

  char *province_path, *employee_path, *user_path, *route_path, *station_path, *bustype_path, *schedule_path, *ticket_path, *printer_path, *tool_path;
  province_path = g_build_filename(g_path_get_dirname(__FILE__), "..", "..", "icons", "thailand.ico", NULL);
  employee_path = g_build_filename(g_path_get_dirname(__FILE__), "..", "..", "icons", "employee.png", NULL);
  user_path = g_build_filename(g_path_get_dirname(__FILE__), "..", "..", "icons", "user.png", NULL);
  route_path = g_build_filename(g_path_get_dirname(__FILE__), "..", "..", "icons", "motorway.png", NULL);
  station_path = g_build_filename(g_path_get_dirname(__FILE__), "..", "..", "icons", "station.png", NULL);
  bustype_path = g_build_filename(g_path_get_dirname(__FILE__), "..", "..", "icons", "bus.png", NULL);
  schedule_path = g_build_filename(g_path_get_dirname(__FILE__), "..", "..", "icons", "schedule.png", NULL);
  ticket_path = g_build_filename(g_path_get_dirname(__FILE__), "..", "..", "icons", "ticket.png", NULL);
  printer_path = g_build_filename(g_path_get_dirname(__FILE__), "..", "..", "icons", "printer.png", NULL);
  tool_path = g_build_filename(g_path_get_dirname(__FILE__), "..", "..", "icons", "tool.png", NULL);

  GdkPixbuf *province_pixbuf = gdk_pixbuf_new_from_file(province_path, NULL);
  GdkPixbuf *employee_pixbuf = gdk_pixbuf_new_from_file(employee_path, NULL);
  GdkPixbuf *user_pixbuf = gdk_pixbuf_new_from_file(user_path, NULL);
  GdkPixbuf *route_pixbuf = gdk_pixbuf_new_from_file(route_path, NULL);
  GdkPixbuf *station_pixbuf = gdk_pixbuf_new_from_file(station_path, NULL);
  GdkPixbuf *bus_pixbuf = gdk_pixbuf_new_from_file(bustype_path, NULL);
  GdkPixbuf *schedule_pixbuf = gdk_pixbuf_new_from_file(schedule_path, NULL);
  GdkPixbuf *ticket_pixbuf = gdk_pixbuf_new_from_file(ticket_path, NULL);
  GdkPixbuf *printer_pixbuf = gdk_pixbuf_new_from_file(printer_path, NULL);
  GdkPixbuf *tool_pixbuf = gdk_pixbuf_new_from_file(tool_path, NULL);

  g_free (province_path);
  g_free (employee_path);
  g_free (user_path);
  g_free (station_path);
  g_free (bustype_path);
  g_free (schedule_path);
  g_free (ticket_path);
  g_free (printer_path);
  g_free (tool_path);

  // Create a tree store to hold the icon data
  GtkTreeStore *store = gtk_tree_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
  GtkTreeIter iter;

  // Add the Province icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, province_pixbuf, 1, "จังหวัด", -1);

  // Add the Employee icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, employee_pixbuf, 1, "พนักงาน", -1);

  // Add the User icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, user_pixbuf, 1, "ผู้ใช้งาน", -1);
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

  // Add the Tool icon
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, tool_pixbuf, 1, "เส้นทาง", -1);

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

  GtkWidget *scrolled = gtk_scrolled_window_new (NULL, NULL);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled, TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(scrolled), icon_view);

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
  GtkWidget *main_window = do_menu (app, NULL);
  gtk_widget_show_all (main_window);
}

int
main (int argc, char **argv){
  int status;
  const gchar *home_dir = g_get_home_dir();
  gchar *app_dir = g_build_filename (home_dir, "projects", "app", NULL);

  if (chdir (app_dir) != 0) {
    perror ("chdir() error");
    g_free (app_dir);
  }else{
    g_print ("Change dir to: %s\n", app_dir);
  }
  g_free (app_dir);

  app = gtk_application_new ("com.pimpanya", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

