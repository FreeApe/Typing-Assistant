#include <gtk/gtk.h>
#include <swing.h>
#include <data_show.h>

extern ENTRY_SHOW show;


//显示子窗口
void show_child_window(GtkWidget *widget,gpointer data)
{
    gtk_widget_show_all(data);
	gtk_window_set_keep_above(GTK_WINDOW (data), TRUE);		//设置窗口总是最前
	gtk_window_set_keep_above(GTK_WINDOW (show.window), FALSE);	
}
//隐藏子窗口
void hide_child_window(GtkWidget *widget,gpointer data)
{
    gtk_widget_hide_all(data);
	gtk_window_set_keep_above(GTK_WINDOW (show.window), TRUE);
}


