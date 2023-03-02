#include <glib.h>
#include <stdio.h>

// Comparison function for sorting strings in alphabetical order
gint compare_strings(gconstpointer a, gconstpointer b) {
    return g_strcmp0((const char*) a, (const char*) b);
}

int main() {
    GList* list = NULL; // initialize an empty list

    // add elements to the list
    list = g_list_append(list, "apple");
    list = g_list_append(list, "banana");
    list = g_list_append(list, "orange");

    // insert a new element in sorted order
    list = g_list_insert_sorted(list, "guava", compare_strings);

    // traverse the list and print each element
    for (GList* node = list; node != NULL; node = node->next) {
        printf("%s\n", (char*) node->data);
    }

    // free the memory allocated by the list
    g_list_free(list);

    return 0;
}
