/*
 * station_widget_funct.c
 * purpose to store all functions about the widgets in specified module
 * name: [module]_widget_fnct.c
 */


#ifndef GTK_GTK_H
#define GTK_GTK_H
#include <gtk/gtk.h>
#endif //GTK_GTK_H
#include "functions.h"
#include "route_struct.h"
#include "station_struct.h"
#include "route_mysql_fnct.h"
#include "route_widget_fnct.h"

/*
 * to declare function name use these form:
 * 1. [widget]_[behavior] for instance, btnSave_click
 * 2. use present simple tense only
 * 3. behavior use lowercase
 * 4. Widget name use camel case lower for abbreve widget and capital for real name of widget
 * for instance btnSave_click, btn is abbreve of GtkButton and Save is name of button and 
 * click is the behavior for the button
 */
 
 void enableWidgets(RouteWidgets *routeObj, gboolean enable) {
    gtk_widget_set_sensitive(GTK_WIDGET(routeObj->entRoute), enable);
    gtk_widget_set_sensitive(GTK_WIDGET(routeObj->cmbDirection), enable);
    gtk_widget_set_sensitive(GTK_WIDGET(routeObj->cmbFrom), enable);
    gtk_widget_set_sensitive(GTK_WIDGET(routeObj->cmbDest), enable);
    gtk_widget_set_sensitive(GTK_WIDGET(routeObj->btnSave), enable);
    gtk_widget_set_sensitive(GTK_WIDGET(routeObj->btnDelete), enable);
}

void clearRouteInput(RouteWidgets *routeWidgets) {
  gtk_entry_set_text(GTK_ENTRY(routeWidgets->entRoute), "");
  gtk_combo_box_set_active(GTK_COMBO_BOX(routeWidgets->cmbDirection), 0);
  gtk_combo_box_set_active(GTK_COMBO_BOX(routeWidgets->cmbFrom), 0);
  gtk_combo_box_set_active(GTK_COMBO_BOX(routeWidgets->cmbDest), 0);
  routeWidgets->edit = 0;
  gtk_button_set_label (GTK_BUTTON (routeWidgets->btnSave), "บันทึก");
}

gboolean validateRouteInput(RouteWidgets *routeWidgets) {
  const gchar *routeText = gtk_entry_get_text(GTK_ENTRY(routeWidgets->entRoute));
  gint directionIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(routeWidgets->cmbDirection));
  gint fromIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(routeWidgets->cmbFrom));
  gint destIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(routeWidgets->cmbDest));

  if (g_strcmp0(routeText, "") == 0 || directionIndex == 0 || fromIndex == 0 || destIndex == 0) {
    if (g_strcmp0(routeText, "") == 0) {
      gtk_widget_grab_focus(routeWidgets->entRoute);
    } else if (directionIndex == 0) {
      gtk_widget_grab_focus(routeWidgets->cmbDirection);
    } else if (fromIndex == 0) {
      gtk_widget_grab_focus(routeWidgets->cmbFrom);
    } else if (destIndex == 0) {
      gtk_widget_grab_focus(routeWidgets->cmbDest);
    }
    gtk_label_set_text(GTK_LABEL(routeWidgets->message), "Please fill all required fields.");
    return FALSE;
  }

  return TRUE;
}

void set_buttons_sensitive(GtkComboBox *cmb_from, GtkComboBox *cmb_dest, GtkButton *btn_save, GtkButton *btn_delete, gint first_item_index) {
  if (gtk_combo_box_get_active(cmb_from) != first_item_index && gtk_combo_box_get_active(cmb_dest) != first_item_index) {
    // Do something if active is not the first item
    gtk_widget_set_sensitive(GTK_WIDGET(btn_save), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(btn_delete), TRUE);
  } else {
    // Do something if active is the first item
    gtk_widget_set_sensitive(GTK_WIDGET(btn_save), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(btn_delete), FALSE);
  }
}

