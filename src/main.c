#include <gtk/gtk.h>

GtkBuilder      *builder; 
GtkWidget       *window;
GtkWidget 		*about_window;
GtkWidget 		*pending_window; 
 
int main(int argc, char *argv[])
{
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/proyecto.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
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
