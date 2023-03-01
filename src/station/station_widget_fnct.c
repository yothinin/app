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
#include "station_struct.h"
#include "station_widget_fnct.h"
#include "station_mysql_fnct.h"

/*
 * to declare function name use these form:
 * 1. [widget]_[behavior] for instance, btnSave_click
 * 2. use present simple tense only
 * 3. behavior use lowercase
 * 4. Widget name use camel case lower for abbreve widget and capital for real name of widget
 * for instance btnSave_click, btn is abbreve of GtkButton and Save is name of button and 
 * click is the behavior for the button
 */

void btnDemo_click (GtkWidget *widget, gpointer userdata){
  StationWidgets *stationObj = (StationWidgets*) userdata;
  g_print ("demo clicked\n");
  
  int i;
  for (i = 0; i < 20000; i++){
    gtk_list_store_append (stationObj->liststore, &stationObj->iter);
    gchar code[6], name[6];
    sprintf (code, "%05d", i);
    sprintf (name, "%05d", i);
    gtk_list_store_set (stationObj->liststore, &stationObj->iter, 0, code, 1, name, -1); // Insert value
  }
}
  
void entStaCode_focus (GtkWidget *widget, gpointer userdata){
  change_keyb ("us");
}

gboolean entStaCode_release (GtkWidget *widget, GdkEventKey *event, gpointer userdata){
  StationWidgets *stationObj = (StationWidgets*) userdata;
  const gchar *stacode = gtk_entry_get_text (GTK_ENTRY(stationObj->entStaCode));
  if (strcmp(gdk_keyval_name(event->keyval), "Return") == 0  ||
      strcmp(gdk_keyval_name(event->keyval), "KP_Enter") == 0){

    //const gchar *staCode = gtk_entry_get_text (GTK_ENTRY (stationObj->entStaCode));
    //GtkTreeIter *checkIter = get_iter(staCode, 0, 0, stationObj); // ext_condition = 0, equal
                                                           // use equal because search to find the code.
    //MYSQL *conn = connect_to_db();
    const gchar *const_staCode = gtk_entry_get_text(GTK_ENTRY(stationObj->entStaCode));
    gchar *staCode = g_strdup(const_staCode);
    Station station = {staCode, NULL};

    station = getStationNameByCode (station);
    gtk_entry_set_text (GTK_ENTRY (stationObj->entStaName), station.staName == NULL?"":station.staName);

    if (station.staName != NULL){
      gtk_widget_set_sensitive (stationObj->entStaCode, FALSE);
      stationObj->edit=1; // UPDATE
      gtk_button_set_label (GTK_BUTTON (stationObj->btnSave), "แก้ไข");
      gtk_widget_set_sensitive (stationObj->btnSave, TRUE);
      gtk_widget_set_sensitive (stationObj->btnDelete, TRUE);
    } // CHECK LATER FOR NEED TO CREATE ELSE BLOCK FOR stationObj->edit = 0;
    
    gtk_widget_grab_focus (stationObj->entStaName);
    
    g_free (staCode);
    g_free (station.staName);

    return TRUE;
  }else {
    if (strlen(stacode) > 0)
      gtk_widget_set_sensitive (stationObj->entStaName, TRUE);
    else
      gtk_widget_set_sensitive (stationObj->entStaName, FALSE);    
  }

  return FALSE;
}

void entStaName_focus (GtkWidget *widget, gpointer userdata){
  change_keyb ("th");
}

gboolean entStaName_release (GtkWidget *widget, GdkEventKey *event, gpointer userdata){
  StationWidgets *stationObj = (StationWidgets*) userdata;
  const gchar *staname = gtk_entry_get_text (GTK_ENTRY(stationObj->entStaName));
  if (strcmp(gdk_keyval_name(event->keyval), "Return") == 0  ||
      strcmp(gdk_keyval_name(event->keyval), "KP_Enter") == 0){

    gtk_widget_grab_focus (stationObj->btnSave);

    return TRUE;
  }else {
    if (strlen(staname) > 0)
      gtk_widget_set_sensitive (stationObj->btnSave, TRUE);
    else
      gtk_widget_set_sensitive (stationObj->btnSave, FALSE);    
  }

  return FALSE;
}

void stationNew_click (GtkWidget *widget, gpointer userdata){
  StationWidgets *stationObj = (StationWidgets*) userdata;

  GtkTreeSelection *selected = gtk_tree_view_get_selection (GTK_TREE_VIEW(stationObj->treeview));
  gtk_tree_selection_unselect_all (GTK_TREE_SELECTION(selected));

  gtk_widget_set_sensitive (stationObj->entStaCode, TRUE);
  gtk_widget_set_sensitive (stationObj->entStaName, FALSE);
  gtk_entry_set_text (GTK_ENTRY(stationObj->entStaCode), "");
  gtk_entry_set_text (GTK_ENTRY(stationObj->entStaName), "");
  gtk_button_set_label (GTK_BUTTON(stationObj->btnSave), "บันทึก");
  gtk_widget_set_sensitive (stationObj->btnSave, FALSE);
  gtk_widget_set_sensitive (stationObj->btnDelete, FALSE);
  gtk_widget_grab_focus (stationObj->entStaCode);
  stationObj->edit = 0; // New, save
}

