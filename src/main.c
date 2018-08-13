#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

GtkBuilder      *builder;
//Declaración de ventanas
GtkWidget      *window, *about_window, *pending_window;
//Declaración de botones
GtkWidget       *rutas_cortas_btn, *tba1_btn, *tba2_btn, *tba3_btn, *tba4_btn;

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/proyecto.glade", NULL);

    //Inicialización de widgets
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    rutas_cortas_btn = GTK_WIDGET(gtk_builder_get_object(builder, "rutas_cortas"));
    tba1_btn = GTK_WIDGET(gtk_builder_get_object(builder, "tba1"));
    tba2_btn = GTK_WIDGET(gtk_builder_get_object(builder, "tba2"));
    tba3_btn = GTK_WIDGET(gtk_builder_get_object(builder, "tba3"));
    tba4_btn = GTK_WIDGET(gtk_builder_get_object(builder, "tba4"));

    //Elementos de tooltip
    gtk_widget_set_tooltip_text(rutas_cortas_btn, "Se utiliza el algoritmo X para encontrar las rutas más cortas");
    gtk_widget_set_tooltip_text(tba1_btn, "Este algoritmo se encuentra en construcción");
    gtk_widget_set_tooltip_text(tba2_btn, "Este algoritmo se encuentra en construcción");
    gtk_widget_set_tooltip_text(tba3_btn, "Este algoritmo se encuentra en construcción");
    gtk_widget_set_tooltip_text(tba4_btn, "Este algoritmo se encuentra en construcción");

    about_window = GTK_WIDGET(gtk_builder_get_object(builder, "about_window"));
    pending_window = GTK_WIDGET(gtk_builder_get_object(builder, "pending_window"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}

// called when about menu button is clicked
void on_press_about()
{
	//about_window = create_about_window();
	gtk_widget_show (about_window);
}

// called when about menu button is clicked
void on_press_pending_window()
{
	//about_window = create_about_window();
	gtk_widget_show (pending_window);
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

// called when window is closed
void on_about_window_destroy()
{
    gtk_widget_hide_on_delete(about_window);
}

// called when window is closed
void on_pending_window_destroy()
{
    gtk_widget_hide_on_delete(pending_window);
}
