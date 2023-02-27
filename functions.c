/*
 * functions.c
 * purpose for store functions to share all module
 */

#ifndef GTK_GTK_H
#define GTK_GTK_H
#include <gtk/gtk.h>
#endif //GTK_GTK_H
#include <glib.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "station_struct.h"
#include "route_struct.h"


char *dateFormat(char *lang, time_t date, char *type) {
    char *result = (char *) malloc(100 * sizeof(char));
    char day[3], month[30], year[5];
    struct tm *timeinfo;

    timeinfo = localtime(&date);

    if (strcmp(lang, "th") == 0) {
        sprintf(day, "%d", timeinfo->tm_mday);
        sprintf(year, "%d", timeinfo->tm_year + 1900 + 543);

        if (strcmp(type, "1") == 0) {
            char *thaimonth[] = {"มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม"};
            strcpy(month, thaimonth[timeinfo->tm_mon]);
        } else {
            char *thaimonth[] = {"ม.ค.", "ก.พ.", "มี.ค.", "เม.ย.", "พ.ค.", "มิ.ย.", "ก.ค.", "ส.ค.", "ก.ย.", "ต.ค.", "พ.ย.", "ธ.ค."};
            strcpy(month, thaimonth[timeinfo->tm_mon]);
        }
    } else {
        if (strcmp(type, "1") == 0) {
            strftime(month, 20, "%B", timeinfo);
        } else {
            strftime(month, 20, "%b", timeinfo);
        }
        sprintf(day, "%d", timeinfo->tm_mday);
        sprintf(year, "%d", timeinfo->tm_year + 1900);
    }

    sprintf(result, "%s %s %s", day, month, year);
    return result;
}

//max value is: ./thai 2147483647.99
//-0.00 is สองพันหนึ่งร้อยสี่สิบเจ็ดล้านสี่แสนแปดหมื่นสามพันหกร้อยสี่สิบเจ็ดบาทเก้าสิบเก้าสตางค์
gchar *read_number(long number) {
    //const gchar *position_call[] = {"แสน", "หมื่น", "พัน", "ร้อย", "สิบ", ""};
    //const gchar *number_call[] = {"", "หนึ่ง", "สอง", "สาม", "สี่", "ห้า", "หก", "เจ็ด", "แปด", "เก้า"};
    const gchar *position_call[] = {"แสน", "หมื่น", "พัน", "ร้อย", "สิบ", ""};
    const gchar *number_call[] = {"", "หนึ่ง", "สอง", "สาม", "สี่", "ห้า", "หก", "เจ็ด", "แปด", "เก้า"};

    GString *ret = g_string_new("");
    if (number == 0) {
        return g_string_free(ret, FALSE);
    }

    if (number > 1000000) {
        g_string_printf(ret, "%s%s", read_number(number / 1000000), "ล้าน");
        number = number % 1000000;
    }

    int divider = 100000;
    int pos = 0;

    while (number > 0) {
        int d = number / divider;

        if (divider == 10 && d == 2) {
            g_string_append(ret, "ยี่");
        } else if (divider == 10 && d == 1) {
            // do nothing
        } else if (divider == 1 && d == 1 && ret->len != 0) {
            g_string_append(ret, "เอ็ด");
        } else {
            g_string_append(ret, number_call[d]);
        }

        if (d) {
            g_string_append(ret, position_call[pos]);
        }

        number = number % divider;
        divider /= 10;
        pos++;
    }

    return g_string_free(ret, FALSE);
}

gchar *thai_baht_conversion(double amount_number) {
    gchar *baht = read_number((long)amount_number);
    gchar *satang = NULL;

    int fraction = (int)((amount_number - (double)((long)amount_number)) * 100.0 + 0.5);

    if (fraction > 0) {
        satang = read_number(fraction);

        if (satang != NULL) {
            size_t len = strlen(baht) + strlen(satang) + 28;
            gchar *ret = g_malloc(len);
            snprintf(ret, len, "%sบาท%sสตางค์", baht, satang);

            g_free(baht);
            g_free(satang);

            return ret;
        }
    }

    if (baht != NULL) {
        size_t len = strlen(baht) + 28;
        gchar *ret = g_malloc(len);
        snprintf(ret, len, "%sบาทถ้วน", baht);

        g_free(baht);

        return ret;
    }

    return NULL;
}

