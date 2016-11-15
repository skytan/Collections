#include <gtk/gtk.h>
#include "common.h"

gboolean messageBox_click_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
	GdkEventKey *key = (GdkEventKey *) event;

	switch (key->keyval)
	{
		case POS_ENTER:
			pint(data);
			gtk_widget_destroy( gtk_widget_get_toplevel( (GtkWidget *)data ) );
			break;
		default:
			break;
	}
		
	return TRUE;
}


int messageBox(char *message)
{
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *table;

	GtkWidget *prompt_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	

	gtk_container_set_border_width(GTK_CONTAINER(prompt_window), 1);
	gtk_widget_set_usize(prompt_window, WINDOWS_WIDTH, WINDOWS_HEIGHT);
	gtk_widget_set_uposition(prompt_window, 0, 0);
	gtk_window_set_modal(GTK_WINDOW(prompt_window), TRUE);

	table = gtk_table_new(2, 1, FALSE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 1);
	gtk_table_set_col_spacings(GTK_TABLE(table), 1);
	gtk_container_add(GTK_CONTAINER(prompt_window), table);

	label = gtk_label_new(message);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);
	gtk_widget_show(label);

	button = gtk_button_new_with_label("È·    ¶¨");
	gtk_table_attach(GTK_TABLE(table), button, 0, 1, 1, 2, 0, 0, 0, 0);
	gtk_signal_connect(GTK_OBJECT(button), "key_press_event", GTK_SIGNAL_FUNC(messageBox_click_event), prompt_window);
	gtk_widget_show(button);

	gtk_widget_show(table);
	gtk_widget_grab_focus(button);
	gtk_widget_show(prompt_window);
	gtk_widget_show_all(prompt_window);
	
	return 0;
}
