#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 999999999

#define V 10

typedef struct {
    GtkWidget *w_sbtn_quantity;
    GtkWidget *w_grid_nodes;
} app_widgets;


int cantidad_nodos;

void printSolution(void);

void myCSS(void);

void floyd(void);

int graph[V][V];
int dist[V][V];
char names[V][25];


GtkWidget *frame, *window;
GtkBuilder *builder;
GtkWidget *mainbox;

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
            dist[i][j] = graph[i][j];

    for (k = 0; k < V; k++)
        for (i = 0; i < V; i++)
            for (j = 0; j < V; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];

    // Print the shortest distance matrix
    printSolution();
}

/* A utility function to print solution */
void printSolution() {
    printf("The following matrix shows the shortest distances"
           " between every pair of vertices \n");
    for (int i = 0; i < cantidad_nodos; i++) {
        printf("%7s", names[i]);
        for (int j = 0; j < cantidad_nodos; j++) {
            if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }
}

void generar_grafo() {
    int i, j;

    FILE *fichero;
    fichero = fopen("test.dot", "w+");
    fputs("digraph G {\n", fichero);

    for (i = 0; i < cantidad_nodos; i++){
        fprintf(fichero, "%s\n", names[i]);
        for (j = 0; j < cantidad_nodos; j++)
            if (graph[i][j] != INF)
                fprintf(fichero, "%s -> %s [ label=\"%d\" ];\n", names[i], names[j], graph[i][j]);
    }
    fputs("}", fichero);
    fclose(fichero);
}

void generar_imagen_grafo() {
    system("dot -Tpng test.dot -o src/graph.png");
}

void refresh() {
    generar_grafo();
    generar_imagen_grafo();

    const gchar *style_file = "src/style.css";
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(style_file), 0);
}

void cambiar_nombres_nodos(GtkWidget *entry, int i ){
    printf("%7dasd", i);
}

void on_btn_cambiar_cant_nodos_clicked(GtkButton *buttonn, app_widgets *app_wdgts) {
    int i, j;
    GtkWidget *nbutton;

    int quantity = 0; // stores integer read from spin button widget
    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_sbtn_quantity));
    cantidad_nodos = quantity;

    refresh();
}

int main(int argc, char *argv[]) {
    int i, j, k;

    app_widgets   *widgets = g_slice_new(app_widgets);

    cantidad_nodos = V;



    int test[] = {INF, 5, INF, 10, 3, 4, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF,
                  INF, INF, INF, INF};

    k = 0;
    //Se inicializa el grafo en infinito
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            graph[i][j] = INF;//test[(i + j) % 20];
        }
    }

    //Se inicializan los nombres
    j = 0;
    for (i = 'A'; i < 'A' + V; i++) {
        char str[25] = {i, '\0'};
        strcpy(names[j++], str);
    }

    generar_grafo();
    generar_imagen_grafo();

    floyd();

    gtk_init(&argc, &argv);
    myCSS();

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/floyd.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    widgets->w_sbtn_quantity = GTK_WIDGET(gtk_builder_get_object(builder, "sbtn_quantity"));
    widgets->w_grid_nodes = GTK_WIDGET(gtk_builder_get_object(builder, "cont_grid"));


    gtk_builder_connect_signals(builder, widgets);
    g_object_unref(builder);

    mainbox = gtk_grid_new();

    for (i = 0; i < cantidad_nodos; i++) {
        GtkWidget *entry = gtk_entry_new ();
        gtk_entry_set_text( GTK_ENTRY (entry), names[i] );
        gtk_grid_attach(GTK_GRID(mainbox), entry, 0, i+1, 1, 1);
        g_signal_connect (entry, "icon-release", G_CALLBACK (cambiar_nombres_nodos), &i);


        entry = gtk_entry_new ();
        gtk_entry_set_text( GTK_ENTRY (entry), names[i] );
        gtk_grid_attach(GTK_GRID(mainbox), entry, i+1, 0, 1, 1);
        g_signal_connect (entry, "icon-release", G_CALLBACK (cambiar_nombres_nodos), &i);
    }

    for (i = 1; i <= cantidad_nodos; i++) {
        for (j = 1; j <= cantidad_nodos; j++) {
            GtkWidget *entry = gtk_entry_new ();
            gtk_widget_set_halign(entry, GTK_ALIGN_START);
            gtk_grid_attach(GTK_GRID(mainbox), entry, i, j, 1, 1);
        }
    }

    gtk_container_add(GTK_CONTAINER(widgets->w_grid_nodes), mainbox);
    gtk_widget_show_all(window);

    gtk_widget_show(window);
    gtk_main();

    g_slice_free(app_widgets, widgets);


    return 0;
}

