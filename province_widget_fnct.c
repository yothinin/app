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
#include "province_struct.h"
#include "province_widget_fnct.h"
#include "province_mysql_fnct.h"

/*
 * to declare function name use these form:
 * 1. [widget]_[behavior] for instance, btnSave_click
 * 2. use present simple tense only
 * 3. behavior use lowercase
 * 4. Widget name use camel case lower for abbreve widget and capital for real name of widget
 * for instance btnSave_click, btn is abbreve of GtkButton and Save is name of button and 
 * click is the behavior for the button
 */


void entPrvCode_focus (GtkWidget *widget, gpointer userdata){
  change_keyb ("us");
}

gboolean entPrvCode_release (GtkWidget *widget, GdkEventKey *event, gpointer userdata){
  ProvinceWidgets *provinceObj = (ProvinceWidgets*) userdata;
  const gchar *prvCode = gtk_entry_get_text (GTK_ENTRY(provinceObj->entPrvCode));
  if (strcmp(gdk_keyval_name(event->keyval), "Return") == 0  ||
      strcmp(gdk_keyval_name(event->keyval), "KP_Enter") == 0){

    //const gchar *prvCode = gtk_entry_get_text (GTK_ENTRY (provinceObj->entPrvCode));
    //GtkTreeIter *checkIter = get_iter(prvCode, 0, 0, provinceObj); // ext_condition = 0, equal
                                                           // use equal because search to find the code.
    //MYSQL *conn = connect_to_db();
    const gchar *const_prvCode = gtk_entry_get_text(GTK_ENTRY(provinceObj->entPrvCode));
    gchar *prvCode = g_strdup(const_prvCode);
    Province province = {prvCode, NULL};

    province = getProvinceNameByCode (province);
    gtk_entry_set_text (GTK_ENTRY (provinceObj->entPrvName), province.prvName == NULL?"":province.prvName);

    if (province.prvName != NULL){
      gtk_widget_set_sensitive (provinceObj->entPrvCode, FALSE);
      provinceObj->edit=1; // UPDATE
      gtk_button_set_label (GTK_BUTTON (provinceObj->btnSave), "แก้ไข");
      gtk_widget_set_sensitive (provinceObj->btnSave, TRUE);
      gtk_widget_set_sensitive (provinceObj->btnDelete, TRUE);
    } // CHECK LATER FOR NEED TO CREATE ELSE BLOCK FOR provinceObj->edit = 0;
    
    gtk_widget_grab_focus (provinceObj->entPrvName);
    
    g_free (prvCode);
    g_free (province.prvName);

    return TRUE;
  }else {
    if (strlen(prvCode) > 0)
      gtk_widget_set_sensitive (provinceObj->entPrvName, TRUE);
    else
      gtk_widget_set_sensitive (provinceObj->entPrvName, FALSE);    
  }

  return FALSE;
}

void entPrvName_focus (GtkWidget *widget, gpointer userdata){
  change_keyb ("th");
}

gboolean entPrvName_release (GtkWidget *widget, GdkEventKey *event, gpointer userdata){
  ProvinceWidgets *provinceObj = (ProvinceWidgets*) userdata;
  const gchar *prvName = gtk_entry_get_text (GTK_ENTRY(provinceObj->entPrvName));
  if (strcmp(gdk_keyval_name(event->keyval), "Return") == 0  ||
      strcmp(gdk_keyval_name(event->keyval), "KP_Enter") == 0){

    gtk_widget_grab_focus (provinceObj->btnSave);

    return TRUE;
  }else {
    if (strlen(prvName) > 0)
      gtk_widget_set_sensitive (provinceObj->btnSave, TRUE);
    else
      gtk_widget_set_sensitive (provinceObj->btnSave, FALSE);    
  }

  return FALSE;
}

void prvNew_click (GtkWidget *widget, gpointer userdata){
  ProvinceWidgets *provinceObj = (ProvinceWidgets*) userdata;

  GtkTreeSelection *selected = gtk_tree_view_get_selection (GTK_TREE_VIEW(provinceObj->treeView));
  gtk_tree_selection_unselect_all (GTK_TREE_SELECTION(selected));

  gtk_widget_set_sensitive (provinceObj->entPrvCode, TRUE);
  gtk_widget_set_sensitive (provinceObj->entPrvName, FALSE);
  gtk_entry_set_text (GTK_ENTRY(provinceObj->entPrvCode), "");
  gtk_entry_set_text (GTK_ENTRY(provinceObj->entPrvName), "");
  gtk_button_set_label (GTK_BUTTON(provinceObj->btnSave), "บันทึก");
  gtk_widget_set_sensitive (provinceObj->btnSave, FALSE);
  gtk_widget_set_sensitive (provinceObj->btnDelete, FALSE);
  gtk_widget_grab_focus (provinceObj->entPrvCode);
  provinceObj->edit = 0; // New, save
}

void prvDelete_click (GtkWidget *widget, gpointer userdata){
  ProvinceWidgets *provinceObj = (ProvinceWidgets*) userdata;
  
    const gchar *prvCode = gtk_entry_get_text (GTK_ENTRY(provinceObj->entPrvCode));
    const gchar *prvName = gtk_entry_get_text (GTK_ENTRY(provinceObj->entPrvName));
    if (prvCode[0] != '\0'){
      g_print ("Delete -> Code: %s, %s\n", prvCode, prvName);
      gboolean result = deleteProvince (prvCode);
      if (result) {
        Province province = {g_strdup (prvCode), g_strdup (prvName)};
        GtkTreeIter *selIter = get_iter (prvCode, 0, 0, provinceObj);
        if (selIter != NULL){
          gtk_tree_store_remove (GTK_TREE_STORE (provinceObj->model), &provinceObj->iter);
        }
        g_free (province.prvCode);
        g_free (province.prvName);
        
        gint number_of_rows = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(provinceObj->treeStore), NULL);
        if (number_of_rows == 0)
          prvNew_click (NULL, provinceObj);

      } else {
        g_print("Failed to delete province.\n");
      }
    }
}