void change_keyb (gchar *new_group){
  Display *dpy = XOpenDisplay(NULL);
  XkbRF_VarDefsRec vd;
  XkbRF_GetNamesProp(dpy, NULL, &vd);

  gint i = 0;
  const gchar *tok = strtok(vd.layout, ",");
  while (tok != NULL) {
    if (g_strcmp0 (tok, new_group) == 0)
      break;
    i++;
    tok = strtok(NULL, ",");
  }

  XkbLockGroup(dpy, XkbUseCoreKbd, i); // change group
  printf ("Changed keyboard to: %s\n", new_group);

  XCloseDisplay(dpy);
}

// return pointer to GtkTreeIter, use *
// ex_condition, we stop looping when result is -1 = before, 0 = equal, 1 = after
GtkTreeIter *get_iter (const gchar* str, gint col, int ext_condition, gpointer userdata){
  MyObjects *mobj = (MyObjects*) userdata;
  gint number_of_rows = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(mobj->liststore), NULL);
  mobj->model = gtk_tree_view_get_model (GTK_TREE_VIEW (mobj->treeview));
  if (gtk_tree_model_get_iter_first (mobj->model, &mobj->iter)){
    gchar *staCode;
    gint count=1;
    do {
      gtk_tree_model_get (mobj->model, &mobj->iter, col, &staCode, -1);

      // sta after staCode, exit and insert before staCode.
      if (ext_condition == -1 && g_utf8_collate (str, staCode) < 0) break;
      // str equal staCode, exit and update this staCode.
      if (ext_condition == 0 && g_utf8_collate (str, staCode) == 0) break;
      // str before staCode, exit we don't use it here.
      if (ext_condition == 1 && g_utf8_collate (str, staCode) > 0) break;

      count++;
      g_free (staCode); // addnew
    } while (gtk_tree_model_iter_next (mobj->model, &mobj->iter));

    if (count > number_of_rows){
      g_print ("EOF, data not found (ALL=%d)\n", number_of_rows);
      return NULL;
    }else{
      g_print ("found at: %d of %d\n", count, number_of_rows);
    }
    //g_free (staCode); //move to do block
  }
  
  return (&mobj->iter); // must use & before iter
}

void display_warning_message(const gchar *message) {
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s", message);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void freeRoute (gpointer data) {
    Route *route = (Route *)data;
    g_free (route->rouCode);
    g_free (route->rouNameFrom);
    g_free (route->rouNameTo);
    g_free (route->rouDirection);
    g_free (route->staFrom);
    g_free (route->staTo);
    g_free (route);
}

void freeStation (gpointer data){
  Station *station = (Station *) data;
  g_free (station->staCode);
  g_free (station->staName);
  g_free (station);
}

void searchCombo(GtkComboBox *combo_box, const gchar *text, GtkTreeIter *result) {
  GtkTreeModel *model;
  GtkTreeIter iter;
  gboolean valid;

  model = gtk_combo_box_get_model(combo_box);
  valid = gtk_tree_model_get_iter_first(model, &iter);

  while (valid) {
    gchar *val;
    gtk_tree_model_get(model, &iter, 0, &val, -1);
    if (g_strcmp0(val, text) == 0) {
      *result = iter;
      g_free(val);
      return;
    }
    g_free(val);
    valid = gtk_tree_model_iter_next(model, &iter);
  }

  // If no match is found, return an invalid iterator
  gtk_tree_model_get_iter_first(model, result);
  gtk_tree_model_get(model, result, 0, &result, -1);
}

/**
 * Finds the index of the item in the combo box with the specified ID.
 * Returns -1 if no item is found.
 */
gint find_combo_box_index(GtkComboBox *combo_box, const gchar *id){
    GtkTreeModel *model;
    GtkTreeIter iter;
    gint index = 0;

    model = gtk_combo_box_get_model(combo_box);

    if (gtk_tree_model_get_iter_first(model, &iter)) {
        do {
            gchar *item_id;

            gtk_tree_model_get(model, &iter, 0, &item_id, -1);

            if (g_strcmp0(item_id, id) == 0) {
                g_free(item_id);
                return index;
            }

            g_free(item_id);
            index++;
        } while (gtk_tree_model_iter_next(model, &iter));
    }

    return -1;
}
