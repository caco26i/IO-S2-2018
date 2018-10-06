#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

GtkBuilder *builder;

//Declaración de widgets
GtkWidget *window, *save, *spinButtonTimeLimit, *spinButtonLife, *spinButtonInitialCost, *scrolleInitialData;



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
