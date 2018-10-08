#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

GtkBuilder *builder;

//Declaración de widgets
GtkWidget *window, *save, *spinButtonTimeLimit, *spinButtonLife, *spinButtonInitialCost, *scrolleInitialData;
GtkWidget       *windowInitialReplace;
GtkWidget       *windowTableData;
GtkWidget       *windowFinalTable;

GtkWidget       ***tableData;
GtkWidget       *g_tableData;

GtkWidget       *g_frame_manualEntry;
GtkWidget       *g_frame_fileEntry;
GtkWidget       *g_filechooser_btn;
GtkWidget       *g_entry_fileName;
GtkWidget       *g_spin_usefulLife;
GtkWidget       *g_spinbutton_timeLimit;
GtkWidget       *g_spinbutton_precioEquipo;
GtkWidget       *windowSave;

GtkWidget       *g_scrolledwindow_initialTableData;
GtkWidget       *g_scrolledwindow_finalTable;
GtkWidget       *g_scrolledwindow_optimalSolution;
FILE            *file_tableData;

const char *rowHeaderInitialTable[3] = {"Año","Precio de Reventa","Mantenimiento"};
const char *rowHeaderFinalTable[4] = {"t","G(t)","Próximo","Ganancia"};

int usefulLife = 0;
int totalUsefulLife = 0;
int initialCost = 0;
int timeLimit = 0;

char buffer[9];
char row[9];

int contado_x;

int Amount;
int timeLimitEquipment;

FILE * fileTableData;

typedef struct  {
    int year[31];
    int value;
    int position;
    int profit;
} FinalTable;


typedef struct {
    int value;
    int year;
} candidate;

typedef struct  {
    int year;
    int sale;
    int maintenance;
} InitialTable;

typedef struct
{
    int year[30];
    int position;
} plans;

void setTotalObjectsCount(int pTotalUsefulLife,int pInitialCost, int pTimeLimit) {
    totalUsefulLife = pTotalUsefulLife;
    initialCost = pInitialCost;
    timeLimit = pTimeLimit;
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


        for(int row =0; row < usefulLife; row++)
        {
          for(int column=0; column < 3; column++)
          {
            fprintf(fichero,"%s;",(gtk_entry_get_text(GTK_ENTRY(tableData[row][column]))));
          }
          fprintf(fichero,"\n");

        }
        fprintf(fichero,"%d\n",inputAmount);
        fprintf(fichero,"%d\n",inputTimeLimit);

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
    //FILE *fichero2;

    char str[1024];
    //char str2[1024];

    gtk_widget_set_sensitive(save, FALSE);
    dialog = gtk_file_chooser_dialog_new("Abrir archivo", GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         ("Cancelar"), GTK_RESPONSE_CANCEL,
                                         ("Abrir"), GTK_RESPONSE_ACCEPT, NULL);

    gint answer = gtk_dialog_run(GTK_DIALOG(dialog));
    if (answer == GTK_RESPONSE_ACCEPT){
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        fichero = fopen(filename, "r");

        /*int flag = loadData(filename);
        if (flag == 1) {
            gtk_widget_hide(windowInitial);
            createTableHV1();

            gtk_widget_show_all(scrolleInitialData);
        }
        /*entrySize = fileSize;
        update(entrySize);

        fclose(fichero);
        fclose(fichero2);*/
    }
    gtk_widget_destroy(dialog);
    //g_free (filename);
}

void createTableData() {
    tableData = calloc(usefulLife,sizeof(GtkWidget**));

    g_tableData = gtk_grid_new ();
    gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTableData), g_tableData);

    char rowNumber[4];

    for(int j = 0; j < usefulLife; j++) {
        tableData[j] = calloc(3,sizeof(GtkWidget*));
    }

    for(int row =0; row < usefulLife; row++)
    {
        for(int column=0; column < 3; column++)
        {
            tableData[row][column] = gtk_entry_new();
            gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);
            if (row == 0){
                gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeaderInitialTable[column]);
                gtk_widget_set_name(tableData[row][column],"header");
                gtk_widget_set_sensitive(tableData[row][column],FALSE);
            }
            if (column ==0 && row!=0){
                sprintf(rowNumber, "%d", row);
                gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowNumber);
                gtk_widget_set_name(tableData[row][column],"header");
                gtk_widget_set_sensitive(tableData[row][column],FALSE);
            }
            if (column == 0){
                gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]), 3);
            } else if (column == 1) {
                gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]), 14);
            }else {
                gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]), 12);
            }
        }
    }
    gtk_widget_show_all(g_scrolledwindow_initialTableData);
}


void on_cambiar_clicked() {
    usefulLife = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonLife)) + 1;
    createTableData();
}

//void on_btn_getFile_clicked() {
//    usefulLife = countUsefulLifeFiles (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(g_filechooser_btn)));
//    int matrixData[usefulLife-1][2];
//    startFill(matrixData,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)));
//    createTableDataFile(matrixData);
//}


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


  g_scrolledwindow_initialTableData = GTK_WIDGET(gtk_builder_get_object(builder, "scrolleInitialData"));

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
