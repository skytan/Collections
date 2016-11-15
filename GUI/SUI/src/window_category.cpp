#include <gtk/gtk.h>
#include "common.h"
#include "window_category.h"
#include "common/category.h"
/*
int Category_delete_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
	gtk_widget_destroy(gtk_widget_get_toplevel( (GtkWidget*)data ) );
	
	return 0;
}
*/
int gui_to_category(struct category_gui* pgui, struct category* pstcate)
{
	pstcate->iNumber = atoi( GET_TEXT(pgui->number) );
	strcpy(pstcate->szName, GET_TEXT(pgui->name) );
	
	return 0;
}

void category_gui_clear(struct category_gui* pgui)
{
	SET_TEXT(pgui->name, "");
	SET_TEXT(pgui->number, "");
	SET_TEXT(pgui->taxName, "");
}

int category_key_enter(struct category_gui* pgui)
{
	struct category stcate;
	int inumber, ret;
	
	if( strlen( GET_TEXT(pgui->number) ) == 0 )
	{
		gtk_widget_grab_focus(pgui->number);
		return 1;
	}
	
	inumber = atoi( GET_TEXT(pgui->number) );
	if( inumber < 0 || inumber > 200 )
	{
		return 2;
	}
	
	if( strlen( GET_TEXT(pgui->name) ) )
	{
		stcate.iNumber = inumber;
		if( ret = category_read( &stcate ) )
		{
			perr();
			return -1;
		}
		
		SET_TEXT(pgui->name, "");
	}
	
	if( category_write( &stcate ) )
	{
		perr();
		return -2;
	}
	
	category_gui_clear(pgui);
	
	gtk_widget_grab_focus(pgui->name);
	return 0;
}


gboolean category_key_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
	GdkEventKey *key = (GdkEventKey *) event;
	struct category_gui* pgui = data;
	pint(pgui->window);

	switch (key->keyval)
	{
		case POS_ENTER:
			return category_key_enter(pgui);
			break;
		case POS_ESC:
			pint(pgui->window);
			gtk_widget_destroy( gtk_widget_get_toplevel( (pgui->window) ) );
			break;
		default:
			break;
	}
		
	return FALSE;
}

int create_category_window()
{
	int iErrorNo;
	GtkWidget *table;
	GtkWidget *label;
	static struct category_gui cate;

	perr();
	cate.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(cate.window, WINDOWS_WIDTH, WINDOWS_HEIGHT);
	gtk_window_set_modal(GTK_WINDOW(cate.window), TRUE);
//	gtk_signal_connect(GTK_OBJECT(cate.window), "destroy", GTK_SIGNAL_FUNC(Category_delete_event), cate.window);

	table = gtk_table_new(3, 2, TRUE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 2);
	gtk_table_set_col_spacings(GTK_TABLE(table), 8);
	gtk_container_add(GTK_CONTAINER(cate.window), table);
	gtk_widget_show(table);

	label = gtk_label_new("部类编号(1-200)");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);
	gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
	gtk_widget_show(label);
	cate.number = gtk_entry_new_with_max_length(3);
	gtk_table_attach_defaults(GTK_TABLE(table), cate.number, 1, 2, 0,1);
	gtk_widget_show(cate.number);

//	gtk_signal_connect(GTK_OBJECT(CategoryNumber), "key_press_event", GTK_SIGNAL_FUNC(category_number_key_press), NULL);

	label = gtk_label_new("部 类 名 称");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);
	gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
	gtk_widget_show(label);
	cate.name = gtk_entry_new_with_max_length(20);
	gtk_table_attach_defaults(GTK_TABLE(table), cate.name, 1, 2, 1, 2);
	gtk_widget_show(cate.name);
//      gtk_fixed_put(GTK_FIXED(fixed),CategoryName,120,21);
//      gtk_signal_connect(GTK_OBJECT(CategoryName), "key_press_event",
//                                         GTK_SIGNAL_FUNC(category_name_key_press), NULL);

	label = gtk_label_new("所属税种代码");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 2, 3);
	gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
	gtk_widget_show(label);
	cate.taxName = gtk_entry_new_with_max_length(20);
	gtk_table_attach_defaults(GTK_TABLE(table), cate.taxName, 1, 2, 2, 3);
	gtk_widget_show(cate.taxName);

//	gtk_signal_connect(GTK_OBJECT(TaxName), "key_press_event", GTK_SIGNAL_FUNC(TaxName_key_press), NULL);
/*
	if (get_working_state(&working_state, &iErrorNo) < 0) {
	one_dialog("config file error!");
	return;
	}

	if (working_state == 1) {
	if (fiscalmemory_read_tax_rate_file(struTaxRate, &iErrorNo) == -1) {
	//      printf("allow_to_using error = %x\n",iErrorNo);
	perr("读取税率文件出错!");
	return;
	}
	}
*/
	gtk_signal_connect(GTK_OBJECT(cate.window), "key_press_event", GTK_SIGNAL_FUNC(category_key_event), &cate);

	gtk_widget_show_all(cate.window);
	gtk_widget_grab_focus(cate.number);           
}