void stationDelete_click (GtkWidget *widget, gpointer userdata){
  StationWidgets *stationObj = (StationWidgets*) userdata;
  
    const gchar *staCode = gtk_entry_get_text (GTK_ENTRY(stationObj->entStaCode));
    const gchar *staName = gtk_entry_get_text (GTK_ENTRY(stationObj->entStaName));
    if (staCode[0] != '\0'){
      g_print ("Delete -> Code: %s, %s\n", staCode, staName);
      gboolean result = deleteStation(staCode);
      if (result) {
        Station station = {g_strdup (staCode), g_strdup (staName)};
        GtkTreeIter *selIter = get_iter (staCode, 0, 0, stationObj);
        if (selIter != NULL){
          gtk_list_store_remove (GTK_LIST_STORE (stationObj->model), &stationObj->iter);
        }
        g_free (station.staCode);
        g_free (station.staName);
        
        gint number_of_rows = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(stationObj->liststore), NULL);
        if (number_of_rows == 0)
          stationNew_click (NULL, stationObj);

      } else {
        g_print("Failed to delete station.\n");
      }
    }
}

void station_row_change (GtkWidget *treeView, gpointer userdata) {
  StationWidgets *stationObj = (StationWidgets*) userdata;
  stationObj->model = gtk_tree_view_get_model (GTK_TREE_VIEW (stationObj->treeview));
  stationObj->selected = gtk_tree_view_get_selection(GTK_TREE_VIEW(stationObj->treeview));
  if (gtk_tree_selection_get_selected(stationObj->selected, &stationObj->model, &stationObj->iter)){
    const gchar *staCode, *staName;
    gtk_tree_model_get (stationObj->model, &stationObj->iter, 0, &staCode, 1, &staName, -1);
    gtk_entry_set_text (GTK_ENTRY(stationObj->entStaCode), staCode);
    
    // Get the station name from the database
    //MYSQL *conn = connect_to_db();
    Station station = {g_strdup (staCode), g_strdup (staName)};
    station = getStationNameByCode (station);
    if (station.staName != NULL) {
      gtk_entry_set_text (GTK_ENTRY(stationObj->entStaName), station.staName);
      gtk_list_store_set(stationObj->liststore, &stationObj->iter, 1, station.staName, -1);
      g_free(station.staName);
    } else {
      gtk_entry_set_text (GTK_ENTRY(stationObj->entStaName), "");
      gtk_list_store_remove (GTK_LIST_STORE (stationObj->model), &stationObj->iter);
    }
    gtk_widget_set_sensitive (stationObj->entStaCode, FALSE);
    gtk_widget_set_sensitive (stationObj->entStaName, TRUE);
    gtk_button_set_label (GTK_BUTTON(stationObj->btnSave), "แก้ไข");
    gtk_widget_set_sensitive (stationObj->btnSave, TRUE);
    gtk_widget_set_sensitive (stationObj->btnDelete, TRUE);
    stationObj->edit = 1; //edit
  }
}

void stationSave_click(GtkWidget *widget, gpointer userdata) {
  StationWidgets *stationObj = (StationWidgets*) userdata;
  const gchar *staCode = gtk_entry_get_text(GTK_ENTRY(stationObj->entStaCode));
  const gchar *staName = gtk_entry_get_text(GTK_ENTRY(stationObj->entStaName));
  
  if (staCode[0] != '\0' && staName[0] != '\0') {
    g_print("Save -> Code: %s, Name: %s\n", staCode, staName);

    Station station = {g_strdup(staCode), g_strdup(staName)};
    if (stationObj->edit == 0) { // Insert mode.
      gboolean result = insertStation (station);
      if (result)
        update_tree_view(stationObj, station, TRUE);
    } else { // Edit mode.
      gboolean result = updateStationName(station);
      if (result)
        update_list_store(stationObj, station, result);
    }
    
    g_free(station.staCode);
    g_free(station.staName);

    stationNew_click(NULL, stationObj);
  }
}

void update_tree_view(StationWidgets *stationObj, Station station, gboolean result) {
  gint number_of_rows = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(stationObj->liststore), NULL);

  GtkTreeIter *befIter = get_iter(station.staCode, 0, -1, stationObj); // ext_condition -1
  if (befIter == NULL || number_of_rows == 0) {
    gtk_list_store_append(stationObj->liststore, &stationObj->iter);
  } else {
    gtk_list_store_insert_before(stationObj->liststore, &stationObj->iter, befIter);
  }

  gtk_list_store_set(stationObj->liststore, &stationObj->iter, 0, station.staCode, 1, station.staName, -1); // Insert value
}

void update_list_store(StationWidgets *stationObj, Station station, gboolean result) {
  if (result) {
    GtkTreeIter *checkIter = get_iter(station.staCode, 0, 0, stationObj); // ext_condition = 0, equal
    if (checkIter != NULL){
      GValue value = G_VALUE_INIT;
      g_value_init(&value, G_TYPE_STRING);
      g_value_set_string(&value, station.staName);
      gtk_list_store_set_value(stationObj->liststore, &stationObj->iter, 1, &value); // Update value
    }
  }
}

void insertDataToListStore(StationWidgets *stationObj) {
  MYSQL *conn;
  conn = connect_to_db();
  if (conn == NULL) {
    fprintf(stderr, "Error: failed to connect to database\n");
    exit (1);
  }
  
  const char *sql = "SELECT sta_code, sta_name FROM station ORDER BY sta_code";
  if (query(conn, sql)) {
    fprintf(stderr, "Error: failed to execute query\n");
    close_db_connection(conn);
    exit (1);
  }
  
  MYSQL_RES *result = mysql_store_result(conn);
  if (result == NULL) {
    fprintf(stderr, "Error: failed to get result\n");
    close_db_connection(conn);
    exit (1);
  }
  
  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result))) {
    gtk_list_store_append (stationObj->liststore, &stationObj->iter);
    for (int i = 0; i < num_fields; i++) {
      gtk_list_store_set (stationObj->liststore, &stationObj->iter, i, row[i], -1);
    }
  }
  
  mysql_free_result(result);
  close_db_connection(conn);
}
