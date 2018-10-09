#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

GtkBuilder *builder;

//Declaración de widgets
GtkWidget *window, *save, *spinButtonTimeLimit, *spinButtonLife, *spinButtonInitialCost, *scrolleInitialData;
GtkWidget *windowInitialReplace;
GtkWidget *windowTableData;
GtkWidget *windowFinalTable;
GtkWidget *notebook;

GtkCssProvider *provider;

GtkWidget ***tableData;
GtkWidget *g_tableData;

GtkWidget *g_frame_manualEntry;
GtkWidget *g_frame_fileEntry;
GtkWidget *g_filechooser_btn;
GtkWidget *g_entry_fileName;
GtkWidget *windowSave;

GtkWidget *g_scrolledwindow_initialTableData;
GtkWidget *g_scrolledwindow_finalTable;
GtkWidget *g_scrolledwindow_optimalSolution;
FILE *file_tableData;

const char *rowHeaderInitialTable[3] = {"Año", "Precio de Reventa", "Mantenimiento"};
const char *rowHeaderFinalTable[4] = {"t", "G(t)", "Próximo", "Ganancia"};

int usefulLife = 0;
int totalUsefulLife = 0;
int initialCost = 0;
int timeLimit = 0;

char buffer[9];
char row[9];
int tableDataFile[100][2];

int contado_x;

int Amount;
int timeLimitEquipment;

FILE *fileTableData;
FILE *infoFile;


typedef struct {
    int year[31];
    int value;
    int position;
    int profit;
} FinalTable;


typedef struct {
    int value;
    int year;
} candidate;

typedef struct {
    int year;
    int sale;
    int maintenance;
} InitialTable;

typedef struct {
    int year[30];
    int position;
} plans;


InitialTable *initialData;

plans planesPosibles[100];
int cont_plans = 0;

void setTotalObjectsCount(int pTotalUsefulLife, int pInitialCost, int pTimeLimit) {
    totalUsefulLife = pTotalUsefulLife;
    initialCost = pInitialCost;
    timeLimit = pTimeLimit;
}

int loadData(char *filename) {
    infoFile = fopen(filename, "r");
    if (infoFile != NULL) {
        fscanf(infoFile, "%d", &initialCost);
        fscanf(infoFile, "%d", &timeLimit);
        fscanf(infoFile, "%d", &usefulLife);


        int i = 0;
        while(!feof(infoFile)) {
            fscanf(infoFile, "%d;%d;", &tableDataFile[i][0], &tableDataFile[i][1]);
            i++;
        }

        fclose(infoFile);
        return 1;
    }
    return 0;
}


void createTableData() {
    char str[10];

    tableData = calloc(usefulLife, sizeof(GtkWidget * *));


    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(g_scrolledwindow_initialTableData));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    g_tableData = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(g_scrolledwindow_initialTableData), g_tableData);

    char rowNumber[4];

    for (int j = 0; j < usefulLife; j++) {
        tableData[j] = calloc(3, sizeof(GtkWidget * ));
    }

    for (int row = 0; row < usefulLife; row++) {
        for (int column = 0; column < 3; column++) {

            if (column != 0 && row != 0) {
                GtkAdjustment *adjustment = gtk_adjustment_new(0, 0, 999999999999999, 1, 1, 0);
                tableData[row][column] = gtk_spin_button_new(adjustment, 0, 0);

                sprintf(str, "%d", tableDataFile[row - 1][column - 1]);
                ///////////TODO CARGAR LOS DATOS DEL ARCHIVO LEIDO EN LA MATRIZ tableDataFile

                gtk_entry_set_text(GTK_ENTRY(tableData[row][column]), str);

            } else {
                tableData[row][column] = gtk_entry_new();

                if (row == 0) {
                    gtk_entry_set_text(GTK_ENTRY(tableData[row][column]), rowHeaderInitialTable[column]);
                    gtk_widget_set_name(tableData[row][column], "header");
                    gtk_widget_set_sensitive(tableData[row][column], FALSE);
                }

                if (column == 0 && row != 0) {
                    sprintf(rowNumber, "%d", row);
                    gtk_entry_set_text(GTK_ENTRY(tableData[row][column]), rowNumber);
                    gtk_widget_set_name(tableData[row][column], "header");
                    gtk_widget_set_sensitive(tableData[row][column], FALSE);
                }
            }


            if (column == 0) {
                gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]), 3);
            } else if (column == 1) {
                gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]), 6);
            } else {
                gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]), 4);
            }

            gtk_grid_attach(GTK_GRID(g_tableData), tableData[row][column], column, row, 1, 1);

        }
    }
    gtk_widget_show_all(g_scrolledwindow_initialTableData);
}

