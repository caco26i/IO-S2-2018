#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <glib.h>
#include <gtk/gtk.h>


//Declaración de ventanas
GtkWidget *window;

GtkBuilder *builder;
GtkWidget *cantJuegos;
GtkWidget *g_Ph;
GtkWidget *g_Pr;
GtkWidget *Acept;
GtkWidget *btn_cargar;

GtkWidget *juego1;
GtkWidget *juego2;
GtkWidget *juego3;
GtkWidget *juego4;
GtkWidget *juego5;
GtkWidget *juego6;
GtkWidget *juego7;
GtkWidget *juego8;
GtkWidget *juego9;
GtkWidget *juego10;
GtkWidget *juego11;

GtkWidget *probabilidades1;
GtkWidget *probabilidades2;
GtkWidget *result;
GtkWidget *entry_cargar;
GtkWidget *btn_calcular;

GtkWidget *folder;
GtkWidget *filenameEntry;
GtkWidget *guardar;


GtkWidget *SalirDelPrograma;

GtkWidget *datos;
GtkWidget *tabla_sol;
GtkWidget *tabla_nuevo;
GtkWidget *grid2;


GtkWidget *label;
GtkWidget *box;

int mitadJuegos = 3;
int totalJuegos = 3;
float ph = 0;
float pr = 0;
float qh = 0;
float qr = 0;

float **tabla;
float resp;


int lugar_juego[11];

float mat[10][10];

//Declaración de funciones
void myCSS(void);


void on_SalirDelPrograma_clicked() {
    gtk_widget_destroy(window);
}


void CrearTabla() {
    int i, j;

    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(tabla_sol));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);


    grid2 = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid2), 2);
    gtk_grid_set_column_spacing(GTK_GRID(grid2), 2);


    for (i = -1; i < (mitadJuegos); i++) {
        for (j = -1; j < (mitadJuegos); j++) {
            if (i == -1) {
                if (j == -1) {
                    label = gtk_label_new("");
                    gtk_widget_set_size_request(label, 470 / (mitadJuegos + 2), 470 / (mitadJuegos + 2));

                    box = gtk_box_new(0, 0);
                    gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
                    const GdkRGBA *color;


                    gtk_grid_attach(GTK_GRID(grid2), box, j + 1, i + 1, 1, 1);

                    gtk_widget_show(label);
                    gtk_widget_show(box);

                } else {
                    char val[30];
                    sprintf(val, "%d", j);

                    label = gtk_label_new(val);
                    gtk_widget_set_size_request(label, 470 / (mitadJuegos + 2), 470 / (mitadJuegos + 2));

                    box = gtk_box_new(0, 0);
                    gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
                    const GdkRGBA *color;

//                    gdk_rgba_parse(color, "#00BFFF");
//                    gtk_widget_modify_bg(box, GTK_STATE_NORMAL, &color);
                    gtk_grid_attach(GTK_GRID(grid2), box, j + 1, i + 1, 1, 1);

                    gtk_widget_show(label);
                    gtk_widget_show(box);
                }
            } else {
                if (j == -1) {
                    char val[30];
                    sprintf(val, "%d", i);

                    label = gtk_label_new(val);
                    gtk_widget_set_size_request(label, 470 / (mitadJuegos + 2), 470 / (mitadJuegos + 2));

                    box = gtk_box_new(0, 0);
                    gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
                    const GdkRGBA *color;

//                    gdk_rgba_parse("#00BFFF", &color);
//                    gtk_widget_modify_bg(box, GTK_STATE_NORMAL, &color);
                    gtk_grid_attach(GTK_GRID(grid2), box, j + 1, i + 1, 1, 1);

                    gtk_widget_show(label);
                    gtk_widget_show(box);
                } else {
                    if ((j == 0) && (i == 0)) {
                        label = gtk_label_new("");
                        gtk_widget_set_size_request(label, 470 / (mitadJuegos + 2), 470 / (mitadJuegos + 2));

                        box = gtk_box_new(0, 0);
                        gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
                        const GdkRGBA *color;
//
//                        gdk_rgba_parse("#A9D0F5", &color);
//                        gtk_widget_modify_bg(box, GTK_STATE_NORMAL, &color);
                        gtk_grid_attach(GTK_GRID(grid2), box, j + 1, i + 1, 1, 1);

                        gtk_widget_show(label);
                        gtk_widget_show(box);
                    }

                    if ((i == mitadJuegos - 1) && (j == mitadJuegos - 1)) {
                        char val[30];
                        sprintf(val, "%.*f", 4, tabla[i][j]);

                        label = gtk_label_new(val);
                        gtk_widget_set_size_request(label, 470 / (mitadJuegos + 2), 470 / (mitadJuegos + 2));

                        box = gtk_box_new(0, 0);
                        gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
                        const GdkRGBA *color;
//
//                        gdk_rgba_parse("#FE2E64", &color);
//                        gtk_widget_modify_bg(box, GTK_STATE_NORMAL, &color);
                        gtk_grid_attach(GTK_GRID(grid2), box, j + 1, i + 1, 1, 1);

                        gtk_widget_show(label);
                        gtk_widget_show(box);

                    } else {
                        char val[30];
                        sprintf(val, "%.*f", 4, tabla[i][j]);


                        label = gtk_label_new(val);
                        gtk_widget_set_size_request(label, 470 / (mitadJuegos + 2), 470 / (mitadJuegos + 2));

                        box = gtk_box_new(0, 0);
                        gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
                        const GdkRGBA *color;

//                        gdk_rgba_parse("#A9D0F5", &color);
//                        gtk_widget_modify_bg(box, GTK_STATE_NORMAL, &color);
                        gtk_grid_attach(GTK_GRID(grid2), box, j + 1, i + 1, 1, 1);

                        gtk_widget_show(label);
                        gtk_widget_show(box);
                    }
                }
            }
        }
    }

    gtk_container_add(GTK_CONTAINER(tabla_sol), grid2);
    gtk_widget_show(grid2);

}

