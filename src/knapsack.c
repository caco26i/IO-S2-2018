#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CANTIDAD_OBJETOS_MAX 20
#define W 1000
int w;
int CANTIDAD_OBJETOS = CANTIDAD_OBJETOS_MAX;

typedef struct {
    char *name;
    int weight;
    int value;
    int count;
} item_t;

typedef struct {
    GtkWidget *name;
    GtkWidget *weight;
    GtkWidget *value;
    GtkWidget *count;
} w_item_t;

typedef struct {
    GtkWidget *w_grid_knapsack;
    GtkWidget *w_midgrid_nodes;
    GtkWidget *sbtn_quantity_count;
    GtkWidget *sbtn_quantity_w;
    GtkWidget *radio10;
    GtkWidget *radioUnbounded;
    GtkWidget *radioBounded;
    GtkWidget *labelCount;

} app_widgets;

typedef struct {
    char *color;
    int valor;
} TABLA;

item_t items[CANTIDAD_OBJETOS_MAX];
TABLA tabla[W][CANTIDAD_OBJETOS_MAX];

w_item_t w_items[CANTIDAD_OBJETOS_MAX];

GtkWidget *file_chooser, *save, *mainbox, *midbox;
app_widgets *widgets;
GtkWidget *window;
GtkWidget *fMatematica;
GtkWidget *restricciones;

//int n = sizeof (items) / sizeof (item_t);
int *count;
int *best;
double best_value;

// solution for 0-1 Knapsack problem

// A utility function that returns maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }

void update(int CANTIDAD_OBJETOS);
//void updateAux();

// Prints the items which are put in a knapsack of capacity W
//void knapsack1_0(int W, int wt[], int val[], int n)
//{
//    int i, w;
//    int K[n + 1][W + 1];
//
//    // Build table K[][] in bottom up manner
//    for (i = 0; i <= n; i++) {
//        for (w = 0; w <= W; w++) {
//            if (i == 0 || w == 0)
//                K[i][w] = 0;
//            else if (wt[i - 1] <= w)
//                K[i][w] = max(val[i - 1] +
//                              K[i - 1][w - wt[i - 1]], K[i - 1][w]);
//            else
//                K[i][w] = K[i - 1][w];
//        }
//    }
//
//    // stores the result of Knapsack
//    int res = K[n][W];
//    printf("%d\n", res);
//
//    w = W;
//    for (i = n; i > 0 && res > 0; i--) {
//
//        // either the result comes from the top
//        // (K[i-1][w]) or from (val[i-1] + K[i-1]
//        // [w-wt[i-1]]) as in Knapsack table. If
//        // it comes from the latter one/ it means
//        // the item is included.
//        if (res == K[i - 1][w])
//            continue;
//        else {
//
//            // This item is included.
//            printf("%d ", wt[i - 1]);
//
//            // Since this weight is included its
//            // value is deducted
//            res = res - val[i - 1];
//            w = w - wt[i - 1];
//        }
//    }
//}

int *knapsack0_1(int w) {
    int i, j, a, b, *mm, **m, *s;
    mm = calloc((CANTIDAD_OBJETOS + 1) * (w + 1), sizeof(int));
    m = malloc((CANTIDAD_OBJETOS + 1) * sizeof(int *));
    m[0] = mm;

    int K[W+1][CANTIDAD_OBJETOS+1];

    for (i = 1; i <= CANTIDAD_OBJETOS; i++) {
        m[i] = &mm[i * (w + 1)];
        for (j = 0; j <= w; j++) {
            if (items[i - 1].weight > j) {
                m[i][j] = m[i - 1][j];
                tabla[j][i-1].color = "red";
            } else {
                a = m[i - 1][j];
                b = m[i - 1][j - items[i - 1].weight] + items[i - 1].value;
                if (a > b) {
                    m[i][j] = a;
                    tabla[j][i-1].color = "red";
                } else {
                    m[i][j] = b;
                    tabla[j][i-1].color = "green";
                }
            }
            tabla[j][i-1].valor = m[i][j];
        }
    }

    s = calloc(CANTIDAD_OBJETOS, sizeof(int));
    for (i = CANTIDAD_OBJETOS, j = w; i > 0; i--) {
        if (m[i][j] > m[i - 1][j]) {
            s[i - 1] = 1;
            j -= items[i - 1].weight;
        }
    }
    free(mm);
    free(m);
    return s;
}