void btnNew_click (GtkWidget *widget, gpointer user_data){
  RouteWidgets *routeObj = (RouteWidgets*) user_data;

  routeObj->treeSelected = gtk_tree_view_get_selection (GTK_TREE_VIEW(routeObj->treeview));
  gtk_tree_selection_unselect_all (GTK_TREE_SELECTION(routeObj->treeSelected));

  gtk_entry_set_text (GTK_ENTRY(routeObj->entRoute), "");
  gtk_combo_box_set_active (GTK_COMBO_BOX(routeObj->cmbDirection), 0);
  gtk_combo_box_set_active (GTK_COMBO_BOX(routeObj->cmbFrom), 0);
  gtk_combo_box_set_active (GTK_COMBO_BOX(routeObj->cmbDest), 0);
  
  gtk_widget_set_sensitive (routeObj->entRoute, TRUE);
  gtk_widget_set_sensitive (routeObj->cmbDirection, FALSE);
  gtk_widget_set_sensitive (routeObj->cmbFrom, FALSE);
  gtk_widget_set_sensitive (routeObj->cmbDest, FALSE);
  gtk_widget_set_sensitive (routeObj->btnNew, TRUE);
  gtk_widget_set_sensitive (routeObj->btnSave, FALSE);
  gtk_widget_set_sensitive (routeObj->btnExit, TRUE);
  gtk_widget_set_sensitive (routeObj->btnDelete, FALSE);  
  routeObj->edit = 0; // New, save
  
  gtk_widget_grab_focus (routeObj->entRoute);
}

void entRoute_focus (GtkWidget *widget, gpointer user_data){
  change_keyb ("us");
}

gboolean entRoute_release (GtkWidget *widget, GdkEventKey *event, gpointer user_data){
  RouteWidgets *routeObj = (RouteWidgets*) user_data;
  const gchar *strRoute = gtk_entry_get_text (GTK_ENTRY(routeObj->entRoute));
  if (strcmp(gdk_keyval_name(event->keyval), "Return") == 0  ||
      strcmp(gdk_keyval_name(event->keyval), "KP_Enter") == 0){

    gtk_combo_box_set_active (GTK_COMBO_BOX (routeObj->cmbDirection), 0);
    gtk_combo_box_set_active (GTK_COMBO_BOX (routeObj->cmbFrom), 0);
    gtk_combo_box_set_active (GTK_COMBO_BOX (routeObj->cmbDest), 0);
    gtk_widget_grab_focus (routeObj->cmbDirection);
    
    return TRUE;
  }else {
    if (strlen(strRoute) > 0)
      gtk_widget_set_sensitive (routeObj->cmbDirection, TRUE);
    else
      gtk_widget_set_sensitive (routeObj->cmbDirection, FALSE);    
  }

  return FALSE;
}

void cmbDirection_change (GtkComboBox *combo_box, gpointer user_data) {
  RouteWidgets *routeObj = (RouteWidgets*) user_data;

  GtkTreeModel *directModel = gtk_combo_box_get_model (GTK_COMBO_BOX (routeObj->cmbDirection));
  GtkTreeIter directIter;
  gtk_combo_box_get_active_iter (GTK_COMBO_BOX (routeObj->cmbDirection), &directIter);
  gchar *directVal;
  gtk_tree_model_get(directModel, &directIter, 0, &directVal, -1);

  gchar *rouCode = (gchar *)gtk_entry_get_text(GTK_ENTRY(routeObj->entRoute));
  Route route = {rouCode, NULL, NULL, directVal, NULL, NULL};

  route = getRouteByCode (route);
  if (route.rouNameFrom != NULL){
    g_print ("route: %s, %s, %s, direct: %s, %s, %s\n", route.rouCode,
                                                        route.rouNameFrom,
                                                        route.rouNameTo,
                                                        route.rouDirection,
                                                        route.staFrom,
                                                        route.staTo);
    GtkTreeIter from_iter, dest_iter;
    searchCombo(GTK_COMBO_BOX (routeObj->cmbFrom), route.staFrom, &from_iter);
    searchCombo(GTK_COMBO_BOX (routeObj->cmbDest), route.staTo, &dest_iter);
  
    gtk_combo_box_set_active_iter(GTK_COMBO_BOX (routeObj->cmbFrom), &from_iter);
    gtk_combo_box_set_active_iter(GTK_COMBO_BOX (routeObj->cmbDest), &dest_iter);
    routeObj->edit = 1; // Edit
    gtk_button_set_label (GTK_BUTTON (routeObj->btnSave), "แก้ไข");
    
    set_buttons_sensitive(GTK_COMBO_BOX(routeObj->cmbFrom), 
                          GTK_COMBO_BOX(routeObj->cmbDest), 
                          GTK_BUTTON (routeObj->btnSave), 
                          GTK_BUTTON (routeObj->btnDelete), 
                          0);

  } else {
    g_print ("Data not found!, %s - %s\n", route.rouCode, route.rouDirection);
    routeObj->edit = 0; // New
    gtk_button_set_label (GTK_BUTTON (routeObj->btnSave), "บันทึก");
    gtk_combo_box_set_active (GTK_COMBO_BOX (routeObj->cmbFrom), 0);
    gtk_combo_box_set_active (GTK_COMBO_BOX (routeObj->cmbDest), 0);
    gtk_widget_set_sensitive (routeObj->btnSave, FALSE);
    gtk_widget_set_sensitive (routeObj->btnDelete, FALSE);

  }
  
  gtk_widget_set_sensitive (routeObj->cmbFrom, TRUE);
  gtk_widget_set_sensitive (routeObj->cmbDest, TRUE);
    
  g_free (route.rouNameFrom);
  g_free (route.rouNameTo);
  g_free (route.staFrom);
  g_free (route.staTo);
  
  //g_free (rouCode); //rouCode isn't allocate, should not be freed by g_free.
  g_free (directVal);
}