float** createFloatMatrix(int rows, int columns)
{
    int i;

    float **matrix = (float **) calloc(rows, sizeof(float *));
    for (i = 0; i < rows; i++)
    {
        matrix[i] = (float *) calloc(columns, sizeof(float));
    }

    return matrix;
}

int on_btn_calcular_clicked() {
    if (ph == 0 && pr == 0) {
        gtk_label_set_text(GTK_LABEL(result), "Los campos no pueden estar vacíos.");
        return 0;
    }
    for (int i = 0; i < totalJuegos; i++) {
        if (lugar_juego[i] == 0) {
            gtk_label_set_text(GTK_LABEL(result), "Falta un formato de la serie.");
            return 0;
        }
    }

    tabla = createFloatMatrix(mitadJuegos, mitadJuegos);

    for (int i = 0; i < mitadJuegos; i++) {
        for (int j = 0; j < mitadJuegos; j++) {
            if (j == 0) {
                tabla[i][j] = 0;
                continue;
            }
            if (i == 0) {
                tabla[i][j] = 1;
                continue;
            }
            if (lugar_juego[totalJuegos - 1 - (i + j - 2)] == 1) {
                tabla[i][j] = ph * tabla[i - 1][j] + qr * tabla[i][j - 1];
            }

            if (lugar_juego[totalJuegos - 1 - (i + j - 2)] == 2) {
                tabla[i][j] = pr * tabla[i - 1][j] + qh * tabla[i][j - 1];
            }


        }

    }

    char val[120];
    strcpy(val, "La probabilidad de que A gane la serie es de: ");
    char v[30];
    sprintf(v, "%f", tabla[mitadJuegos - 1][mitadJuegos - 1]);
    strcat(val, v);

    CrearTabla();


    gtk_label_set_text(GTK_LABEL(result), val);
}


void writeFile(char *filename) {

    FILE *file;
    file = fopen(filename, "w");


    fprintf(file, "%i\n", mitadJuegos);
    fprintf(file, "%i\n", totalJuegos);
    fprintf(file, "%f\n", ph);
    fprintf(file, "%f\n", pr);
    fprintf(file, "%f\n", qh);
    fprintf(file, "%f\n", qr);

    for (int i = 0; i < totalJuegos; i++) {
        fprintf(file, "%i\n", lugar_juego[i]);
    }

    fclose(file);
}

