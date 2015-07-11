#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <handle.h>
#include <swing.h>
#include <string.h>
#include <gdk/gdkkeysyms.h>

#include "path.h"
#include "net.h"


#define F	0
#define T	1
#define NUL	-1
static void handle_error (GError *error);
//光标移动或改变信号回调函数
static void mark_set_callback(GtkTextBuffer *buffer,
    const GtkTextIter *new_location, GtkTextMark *mark,
    gpointer data);
//textview回车回调函数
static gboolean textview_key_press(GtkWidget *widget, GdkEvent *e
                                            , gpointer data);

extern GtkWidget *stop_count;
extern USER_INFO local_userinfo;	
int time_flag;
gdouble start_time = 0.0;

char *page;
unsigned cont_len;										//剩余的文件的长度
gsize length ;											//获取的文件的长度
gchar *content;
gint view_focus = 1;									//聚焦在哪个textview
gint viewnum = 1;							

gint correct_num;
gint error_num;
gint all_num;

gint before_bp;											//回删之前字符对比状态
gint backspace_status;									//是否是连续回删
gint backspacefg;										//回删标记
gint after_bp;											//回删之后字符对比状态


//len:剩余长度  cont:剩余待显示文本内容
char* buffer_get_text_from_content(gchar *cont,gsize len)
{
	GtkTextIter start, end, iter;
	GtkTextMark *mark;
	int t_num =0 ;
	
	cont_len = len-LINE_NUMBER*5;
	if(cont_len <= LINE_NUMBER*5 ){
		cont_len = length;							//重新赋值原文本长度
		printf("length= %d cont_len= %d there is not any word to read in bye.txt!\n",
			length,cont_len);
		return NULL;								//已经没有文字可显示了,then...
	}
	int i = 0;
	for(i =0 ;i< 10 ;i++){
		gtk_text_buffer_get_bounds(buffer[i],&start,&end);//获取当前buffer中文本开始结束标记		
		gtk_text_buffer_delete(buffer[i],&start,&end);  //清空文本缓冲区
	}
	//view[0]显示文本
    mark = gtk_text_buffer_get_insert (buffer[0]);
    gtk_text_buffer_get_iter_at_mark (buffer[0], &iter, mark);
	for(t_num =0;t_num <LINE_NUMBER;t_num++){
		if((*cont == '\n') || (*cont == '\r')){cont++;t_num--;continue;}
		gtk_text_buffer_insert(buffer[0],&iter,cont++,1);	
	}
	//view[2]显示文本
    mark = gtk_text_buffer_get_insert (buffer[2]);
    gtk_text_buffer_get_iter_at_mark (buffer[2], &iter, mark);
	for(t_num =0;t_num <LINE_NUMBER;t_num++){
		if((*cont == '\n') || (*cont == '\r')){cont++;t_num--;continue;}
		gtk_text_buffer_insert(buffer[2],&iter,cont++,1);
	}
	//view[4]显示文本
    mark = gtk_text_buffer_get_insert (buffer[4]);
    gtk_text_buffer_get_iter_at_mark (buffer[4], &iter, mark);
	for(t_num =0;t_num <LINE_NUMBER;t_num++){
		if((*cont == '\n') || (*cont == '\r')){cont++;t_num--;continue;}
		gtk_text_buffer_insert(buffer[4],&iter,cont++,1);	
	}
	//view[6]显示文本
    mark = gtk_text_buffer_get_insert (buffer[6]);
    gtk_text_buffer_get_iter_at_mark (buffer[6], &iter, mark);
	for(t_num =0;t_num <LINE_NUMBER;t_num++){
		if((*cont == '\n') || (*cont == '\r')){cont++;t_num--;continue;}
		gtk_text_buffer_insert(buffer[6],&iter,cont++,1);
	}
	//view[8]显示文本
    mark = gtk_text_buffer_get_insert (buffer[8]);
    gtk_text_buffer_get_iter_at_mark (buffer[8], &iter, mark);
	for(t_num =0;t_num <LINE_NUMBER;t_num++){
		if((*cont == '\n') || (*cont == '\r')){cont++;t_num--;continue;}
		gtk_text_buffer_insert(buffer[8],&iter,cont++,1);	//保留待第二页显示	
	}	
	gtk_window_set_focus(GTK_WINDOW(show.window),view[1]);	//聚焦到第一个文本输入框
	return cont;
}
//显示文件中的文本到对照文本框中
int show_file_text(char *dir,char *fname)
{
	GtkTextIter iter;
	GtkTextMark *mark;
	GError *error = NULL;

	gchar *filename;
	filename = g_build_filename(FILE_FILES,"select article.txt",NULL);//打开文件
    if (!g_file_test (filename, G_FILE_TEST_EXISTS)){
        g_error ("Error: File does not exist!please check!");
		return -1;
    }
	g_file_get_contents(filename,&content,&length,&error);
	handle_error(error);

	page = buffer_get_text_from_content(content,length);	//初次将文件中文字显示到文本框中

	//创建标记
	create_buffer_tag();
}
//输入文本框中的信号处理
void text_signal_pro()
{
	int i = 0;

	for(i = 0;i < 10;i++){
		if((i%2) != 0){
			g_signal_connect(buffer[i], "changed",				//文本改变信号
			    G_CALLBACK(start_enter_text_callback), NULL);
			g_signal_connect(view[i], "backspace",				//文本框回删信号
			    G_CALLBACK(delete_backspace_text_callback), NULL);
    		g_signal_connect(view[i],"key-press-event"		//注册按键检测信息
               	, G_CALLBACK(textview_key_press), view[i]);

		}
		g_signal_connect_object(buffer[i], "mark_set", 			//光标移动，mark_set信号被发送出去
			G_CALLBACK(mark_set_callback), NULL, 0);			//NULL可被替换为构件，即data
	}
}
static void
handle_error (GError *error)
{
    if (error != NULL)
    {
        g_printf (error->message);
        g_clear_error (&error);
    }
}