void leer_archivo() {
    int i, j = 0, k, fileSize = 0, entrySize;

    char *filename = NULL;
    GtkWidget *dialog;
    FILE *fichero;

    char str[1024];
    //char str2[1024];

    gtk_widget_set_sensitive(save, FALSE);
    dialog = gtk_file_chooser_dialog_new("Abrir archivo", GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         ("Cancelar"), GTK_RESPONSE_CANCEL,
                                         ("Abrir"), GTK_RESPONSE_ACCEPT, NULL);

    gint answer = gtk_dialog_run(GTK_DIALOG(dialog));
    if (answer == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        fichero = fopen(filename, "r");

        int flag = loadData(filename);
        if (flag == 1) {
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinButtonInitialCost), initialCost);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinButtonLife), usefulLife++);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinButtonTimeLimit), timeLimit);

            createTableData();
        }

        fclose(fichero);
    }
    gtk_widget_destroy(dialog);
    //g_free (filename);
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

        int inputAmount = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinButtonInitialCost));
        int inputTimeLimit = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinButtonTimeLimit));

        fprintf(fichero, "%d\n", inputAmount);
        fprintf(fichero, "%d\n", inputTimeLimit);

        for (int row = 1; row < usefulLife; row++) {
            for (int column = 0; column < 3; column++) {
                fprintf(fichero, "%s;", (gtk_entry_get_text(GTK_ENTRY(tableData[row][column]))));
            }
            fprintf(fichero, "\n");

        }


        fclose(fichero);
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
    gtk_widget_set_sensitive(save, FALSE);
}

void createFinalTableData(int timeLimit, FinalTable finalData[timeLimit + 1]) {
    GtkStyleContext *context;

    free(tableData);
    int ptimeLimit = timeLimit + 2;
    tableData = calloc(ptimeLimit, sizeof(GtkWidget * *));

    g_tableData = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(g_scrolledwindow_finalTable), g_tableData);

    char rowNumber[3];

    for (int j = 0; j < ptimeLimit; j++) {
        tableData[j] = calloc(4, sizeof(GtkWidget * ));
    }

    for (int row = 0; row < ptimeLimit; row++) {
        for (int column = 0; column < 4; column++) {
            tableData[row][column] = gtk_entry_new();

            context = gtk_widget_get_style_context(tableData[row][column]);
            gtk_style_context_add_class(context,"cell_res");

            gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]), 10);
            gtk_widget_set_sensitive(tableData[row][column], FALSE);
            gtk_grid_attach(GTK_GRID(g_tableData), tableData[row][column], column, row, 1, 1);
            if (row == 0) {
                gtk_entry_set_text(GTK_ENTRY(tableData[row][column]), rowHeaderFinalTable[column]);
                gtk_widget_set_name(tableData[row][column], "header");
                gtk_widget_set_sensitive(tableData[row][column], FALSE);
            }
            if (column == 0) {
                gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]), 4);
            } else {
                gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]), 12);
            }
        }
        if (row != 0) {
            FinalTable data = finalData[row - 1];
            char gt[50];
            char prox[50];
            char profit[50];

            strcpy(prox, " ");

            sprintf(rowNumber, "%d", row - 1);
            gtk_entry_set_text(GTK_ENTRY(tableData[row][0]), rowNumber);
            gtk_widget_set_name(tableData[row][0], "header");

            sprintf(gt, "%d", data.value);
            gtk_entry_set_text(GTK_ENTRY(tableData[row][1]), gt);
            gtk_widget_set_name(tableData[row][1], "allEntries");


            for (int i = 0; i < data.position; i++) {
                char aux[3];
                sprintf(aux, "%d", data.year[i]);
                strcat(prox, aux);
                if (i < data.position - 1) {
                    strcat(prox, ",");
                }

            }

            gtk_entry_set_text(GTK_ENTRY(tableData[row][2]), prox);
            gtk_widget_set_name(tableData[row][2], "allEntries");

            sprintf(profit, "%d", data.profit);
            gtk_entry_set_text(GTK_ENTRY(tableData[row][3]), profit);
            gtk_widget_set_name(tableData[row][3], "allEntries");
            memset(prox, '\0', strlen(prox));
            memset(profit, '\0', strlen(profit));
        }
    }
    gtk_widget_show_all(g_scrolledwindow_finalTable);
}

