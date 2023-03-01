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
#include "bustype_struct.h"
#include "bustype_widget_fnct.h"
#include "bustype_mysql_fnct.h"

/*
 * to declare function name use these form:
 * 1. [widget]_[behavior] for instance, btnSave_click
 * 2. use present simple tense only
 * 3. behavior use lowercase
 * 4. Widget name use camel case lower for abbreve widget and capital for real name of widget
 * for instance btnSave_click, btn is abbreve of GtkButton and Save is name of button and 
 * click is the behavior for the button
 */


void entBusCode_focus (GtkWidget *widget, gpointer userdata){
  change_keyb ("us");
}

gboolean entBusCode_release (GtkWidget *widget, GdkEventKey *event, gpointer userdata){
  BustypeWidgets *bustypeObj = (BustypeWidgets*) userdata;
  const gchar *busCode = gtk_entry_get_text (GTK_ENTRY(bustypeObj->entBusCode));
  if (strcmp(gdk_keyval_name(event->keyval), "Return") == 0  ||
      strcmp(gdk_keyval_name(event->keyval), "KP_Enter") == 0){

    //const gchar *busCode = gtk_entry_get_text (GTK_ENTRY (bustypeObj->entBusCode));
    //GtkTreeIter *checkIter = get_iter(busCode, 0, 0, bustypeObj); // ext_condition = 0, equal
                                                           // use equal because search to find the code.
    //MYSQL *conn = connect_to_db();
    const gchar *const_busCode = gtk_entry_get_text(GTK_ENTRY(bustypeObj->entBusCode));
    gchar *busCode = g_strdup(const_busCode);
    Bustype bustype = {busCode, NULL};

    bustype = getBustypeNameByCode (bustype);
    gtk_entry_set_text (GTK_ENTRY (bustypeObj->entBusName), bustype.busName == NULL?"":bustype.busName);

    if (bustype.busName != NULL){
      gtk_widget_set_sensitive (bustypeObj->entBusCode, FALSE);
      bustypeObj->edit=1; // UPDATE
      gtk_button_set_label (GTK_BUTTON (bustypeObj->btnSave), "แก้ไข");
      gtk_widget_set_sensitive (bustypeObj->btnSave, TRUE);
      gtk_widget_set_sensitive (bustypeObj->btnDelete, TRUE);
    } // CHECK LATER FOR NEED TO CREATE ELSE BLOCK FOR bustypeObj->edit = 0;
    
    gtk_widget_grab_focus (bustypeObj->entBusName);
    
    g_free (busCode);
    g_free (bustype.busName);

    return TRUE;
  }else {
    if (strlen(busCode) > 0)
      gtk_widget_set_sensitive (bustypeObj->entBusName, TRUE);
    else
      gtk_widget_set_sensitive (bustypeObj->entBusName, FALSE);    
  }

  return FALSE;
}

void entBusName_focus (GtkWidget *widget, gpointer userdata){
  change_keyb ("th");
}

gboolean entBusName_release (GtkWidget *widget, GdkEventKey *event, gpointer userdata){
  BustypeWidgets *bustypeObj = (BustypeWidgets*) userdata;
  const gchar *busName = gtk_entry_get_text (GTK_ENTRY(bustypeObj->entBusName));
  if (strcmp(gdk_keyval_name(event->keyval), "Return") == 0  ||
      strcmp(gdk_keyval_name(event->keyval), "KP_Enter") == 0){

    gtk_widget_grab_focus (bustypeObj->btnSave);

    return TRUE;
  }else {
    if (strlen(busName) > 0)
      gtk_widget_set_sensitive (bustypeObj->btnSave, TRUE);
    else
      gtk_widget_set_sensitive (bustypeObj->btnSave, FALSE);    
  }

  return FALSE;
}

void busNew_click (GtkWidget *widget, gpointer userdata){
  BustypeWidgets *bustypeObj = (BustypeWidgets*) userdata;

  GtkTreeSelection *selected = gtk_tree_view_get_selection (GTK_TREE_VIEW(bustypeObj->treeView));
  gtk_tree_selection_unselect_all (GTK_TREE_SELECTION(selected));

  gtk_widget_set_sensitive (bustypeObj->entBusCode, TRUE);
  gtk_widget_set_sensitive (bustypeObj->entBusName, FALSE);
  gtk_entry_set_text (GTK_ENTRY(bustypeObj->entBusCode), "");
  gtk_entry_set_text (GTK_ENTRY(bustypeObj->entBusName), "");
  gtk_button_set_label (GTK_BUTTON(bustypeObj->btnSave), "บันทึก");
  gtk_widget_set_sensitive (bustypeObj->btnSave, FALSE);
  gtk_widget_set_sensitive (bustypeObj->btnDelete, FALSE);
  gtk_widget_grab_focus (bustypeObj->entBusCode);
  bustypeObj->edit = 0; // New, save
}

void busDelete_click (GtkWidget *widget, gpointer userdata){
  BustypeWidgets *bustypeObj = (BustypeWidgets*) userdata;
  
    const gchar *busCode = gtk_entry_get_text (GTK_ENTRY(bustypeObj->entBusCode));
    const gchar *busName = gtk_entry_get_text (GTK_ENTRY(bustypeObj->entBusName));
    if (busCode[0] != '\0'){
      g_print ("Delete -> Code: %s, %s\n", busCode, busName);
      gboolean result = deleteBustype (busCode);
      if (result) {
        Bustype bustype = {g_strdup (busCode), g_strdup (busName)};
        GtkTreeIter *selIter = get_iter (busCode, 0, 0, bustypeObj);
        if (selIter != NULL){
          gtk_tree_store_remove (GTK_TREE_STORE (bustypeObj->model), &bustypeObj->iter);
        }
        g_free (bustype.busCode);
        g_free (bustype.busName);
        
        gint number_of_rows = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(bustypeObj->treeStore), NULL);
        if (number_of_rows == 0)
          busNew_click (NULL, bustypeObj);

      } else {
        g_print("Failed to delete bustype.\n");
      }
    }
}