//去除当前文本框光标所在的一个字符
static void remove_textview_enter_keypress(GtkWidget *widget, gpointer  data)
{
	GtkTextIter b_end1,b_end2;
	gint chnum = 0;
	gchar * text1,*text2;

	chnum = gtk_text_buffer_get_char_count(buffer[view_focus]);	//获取当前buffer中的字符个数
	if(chnum < LINE_NUMBER){
		gtk_text_buffer_get_end_iter(buffer[view_focus], &b_end1);//获取当前buffer中文本开始结束标记		
		b_end2 = b_end1;
		gtk_text_iter_backward_char(&b_end2);	
		gtk_text_buffer_delete(buffer[view_focus],&b_end2,&b_end1);

		gtk_window_set_focus(GTK_WINDOW(show.window),view[view_focus]);
	}	
	else ;
}

//textview回车回调函数
static gboolean textview_key_press(GtkWidget *widget, GdkEvent *e
                                            , gpointer data)
{
    GdkEventKey *event = (GdkEventKey*)e;
    if(event ->keyval == GDK_KEY_Return || event ->keyval == GDK_KEY_KP_Enter
                        || event ->keyval == GDK_KEY_ISO_Enter){
        if((event ->state & GDK_CONTROL_MASK) != 0
                        || (event ->state & GDK_SHIFT_MASK) != 0){
            return FALSE;
        }
		//printf("Enter is pressing....\n");
        return TRUE;
    }
    return FALSE;
}
//光标移动或改变信号回调函数
static void mark_set_callback(GtkTextBuffer *bufferret,
    const GtkTextIter *new_location, GtkTextMark *mark,
    gpointer data)
{
	GtkTextIter iter;
	gint viewf = -1;
	gint chcount = 0, col = 0;
	gint i = 0;
	gint cursor  = 0 ;
	for(i = 0;i < 10;i++){
		if(gtk_widget_is_focus(view[i]))viewf = i;	
	}
	//printf("viewf = %d  view_focus = %d\n",viewf,view_focus);
	if(view_focus != viewf){	
		gtk_window_set_focus(GTK_WINDOW(show.window),view[view_focus]);
	}
	else {
//		gtk_text_buffer_get_iter_at_mark(buffer[view_focus],
//      		&iter, gtk_text_buffer_get_insert(buffer[view_focus]));
//	    chcount = gtk_text_buffer_get_char_count(buffer[view_focus]);//获取buffer中所有字符数
//		col = gtk_text_iter_get_line_offset(&iter);					 //获取光标所在列数
//		if(col != chcount){
			//光标置行尾(未解决);  
//			cursor = gtk_editable_get_position(GTK_EDITABLE(view[view_focus]));
//			gtk_editable_set_position(GTK_EDITABLE(view[view_focus]),chcount-1);         
//			printf("chcount=%d col=%d cursor=%d\n",chcount,col,cursor);
//		}
	}
}