int repeat (plans planesPosibles[300],int indice){
    int result = 0;
    int coincidencia = 0;
    if (indice > 0){
        for (int i=0;i<indice;i++){
            if (planesPosibles[i].position == planesPosibles[indice].position){
                for (int x =0;x<planesPosibles[indice].position;x++){
                    if (planesPosibles[indice].year[x] == planesPosibles[i].year[x]){
                        result = 1;
                        coincidencia ++;
                    }
                    else{
                        result = 0;
                        coincidencia = 0;
                    }
                }
            }
        }
    }

    if (result == 1 && coincidencia==planesPosibles[indice].position+1){
        return 1;
    }
    else{
        return 0;
    }
}

void generar_imagen_grafo() {
    system("dot -Tpng test.dot -o src/graph.png");

    const gchar *style_file = "src/style.css";
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(style_file), 0);
}

void createOptimalSolution(plans planesPosibles[300]) {
    char text[1000];
    char buffer[25];
    strcpy(text, " ");


    FILE *fichero;
    fichero = fopen("test.dot", "w+");
    fputs("digraph G {\n", fichero);

    for (int i =0; i < cont_plans;i++){
        plans aux = planesPosibles[i];
        if (repeat(planesPosibles,i) ==0) {
            for (int x = 0;x<=aux.position;x++) {
                fprintf(fichero, "%d", aux.year[x]);

                if (x<aux.position){
                    fprintf(fichero, " -> ");
                }
                else{
                    int R = rand() % 255;
                    int G = rand() % 255;
                    int B = rand() % 255;

                    fprintf(fichero, " [fillcolor = \"#%x%x%x\", color = \"#%x%x%x\"]\n", R, G, B, R, G, B); // modified to append string

                }
            }
        }
    }
    fprintf(fichero, "}");


    fclose(fichero);

    generar_imagen_grafo();

    GtkWidget *g_lblSolution = gtk_label_new (text);
    gtk_container_add (GTK_CONTAINER (g_scrolledwindow_optimalSolution), g_lblSolution);
    gtk_widget_set_name(g_lblSolution,"label");

    gtk_widget_show_all(windowFinalTable);
    memset(text,'\0',strlen(text));
}

void mostrar_respuesta(FinalTable finalData[timeLimit+1],plans planesPosibles[300],int cont_plans){
    int stop = 0;
    int contador = 0;

    plans planes;
    planes.position = 0;
    planes.year[0] = 0;

    FinalTable answer = finalData[contador];
    while(stop==0){
        if (answer.position==0){
            stop = 1;
        }
        else{
            planes.position = planes.position +1;
            planes.year[planes.position] = answer.year[answer.position-1];

            answer = finalData[answer.year[answer.position-1]];
        }
    }
    planesPosibles[contado_x] = planes;
    cont_plans ++;
    contado_x ++;
}

void planes(FinalTable G[timeLimit+1],plans planesPosibles[300],int cont_plans){
    for (int i=0;i<=timeLimit;i++){
        if (G[i].position > 1){
            for (int x = G[i].position;x>=1;x--){
                mostrar_respuesta(G,planesPosibles, cont_plans);
                G[i].position = G[i].position - 1;

                if (G[i].position == 0){
                    G[i].position = 1;
                }

            }
        }
    }
    cont_plans = contado_x;
}

