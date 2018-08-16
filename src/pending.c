#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

GtkBuilder      *builder;
//Declaración de ventanas
GtkWidget      *window;


int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/pending.glade", NULL);
    gtk_main();

    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
