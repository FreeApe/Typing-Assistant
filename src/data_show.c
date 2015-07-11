#include <gtk/gtk.h>
#include <handle.h>
#include <swing.h>
#include <data_show.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern correct_num;
extern error_num;
extern all_num;
extern gdouble start_time;
gdouble sp = 0;

//在单行文本框中显示数据
void entry_show_data()
{
	gdouble accuracy = 0.0;
	gchar buf[20] = "";
	sprintf(buf,"%d",correct_num);
	gtk_entry_set_text(GTK_ENTRY(show.entry_correct),buf);		//显示正确个数

	sprintf(buf,"%d",error_num);
	gtk_entry_set_text(GTK_ENTRY(show.entry_error),buf);		//显示错误个数

	accuracy =((gdouble)correct_num * 100)/ (gdouble)all_num;
	sprintf(buf,"%.1f",accuracy);
	gtk_entry_set_text(GTK_ENTRY(show.entry_AR),buf);			//显示正确率

	sp = (gdouble)correct_num * 60.0/ start_time;
	sprintf(buf,"%.0f",sp);
	gtk_entry_set_text(GTK_ENTRY(show.entry_speed),buf);		//显示速度
}