void bus_row_change (GtkWidget *treeView, gpointer userdata) {
  BustypeWidgets *bustypeObj = (BustypeWidgets*) userdata;
  bustypeObj->model = gtk_tree_view_get_model (GTK_TREE_VIEW (bustypeObj->treeView));
  bustypeObj->selected = gtk_tree_view_get_selection(GTK_TREE_VIEW(bustypeObj->treeView));
  if (gtk_tree_selection_get_selected(bustypeObj->selected, &bustypeObj->model, &bustypeObj->iter)){
    const gchar *busCode, *busName;
    gtk_tree_model_get (bustypeObj->model, &bustypeObj->iter, 0, &busCode, 1, &busName, -1);
    gtk_entry_set_text (GTK_ENTRY(bustypeObj->entBusCode), busCode);
    
    // Get the province name from the database
    //MYSQL *conn = connect_to_db();
    Bustype bustype = {g_strdup (busCode), g_strdup (busName)};
    bustype = getBustypeNameByCode (bustype);
    if (bustype.busName != NULL) {
      gtk_entry_set_text (GTK_ENTRY(bustypeObj->entBusName), bustype.busName);
      gtk_tree_store_set (bustypeObj->treeStore, &bustypeObj->iter, 1, bustype.busName, -1);
      g_free(bustype.busName);
    } else {
      gtk_entry_set_text (GTK_ENTRY(bustypeObj->entBusName), "");
      gtk_list_store_remove (GTK_LIST_STORE (bustypeObj->model), &bustypeObj->iter);
    }
    gtk_widget_set_sensitive (bustypeObj->entBusCode, FALSE);
    gtk_widget_set_sensitive (bustypeObj->entBusName, TRUE);
    gtk_button_set_label (GTK_BUTTON(bustypeObj->btnSave), "แก้ไข");
    gtk_widget_set_sensitive (bustypeObj->btnSave, TRUE);
    gtk_widget_set_sensitive (bustypeObj->btnDelete, TRUE);
    bustypeObj->edit = 1; //edit
  }
}

void busSave_click(GtkWidget *widget, gpointer userdata) {
  BustypeWidgets *bustypeObj = (BustypeWidgets*) userdata;
  const gchar *busCode = gtk_entry_get_text(GTK_ENTRY(bustypeObj->entBusCode));
  const gchar *busName = gtk_entry_get_text(GTK_ENTRY(bustypeObj->entBusName));
  
  if (busCode[0] != '\0' && busName[0] != '\0') {
    g_print("Save -> Code: %s, Name: %s\n", busCode, busName);

    Bustype bustype = {g_strdup(busCode), g_strdup(busName)};
    if (bustypeObj->edit == 0) { // Insert mode.
      gboolean result = insertBustype (bustype);
      if (result)
        bus_update_tree_view(bustypeObj, bustype, TRUE);
    } else { // Edit mode.
      gboolean result = updateBustypeName (bustype);
      if (result)
        bus_update_tree_store(bustypeObj, bustype, result);
    }
    
    g_free(bustype.busCode);
    g_free(bustype.busName);

    busNew_click(NULL, bustypeObj);
  }
}

void bus_update_tree_view(BustypeWidgets *bustypeObj, Bustype bustype, gboolean result) {
  gint number_of_rows = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(bustypeObj->treeStore), NULL);

  GtkTreeIter *befIter = get_iter(bustype.busCode, 0, -1, bustypeObj); // ext_condition -1
  if (befIter == NULL || number_of_rows == 0) {
    gtk_tree_store_append(GTK_TREE_STORE (bustypeObj->treeStore), &bustypeObj->iter, NULL);
  } else {
    gtk_tree_store_insert_before(GTK_TREE_STORE (bustypeObj->treeStore), &bustypeObj->iter, befIter, NULL);
  }

  gtk_tree_store_set(GTK_TREE_STORE (bustypeObj->treeStore), &bustypeObj->iter, 0, bustype.busCode, 1, bustype.busName, -1); // Insert value
}

void bus_update_tree_store(BustypeWidgets *bustypeObj, Bustype bustype, gboolean result) {
  if (result) {
    GtkTreeIter *checkIter = get_iter(bustype.busCode, 0, 0, bustypeObj); // ext_condition = 0, equal
    if (checkIter != NULL){
      GValue value = G_VALUE_INIT;
      g_value_init(&value, G_TYPE_STRING);
      g_value_set_string(&value, bustype.busName);
      gtk_tree_store_set_value(GTK_TREE_STORE (bustypeObj->treeStore), &bustypeObj->iter, 1, &value); // Update value
    }
  }
}

void insertBustypeToListStore(BustypeWidgets *bustypeObj) {
  MYSQL *conn;
  conn = connect_to_db();
  if (conn == NULL) {
    fprintf(stderr, "Error: failed to connect to database\n");
    exit (1);
  }
  
  const char *sql = "SELECT bus_code, bus_name FROM bustype ORDER BY bus_code";
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
    gtk_tree_store_append (bustypeObj->treeStore, &bustypeObj->iter, NULL);
    for (int i = 0; i < num_fields; i++) {
      gtk_tree_store_set (bustypeObj->treeStore, &bustypeObj->iter, i, row[i], -1);
    }
  }
  
  mysql_free_result(result);
  close_db_connection(conn);
}
