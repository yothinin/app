#include <glib.h>
#include <stdio.h>

gint compare_thai_consonants(gconstpointer a, gconstpointer b) {
    gchar *x = g_utf8_strdown(a, -1);
    gchar *y = g_utf8_strdown(b, -1);

    gchar *ptr_x = x;
    gchar *ptr_y = y;

    g_print ("x %s\n", x);
    g_print ("y %s\n", y);


    while (*ptr_x && *ptr_y) {
        gunichar ac = g_utf8_get_char_validated(ptr_x, -1);
        gunichar bc = g_utf8_get_char_validated(ptr_y, -1);

        // Remove Thai vowels and tones
        if (ac >= 0xE0 && ac <= 0xE4) {
            ptr_x += g_utf8_offset_to_pointer(ptr_x, 3) - ptr_x;
            continue;
        }

        if (bc >= 0xE0 && bc <= 0xE4) {
            ptr_y += g_utf8_offset_to_pointer(ptr_y, 3) - ptr_y;
            continue;
        }

        if (ac != bc) {
            g_free(x);
            g_free(y);
            return ac - bc;
        }

        ptr_x = g_utf8_next_char(ptr_x);
        ptr_y = g_utf8_next_char(ptr_y);
    }
    gint result = g_utf8_collate(x, y);
    g_free(x);
    g_free(y);
    return result;
}

int main() {
    GList *list = NULL;
    list = g_list_insert_sorted(list, "กรุงเทพฯ", (GCompareFunc) compare_thai_consonants);
    list = g_list_insert_sorted(list, "เชียงใหม่", (GCompareFunc) compare_thai_consonants);
    list = g_list_insert_sorted(list, "ขอนแก่น", (GCompareFunc) compare_thai_consonants);
    list = g_list_insert_sorted(list, "น่าน", (GCompareFunc) compare_thai_consonants);
    list = g_list_insert_sorted(list, "ชัยภูมิ", (GCompareFunc) compare_thai_consonants);

    GList *iter;
    for (iter = list; iter; iter = iter->next) {
        printf("%s\n", (char*) iter->data);
    }

    g_list_free(list);
    return 0;
}
