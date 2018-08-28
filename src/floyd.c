#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define INF 999999999

#define V 10
GtkWidget *inicio;
GtkWidget *final;
typedef struct {
    GtkWidget *w_sbtn_quantity;
    GtkWidget *w_grid_nodes;
    GtkWidget *w_midgrid_nodes;
    //GtkWidget *inicio;
    //GtkWidget *final;
} app_widgets;

//GtkWidget *gtk_entry_new (void);


int tablas_intermedias[V][V][V];

int cantidad_nodos;
int flag = -1;
char *nodo_inicio, *nodo_final;

void printSolution(void);

int rutas(char *nodo_inicio, char *nodo_final);

void myCSS(void);

void floyd(void);

int route[V][V];
int graph[V][V];
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

void floyd() {
    int i, j, k;

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

    //rutas("B", "E");
}

void rutasAux(){
    printf("Prueba a ver \n");
    const gchar *entry_inicio;
    const gchar *entry_final;

    //printf("Lo que esta escrito es!: %s\n", gtk_entry_get_text (final));
    entry_inicio = gtk_entry_get_text(inicio);
    entry_final = gtk_entry_get_text(final);
    //printf("Erick tiene!: %s\n", ent);

    rutas(entry_inicio, entry_final);
}

int rutas(char *nodo_inicio, char *nodo_final) {
    int pos_inicio, pos_final, pos_intermedia, i;
    flag++;

    printf("Ruta óptima\n");
    //printf("La micada: %d\n", route[pos_inicio][pos_final]);
    for (i = 0; i < cantidad_nodos; i++) {
        if (strcmp(nodo_inicio, names[i]) == 0) {
            pos_inicio = i;
            printf("inicio: %d\t", pos_inicio);
            printf("%s\n", names[i]);
        }
        if (strcmp(nodo_final, names[i]) == 0) {
            pos_final = i;
            printf("final: %d\t", pos_final);
            printf("%s\n", names[i]);
        }
    }

    if (route[pos_inicio][pos_final] == 0) {
        lista_rutas[flag] = route[pos_inicio][pos_final];
        return route[pos_inicio][pos_final];
    } else {
        pos_intermedia = route[pos_inicio][pos_final];
        lista_rutas[flag] = route[pos_inicio][pos_final];

        rutas(names[pos_intermedia - 1], names[pos_final]);
    }
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

void imprimirTablasIntermedias(){
    floyd();

    int i, j, iaux, jaux, k;


    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(widgets->w_midgrid_nodes));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    midbox = gtk_grid_new();
    gtk_widget_set_halign(midbox, GTK_ALIGN_CENTER);

    char buffer[25];
    jaux = 0;
    for (int k = 0; k < cantidad_nodos; k++) {

        sprintf(buffer, "TABLA D(%d)", k); // modified to append string

        GtkWidget *entry = gtk_label_new(buffer);
        gtk_grid_attach(GTK_GRID(midbox), entry, 0, i+2, 1, 1);


        GtkStyleContext *context;
        context = gtk_widget_get_style_context(entry);
        gtk_style_context_add_class(context,"title_table");


        gtk_container_add(GTK_CONTAINER(widgets->w_midgrid_nodes), midbox);

        midbox = gtk_grid_new();
        gtk_widget_set_halign(midbox, GTK_ALIGN_CENTER);

        for (i = 0; i < cantidad_nodos; i++) {
            GtkWidget *entry = gtk_label_new(names[i]);
            gtk_grid_attach(GTK_GRID(midbox), entry, 0, i+1, 1, 1);

            entry = gtk_label_new(names[i]);
            gtk_grid_attach(GTK_GRID(midbox), entry, i+1, 0, 1, 1);
        }

        for (i = 0; i < cantidad_nodos; i++) {
            for (j = 0; j < cantidad_nodos; j++) {
                int nj = j;

                GtkWidget *entry;

                graph_nodes[i][j] = entry;
                if (tablas_intermedias[k][i][j] == INF)
                    entry= gtk_label_new(" INF ");
                else {
                    sprintf(buffer, " %d ", tablas_intermedias[k][i][j]);
                    entry= gtk_label_new(buffer);
                }

                GtkStyleContext *context;
                context = gtk_widget_get_style_context(entry);
                gtk_style_context_add_class(context,"numero");

                if(k>0 && tablas_intermedias[k-1][i][j] != tablas_intermedias[k][i][j]){
                    GtkStyleContext *context;
                    context = gtk_widget_get_style_context(entry);
                    gtk_style_context_add_class(context,"diferente");
                }

                gtk_grid_attach(GTK_GRID(midbox), entry, i+1, j+1, 1, 1);
            }
        }

        gtk_container_add(GTK_CONTAINER(widgets->w_midgrid_nodes), midbox);

    }

    gtk_widget_show_all(widgets->w_midgrid_nodes);
}

void printRutas(/* arguments */) {
  /* code */
}

void printGraph() {
    printf("The following matrix shows the shortest distances"
           " between every pair of vertices \n");
    for (int i = 0; i < cantidad_nodos; i++) {
        printf("%7s", names[i]);
        for (int j = 0; j < cantidad_nodos; j++) {
            if (graph[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", graph[i][j]);
        }
        printf("\n");
    }
}

void generar_grafo() {
    int i, j;

    FILE *fichero;
    fichero = fopen("test.dot", "w+");
    fputs("digraph G {\n", fichero);

    for (i = 0; i < cantidad_nodos; i++) {
        fprintf(fichero, "\"%s\"\n", names[i]);
        for (j = 0; j < cantidad_nodos; j++)
            if (graph[i][j] != INF)
                if(i != j)
                    fprintf(fichero, "\"%s\" -> \"%s\" [ label=\"%d\" ];\n", names[i], names[j], graph[i][j]);
                else if (graph[i][j] != 0)
                    fprintf(fichero, "\"%s\" -> \"%s\" [ label=\"%d\" ];\n", names[i], names[j], graph[i][j]);


    }
    fputs("}", fichero);
    fclose(fichero);
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

    cantidad_nodos = V;


    int test[] = {INF, 5, INF, 10, 3, 4, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF,
                  INF, INF, INF, INF};

    k = 0;
    //Se inicializa el grafo en infinito
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }

    //Se inicializan los nombres
    j = 0;
    for (i = 'A'; i < 'A' + V; i++) {
        char str[25] = {i, '\0'};
        strcpy(names[j++], str);
    }

    for (k = 0; k < V; k++) {
        for (i = 0; i < V; i++) {
            for (j = 0; j < V; j++) {
                tablas_intermedias[k][i][j] = INF;
            }
        }
    }

    generar_grafo();
    generar_imagen_grafo();
    generar_imagen_rutas();

    floyd();

    gtk_init(&argc, &argv);
    myCSS();

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/floyd.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    inicio = GTK_WIDGET(gtk_builder_get_object(builder, "inicio"));
    final = GTK_WIDGET(gtk_builder_get_object(builder, "final"));
    widgets->w_sbtn_quantity = GTK_WIDGET(gtk_builder_get_object(builder, "sbtn_quantity"));
    widgets->w_grid_nodes = GTK_WIDGET(gtk_builder_get_object(builder, "cont_grid"));
    widgets->w_midgrid_nodes = GTK_WIDGET(gtk_builder_get_object(builder, "cont_tablas_intermedias"));
    widgets->w_midgrid_nodes = GTK_WIDGET(gtk_builder_get_object(builder, "cont_tablas_intermedias"));

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