int *boundedKnapsack(int w) {
    int i, j, k, v, *mm, **m, *s;
    mm = calloc((CANTIDAD_OBJETOS + 1) * (w + 1), sizeof(int));
    m = malloc((CANTIDAD_OBJETOS + 1) * sizeof(int *));
    m[0] = mm;
    for (i = 1; i <= CANTIDAD_OBJETOS; i++) {
        m[i] = &mm[i * (w + 1)];
        for (j = 0; j <= w; j++) {
            m[i][j] = m[i - 1][j];
            for (k = 1; k <= items[i - 1].count; k++) {
                if (k * items[i - 1].weight > j) {
                    break;
                }
                v = m[i - 1][j - k * items[i - 1].weight] + k * items[i - 1].value;
                if (v > m[i][j]) {
                    m[i][j] = v;
                }
            }

            tabla[j][i-1].valor = m[i][j];
        }
    }
    s = calloc(CANTIDAD_OBJETOS, sizeof(int));
    for (i = CANTIDAD_OBJETOS, j = w; i > 0; i--) {
        int v = m[i][j];
        for (k = 0; v != m[i - 1][j] + k * items[i - 1].value; k++) {
            s[i - 1]++;
            j -= items[i - 1].weight;
        }
    }
    free(mm);
    free(m);
    return s;
}

int unboundedKnapsack(int w) {
    // dp[i] is going to store maximum value
    // with knapsack capacity i.
    int dp[w + 1];
    memset(dp, 0, sizeof dp);

    // Fill dp[] using above recursive formula
    for (int i = 0; i <= w; i++)
        for (int j = 0; j < CANTIDAD_OBJETOS; j++) {
            best[j] = 0;
            if (items[j].weight <= i) {
                int newVal = dp[i - items[j].weight] + items[j].value;
                if (newVal > dp[i]) {
                    dp[i] = newVal;
                    best[j] = w / items[j].weight;
                }
            }
        }
    best_value = dp[w];
    return dp[w];
}

void update_values_knapsack(GtkEntry *entry) {
    int length, k;
    int i = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(entry), "i"));
    const gchar *value = gtk_entry_get_text(entry);

    items[i].name = (char *) gtk_entry_get_text(GTK_ENTRY(w_items[i].name));
    items[i].weight = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w_items[i].weight));
    items[i].value = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w_items[i].value));
    items[i].count = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w_items[i].count));
}

void updateAux(){
  CANTIDAD_OBJETOS = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->sbtn_quantity_count));
  update(CANTIDAD_OBJETOS);
  //return CANTIDAD_OBJETOS;
}

void update(int CA_OBS) {
    int i;

    CANTIDAD_OBJETOS = CA_OBS;

    w = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->sbtn_quantity_w));


    for (i = 0; i < CANTIDAD_OBJETOS_MAX; i++) {
        if (i < CANTIDAD_OBJETOS) {
            gtk_widget_show(GTK_WIDGET(w_items[i].name));
            gtk_widget_show(GTK_WIDGET(w_items[i].weight));
            gtk_widget_show(GTK_WIDGET(w_items[i].value));
            gtk_widget_show(GTK_WIDGET(w_items[i].count));
        } else {
            gtk_widget_hide(GTK_WIDGET(w_items[i].name));
            gtk_widget_hide(GTK_WIDGET(w_items[i].weight));
            gtk_widget_hide(GTK_WIDGET(w_items[i].value));
            gtk_widget_hide(GTK_WIDGET(w_items[i].count));
        }
    }
}