void replaceAlgorithm(InitialTable initialData[totalUsefulLife], FinalTable finalData[timeLimit+1]) {

    int cost[totalUsefulLife];
    int mantain = 0;

    for (int year = 0; year < totalUsefulLife; year++) {
        InitialTable matrizYear = initialData[year];
        for (int rango=0; rango <=year; rango++) {
            InitialTable matrizRango = initialData[rango];
            mantain = mantain + matrizRango.maintenance;
        }
        cost[year] = initialCost + mantain - matrizYear.sale;
        mantain = 0;
    }

    for (int i = timeLimit;i>=0;i--){
        int min = 0;
        int flag = 0;
        candidate options[totalUsefulLife];
        candidate posibilites;
        posibilites.value = 0;
        posibilites.year = i;
        FinalTable answer;
        for (int x = i+1;x<=timeLimit;x++){
            if (flag < totalUsefulLife){
                posibilites.value = cost[(x-i)-1] + finalData[x].value;
                posibilites.year = x;
                options[flag] = posibilites;
                flag ++;
            }
        }


        if (flag == 0){
            answer.value = posibilites.value;
            answer.year[0] = posibilites.year;
            answer.position = 0;
            answer.profit = initialCost;
        }
        for (int i = 0;i<flag;i++){
            candidate aux;
            aux = options[i];
            if (i==0){
                min = aux.value;
                answer.year[0] = aux.year;
                answer.position = 1;
            }
            else{
                if (aux.value == min){
                    answer.year[answer.position] = aux.year;
                    answer.position ++;
                }

                if (aux.value<min){
                    answer.year[0] = aux.year;
                    answer.position = 1;
                    min = aux.value;
                }

            }
            answer.value = min;
            answer.profit = initialCost - min;
        }
        finalData[i] = answer;
    }
}

void createObjects() {
    initialData = calloc(usefulLife,sizeof(InitialTable));
    for (int row = 1; row < usefulLife;row ++)
    {
        InitialTable data;
        data.year = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][0])));
        data.sale = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][1])));
        data.maintenance = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][2])));
        initialData[row-1] = data;
    }
}

void on_calcular_clicked() {
    int initialCost = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonInitialCost));
    timeLimit = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonTimeLimit));

    setTotalObjectsCount(usefulLife-1, initialCost, timeLimit);
    createObjects();

    FinalTable *finalData = calloc (timeLimit+1,sizeof(FinalTable));
    replaceAlgorithm(initialData,finalData);
    free(initialData);
    createFinalTableData(timeLimit,finalData);
    planes(finalData,planesPosibles,cont_plans);
    cont_plans = contado_x;
    createOptimalSolution(planesPosibles);

    gtk_notebook_next_page (GTK_NOTEBOOK(notebook));
    free(finalData);
}

void on_cambiar_clicked() {
    usefulLife = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinButtonLife)) + 1;
    timeLimit = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonTimeLimit));
    initialCost = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonInitialCost));
    createTableData();
}

//void on_btn_getFile_clicked() {
//    usefulLife = countUsefulLifeFiles (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(g_filechooser_btn)));
//    int matrixData[usefulLife-1][2];
//    startFill(matrixData,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)));
//    createTableDataFile(matrixData);
//}

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

int main(int argc, char *argv[]) {
    GtkBuilder *builder;

    gtk_init(&argc, &argv);
    myCSS();

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/equipos.glade", NULL);

    spinButtonTimeLimit = GTK_WIDGET(gtk_builder_get_object(builder, "spinButtonTimeLimit"));
    spinButtonLife = GTK_WIDGET(gtk_builder_get_object(builder, "spinButtonLife"));
    spinButtonInitialCost = GTK_WIDGET(gtk_builder_get_object(builder, "spinButtonInitialCost"));
    scrolleInitialData = GTK_WIDGET(gtk_builder_get_object(builder, "scrolleInitialData"));

    notebook = GTK_WIDGET(gtk_builder_get_object(builder, "notebookTabs"));

    g_scrolledwindow_initialTableData   = GTK_WIDGET(gtk_builder_get_object(builder, "scrolleInitialData"));
    g_scrolledwindow_finalTable         = GTK_WIDGET(gtk_builder_get_object(builder, "scrolleSolOptima"));

    //Inicialización de widgets
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

// called when window is closed
void on_window_main_destroy() {
    gtk_main_quit();
}