int on_guardar_clicked() {
    char filename[250];

    const char *name = gtk_entry_get_text(GTK_ENTRY(filenameEntry));

    if (strlen(name) == 0) {
        gtk_label_set_text(GTK_LABEL(result), "Escriba el nombre del archivo.");
        return 0;
    }

    char *folderfile = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(folder));
    if (folderfile == NULL) {
        gtk_label_set_text(GTK_LABEL(result), "Selecione un folder.");
        return 0;
    }

    sprintf(filename, "%s/%s", folderfile, name);

    writeFile(filename);

    gtk_entry_set_text(GTK_ENTRY(filenameEntry), "");
    gtk_label_set_text(GTK_LABEL(result), "Se guardó exitosamente.");

}
void readFile(char *filename) {


    FILE *file;
    file = fopen(filename, "r");

    fscanf(file, "%i", &mitadJuegos);
    fscanf(file, "%i", &totalJuegos);
    fscanf(file, "%f", &ph);
    fscanf(file, "%f", &pr);
    fscanf(file, "%f", &qh);
    fscanf(file, "%f", &qr);


    int i = 0;

    while (i < totalJuegos) {
        fscanf(file, "%i", &lugar_juego[i]);
        i++;
    }

    fclose(file);


    if (totalJuegos == 3) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(cantJuegos), 0);
    }
    if (totalJuegos == 5) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(cantJuegos), 1);
    }
    if (totalJuegos == 7) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(cantJuegos), 2);
    }
    if (totalJuegos == 9) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(cantJuegos), 3);
    }
    if (totalJuegos == 11) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(cantJuegos), 4);
    }

    gtk_combo_box_set_active(GTK_COMBO_BOX(juego1), lugar_juego[0]);
//    gtk_combo_box_set_active(juego2, lugar_juego[1]);
//    gtk_combo_box_set_active(juego3, lugar_juego[2]);
//    gtk_combo_box_set_active(juego4, lugar_juego[3]);
//    gtk_combo_box_set_active(juego5, lugar_juego[4]);
//    gtk_combo_box_set_active(juego6, lugar_juego[5]);
//    gtk_combo_box_set_active(juego7, lugar_juego[6]);
//    gtk_combo_box_set_active(juego8, lugar_juego[7]);
//    gtk_combo_box_set_active(juego9, lugar_juego[8]);
//    gtk_combo_box_set_active(juego10, lugar_juego[9]);
//    gtk_combo_box_set_active(juego11, lugar_juego[10]);

    char array[10];

    int pht = ph * 10000;
    int prt = pr * 10000;


    snprintf(array, sizeof(array), "0.%d", pht);
    gtk_entry_set_text(GTK_ENTRY(g_Ph), array);

    snprintf(array, sizeof(array), "0.%d", prt);
    gtk_entry_set_text(GTK_ENTRY(g_Pr), array);

    char val[40];
    strcpy(val, "Ph = ");
    char v[12];
    sprintf(v, "%f", ph);
    strcat(val, v);

    strcat(val, "\n");

    strcat(val, "Pr = ");
    char v1[12];
    sprintf(v1, "%f", pr);

    strcat(val, v1);

    gtk_label_set_text(GTK_LABEL(probabilidades1), val);

    strcpy(val, "Qh = ");
    char v2[12];
    sprintf(v2, "%f", qh);
    strcat(val, v2);

    strcat(val, "\n");

    strcat(val, "Qr = ");
    char v3[8];
    sprintf(v3, "%f", qr);
    strcat(val, v3);

    gtk_label_set_text(GTK_LABEL(probabilidades2), val);

}


int on_btn_cargar_clicked() {
    const gchar *filename;
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(entry_cargar));
    if (filename == NULL) {
        gtk_label_set_text(GTK_LABEL(result), "Selecione un archivo.");
        return 0;
    }
    readFile((char *) filename);
}

float stof(const char *s) {
    float rez = 0, fact = 1;
    if (*s == '-') {
        s++;
        fact = -1;
    };
    for (int point_seen = 0; *s; s++) {
        if (*s == '.') {
            point_seen = 1;
            continue;
        };
        int d = *s - '0';
        if (d >= 0 && d <= 9) {
            if (point_seen) fact /= 10.0f;
            rez = rez * 10.0f + (float) d;
        };
    };
    return rez * fact;
};