//信号调用:开始输入文本
void start_enter_text_callback(GtkTextBuffer *widget,
    gpointer  *data)
{
    GtkTextIter start, end, iter;
    gchar *text;
	gchar mirror_ch, enter_ch;
	gint chnum = 0 ;
	//g_printf("=====================================enter======\n");
	//g_printf("viewnum= %d  viewfocus=%d\n",viewnum,view_focus);
	chnum = gtk_text_buffer_get_char_count(widget);			//获取当前buffer中的字符个数

	gtk_text_buffer_get_bounds(widget,&start,&end);			//获取当前buffer中文本开始结束标记
	text=gtk_text_buffer_get_text(widget,&start,&end,FALSE);//获取当前buffer中的文本
	enter_ch = *(text+strlen(text)-1);

	if(view_focus < 11){
		mirror_ch = mirror_textview_get_char(chnum);
		//printf("#enter==%c==#mirror===%c===\n",enter_ch,mirror_ch);
		delete_current_line_textchar(mirror_ch, enter_ch);	//当前行输入处理
	}
		//得到输入的总字符数=正确数 + 错误数
	all_num = correct_num + error_num;						//输入的总字符数
	//printf("all_num=%d correct_num=%d error_num=%d\n",all_num,correct_num,error_num);
		//切换textview
	if(chnum > (LINE_NUMBER-1)){
		viewnum = viewnum+2;
		view_focus = viewnum;
		//printf("viewnum = %d\n",viewnum);

		if(viewnum < 11){
			gtk_window_set_focus(GTK_WINDOW(show.window),view[viewnum]);//一行输满自动切换行	
		}
		else {
			//printf("page up\n");
			if((NULL !=(page=buffer_get_text_from_content(page,cont_len)))){
			}
			else {
				page = content;								//重新赋值原文本内容，循环文本
				page = buffer_get_text_from_content(page,cont_len);	//将文件中文字显示到文本框中
			}
			viewnum = 1;									//翻页重新定位到第一行
			view_focus = viewnum;
			gtk_window_set_focus(GTK_WINDOW(show.window),view[viewnum]);//一行输满自动切换行			
		}
	}
}
//删除文本回调函数
void delete_backspace_text_callback(GtkTextBuffer *widget,
    gpointer  *data)
{
	GtkTextIter b_end1,b_end2;
	gint chnum = 0;
	gchar * text1,*text2;
	
	if ((all_num % (LINE_NUMBER*5)) != 0)backspacefg =1;
	else backspacefg = 0;
	//printf("backspacefg = %d\n",backspacefg);
	chnum = gtk_text_buffer_get_char_count(buffer[view_focus]);	//获取当前buffer中的字符个数
	if(chnum == 0){
		if(view_focus ==1) ;								//第一行无动作
		else {
			//获得并比较待删除的行末字符上下是否相同
			gtk_text_buffer_get_end_iter(buffer[view_focus-3], &b_end1);//获取当前buffer中文本开始结束标记		
			b_end2 = b_end1;
			gtk_text_iter_backward_char(&b_end2);	
			text1 = gtk_text_buffer_get_text (buffer[view_focus-3], &b_end2, &b_end1, FALSE);

			gtk_text_buffer_get_end_iter(buffer[view_focus-2], &b_end1);//获取当前buffer中文本开始结束标记		
			b_end2 = b_end1;
			gtk_text_iter_backward_char(&b_end2);	
			text2 = gtk_text_buffer_get_text (buffer[view_focus-2], &b_end2, &b_end1, FALSE);
			//printf("text1=%c text2=%c\n",*text1,*text2);
			if(char_is_equal(*(text1), *(text2))) after_bp = 1;
			else after_bp = 0;
			view_focus = view_focus - 2;
			viewnum = viewnum -2;
			//删除上一行最后一个字符，然后聚焦到上一行
			gtk_text_buffer_delete(buffer[view_focus],&b_end2,&b_end1);
							///此处被changed信号中断,先执行changed再回来这里执行
			//printf("$viewnum= %d  viewfocus=%d\n",viewnum,view_focus);

			gtk_window_set_focus(GTK_WINDOW(show.window),view[view_focus]);
		}	
	}	
	else ;
}
//获取对照文本框中相应位置的字符
gchar mirror_textview_get_char(gint pos)
{
    GtkTextIter start, end;
    gchar *text;
    gtk_text_buffer_get_bounds (buffer[view_focus-1], &start, &end);
    text = gtk_text_buffer_get_text (buffer[view_focus-1], &start, &end, FALSE);
	return (*(text+pos-1));
}
//判断两个字符是否相等
int char_is_equal(gchar ch1,gchar ch2)
{
	if((ch1 == ch2)){
		if(ch1 > 0 )return T;
		else return NUL;
	}
	return F;
}

