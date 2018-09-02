#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

GtkBuilder      *builder;
//Declaración de ventanas
GtkWidget      *window, *about_window, *pending_rutas_cortas, *pending_knapsack, *pending_tba2, *pending_tba3, *pending_tba4;
//Declaración de botones
GtkWidget       *rutas_cortas_btn, *tba1_btn, *tba2_btn, *tba3_btn, *tba4_btn;
//Declaración de tooltips
GtkWidget       *tt_tba0, *tt_tba1, *tt_tba2, *tt_tba3, *tt_tba4;

//Declaración de funciones
void myCSS(void);

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
	myCSS();

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/proyecto.glade", NULL);

    //Inicialización de widgets
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    rutas_cortas_btn = GTK_WIDGET(gtk_builder_get_object(builder, "rutas_cortas"));
    tba1_btn = GTK_WIDGET(gtk_builder_get_object(builder, "tba1"));
    tba2_btn = GTK_WIDGET(gtk_builder_get_object(builder, "tba2"));
    tba3_btn = GTK_WIDGET(gtk_builder_get_object(builder, "tba3"));
    tba4_btn = GTK_WIDGET(gtk_builder_get_object(builder, "tba4"));

    //tooltips
    tt_tba0 = GTK_WIDGET(gtk_builder_get_object(builder, "tt_tba0"));
    tt_tba1 = GTK_WIDGET(gtk_builder_get_object(builder, "tt_tba1"));
    tt_tba2 = GTK_WIDGET(gtk_builder_get_object(builder, "tt_tba2"));
    tt_tba3 = GTK_WIDGET(gtk_builder_get_object(builder, "tt_tba3"));
    tt_tba4 = GTK_WIDGET(gtk_builder_get_object(builder, "tt_tba4"));

    //Elementos de tooltip
    gtk_widget_set_tooltip_text(tt_tba0, "Se utiliza el algoritmo X para encontrar las rutas más cortas");
    gtk_widget_set_tooltip_text(tt_tba1, "Allgoritmo de optimización de objetos o de la mochila");
    gtk_widget_set_tooltip_text(tt_tba2, "Este algoritmo se encuentra en construcción");
    gtk_widget_set_tooltip_text(tt_tba3, "Este algoritmo se encuentra en construcción");
    gtk_widget_set_tooltip_text(tt_tba4, "Este algoritmo se encuentra en construcción");

    about_window = GTK_WIDGET(gtk_builder_get_object(builder, "about_window"));
    pending_rutas_cortas = GTK_WIDGET(gtk_builder_get_object(builder, "pending_rutas_cortas"));
    pending_knapsack = GTK_WIDGET(gtk_builder_get_object(builder, "pending_knapsack"));
    pending_tba2 = GTK_WIDGET(gtk_builder_get_object(builder, "pending_tba2"));
    pending_tba3 = GTK_WIDGET(gtk_builder_get_object(builder, "pending_tba3"));
    pending_tba4 = GTK_WIDGET(gtk_builder_get_object(builder, "pending_tba4"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void myCSS(void){
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *style_file = "src/style.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(style_file), &error);
    g_object_unref (provider);
}

// called when about menu button is clicked
void on_press_about()
{
	//about_window = create_about_window();
	gtk_widget_show (about_window);
}

// called when about menu button is clicked
void on_press_floyd_open()
{
	system("./floyd &");
}

void on_knapsack_clicked()
{
    system("./knapsack &");
}

// called when about menu button is clicked
void on_press_pending_open()
{
	system("./pending &");
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
