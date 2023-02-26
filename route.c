#include <gtk/gtk.h>
#include "route.h"

static void on_window_closed(GtkWidget *widget, gpointer data) {
    gboolean *is_running_ptr = (gboolean *) data;
    *is_running_ptr = FALSE;
    //gtk_main_quit();
}

GtkWidget *
do_route (GtkApplication *app, gpointer user_data){
  GtkWidget *window = NULL;
  
  gboolean *is_running_ptr = (gboolean *) user_data;
  *is_running_ptr = TRUE;

  if (!window){
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "route");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), is_running_ptr);
  }

  return window;
}
