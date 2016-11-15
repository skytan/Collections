#include <stdio.h>
#include <gtk/gtk.h>

void destroy( GtkWidget *widget, gpointer   data )
{
	gtk_main_quit ();
}

int main(int argc, char *argv[])
{
	static GtkWidget *window;
	GtkWidget *scrolled_window;
	GtkWidget *table;
	GtkWidget *button;
	char buffer[32];
	int i, j;
	
	gtk_init (&argc, &argv);
	
	/* 创建一个新的对话框窗口，滚动窗口就放在这个窗口上 */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
//	gtk_window_set_title (GTK_WINDOW (window), "GtkScrolledWindow example");
	gtk_container_set_border_width(GTK_CONTAINER(window), 0);
	gtk_widget_set_usize(window, 240, 64);
	
	/* 创建一个新的滚动窗口。 */
	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	
	gtk_container_set_border_width(GTK_CONTAINER(scrolled_window), 1);
	
	/* 滚动条的出现方式可以是GTK_POLICY_AUTOMATIC 或GTK_POLICY_ALWAYS。
	*      * 设为GTK_POLICY_AUTOMATIC将自动决定是否需要出现滚动条
	*           * 而设为GTK_POLICY_ALWAYS，将一直显示一个滚动条
	*                * 第一个是设置水平滚动条，第二个是垂直滚动条 */
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	/* 对话框窗口内部包含一个vbox构件 */								
//	gtk_box_pack_start (GTK_BOX (GTK_WINDOW(window)->vbox), scrolled_window, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), scrolled_window);
	gtk_widget_show(scrolled_window);
	
	/* 创建一个包含10×10个格子的表格 */
	table = gtk_table_new(10, 10, FALSE);
	
	/* 设置x和y方向的行间间距为10像素 */
//	gtk_table_set_row_spacings (GTK_TABLE (table), 10);
//	gtk_table_set_col_spacings (GTK_TABLE (table), 10);
	
	/* 将表格组装到滚动窗口中 */
//	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (scrolled_window), table);
//	gtk_widget_show (table);
	
	/* 简单地在表格中添加许多开关按钮以展示滚动窗口 */
/*	for (i = 0; i < 10; i++)
	for (j = 0; j < 10; j++) {
	sprintf (buffer, "button (%d,%d)\n", i, j);
	button = gtk_toggle_button_new_with_label (buffer);
	gtk_table_attach_defaults (GTK_TABLE (table), button,
	i, i+1, j, j+1);
	gtk_widget_show (button);
	}*/
/*	
	// 在对话框的底部添加一个"close"按钮
	button = gtk_button_new_with_label ("close");
	g_signal_connect_swapped(G_OBJECT (button), "clicked",
	G_CALLBACK(gtk_widget_destroy),
	window);
*/	
	/* 让按钮能被缺省 */
	
//	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
//	gtk_box_pack_start(GTK_BOX (GTK_DIALOG (window)->action_area), button, TRUE, TRUE, 0);
	
	/* 将按钮固定为缺省按钮，只要按回车键就相当于点击了这个按钮 */
//	gtk_widget_grab_default(button);
//	gtk_widget_show(button);
	
	gtk_widget_show(window);
	
	gtk_main();
	
	return 0;
}