int on_Acept_clicked() {

    const gchar *phS;
    phS = gtk_entry_get_text(GTK_ENTRY(g_Ph));
    ph = stof(phS);
    const gchar *prS;
    prS = gtk_entry_get_text(GTK_ENTRY(g_Pr));
    pr = stof(prS);

    if (strlen(phS) == 0 || strlen(prS) == 0) {
        gtk_label_set_text(GTK_LABEL(result), "Los campos no pueden estar vacíos.");
        return 0;
    }
    for (int i = 0; i < strlen(phS); i++) {

        if (phS[i] == '.') {
            continue;
        }
        if (isdigit(phS[i]) == FALSE) {
            gtk_label_set_text(GTK_LABEL(result), "ph y pr tiene que ser entre 0 y 1.");
            return 0;
        }
    }
    for (int i = 0; i < strlen(prS); i++) {
        if (prS[i] == '.') {
            continue;
        }
        if (isdigit(prS[i]) == FALSE) {
            gtk_label_set_text(GTK_LABEL(result), "ph y pr tiene que ser entre 0 y 1.");
            return 0;
        }
    }

    if ((ph >= 0 && ph <= 1) && (pr >= 0 && pr <= 1)) {
        qr = 1 - ph;
        qh = 1 - pr;
        gtk_label_set_text(GTK_LABEL(result), "");
    } else {

        gtk_label_set_text(GTK_LABEL(result), "ph y pr tiene que ser entre 0 y 1.");
        return 0;
    }


    char val[40];
    strcpy(val, "Ph = ");
    char v[12];
    sprintf(v, "%f", ph);
    strcat(val, v);

    strcat(val, "\n");

    strcat(val, "Pr = ");
    char v1[12];
    sprintf(v1, "%f", pr);

    strcat(val, v1);

    gtk_label_set_text(GTK_LABEL(probabilidades1), val);

    strcpy(val, "Qh = ");
    char v2[12];
    sprintf(v2, "%f", qh);
    strcat(val, v2);

    strcat(val, "\n");

    strcat(val, "Qr = ");
    char v3[8];
    sprintf(v3, "%f", qr);
    strcat(val, v3);

    gtk_label_set_text(GTK_LABEL(probabilidades2), val);

}

void on_juego1_changed(GtkWidget *widget, GtkWidget *widget2) {

    GtkComboBox *combo_box = GTK_COMBO_BOX(widget);
    /*gint pos = gtk_combo_box_get_active (combo_box);
    gint first = 0;
    gtk_widget_get_active(widget);*/
    /*if(pos != first){
        gtk_widget_set_sensitive (widget2, TRUE);
    }*/
    lugar_juego[0] = gtk_combo_box_get_active(combo_box);


}

void on_cantJuegos_changed(GtkWidget *widget) {
    //gtk_widget_set_sensitive (widget2, TRUE);
    mitadJuegos = 3;
    GtkComboBox *combo_box = GTK_COMBO_BOX(widget);
    switch (gtk_combo_box_get_active(combo_box)) {
        case 0:
            mitadJuegos = 3;
            break;
        case 1:
            mitadJuegos = 4;
            break;
        case 2:
            mitadJuegos = 5;
            break;
        case 3:
            mitadJuegos = 6;
            break;
        case 4:
            mitadJuegos = 7;
            break;

    }
    switch (gtk_combo_box_get_active(combo_box)) {
        case 0:
            totalJuegos = 3;
            break;
        case 1:
            totalJuegos = 5;
            break;
        case 2:
            totalJuegos = 7;
            break;
        case 3:
            totalJuegos = 9;
            break;
        case 4:
            totalJuegos = 11;
            break;
    }
}


int main(int argc, char *argv[]) {

    GtkBuilder *builder;

    gtk_init(&argc, &argv);
    myCSS();

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/deportivas.glade", NULL);

    //Inicialización de widgets
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void myCSS(void) {
    GtkCssProvider *provider;
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
