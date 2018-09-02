#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


int cantidad_nodos;

void printSolution(void);

void myCSS(void);

void knapsack(void);

int table[V][V];
GtkWidget *graph_nodes[V][V];
int dist[V][V];
char names[V][25];
char lista_rutas[V];
GtkWidget *names_nodes_r[V];
GtkWidget *names_nodes_c[V];


app_widgets *widgets;

GtkWidget *frame, *window, *frame_rutas;
GtkBuilder *builder;
GtkWidget *mainbox, *midbox;

GtkCssProvider *provider;

void myCSS(void) {
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *style_file = "src/style.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(style_file), &error);
    g_object_unref(provider);
}

// called when window is closed
void on_window_main_destroy() {
    gtk_main_quit();
}

void knapsack() {
    int i, j, k;
    /*Tabla [i][j] =
    – max( Tabla [i][j-1],
    1 * vj + Tabla [i-1 * cj
    ][j-1],
    2 * vj + Tabla [i-2 * cj
    ][j-1],
    3 * vj + Tabla [i-3 * cj
    ][j-1],
    ..................
    Q * vj + Tabla [i-Q * cj
    ][j-1])*/

    /* Initialize the solution matrix same as input graph matrix */
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            tablas_intermedias[0][i][j] = dist[i][j] = graph[i][j];

    for (k = 0; k < V; k++)
        for (i = 0; i < V; i++)
            for (j = 0; j < V; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]){
                    tablas_intermedias[k][i][j] = dist[i][j] = dist[i][k] + dist[k][j];
                    route[i][j] = k + 1;
                  } else
                    tablas_intermedias[k][i][j] = dist[i][j];
            }

    // Print the shortest distance matrix
    printSolution();
}

/* A utility function to print solution */
void printSolution() {
    int i, j, k;

    printf("Distancia minima entre cada nodo\n");
    for (i = 0; i < cantidad_nodos; i++) {
        printf("%7s", names[i]);
        for (j = 0; j < cantidad_nodos; j++) {
            if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    printf("Tabla de rutas\n");
    for (i = 0; i < cantidad_nodos; i++) {
        printf("LISTA RUTA:%d\n", lista_rutas[i]);

        printf("%7s", names[i]);
        for (j = 0; j < cantidad_nodos; j++) {
            printf("%7d", route[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    for (int k = 0; k < cantidad_nodos; k++) {
        printf("\nTABLA D(%d)\n", k);

        for (i = 0; i < cantidad_nodos; i++) {
            printf("%7s", names[i]);
            for (j = 0; j < cantidad_nodos; j++) {
                if (tablas_intermedias[k][i][j] == INF)
                    printf("%7s", "INF");
                else
                    printf("%7d", tablas_intermedias[k][i][j]);
            }
            printf("\n");
        }
    }
}

void generar_imagen_grafo() {
    system("dot -Tpng test.dot -o src/graph.png");
}

void generar_imagen_rutas() {
    system("dot -Tpng test.dot -o src/rutas.png");
}

void cambiar_nombres_grafos() {
    int i;
    for (i = 0; i < V; i++) {
        gtk_entry_set_text(GTK_ENTRY(names_nodes_r[i]), names[i]);
        gtk_entry_set_text(GTK_ENTRY(names_nodes_c[i]), names[i]);
    }
}

void refresh() {
    cambiar_nombres_grafos();
    generar_grafo();
    generar_imagen_grafo();
    generar_imagen_rutas();

    const gchar *style_file = "src/style.css";
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(style_file), 0);
}


void icon_pressed_cb(GtkEntry *entry, gpointer user_data) {
    strcpy(names[GPOINTER_TO_UINT(user_data)], gtk_entry_get_text(entry));
    refresh();
}

void icon_pressed_node(GtkEntry *entry, gpointer user_data) {
    int length, k;
    int i = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(entry), "i"));
    int j = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(entry), "j"));

    const gchar * value = gtk_entry_get_text(entry);


    printf("%d %d \n", i, j);
    printf("%s\n\n", value);

    length = strlen(value);
    bool is_num = true;
    for (k = 0; k < length; k++)
        if (!isdigit(value[k])) {
            is_num = false;
        }

    if(is_num){
        graph[i][j] = atoi(value);
    } else {
        graph[i][j] = INF;
    }

    printSolution();

    refresh();
}

void on_btn_cambiar_cant_nodos_clicked(GtkButton *buttonn, app_widgets *app_wdgts) {
    int i, j;
    GtkWidget *nbutton;

    int quantity = 0; // stores integer read from spin button widget
    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_sbtn_quantity));
    cantidad_nodos = quantity;

    GList *children, *iter;

    for (i = 0; i <= V; i++) {
        if (i < cantidad_nodos) {
            gtk_widget_show(GTK_WIDGET(names_nodes_r[i]));
            gtk_widget_show(GTK_WIDGET(names_nodes_c[i]));
        } else {
            gtk_widget_hide(GTK_WIDGET(names_nodes_r[i]));
            gtk_widget_hide(GTK_WIDGET(names_nodes_c[i]));
        }
        for (j = 0; j <= V; j++)
            if (i < cantidad_nodos && j < cantidad_nodos) {
                gtk_widget_show(GTK_WIDGET(graph_nodes[i][j]));
            } else {
                gtk_widget_hide(GTK_WIDGET(graph_nodes[i][j]));
            }
    }


    refresh();
}