//改变字符背景颜色:
void change_textview_bg(GtkTextBuffer *buf,gboolean tf)
{
	GtkTextIter iter, start, end;

	
	gtk_text_buffer_get_bounds(buf, &start, &end);
	start = end;
	
	gtk_text_iter_backward_char(&start);			//start标记退后一个字符
		
	if(tf){
		gtk_text_buffer_apply_tag_by_name(buf,"bluebg",&start,&end);
	}
	else {
		gtk_text_buffer_apply_tag_by_name(buf,"redbg",&start,&end);
	}
}
//改变字体的颜色:正确则灰色，错误则红色
void change_textview_font_color(GtkTextBuffer *buf,GtkTextBuffer *buf1,gint view_f,gboolean tf)
{
	GtkTextIter start1, end1,start2,end2;

	
	gtk_text_buffer_get_bounds(buf, &start1, &end1);
	gtk_text_buffer_get_bounds(buf1, &start2, &end2);

	start1 = end1;
	start2 = end2;
	
	gtk_text_iter_backward_char(&start1);			//start标记退后一个字符
	gtk_text_iter_backward_char(&start2);			//start标记退后一个字符
		
	if(tf){
		gtk_text_buffer_apply_tag_by_name(buf1,"blue_fg",&start2,&end2);			
		gtk_text_buffer_apply_tag_by_name(buf,"blue_fg",&start1,&end1);
	}
	else {
		gtk_text_buffer_apply_tag_by_name(buf,"red_fg",&start1,&end1);
		gtk_text_buffer_apply_tag_by_name(buf1,"red_fg",&start2,&end2);	
	}

}
//创建标记tag
void create_buffer_tag()
{
	//创建标记
	int i = 0 ;
	for(i = 0;i< 10 ;i++){
		if((i%2) != 0){
			gtk_text_buffer_create_tag(buffer[i], "bluebg", 
			  "background", "gray", NULL); 
			gtk_text_buffer_create_tag(buffer[i], "blue_fg", 
			  "foreground", "gray", NULL); 
			gtk_text_buffer_create_tag(buffer[i], "red_fg", 
			  "foreground", "red", NULL); 		
			gtk_text_buffer_create_tag(buffer[i], "redbg", 
			  "background", "red", NULL); 
		}
	}
}

//删除当前行文本字符
void delete_current_line_textchar(gchar ch1,gchar ch2)
{
	if(backspacefg == 1){			//是回删
		backspacefg = 0;
		backspace_status++;
		if(backspace_status == 1){	//不连续回删
			if(before_bp == 1){
				correct_num--;
				if(char_is_equal(ch1, ch2))after_bp = 1;
				else after_bp = 0;
			}
			else {
				error_num--;
				if(char_is_equal(ch1, ch2))after_bp = 1;
				else after_bp = 0;					
			}
		}
		else {						//连续回删
			if(after_bp == 1){
				correct_num--;
				if(char_is_equal(ch1, ch2))after_bp = 1;
				else after_bp = 0;
			}
			else {
				error_num--;
				if(char_is_equal(ch1, ch2))after_bp = 1;
				else after_bp = 0;
			}
		}
		//printf("********it is backspace %d\n",backspace_status);
	}
	else {							//不是回删
		backspace_status = 0;
		if(char_is_equal(ch1, ch2) == T){
			before_bp = 1;
			correct_num++;
			change_textview_bg(buffer[view_focus],TRUE);
		}
		else if(char_is_equal(ch1, ch2) == F){
			before_bp = 0;
			error_num++;
			change_textview_bg(buffer[view_focus],FALSE);
		}
		else ;					//光标上下不是有效字符，不做处理
		//printf("********it is[ not ]backspace \n");
	}		
}