void prv_row_change (GtkWidget *treeView, gpointer userdata) {
  ProvinceWidgets *provinceObj = (ProvinceWidgets*) userdata;
  provinceObj->model = gtk_tree_view_get_model (GTK_TREE_VIEW (provinceObj->treeView));
  provinceObj->selected = gtk_tree_view_get_selection(GTK_TREE_VIEW(provinceObj->treeView));
  if (gtk_tree_selection_get_selected(provinceObj->selected, &provinceObj->model, &provinceObj->iter)){
    const gchar *prvCode, *prvName;
    gtk_tree_model_get (provinceObj->model, &provinceObj->iter, 0, &prvCode, 1, &prvName, -1);
    gtk_entry_set_text (GTK_ENTRY(provinceObj->entPrvCode), prvCode);
    
    // Get the province name from the database
    //MYSQL *conn = connect_to_db();
    Province province = {g_strdup (prvCode), g_strdup (prvName)};
    province = getProvinceNameByCode (province);
    if (province.prvName != NULL) {
      gtk_entry_set_text (GTK_ENTRY(provinceObj->entPrvName), province.prvName);
      gtk_tree_store_set (provinceObj->treeStore, &provinceObj->iter, 1, province.prvName, -1);
      g_free(province.prvName);
    } else {
      gtk_entry_set_text (GTK_ENTRY(provinceObj->entPrvName), "");
      gtk_list_store_remove (GTK_LIST_STORE (provinceObj->model), &provinceObj->iter);
    }
    gtk_widget_set_sensitive (provinceObj->entPrvCode, FALSE);
    gtk_widget_set_sensitive (provinceObj->entPrvName, TRUE);
    gtk_button_set_label (GTK_BUTTON(provinceObj->btnSave), "แก้ไข");
    gtk_widget_set_sensitive (provinceObj->btnSave, TRUE);
    gtk_widget_set_sensitive (provinceObj->btnDelete, TRUE);
    provinceObj->edit = 1; //edit
  }
}

void prvSave_click(GtkWidget *widget, gpointer userdata) {
  ProvinceWidgets *provinceObj = (ProvinceWidgets*) userdata;
  const gchar *prvCode = gtk_entry_get_text(GTK_ENTRY(provinceObj->entPrvCode));
  const gchar *prvName = gtk_entry_get_text(GTK_ENTRY(provinceObj->entPrvName));
  
  if (prvCode[0] != '\0' && prvName[0] != '\0') {
    g_print("Save -> Code: %s, Name: %s\n", prvCode, prvName);

    Province province = {g_strdup(prvCode), g_strdup(prvName)};
    if (provinceObj->edit == 0) { // Insert mode.
      gboolean result = insertProvince (province);
      if (result)
        prv_update_tree_view(provinceObj, province, TRUE);
    } else { // Edit mode.
      gboolean result = updateProvinceName (province);
      if (result)
        prv_update_tree_store(provinceObj, province, result);
    }
    
    g_free(province.prvCode);
    g_free(province.prvName);

    prvNew_click(NULL, provinceObj);
  }
}

void prv_update_tree_view(ProvinceWidgets *provinceObj, Province province, gboolean result) {
  gint number_of_rows = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(provinceObj->treeStore), NULL);

  GtkTreeIter *befIter = get_iter(province.prvCode, 0, -1, provinceObj); // ext_condition -1
  if (befIter == NULL || number_of_rows == 0) {
    gtk_tree_store_append(GTK_TREE_STORE (provinceObj->treeStore), &provinceObj->iter, NULL);
  } else {
    gtk_tree_store_insert_before(GTK_TREE_STORE (provinceObj->treeStore), &provinceObj->iter, befIter, NULL);
  }

  gtk_tree_store_set(GTK_TREE_STORE (provinceObj->treeStore), &provinceObj->iter, 0, province.prvCode, 1, province.prvName, -1); // Insert value
}

void prv_update_tree_store(ProvinceWidgets *provinceObj, Province province, gboolean result) {
  if (result) {
    GtkTreeIter *checkIter = get_iter(province.prvCode, 0, 0, provinceObj); // ext_condition = 0, equal
    if (checkIter != NULL){
      GValue value = G_VALUE_INIT;
      g_value_init(&value, G_TYPE_STRING);
      g_value_set_string(&value, province.prvName);
      gtk_tree_store_set_value(GTK_TREE_STORE (provinceObj->treeStore), &provinceObj->iter, 1, &value); // Update value
    }
  }
}

void insertProvinceToListStore(ProvinceWidgets *provinceObj) {
  MYSQL *conn;
  conn = connect_to_db();
  if (conn == NULL) {
    fprintf(stderr, "Error: failed to connect to database\n");
    exit (1);
  }
  
  const char *sql = "SELECT prv_code, prv_name FROM province ORDER BY prv_code";
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
    gtk_tree_store_append (provinceObj->treeStore, &provinceObj->iter, NULL);
    for (int i = 0; i < num_fields; i++) {
      gtk_tree_store_set (provinceObj->treeStore, &provinceObj->iter, i, row[i], -1);
    }
  }
  
  mysql_free_result(result);
  close_db_connection(conn);
}
