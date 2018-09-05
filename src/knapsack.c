#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CANTIDAD_OBJETOS_MAX 10

int W = 20;
int CANTIDAD_OBJETOS = 10;

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
} app_widgets;

item_t items[] = {
        {"map",                      9,   150,   1},
        {"compass",                 13,    35,   1},
        {"water",                  153,   200,   2},
        {"sandwich",                50,    60,   2},
        {"glucose",                 15,    60,   2},
        {"tin",                     68,    45,   3},
        {"banana",                  27,    60,   3},
        {"apple",                   39,    40,   3},
        {"cheese",                  23,    30,   1},
        {"beer",                    52,    10,   3},
        {"suntan cream",            11,    70,   1},
        {"camera",                  32,    30,   1},
        {"T-shirt",                 24,    15,   2},
        {"trousers",                48,    10,   2},
        {"umbrella",                73,    40,   1},
        {"waterproof trousers",     42,    70,   1},
        {"waterproof overclothes",  43,    75,   1},
        {"note-case",               22,    80,   1},
        {"sunglasses",               7,    20,   1},
        {"towel",                   18,    12,   2},
        {"socks",                    4,    50,   1},
        {"book",                    30,    10,   2},
};

w_item_t w_items[CANTIDAD_OBJETOS_MAX];

GtkWidget       *file_chooser, *save, *mainbox;
app_widgets *widgets;
GtkWidget       *window;

int n = sizeof (items) / sizeof (item_t);
int *count;
int *best;
double best_value;

// solution for 0-1 Knapsack problem

// A utility function that returns maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }

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

int *knapsack0_1 (int w) {
    int i, j, a, b, *mm, **m, *s;
    mm = calloc((n + 1) * (w + 1), sizeof (int));
    m = malloc((n + 1) * sizeof (int *));
    m[0] = mm;
    for (i = 1; i <= n; i++) {
        m[i] = &mm[i * (w + 1)];
        for (j = 0; j <= w; j++) {
            if (items[i - 1].weight > j) {
                m[i][j] = m[i - 1][j];
            }
            else {
                a = m[i - 1][j];
                b = m[i - 1][j - items[i - 1].weight] + items[i - 1].value;
                m[i][j] = a > b ? a : b;
            }
        }
    }
    s = calloc(n, sizeof (int));
    for (i = n, j = w; i > 0; i--) {
        if (m[i][j] > m[i - 1][j]) {
            s[i - 1] = 1;
            j -= items[i - 1].weight;
        }
    }
    free(mm);
    free(m);
    return s;
}

int *boundedKnapsack (int w) {
    int i, j, k, v, *mm, **m, *s;
    mm = calloc((n + 1) * (w + 1), sizeof (int));
    m = malloc((n + 1) * sizeof (int *));
    m[0] = mm;
    for (i = 1; i <= n; i++) {
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
        }
    }
    s = calloc(n, sizeof (int));
    for (i = n, j = w; i > 0; i--) {
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

// Returns the maximum value with knapsack of
// W capacity


void unboundedKnapsackAux (int i, double value, double w) {
    int j, m1, m2, m;
    if (i == n) {
        if (value > best_value) {
            best_value = value;
            for (j = 0; j < n; j++) {
                best[j] = count[j];
            }
        }
        return;
    }
    m = w / items[i].weight;
    for (count[i] = m; count[i] >= 0; count[i]--) {
        unboundedKnapsackAux(
                i+1,
                value + count[i] * items[i].value,
                w - count[i] * items[i].weight
        );
    }
}
void unboundedKnapsack(int w) {
        unboundedKnapsackAux (0, 0, w);
};


int unboundedKnapsack2(int W)
{
    // dp[i] is going to store maximum value
    // with knapsack capacity i.
    int dp[W+1];
    memset(dp, 0, sizeof dp);

    // Fill dp[] using above recursive formula
    for (int i=0; i<=W; i++)
        for (int j=0; j<n; j++) {
            best[j] = 0;
            if (items[j].weight <= i) {
                int newVal = dp[i - items[j].weight] + items[j].value;
                if(newVal > dp[i]){
                    dp[i] = newVal;
                    best[j] = W / items[j].weight;
                }
            }
        }
    best_value = dp[W];
    return dp[W];
}

void update_values_knapsack(GtkEntry *entry) {
    int length, k;
    int i = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(entry), "i"));
    const gchar * value = gtk_entry_get_text(entry);

    items[i].name = (char *) gtk_entry_get_text(GTK_ENTRY(w_items[i].name));
    items[i].weight = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w_items[i].weight));
    items[i].value = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w_items[i].value));
    items[i].count = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w_items[i].count));
}