void actualizar_algoritmo() {
    int i;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->radio10)) ||
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->radioUnbounded))) {
        for (i = 0; i < CANTIDAD_OBJETOS; i++) {
            gtk_widget_hide(GTK_WIDGET(widgets->labelCount));
            gtk_widget_hide(GTK_WIDGET(w_items[i].count));
        }
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->radioBounded))) {
        for (i = 0; i < CANTIDAD_OBJETOS; i++) {
            gtk_widget_show(GTK_WIDGET(widgets->labelCount));
            gtk_widget_show(GTK_WIDGET(w_items[i].count));
        }
    }
}


void print_solution() {
    int i, j;
    int tw = 0, tv = 0, *s;
    char buffer[250];
    GtkWidget *entry;


    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(widgets->w_midgrid_nodes));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    midbox = gtk_grid_new();
    gtk_widget_set_halign(midbox, GTK_ALIGN_CENTER);

    entry= gtk_label_new("SOLUCIÓN OPTIMA:");
    gtk_grid_attach(GTK_GRID(midbox), entry, 0, 0, 1, 1);
    gtk_container_add(GTK_CONTAINER(widgets->w_midgrid_nodes), midbox);


    midbox = gtk_grid_new();
    gtk_widget_set_halign(midbox, GTK_ALIGN_CENTER);

    int l = 0;
    int k = 0;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->radio10))) {
        //< KNAPSACK 1/0
        printf("\n\nKNAPSACK 1/0\n");

        s = knapsack0_1(w);

        for (i = 0; i < CANTIDAD_OBJETOS; i++) {
            if (s[i]) {
                sprintf(buffer, "%-22s %5d %5d\n", items[i].name, items[i].weight, items[i].value);
                entry= gtk_label_new(buffer);
                gtk_grid_attach(GTK_GRID(midbox), entry, k++, l, 1, 1);

                l++;
                k=0;

                printf("%-22s %5d %5d\n", items[i].name, items[i].weight, items[i].value);
                tw += items[i].weight;
                tv += items[i].value;
            }
        }

        sprintf(buffer, "%-22s %5d %5d\n", "PESO, VALOR:", tw, tv);
        entry= gtk_label_new(buffer);
        gtk_grid_attach(GTK_GRID(midbox), entry, k++, l, 1, 1);

        gtk_container_add(GTK_CONTAINER(widgets->w_midgrid_nodes), midbox);

        printf("%-22s %5d %5d\n", "weight, value:", tw, tv);

        midbox = gtk_grid_new();
        gtk_widget_set_halign(midbox, GTK_ALIGN_CENTER);
//        gtk_grid_set_column_spacing (GTK_GRID(midbox), 10);

        for (i = 0; i <= w; i++) {
            for (j = 0; j < CANTIDAD_OBJETOS; j++) {
                sprintf(buffer, " %d ", tabla[i][j].valor);
                entry= gtk_label_new(buffer);
                gtk_grid_attach(GTK_GRID(midbox), entry, j, i, 1, 1);

                printf("%2d %s", tabla[i][j].valor, tabla[i][j].color);
            }
            printf("\n");
        }
        //</ KNAPSACK 1/0
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->radioBounded))) {
        //< KNAPSACK bounded
        printf("\n\nKNAPSACK bounded\n");

        int tc = 0;
        tw = 0, tv = 0;

        s = boundedKnapsack(w);
        for (i = 0; i < CANTIDAD_OBJETOS; i++) {
            if (s[i]) {
                printf("%-22s %5d %5d %5d\n", items[i].name, s[i], s[i] * items[i].weight, s[i] * items[i].value);

                sprintf(buffer, "%-22s %5d %5d %5d\n", items[i].name, s[i], s[i] * items[i].weight, s[i] * items[i].value);
                entry= gtk_label_new(buffer);
                gtk_grid_attach(GTK_GRID(midbox), entry, k++, l, 1, 1);

                tc += s[i];
                tw += s[i] * items[i].weight;
                tv += s[i] * items[i].value;

                l++;
                k=0;
            }
        }
        sprintf(buffer, "%-22s %5d %5d %5d\n", "CANTIDAD, PESO, VALOR:", tc, tw, tv);
        entry= gtk_label_new(buffer);
        gtk_grid_attach(GTK_GRID(midbox), entry, k++, l, 1, 1);

        printf("%-22s %5d %5d %5d\n", "count, weight, value:", tc, tw, tv);
        //</ KNAPSACK bounded
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->radioUnbounded))) {
        //< KNAPSACK unbounded
        printf("\n\nKNAPSACK unbounded\n");

        count = malloc(CANTIDAD_OBJETOS * sizeof(int));
        best = malloc(CANTIDAD_OBJETOS * sizeof(int));
        best_value = 0;
        unboundedKnapsack(w);
        for (i = 0; i < CANTIDAD_OBJETOS; i++) {
            printf("%d %s\n", best[i], items[i].name);


            sprintf(buffer, "Llevar %d %s\n", best[i], items[i].name);
            entry= gtk_label_new(buffer);
            gtk_grid_attach(GTK_GRID(midbox), entry, k++, l, 1, 1);

            l++;
            k=0;

        }
        sprintf(buffer, "MEJOR VALOR: %.0f\n", best_value);
        entry= gtk_label_new(buffer);
        gtk_grid_attach(GTK_GRID(midbox), entry, k++, l, 1, 1);

        printf("best value: %.0f\n", best_value);
        free(count);
        free(best);
        //</ KNAPSACK unbounded
    }

    gtk_container_add(GTK_CONTAINER(widgets->w_midgrid_nodes), midbox);
    gtk_widget_show_all(widgets->w_midgrid_nodes);
}
void forma_matematica() {

  int i, j;
  char funcion[100000];
  char restriccion[100000];
  char str[100];
  char str2[100];


  //strcat(funcion, "Z = ");
  sprintf(funcion, "%s", "Z = ");
  //printf("hola\n");
  //printf("%s", funcion);
  for (i = 0; i < CANTIDAD_OBJETOS; i++) {
      //printf("entro\n");
      sprintf(str, "%d", items[i].value);
      sprintf(funcion, "%s%s", funcion, str);
      sprintf(funcion, "%s(%s", funcion, "X");
      //strcat(funcion, "X");
      sprintf(funcion, "%s%s)", funcion, items[i].name);
      //strcat(funcion, items[i].name);
      sprintf(funcion, "%s %s", funcion, " + ");
      //strcat(funcion, " + ");
  }
  printf("%s\n", funcion);

  sprintf(restriccion, "%s", " ");
  for (j = 0; j < CANTIDAD_OBJETOS; j++) {
      sprintf(str2, "%d", items[j].weight);
      sprintf(restriccion, "%s%s", restriccion, str2);
      //strcat(restriccion, str2);
      //strcat(restriccion, "X");
      sprintf(restriccion, "%s(%s", restriccion, "X");
      //strcat(restriccion, );
      sprintf(restriccion, "%s%s)", restriccion, items[j].name);
      sprintf(restriccion, "%s%s", restriccion, " + ");
      //strcat(restriccion, " + ");
  }
  sprintf(restriccion, "%s%s", restriccion, " ≤ ");
  //strcat(restriccion, " ≤ ");
  sprintf(str2, "%d", w);
  sprintf(restriccion, "%s%s", restriccion, str2);
  //strcat(restriccion, str2);
  printf("%s\n", restriccion);

  gtk_label_set_text(GTK_LABEL(fMatematica), funcion);
  gtk_label_set_text(GTK_LABEL(restricciones), restriccion);
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    widgets = g_slice_new(app_widgets);
    int i;

    w = 200;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/knapsack.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    widgets->w_grid_knapsack = GTK_WIDGET(gtk_builder_get_object(builder, "cont_grid"));
    widgets->w_midgrid_nodes = GTK_WIDGET(gtk_builder_get_object(builder, "cont_solution"));

    widgets->sbtn_quantity_count = GTK_WIDGET(gtk_builder_get_object(builder, "sbtn_quantity_count"));
    widgets->sbtn_quantity_w = GTK_WIDGET(gtk_builder_get_object(builder, "sbtn_quantity_w"));
    widgets->radio10 = GTK_WIDGET(gtk_builder_get_object(builder, "radio10"));
    widgets->radioBounded = GTK_WIDGET(gtk_builder_get_object(builder, "radioBounded"));
    widgets->radioUnbounded = GTK_WIDGET(gtk_builder_get_object(builder, "radioUnbounded"));
    fMatematica = GTK_WIDGET(gtk_builder_get_object(builder, "fMatematica"));
    restricciones = GTK_WIDGET(gtk_builder_get_object(builder, "restricciones"));

    save = GTK_WIDGET(gtk_builder_get_object(builder, "save"));
    gtk_widget_set_sensitive(save, FALSE);

    g_object_unref(builder);



    mainbox = gtk_grid_new();
    gtk_widget_set_halign(mainbox, GTK_ALIGN_CENTER);
    midbox = gtk_grid_new();
    gtk_widget_set_halign(mainbox, GTK_ALIGN_CENTER);

    item_t items[CANTIDAD_OBJETOS];

    GtkWidget *label = gtk_label_new("NOMBRE  ");
    gtk_grid_attach(GTK_GRID(mainbox), label, 1, 0, 1, 1);
    label = gtk_label_new("PESO  ");
    gtk_grid_attach(GTK_GRID(mainbox), label, 2, 0, 1, 1);
    label = gtk_label_new("VALOR  ");
    gtk_grid_attach(GTK_GRID(mainbox), label, 3, 0, 1, 1);
    label = gtk_label_new("CANTIDAD  ");
    widgets->labelCount = label;
    gtk_grid_attach(GTK_GRID(mainbox), label, 4, 0, 1, 1);

    char buffer[10];
    for (i = 0; i < CANTIDAD_OBJETOS; i++) {
        GtkWidget *button;
        GtkAdjustment *adjustment;


        sprintf(buffer, "ITEM #%d", i + 1); // modified to append string

        GtkWidget *entry = gtk_entry_new();
        g_object_set_data(G_OBJECT(entry), "i", GINT_TO_POINTER(i));
        w_items[i].name = entry;
        gtk_entry_set_text(GTK_ENTRY(entry), buffer);
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 10);
        gtk_grid_attach(GTK_GRID(mainbox), entry, 1, i + 1, 1, 1);
        g_signal_connect(entry, "changed", G_CALLBACK(update_values_knapsack), NULL);

        //peso
        adjustment = gtk_adjustment_new(0, 0, 999999999999999, 1, 1, 0);
        entry = gtk_spin_button_new(adjustment, 0, 0);
        g_object_set_data(G_OBJECT(entry), "i", GINT_TO_POINTER(i));
        w_items[i].weight = entry;
        gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(entry), true);
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
        gtk_grid_attach(GTK_GRID(mainbox), entry, 2, i + 1, 1, 1);
        g_signal_connect(entry, "changed", G_CALLBACK(update_values_knapsack), NULL);

        //valor
        adjustment = gtk_adjustment_new(0, 0, 999999999999999, 1, 1, 0);
        entry = gtk_spin_button_new(adjustment, 0, 0);
        g_object_set_data(G_OBJECT(entry), "i", GINT_TO_POINTER(i));
        w_items[i].value = entry;
        gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(entry), true);
        gtk_entry_set_text(GTK_ENTRY(entry), "Valor");
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
        gtk_grid_attach(GTK_GRID(mainbox), entry, 3, i + 1, 1, 1);
        g_signal_connect(entry, "changed", G_CALLBACK(update_values_knapsack), NULL);

        //cantidad
        adjustment = gtk_adjustment_new(0, 0, 999999999999999, 1, 1, 0);
        entry = gtk_spin_button_new(adjustment, 0, 0);
        g_object_set_data(G_OBJECT(entry), "i", GINT_TO_POINTER(i));
        w_items[i].count = entry;
        gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(entry), true);
        gtk_entry_set_text(GTK_ENTRY(entry), "Valor");
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
        gtk_grid_attach(GTK_GRID(mainbox), entry, 4, i + 1, 1, 1);
        g_signal_connect(entry, "changed", G_CALLBACK(update_values_knapsack), NULL);
    }

    actualizar_algoritmo();
    gtk_container_add(GTK_CONTAINER(widgets->w_grid_knapsack), mainbox);
    gtk_widget_show_all(widgets->w_grid_knapsack);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void generar_archivo() {
    int i, j;

    GtkWidget *dialog;
    char *filename = NULL;
    FILE *fichero;
    dialog = gtk_file_chooser_dialog_new("Guardar archivo", GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         ("Cancelar"), GTK_RESPONSE_CANCEL,
                                         ("Guardar"), GTK_RESPONSE_ACCEPT, NULL);
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
    gint answer = gtk_dialog_run(GTK_DIALOG(dialog));
    if (answer == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        fichero = fopen(filename, "w+");
        printf("CANTIDAD_OBJETOS: %d\n", CANTIDAD_OBJETOS);
        for (i = 0; i < CANTIDAD_OBJETOS; i++) {
            fprintf(fichero, "%s,", (char *) items[i].name);
            fprintf(fichero, "%i,", items[i].weight);
            fprintf(fichero, "%i,", items[i].value);
            fprintf(fichero, "%i", items[i].count);
            fprintf(fichero, "\n");
        }
        fclose(fichero);
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
    gtk_widget_set_sensitive(save, FALSE);
}

void leer_archivo() {
    int i, j = 0, k, fileSize = 0, entrySize;
    char *filename = NULL;
    GtkWidget *dialog;
    FILE *fichero;
    FILE *fichero2;

    char str[1024];
    char str2[1024];

    gtk_widget_set_sensitive(save, FALSE);
    dialog = gtk_file_chooser_dialog_new("Abrir archivo", GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         ("Cancelar"), GTK_RESPONSE_CANCEL,
                                         ("Abrir"), GTK_RESPONSE_ACCEPT, NULL);

    gint answer = gtk_dialog_run(GTK_DIALOG(dialog));
    if (answer == GTK_RESPONSE_ACCEPT){
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        fichero = fopen(filename, "r");
        fichero2 = fopen(filename, "r");
        //printf("archivo abierto\n");

        while (fscanf(fichero2, "%s", str2)!=EOF) {
            i = 0;
            fgets(str, 1024, fichero);

            char delims[] = ",";
            char *result = NULL;
            result = strtok(str, delims);

            while( result != NULL ) {
                if (i == 0) {
                    items[j].name = result;
                    gtk_entry_set_text(GTK_ENTRY(w_items[j].name), result);
                } else if (i == 1) {
                    items[j].weight = atoi(result);
                    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w_items[j].weight), atoi(result));
                } else if (i == 2) {
                    items[j].value = atoi(result);
                    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w_items[j].value), atoi(result));
                } else if (i == 3) {
                    items[j].count = atoi(result);
                    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w_items[j].count), atoi(result));
                }
                result = strtok(NULL, delims);
                i++;
            }
            j++;
            fileSize++;
        }
        entrySize = fileSize;
        update(entrySize);

        fclose(fichero);
        fclose(fichero2);
    }
    //printf("salgo\n");
    gtk_widget_destroy(dialog);

    //g_free (filename);
}

void on_press_file_chooser() {
    gtk_widget_show(file_chooser);
}

// called when window is closed
void on_window_main_destroy() {
    gtk_main_quit();
}

void on_file_chooser_destroy() {
    gtk_widget_hide_on_delete(file_chooser);
}
