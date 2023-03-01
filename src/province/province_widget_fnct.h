/*
 * header for station_widget_fnct.c
 */
 
#ifndef PROVINCE_WIDGET_FNCT_H
#define PROVINCE_WIDGET_FNCT_H
#include "province_struct.h"

gboolean entPrvCode_release (GtkWidget *widget, GdkEventKey *event, gpointer userdata);
gboolean entPrvName_release (GtkWidget *widget, GdkEventKey *event, gpointer userdata);
void entPrvCode_focus (GtkWidget *widget, gpointer userdata);
void entPrvName_focus (GtkWidget *widget, gpointer userdata);
void prvExit_click (GtkWidget *widget, gpointer userdata);
void prvNew_click (GtkWidget *widget, gpointer userdata);
void prvSave_click (GtkWidget *widget, gpointer user_data);
void prvDelete_click (GtkWidget *widget, gpointer user_data);
void prv_row_change (GtkWidget *treeView, gpointer userdata);
void prv_update_tree_view(ProvinceWidgets *provinceObj, Province province, gboolean result);
void prv_update_tree_store(ProvinceWidgets *provinceObj, Province province, gboolean result);
void insertProvinceToListStore (ProvinceWidgets *provinceObj);

#endif // PROVINCE_WIDGET_FNCT_H