int main(int argc, char *argv[])
{
    GtkBuilder      *builder;


    widgets = g_slice_new(app_widgets);

    int w = 150;

    //< KNAPSACK 1/0
    printf("\n\nKNAPSACK 1/0\n");
    int i, n, tw = 0, tv = 0, *s;
    n = sizeof (items) / sizeof (item_t);
    s = knapsack0_1(w);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/knapsack.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    for (i = 0; i < n; i++) {
        if (s[i]) {
            printf("%-22s %5d %5d\n", items[i].name, items[i].weight, items[i].value);
            tw += items[i].weight;
            tv += items[i].value;
        }
    }
    printf("%-22s %5d %5d\n", "weight, value:", tw, tv);

    //</ KNAPSACK 1/0

    //< KNAPSACK bounded
    printf("\n\nKNAPSACK bounded\n");

    int tc = 0;
    tw = 0, tv = 0;

    s = boundedKnapsack(w);
    for (i = 0; i < n; i++) {
        if (s[i]) {
            printf("%-22s %5d %5d %5d\n", items[i].name, s[i], s[i] * items[i].weight, s[i] * items[i].value);
            tc += s[i];
            tw += s[i] * items[i].weight;
            tv += s[i] * items[i].value;
        }
    }
    printf("%-22s %5d %5d %5d\n", "count, weight, value:", tc, tw, tv);
    //</ KNAPSACK bounded

    //< KNAPSACK unbounded
    printf("\n\nKNAPSACK unbounded\n");

    count = malloc(n * sizeof (int));
    best = malloc(n * sizeof (int));
    best_value = 0;
    unboundedKnapsack(w);

    for (i = 0; i < n; i++) {
        printf("%d %s\n", best[i], items[i].name);
    }
    printf("best value: %.0f\n", best_value);
    free(count); free(best);
    //</ KNAPSACK unbounded

    //< KNAPSACK unbounded 2
    printf("\n\nKNAPSACK unbounded 2\n");

    count = malloc(n * sizeof (int));
    best = malloc(n * sizeof (int));
    best_value = 0;
    unboundedKnapsack2(w);
    for (i = 0; i < n; i++) {
        printf("%d %s\n", best[i], items[i].name);
    }
    printf("best value: %.0f\n", best_value);
    free(count); free(best);
    //</ KNAPSACK unbounded


    file_chooser = GTK_WIDGET(gtk_builder_get_object(builder, "file_chooser"));

    widgets->w_grid_knapsack = GTK_WIDGET(gtk_builder_get_object(builder, "cont_grid"));

    save=GTK_WIDGET(gtk_builder_get_object(builder,"save"));
  	gtk_widget_set_sensitive(save,FALSE);

    g_object_unref(builder);

    mainbox = gtk_grid_new();
    gtk_widget_set_halign(mainbox, GTK_ALIGN_CENTER);

    item_t items[CANTIDAD_OBJETOS];

    GtkWidget *label = gtk_label_new("NOMBRE  ");
    gtk_grid_attach(GTK_GRID(mainbox), label, 1, 0, 1, 1);
    label = gtk_label_new("PESO  ");
    gtk_grid_attach(GTK_GRID(mainbox), label, 2, 0, 1, 1);
    label = gtk_label_new("VALOR  ");
    gtk_grid_attach(GTK_GRID(mainbox), label, 3, 0, 1, 1);
    label = gtk_label_new("CANTIDAD  ");
    gtk_grid_attach(GTK_GRID(mainbox), label, 4, 0, 1, 1);

    char buffer[10];
    for (i = 0; i < CANTIDAD_OBJETOS; i++) {
        GtkWidget *button;
        GtkAdjustment *adjustment;


        sprintf(buffer, "ITEM #%d", i+1); // modified to append string

        GtkWidget *label = gtk_label_new(buffer);
        gtk_grid_attach(GTK_GRID(mainbox), label, 0, i+1, 1, 1);


        GtkWidget *entry = gtk_entry_new();
        g_object_set_data(G_OBJECT(entry), "i", GINT_TO_POINTER(i));
        w_items[i].name = entry;
        gtk_entry_set_text(GTK_ENTRY(entry), buffer);
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 10);
        gtk_grid_attach(GTK_GRID(mainbox), entry, 1, i+1, 1, 1);
        g_signal_connect(entry, "changed", G_CALLBACK(update_values_knapsack), NULL);

        //peso
        adjustment = gtk_adjustment_new (0, 0, 999999999999999, 1, 1, 0);
        entry = gtk_spin_button_new (adjustment, 0, 0);
        g_object_set_data(G_OBJECT(entry), "i", GINT_TO_POINTER(i));
        w_items[i].weight = entry;
        gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(entry), true);
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
        gtk_grid_attach(GTK_GRID(mainbox), entry, 2, i+1, 1, 1);
        g_signal_connect(entry, "changed", G_CALLBACK(update_values_knapsack), NULL);

        //valor
        adjustment = gtk_adjustment_new (0, 0, 999999999999999, 1, 1, 0);
        entry = gtk_spin_button_new (adjustment, 0, 0);
        g_object_set_data(G_OBJECT(entry), "i", GINT_TO_POINTER(i));
        w_items[i].value = entry;
        gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(entry), true);
        gtk_entry_set_text(GTK_ENTRY(entry), "Valor");
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
        gtk_grid_attach(GTK_GRID(mainbox), entry, 3, i+1, 1, 1);
        g_signal_connect(entry, "changed", G_CALLBACK(update_values_knapsack), NULL);

        //cantidad
        adjustment = gtk_adjustment_new (0, 0, 999999999999999, 1, 1, 0);
        entry = gtk_spin_button_new (adjustment, 0, 0);
        g_object_set_data(G_OBJECT(entry), "i", GINT_TO_POINTER(i));
        w_items[i].count = entry;
        gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(entry), true);
        gtk_entry_set_text(GTK_ENTRY(entry), "Valor");
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
        gtk_grid_attach(GTK_GRID(mainbox), entry, 4, i+1, 1, 1);
        g_signal_connect(entry, "changed", G_CALLBACK(update_values_knapsack), NULL);
    }

    gtk_container_add(GTK_CONTAINER(widgets->w_grid_knapsack), mainbox);
    gtk_widget_show_all(widgets->w_grid_knapsack);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void generar_archivo() {
    int i, j;

    GtkWidget* dialog;
  	char* filename = NULL;
  	FILE* fichero;
  	dialog = gtk_file_chooser_dialog_new("Guardar archivo", GTK_WINDOW(window),
                              						GTK_FILE_CHOOSER_ACTION_SAVE,
                              						("Cancelar"),GTK_RESPONSE_CANCEL,
                              						("Guardar"),GTK_RESPONSE_ACCEPT,NULL);
  	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
  	gint answer = gtk_dialog_run(GTK_DIALOG(dialog));
  	if(answer == GTK_RESPONSE_ACCEPT){
  		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  		fichero = fopen(filename,"w+");
  		//fprintf(fichero,"%i\n%i\n",rows,columns);
  		//for(i=0;i<rows;i++){
  			//for(j=0;j<columns;j++){
  				//fprintf(fichero,"%i\t",spanningTree[i][j]);
<<<<<<< HEAD
  			//}
  			fprintf(fichero,"\n");
  		//}
=======

  			fprintf(fichero,"\n");

>>>>>>> master
  	}
  	fclose(fichero);
  	g_free(filename);
  	gtk_widget_destroy(dialog);
  	gtk_widget_set_sensitive(save,FALSE);

    /*FILE *fichero;
    fichero = fopen("knapsack.csv", "w+");
    //fputs("prueba {\n", fichero);

    for (i = 0; i < 5; i++) {
        fprintf(fichero, "%d,", i);
    }
    //fputs("}", fichero);
    fclose(fichero);*/
}

void leer_archivo(/*char filename*/) {

    int i, j;
    char *filename = NULL;
    GtkWidget *dialog;
    FILE *fichero;
    gtk_widget_set_sensitive(save, FALSE);
    dialog = gtk_file_chooser_dialog_new("Abrir archivo", GTK_WINDOW(window),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          ("Cancelar"), GTK_RESPONSE_CANCEL,
                                          ("Abrir"), GTK_RESPONSE_ACCEPT, NULL);

    gint answer = gtk_dialog_run(GTK_DIALOG(dialog));
    if (answer == GTK_RESPONSE_ACCEPT){
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        fichero = fopen(filename, "r");
        //fscanf(fichero, );
        printf("archivo abierto\n");
    }

    gtk_widget_destroy(dialog);
    fclose(fichero);
    g_free (filename);
    gtk_widget_show(window);
}

void on_press_file_chooser() {
  gtk_widget_show (file_chooser);
}

// called when window is closed
void on_window_main_destroy() {
    gtk_main_quit();
}

void on_file_chooser_destroy()
{
    gtk_widget_hide_on_delete(file_chooser);
}