//定时器初始化
void timer_init()
{
	GTimer *timer;					//定时器

	//定时器计时
    timer = g_timer_new ();
	g_timer_start(timer);
	gtk_timeout_add(1000,(GSourceFunc)timeout_callback,NULL);//每隔一秒调用回调函数
}

//stop count time 回调函数
int stop_count_time(GtkWidget *widget,gpointer data)
{
	GtkTextIter start, end;
	//printf("time flag =%d=\n",time_flag);
	time_flag = !time_flag;
	if(time_flag == 1){
		correct_num = 0;								//bug,,,每次start后correct_num=2
		error_num = 0;
		all_num = 0;
		view_focus = 1;									//聚焦在哪个textview
		viewnum = 1;							
		start_time=0;
		gtk_button_set_label(GTK_BUTTON(stop_count),"START");
		return FALSE;
	}
	else {
		//将textview内容清空
		int i = 0;
		//printf("======%d=%d=====\n",i,correct_num);

		for(i =0 ;i< 10 ;i++){
			if((i % 2) != 0){
				gtk_text_buffer_get_bounds(buffer[i],&start,&end);//获取当前buffer中文本开始结束标记		
				gtk_text_buffer_delete(buffer[i],&start,&end);  //清空文本缓冲区
			}
			//printf("======%d=%d=====\n",i,correct_num);
		}
		gtk_window_set_focus(GTK_WINDOW(show.window),view[1]);	//聚焦到第一个文本输入框
		gtk_button_set_label(GTK_BUTTON(stop_count),"STOP");
		return TRUE;
	}	
}

//定时器溢出回调函数:返回TRUE时，定时器继续，FALSE时定时器停止
gboolean timeout_callback(gpointer *data)
{
	gchar time_num[20];
	static i = 0;
	if(time_flag == 1){
		
	}
	else {
    	start_time++; 
		sprintf(time_num,"%.0f",start_time);		
		//printf("ctime: %s \n",time_num);
		gtk_entry_set_text(GTK_ENTRY(show.entry_ctime),time_num);	//显示到entry_ctime
		entry_show_data();
		return TRUE;
	}
	return TRUE;
}

//任何时刻按下ESC键退出信号回调函数
gint ESC_quit(GtkWidget* widget,GdkEventKey* event,gpointer data)
{
	gchar c;
	GtkWidget *dialog;
	gint result;
	
	c=(gchar)event->keyval;
	if(c == 0x1b){//ascii码: ESC键
		dialog = gtk_message_dialog_new((GtkWindow *)show.window,
		            GTK_DIALOG_DESTROY_WITH_PARENT,
		            GTK_MESSAGE_QUESTION,
		            GTK_BUTTONS_YES_NO,
		            "Are you sure to quit?");
		gtk_window_set_title(GTK_WINDOW(dialog), "quit app");

		time_flag = 1;//g_timer_destroy(timer);								//摧毁定时器达到停止计时目的
		result = gtk_dialog_run(GTK_DIALOG(dialog));		//堵塞在这里，等待用户回应
		switch(result){
			case -1 : printf("case1\n");break;
			case -2 : printf("case2\n");break;
			case -3 : printf("case3\n");break;
			case -4 : printf("case4\n");break;
			case -5 : printf("case5\n");break;
			case -6 : printf("case6\n");break;
			case -7 : printf("case7\n");break;
			case -8 : printf("case8\n");local_userinfo.u_status = 0;gtk_exit(0);break;//yes
			case -9 : printf("case9\n");break;//NO
			case -10: printf("case7\n");break;
			case -11 : printf("case7\n");break;
			
			default : printf("error\n");
		}
		gtk_widget_destroy(dialog);
	}
	return 0;
}