gboolean cmbDirection_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    RouteWidgets *routeObj = (RouteWidgets*) user_data;
    if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter) {
        gtk_widget_grab_focus (routeObj->cmbFrom);
        // Return TRUE to indicate that the event was handled
        return TRUE;
    }
    // Return FALSE to indicate that the event was not handled
    return FALSE;
}

gboolean cmbFrom_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    RouteWidgets *routeObj = (RouteWidgets*) user_data;
    if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter) {
        gtk_widget_grab_focus (routeObj->cmbDest);
        // Return TRUE to indicate that the event was handled
        return TRUE;
    }
    // Return FALSE to indicate that the event was not handled
    return FALSE;
}

gboolean cmbDest_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    RouteWidgets *routeObj = (RouteWidgets*) user_data;
    if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter) {
        gtk_widget_grab_focus (routeObj->btnSave);
        return TRUE;
    }
    return FALSE;
}

void cmbFrom_change(GtkComboBox *cmbFrom, gpointer user_data) {
  RouteWidgets *routeObj = (RouteWidgets *) user_data;
  gint first_item_index = 0;
  set_buttons_sensitive(cmbFrom, GTK_COMBO_BOX(routeObj->cmbDest), GTK_BUTTON(routeObj->btnSave), GTK_BUTTON(routeObj->btnDelete), first_item_index);
}

void cmbDest_change(GtkComboBox *cmbDest, gpointer user_data) {
  RouteWidgets *routeObj = (RouteWidgets *) user_data;
  gint first_item_index = 0;
  set_buttons_sensitive(GTK_COMBO_BOX(routeObj->cmbFrom), cmbDest, GTK_BUTTON(routeObj->btnSave), GTK_BUTTON(routeObj->btnDelete), first_item_index);
}

void btnSave_click(GtkButton *button, gpointer user_data) {
  RouteWidgets *routeObj = (RouteWidgets *)user_data;

  if (!validateRouteInput(routeObj)) {
    return;
  }

  Route route = {
      .rouCode = (gchar *)gtk_entry_get_text(GTK_ENTRY(routeObj->entRoute)),
      .rouNameFrom = NULL,
      .rouNameTo = NULL,
      .rouDirection = NULL,
      .staFrom = NULL,
      .staTo = NULL
  };
  
  GtkTreeIter iter;
  GtkTreeModel *model;
  if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(routeObj->cmbDirection), &iter)) {
    model = gtk_combo_box_get_model(GTK_COMBO_BOX(routeObj->cmbDirection));
    gtk_tree_model_get(model, &iter, 0, &(route.rouDirection), -1);
  }
  
  GtkTreeIter fromIter;
  GtkListStore *fromStore = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(routeObj->cmbFrom)));
  if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(routeObj->cmbFrom), &fromIter))
    gtk_tree_model_get(GTK_TREE_MODEL (fromStore), &fromIter, 0, &(route.staFrom), -1);

  GtkTreeIter destIter;
  GtkListStore *destStore = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(routeObj->cmbDest)));  
  if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(routeObj->cmbDest), &destIter))
    gtk_tree_model_get(GTK_TREE_MODEL (destStore), &destIter, 0, &(route.staTo), -1);

  if (routeObj->edit == 0)
    insertRoute (route);
  else
    updateRoute (route);
    
  g_print ("route: %s, %s, %s - %s\n", route.rouCode, route.rouDirection, route.staFrom, route.staTo);

  // clear the input fields
  clearRouteInput(routeObj);
  insertDataToTreeListStore(routeObj);
  btnNew_click (NULL, routeObj);
}

void btnDelete_click (GtkWidget *widget, gpointer userdata){
  RouteWidgets *routeObj = (RouteWidgets*) userdata;

  Route *route = g_new(Route, 1);
  memset(route, 0, sizeof(Route));

  // Get the route code
  const gchar *rouCode = gtk_entry_get_text (GTK_ENTRY(routeObj->entRoute));
  route->rouCode = g_strdup(rouCode);

  // Get the route direction
  GtkTreeModel *model;
  GtkTreeIter iter;
  if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(routeObj->cmbDirection), &iter)) {
    model = gtk_combo_box_get_model(GTK_COMBO_BOX(routeObj->cmbDirection));
    gtk_tree_model_get(model, &iter, 0, &(route->rouDirection), -1);
  }

  g_print ("route: %s, direction: %s\n", route->rouCode, route->rouDirection);

  if (deleteRoute (route)){
    insertDataToTreeListStore(routeObj);
    btnNew_click (NULL, routeObj);
  }

  g_free (route->rouCode);
  g_free (route->rouDirection);
}

