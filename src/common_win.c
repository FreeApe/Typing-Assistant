#include <gtk/gtk.h>
#include <swing.h>
#include <data_show.h>

extern ENTRY_SHOW show;


//��ʾ�Ӵ���
void show_child_window(GtkWidget *widget,gpointer data)
{
    gtk_widget_show_all(data);
	gtk_window_set_keep_above(GTK_WINDOW (data), TRUE);		//���ô���������ǰ
	gtk_window_set_keep_above(GTK_WINDOW (show.window), FALSE);	
}
//�����Ӵ���
void hide_child_window(GtkWidget *widget,gpointer data)
{
    gtk_widget_hide_all(data);
	gtk_window_set_keep_above(GTK_WINDOW (show.window), TRUE);
}


