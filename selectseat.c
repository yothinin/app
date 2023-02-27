#include <gtk/gtk.h>

static GHashTable *selected_colors = NULL;
static GtkWidget *selected_seats_label = NULL;
static GList *selected_seats_list = NULL;

typedef struct {
  gchar row;
  gchar col;
  gboolean is_reserved;
  gboolean is_selected; // new member
} Seat;

static Seat *create_seat(const gchar *seat_name) {
  Seat *seat = g_new(Seat, 1);
  seat->row = seat_name[0];
  seat->col = seat_name[1];
  return seat;
}

static void print_selected_seats(GList *list) {
  GString *str = g_string_new("");
  g_print("Selected seats: ");
  for (GList *l = list; l != NULL; l = l->next) {
    Seat *seat = l->data;
    g_print("%c%c ", seat->row, seat->col);
    g_string_append_printf(str, "%c%c ", seat->row, seat->col);
  }
  gtk_label_set_text (GTK_LABEL (selected_seats_label), str->str);
  g_string_free (str, TRUE);
  g_print("\n");
}

static void print_selected_colors(gpointer key, gpointer value, gpointer user_data) {
  GdkRGBA *rgba_value = value;
  const gchar *button_label = gtk_button_get_label(GTK_BUTTON(key));
  g_print("Button %s - RGBA(%.2f, %.2f, %.2f, %.2f)\n", button_label,
            rgba_value->red, rgba_value->green, rgba_value->blue, rgba_value->alpha);
}

void print_hash_table(GHashTable *hash_table) {
    g_hash_table_foreach(hash_table, print_selected_colors, NULL);
}

static void toggle_seat_selection(GtkWidget *widget, gpointer data) {
  GdkRGBA *selected_color = g_hash_table_lookup(selected_colors, widget);
  GdkRGBA current_color;
  
  GtkStyleContext *style_context = gtk_widget_get_style_context(widget);
  gtk_style_context_get_color (style_context, gtk_style_context_get_state (style_context), &current_color);
  const gchar *seat_name = gtk_button_get_label(GTK_BUTTON(widget));
  Seat *seat = create_seat(seat_name);

  if (selected_color == NULL) {
    gtk_style_context_add_class (style_context, "my-class");
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, ".my-class { background-color: #00ff00; }", -1, NULL);
    gtk_style_context_add_provider (style_context, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    g_hash_table_insert(selected_colors, widget, &(GdkRGBA){0, 1, 0, 1});
    //g_print ("selected  : %s\n", seat_name);

    selected_seats_list = g_list_append(selected_seats_list, seat);
  }else {
    gtk_style_context_add_class (style_context, "my-class");
    // Set the alpha channel of the GdkRGBA color to 0
    current_color.alpha = 0;
    // Convert the GdkRGBA color to a CSS color string
    gchar *color_str;
    color_str = gdk_rgba_to_string(&current_color);
    // Append the color string to the CSS data
    char css_data[100];
    g_snprintf(css_data, sizeof(css_data), ".my-class { background-color: %s; }", color_str);
    // Load the CSS provider from the data
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
    gtk_style_context_add_provider (style_context, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_hash_table_remove(selected_colors, widget);
    //g_print ("deselected: %s\n", seat_name);

    //selected_seats_list = g_list_remove(selected_seats_list, seat);
    GList *iter = selected_seats_list;
    while (iter != NULL) {
      Seat *curr_seat = (Seat *)iter->data;
      //if (curr_seat == seat) {
      if (curr_seat != NULL && seat != NULL &&
          curr_seat->row == seat->row && curr_seat->col == seat->col) {
        selected_seats_list = g_list_delete_link(selected_seats_list, iter);
        break;
      }
      iter = g_list_next(iter);
    }
    g_free(seat);
  }
  //print_hash_table(selected_colors);
  print_selected_seats(selected_seats_list);
}

int main(int argc, char *argv[]) {
  GtkWidget *window;
  GtkWidget *box;
  GtkWidget *button;
  char seat_map[] = "00111111111111110011111111111100";

  gtk_init(&argc, &argv);
  selected_colors = g_hash_table_new(g_direct_hash, g_direct_equal);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Bus Seat Map");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window), box);

  int row, col, index;
  char seat_name[3];
  for (row = 1; row <= 8; row++) {
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 0);
    for (col = 1; col <= 4; col++) {
      index = (row - 1) * 4 + col - 1;
      if (seat_map[index] == '1') {
        sprintf(seat_name, "%d%c", row, col + 64); // convert column index to letter
        button = gtk_button_new_with_label(seat_name);
        gtk_widget_set_can_focus(button, FALSE);
        gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE); // remove button border
        gtk_widget_set_size_request(button, 60, 60); // set button size
        g_signal_connect(button, "clicked", G_CALLBACK(toggle_seat_selection), NULL);
        gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
      } else {
        GtkWidget *lblEmpty = gtk_label_new ("");
        gtk_label_set_width_chars (GTK_LABEL (lblEmpty), 7);
        gtk_box_pack_start(GTK_BOX(hbox), lblEmpty, FALSE, FALSE, 0);
      }
    }
  }

  // create the selected seats label
  selected_seats_label = gtk_label_new("Selected: ");
  gtk_box_pack_start(GTK_BOX(box), selected_seats_label, FALSE, FALSE, 0);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