void row_change(GtkTreeSelection *selection, gpointer user_data) {
    RouteWidgets *routeObj = (RouteWidgets*) user_data;
       
    if (gtk_tree_selection_get_selected(routeObj->treeSelected, &routeObj->model, &routeObj->treeIter)) {
        // Get the data from the selected row
        gchar *code, *nameFrom, *nameTo, *direction, *staFrom, *staTo;
        gtk_tree_model_get(routeObj->model, &routeObj->treeIter, 0, &code, 1, &nameFrom, 2, &nameTo, 3, &direction, 4, &staFrom, 5, &staTo, -1);

        // Set the data to the Route struct
        Route* route = g_new(Route, 1);
        route->rouCode = g_strdup(code);
        route->rouNameFrom = g_strdup(nameFrom);
        route->rouNameTo = g_strdup(nameTo);
        route->rouDirection = g_strdup(direction);
        route->staFrom = g_strdup(staFrom);
        route->staTo = g_strdup(staTo);
        
        // Set the Route data to the widgets
        gtk_entry_set_text(GTK_ENTRY(routeObj->entRoute), route->rouCode);
        gtk_combo_box_set_active(GTK_COMBO_BOX(routeObj->cmbDirection), find_combo_box_index(GTK_COMBO_BOX(routeObj->cmbDirection), route->rouDirection));
        gtk_combo_box_set_active(GTK_COMBO_BOX(routeObj->cmbFrom), find_combo_box_index(GTK_COMBO_BOX(routeObj->cmbFrom), route->staFrom));
        gtk_combo_box_set_active(GTK_COMBO_BOX(routeObj->cmbDest), find_combo_box_index(GTK_COMBO_BOX(routeObj->cmbDest), route->staTo));
        
        enableWidgets (routeObj, TRUE);

        // Free the memory used by the strings
        g_free(code);
        g_free(nameFrom);
        g_free(nameTo);
        g_free(direction);
        g_free(staFrom);
        g_free(staTo);
    }
}

void insertDataToTreeListStore(RouteWidgets *routeObj) {
  gtk_list_store_clear(routeObj->treeListStore);
  // This function insert data from GList to GtkListStore
  GList *routeList = getAllRoutes ();
  for (GList *l = routeList; l != NULL; l = l->next) {
    Route *route = (Route *)l->data;
    g_print("rouCode: %s, routNameFrom: %s, rouNameTo: %s, rouDirection: %s, staFrom: %s, staTo: %s\n", route->rouCode, route->rouNameFrom, route->rouNameTo, route->rouDirection, route->staFrom, route->staTo);
    gtk_list_store_append (routeObj->treeListStore, &routeObj->treeIter);
    gtk_list_store_set (routeObj->treeListStore, &routeObj->treeIter, 0, route->rouCode,
                                                              1, route->rouNameFrom,
                                                              2, route->rouNameTo,
                                                              3, route->rouDirection,
                                                              4, route->staFrom,
                                                              5, route->staTo, -1);
  }
  g_list_free_full(routeList, freeRoute);
}

void insertDataToCmbListStore(RouteWidgets *routeObj) {
  // This function insert data from GList to GtkListStore
  GtkTreeIter cmbIter;
  GList *stationList = getAllStations ();
  GtkListStore *listStore = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(routeObj->cmbFrom)));
  gtk_list_store_clear (listStore);
  gtk_list_store_append (listStore, &cmbIter);
  gtk_list_store_set (listStore, &cmbIter, 0, "0", 1, " ", -1);
  for (GList *l = stationList; l != NULL; l = l->next) {
    Station *station = (Station *)l->data;
    g_print("staCode: %s, staName: %s\n", station->staCode, station->staName);
    
    gtk_list_store_append (listStore, &cmbIter);
    gtk_list_store_set (listStore, &cmbIter, 0, station->staCode, 1, station->staName, -1);
  }
  gtk_combo_box_set_active (GTK_COMBO_BOX (routeObj->cmbFrom), 0);
  gtk_combo_box_set_active (GTK_COMBO_BOX (routeObj->cmbDest), 0);
  g_list_free_full(stationList, freeStation);
}