int main(int argc, char *argv[]) {
    int i, j, k;

    widgets = g_slice_new(app_widgets);

    //Se inicializan los nombres
    j = 0;
    for (i = 'A'; i < 'A' + V; i++) {
        char str[25] = {i, '\0'};
        strcpy(names[j++], str);
    }

    generar_grafo();

    knapsack();

    gtk_init(&argc, &argv);
    myCSS();

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/knapsack.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    gtk_builder_connect_signals(builder, widgets);
    g_object_unref(builder);

    mainbox = gtk_grid_new();
    midbox = gtk_grid_new();
    //inicio = gtk_entry_new();

    gtk_widget_set_halign(mainbox, GTK_ALIGN_CENTER);


    for (i = 0; i < V; i++) {
        GtkWidget *entry = gtk_entry_new();
        names_nodes_r[i] = entry;
        gtk_entry_set_text(GTK_ENTRY(entry), names[i]);
        gtk_grid_attach(GTK_GRID(mainbox), entry, 0, i + 1, 1, 1);
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
        gpointer ptr = GINT_TO_POINTER(i);
        g_signal_connect(entry, "changed",
                         G_CALLBACK(icon_pressed_cb), GINT_TO_POINTER(i));

        entry = gtk_entry_new();
        names_nodes_c[i] = entry;
        gtk_entry_set_text(GTK_ENTRY(entry), names[i]);
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
        gtk_grid_attach(GTK_GRID(mainbox), entry, i + 1, 0, 1, 1);
        g_signal_connect(entry, "changed",
                         G_CALLBACK(icon_pressed_cb), GINT_TO_POINTER(i));
    }

    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            GtkWidget *entry = gtk_entry_new();

            g_object_set_data(G_OBJECT(entry), "i", GINT_TO_POINTER(j));
            g_object_set_data(G_OBJECT(entry), "j", GINT_TO_POINTER(i));
            graph_nodes[i][j] = entry;
            gtk_entry_set_text(GTK_ENTRY(entry), (i == j) ? "0" : "*");
            gtk_entry_set_width_chars(GTK_ENTRY(entry), 4);
            gtk_widget_set_halign(entry, GTK_ALIGN_CENTER);
            gtk_grid_attach(GTK_GRID(mainbox), entry, i+1, j+1, 1, 1);

            g_signal_connect(entry, "changed",
                             G_CALLBACK(icon_pressed_node), NULL);
        }
    }

    gtk_container_add(GTK_CONTAINER(widgets->w_grid_nodes), mainbox);
    gtk_widget_show_all(window);

    gtk_widget_show(window);
    gtk_main();

    g_slice_free(app_widgets, widgets);


    return 0;
}
