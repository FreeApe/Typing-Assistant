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
//����ƶ���ı��źŻص�����
static void mark_set_callback(GtkTextBuffer *buffer,
    const GtkTextIter *new_location, GtkTextMark *mark,
    gpointer data);
//textview�س��ص�����
static gboolean textview_key_press(GtkWidget *widget, GdkEvent *e
                                            , gpointer data);

extern GtkWidget *stop_count;
extern USER_INFO local_userinfo;	
int time_flag;
gdouble start_time = 0.0;

char *page;
unsigned cont_len;										//ʣ����ļ��ĳ���
gsize length ;											//��ȡ���ļ��ĳ���
gchar *content;
gint view_focus = 1;									//�۽����ĸ�textview
gint viewnum = 1;							

gint correct_num;
gint error_num;
gint all_num;

gint before_bp;											//��ɾ֮ǰ�ַ��Ա�״̬
gint backspace_status;									//�Ƿ���������ɾ
gint backspacefg;										//��ɾ���
gint after_bp;											//��ɾ֮���ַ��Ա�״̬


//len:ʣ�೤��  cont:ʣ�����ʾ�ı�����
char* buffer_get_text_from_content(gchar *cont,gsize len)
{
	GtkTextIter start, end, iter;
	GtkTextMark *mark;
	int t_num =0 ;
	
	cont_len = len-LINE_NUMBER*5;
	if(cont_len <= LINE_NUMBER*5 ){
		cont_len = length;							//���¸�ֵԭ�ı�����
		printf("length= %d cont_len= %d there is not any word to read in bye.txt!\n",
			length,cont_len);
		return NULL;								//�Ѿ�û�����ֿ���ʾ��,then...
	}
	int i = 0;
	for(i =0 ;i< 10 ;i++){
		gtk_text_buffer_get_bounds(buffer[i],&start,&end);//��ȡ��ǰbuffer���ı���ʼ�������		
		gtk_text_buffer_delete(buffer[i],&start,&end);  //����ı�������
	}
	//view[0]��ʾ�ı�
    mark = gtk_text_buffer_get_insert (buffer[0]);
    gtk_text_buffer_get_iter_at_mark (buffer[0], &iter, mark);
	for(t_num =0;t_num <LINE_NUMBER;t_num++){
		if((*cont == '\n') || (*cont == '\r')){cont++;t_num--;continue;}
		gtk_text_buffer_insert(buffer[0],&iter,cont++,1);	
	}
	//view[2]��ʾ�ı�
    mark = gtk_text_buffer_get_insert (buffer[2]);
    gtk_text_buffer_get_iter_at_mark (buffer[2], &iter, mark);
	for(t_num =0;t_num <LINE_NUMBER;t_num++){
		if((*cont == '\n') || (*cont == '\r')){cont++;t_num--;continue;}
		gtk_text_buffer_insert(buffer[2],&iter,cont++,1);
	}
	//view[4]��ʾ�ı�
    mark = gtk_text_buffer_get_insert (buffer[4]);
    gtk_text_buffer_get_iter_at_mark (buffer[4], &iter, mark);
	for(t_num =0;t_num <LINE_NUMBER;t_num++){
		if((*cont == '\n') || (*cont == '\r')){cont++;t_num--;continue;}
		gtk_text_buffer_insert(buffer[4],&iter,cont++,1);	
	}
	//view[6]��ʾ�ı�
    mark = gtk_text_buffer_get_insert (buffer[6]);
    gtk_text_buffer_get_iter_at_mark (buffer[6], &iter, mark);
	for(t_num =0;t_num <LINE_NUMBER;t_num++){
		if((*cont == '\n') || (*cont == '\r')){cont++;t_num--;continue;}
		gtk_text_buffer_insert(buffer[6],&iter,cont++,1);
	}
	//view[8]��ʾ�ı�
    mark = gtk_text_buffer_get_insert (buffer[8]);
    gtk_text_buffer_get_iter_at_mark (buffer[8], &iter, mark);
	for(t_num =0;t_num <LINE_NUMBER;t_num++){
		if((*cont == '\n') || (*cont == '\r')){cont++;t_num--;continue;}
		gtk_text_buffer_insert(buffer[8],&iter,cont++,1);	//�������ڶ�ҳ��ʾ	
	}	
	gtk_window_set_focus(GTK_WINDOW(show.window),view[1]);	//�۽�����һ���ı������
	return cont;
}
//��ʾ�ļ��е��ı��������ı�����
int show_file_text(char *dir,char *fname)
{
	GtkTextIter iter;
	GtkTextMark *mark;
	GError *error = NULL;

	gchar *filename;
	filename = g_build_filename(FILE_FILES,"select article.txt",NULL);//���ļ�
    if (!g_file_test (filename, G_FILE_TEST_EXISTS)){
        g_error ("Error: File does not exist!please check!");
		return -1;
    }
	g_file_get_contents(filename,&content,&length,&error);
	handle_error(error);

	page = buffer_get_text_from_content(content,length);	//���ν��ļ���������ʾ���ı�����

	//�������
	create_buffer_tag();
}
//�����ı����е��źŴ���
void text_signal_pro()
{
	int i = 0;

	for(i = 0;i < 10;i++){
		if((i%2) != 0){
			g_signal_connect(buffer[i], "changed",				//�ı��ı��ź�
			    G_CALLBACK(start_enter_text_callback), NULL);
			g_signal_connect(view[i], "backspace",				//�ı����ɾ�ź�
			    G_CALLBACK(delete_backspace_text_callback), NULL);
    		g_signal_connect(view[i],"key-press-event"		//ע�ᰴ�������Ϣ
               	, G_CALLBACK(textview_key_press), view[i]);

		}
		g_signal_connect_object(buffer[i], "mark_set", 			//����ƶ���mark_set�źű����ͳ�ȥ
			G_CALLBACK(mark_set_callback), NULL, 0);			//NULL�ɱ��滻Ϊ��������data
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

//ȥ����ǰ�ı��������ڵ�һ���ַ�
static void remove_textview_enter_keypress(GtkWidget *widget, gpointer  data)
{
	GtkTextIter b_end1,b_end2;
	gint chnum = 0;
	gchar * text1,*text2;

	chnum = gtk_text_buffer_get_char_count(buffer[view_focus]);	//��ȡ��ǰbuffer�е��ַ�����
	if(chnum < LINE_NUMBER){
		gtk_text_buffer_get_end_iter(buffer[view_focus], &b_end1);//��ȡ��ǰbuffer���ı���ʼ�������		
		b_end2 = b_end1;
		gtk_text_iter_backward_char(&b_end2);	
		gtk_text_buffer_delete(buffer[view_focus],&b_end2,&b_end1);

		gtk_window_set_focus(GTK_WINDOW(show.window),view[view_focus]);
	}	
	else ;
}

//textview�س��ص�����
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
//����ƶ���ı��źŻص�����
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
//	    chcount = gtk_text_buffer_get_char_count(buffer[view_focus]);//��ȡbuffer�������ַ���
//		col = gtk_text_iter_get_line_offset(&iter);					 //��ȡ�����������
//		if(col != chcount){
			//�������β(δ���);  
//			cursor = gtk_editable_get_position(GTK_EDITABLE(view[view_focus]));
//			gtk_editable_set_position(GTK_EDITABLE(view[view_focus]),chcount-1);         
//			printf("chcount=%d col=%d cursor=%d\n",chcount,col,cursor);
//		}
	}
}

//�źŵ���:��ʼ�����ı�
void start_enter_text_callback(GtkTextBuffer *widget,
    gpointer  *data)
{
    GtkTextIter start, end, iter;
    gchar *text;
	gchar mirror_ch, enter_ch;
	gint chnum = 0 ;
	//g_printf("=====================================enter======\n");
	//g_printf("viewnum= %d  viewfocus=%d\n",viewnum,view_focus);
	chnum = gtk_text_buffer_get_char_count(widget);			//��ȡ��ǰbuffer�е��ַ�����

	gtk_text_buffer_get_bounds(widget,&start,&end);			//��ȡ��ǰbuffer���ı���ʼ�������
	text=gtk_text_buffer_get_text(widget,&start,&end,FALSE);//��ȡ��ǰbuffer�е��ı�
	enter_ch = *(text+strlen(text)-1);

	if(view_focus < 11){
		mirror_ch = mirror_textview_get_char(chnum);
		//printf("#enter==%c==#mirror===%c===\n",enter_ch,mirror_ch);
		delete_current_line_textchar(mirror_ch, enter_ch);	//��ǰ�����봦��
	}
		//�õ���������ַ���=��ȷ�� + ������
	all_num = correct_num + error_num;						//��������ַ���
	//printf("all_num=%d correct_num=%d error_num=%d\n",all_num,correct_num,error_num);
		//�л�textview
	if(chnum > (LINE_NUMBER-1)){
		viewnum = viewnum+2;
		view_focus = viewnum;
		//printf("viewnum = %d\n",viewnum);

		if(viewnum < 11){
			gtk_window_set_focus(GTK_WINDOW(show.window),view[viewnum]);//һ�������Զ��л���	
		}
		else {
			//printf("page up\n");
			if((NULL !=(page=buffer_get_text_from_content(page,cont_len)))){
			}
			else {
				page = content;								//���¸�ֵԭ�ı����ݣ�ѭ���ı�
				page = buffer_get_text_from_content(page,cont_len);	//���ļ���������ʾ���ı�����
			}
			viewnum = 1;									//��ҳ���¶�λ����һ��
			view_focus = viewnum;
			gtk_window_set_focus(GTK_WINDOW(show.window),view[viewnum]);//һ�������Զ��л���			
		}
	}
}
//ɾ���ı��ص�����
void delete_backspace_text_callback(GtkTextBuffer *widget,
    gpointer  *data)
{
	GtkTextIter b_end1,b_end2;
	gint chnum = 0;
	gchar * text1,*text2;
	
	if ((all_num % (LINE_NUMBER*5)) != 0)backspacefg =1;
	else backspacefg = 0;
	//printf("backspacefg = %d\n",backspacefg);
	chnum = gtk_text_buffer_get_char_count(buffer[view_focus]);	//��ȡ��ǰbuffer�е��ַ�����
	if(chnum == 0){
		if(view_focus ==1) ;								//��һ���޶���
		else {
			//��ò��Ƚϴ�ɾ������ĩ�ַ������Ƿ���ͬ
			gtk_text_buffer_get_end_iter(buffer[view_focus-3], &b_end1);//��ȡ��ǰbuffer���ı���ʼ�������		
			b_end2 = b_end1;
			gtk_text_iter_backward_char(&b_end2);	
			text1 = gtk_text_buffer_get_text (buffer[view_focus-3], &b_end2, &b_end1, FALSE);

			gtk_text_buffer_get_end_iter(buffer[view_focus-2], &b_end1);//��ȡ��ǰbuffer���ı���ʼ�������		
			b_end2 = b_end1;
			gtk_text_iter_backward_char(&b_end2);	
			text2 = gtk_text_buffer_get_text (buffer[view_focus-2], &b_end2, &b_end1, FALSE);
			//printf("text1=%c text2=%c\n",*text1,*text2);
			if(char_is_equal(*(text1), *(text2))) after_bp = 1;
			else after_bp = 0;
			view_focus = view_focus - 2;
			viewnum = viewnum -2;
			//ɾ����һ�����һ���ַ���Ȼ��۽�����һ��
			gtk_text_buffer_delete(buffer[view_focus],&b_end2,&b_end1);
							///�˴���changed�ź��ж�,��ִ��changed�ٻ�������ִ��
			//printf("$viewnum= %d  viewfocus=%d\n",viewnum,view_focus);

			gtk_window_set_focus(GTK_WINDOW(show.window),view[view_focus]);
		}	
	}	
	else ;
}
//��ȡ�����ı�������Ӧλ�õ��ַ�
gchar mirror_textview_get_char(gint pos)
{
    GtkTextIter start, end;
    gchar *text;
    gtk_text_buffer_get_bounds (buffer[view_focus-1], &start, &end);
    text = gtk_text_buffer_get_text (buffer[view_focus-1], &start, &end, FALSE);
	return (*(text+pos-1));
}
//�ж������ַ��Ƿ����
int char_is_equal(gchar ch1,gchar ch2)
{
	if((ch1 == ch2)){
		if(ch1 > 0 )return T;
		else return NUL;
	}
	return F;
}

//�ı��ַ�������ɫ:
void change_textview_bg(GtkTextBuffer *buf,gboolean tf)
{
	GtkTextIter iter, start, end;

	
	gtk_text_buffer_get_bounds(buf, &start, &end);
	start = end;
	
	gtk_text_iter_backward_char(&start);			//start����˺�һ���ַ�
		
	if(tf){
		gtk_text_buffer_apply_tag_by_name(buf,"bluebg",&start,&end);
	}
	else {
		gtk_text_buffer_apply_tag_by_name(buf,"redbg",&start,&end);
	}
}
//�ı��������ɫ:��ȷ���ɫ���������ɫ
void change_textview_font_color(GtkTextBuffer *buf,GtkTextBuffer *buf1,gint view_f,gboolean tf)
{
	GtkTextIter start1, end1,start2,end2;

	
	gtk_text_buffer_get_bounds(buf, &start1, &end1);
	gtk_text_buffer_get_bounds(buf1, &start2, &end2);

	start1 = end1;
	start2 = end2;
	
	gtk_text_iter_backward_char(&start1);			//start����˺�һ���ַ�
	gtk_text_iter_backward_char(&start2);			//start����˺�һ���ַ�
		
	if(tf){
		gtk_text_buffer_apply_tag_by_name(buf1,"blue_fg",&start2,&end2);			
		gtk_text_buffer_apply_tag_by_name(buf,"blue_fg",&start1,&end1);
	}
	else {
		gtk_text_buffer_apply_tag_by_name(buf,"red_fg",&start1,&end1);
		gtk_text_buffer_apply_tag_by_name(buf1,"red_fg",&start2,&end2);	
	}

}
//�������tag
void create_buffer_tag()
{
	//�������
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

//ɾ����ǰ���ı��ַ�
void delete_current_line_textchar(gchar ch1,gchar ch2)
{
	if(backspacefg == 1){			//�ǻ�ɾ
		backspacefg = 0;
		backspace_status++;
		if(backspace_status == 1){	//��������ɾ
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
		else {						//������ɾ
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
	else {							//���ǻ�ɾ
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
		else ;					//������²�����Ч�ַ�����������
		//printf("********it is[ not ]backspace \n");
	}		
}

//��ʱ����ʼ��
void timer_init()
{
	GTimer *timer;					//��ʱ��

	//��ʱ����ʱ
    timer = g_timer_new ();
	g_timer_start(timer);
	gtk_timeout_add(1000,(GSourceFunc)timeout_callback,NULL);//ÿ��һ����ûص�����
}

//stop count time �ص�����
int stop_count_time(GtkWidget *widget,gpointer data)
{
	GtkTextIter start, end;
	//printf("time flag =%d=\n",time_flag);
	time_flag = !time_flag;
	if(time_flag == 1){
		correct_num = 0;								//bug,,,ÿ��start��correct_num=2
		error_num = 0;
		all_num = 0;
		view_focus = 1;									//�۽����ĸ�textview
		viewnum = 1;							
		start_time=0;
		gtk_button_set_label(GTK_BUTTON(stop_count),"START");
		return FALSE;
	}
	else {
		//��textview�������
		int i = 0;
		//printf("======%d=%d=====\n",i,correct_num);

		for(i =0 ;i< 10 ;i++){
			if((i % 2) != 0){
				gtk_text_buffer_get_bounds(buffer[i],&start,&end);//��ȡ��ǰbuffer���ı���ʼ�������		
				gtk_text_buffer_delete(buffer[i],&start,&end);  //����ı�������
			}
			//printf("======%d=%d=====\n",i,correct_num);
		}
		gtk_window_set_focus(GTK_WINDOW(show.window),view[1]);	//�۽�����һ���ı������
		gtk_button_set_label(GTK_BUTTON(stop_count),"STOP");
		return TRUE;
	}	
}

//��ʱ������ص�����:����TRUEʱ����ʱ��������FALSEʱ��ʱ��ֹͣ
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
		gtk_entry_set_text(GTK_ENTRY(show.entry_ctime),time_num);	//��ʾ��entry_ctime
		entry_show_data();
		return TRUE;
	}
	return TRUE;
}

//�κ�ʱ�̰���ESC���˳��źŻص�����
gint ESC_quit(GtkWidget* widget,GdkEventKey* event,gpointer data)
{
	gchar c;
	GtkWidget *dialog;
	gint result;
	
	c=(gchar)event->keyval;
	if(c == 0x1b){//ascii��: ESC��
		dialog = gtk_message_dialog_new((GtkWindow *)show.window,
		            GTK_DIALOG_DESTROY_WITH_PARENT,
		            GTK_MESSAGE_QUESTION,
		            GTK_BUTTONS_YES_NO,
		            "Are you sure to quit?");
		gtk_window_set_title(GTK_WINDOW(dialog), "quit app");

		time_flag = 1;//g_timer_destroy(timer);								//�ݻٶ�ʱ���ﵽֹͣ��ʱĿ��
		result = gtk_dialog_run(GTK_DIALOG(dialog));		//����������ȴ��û���Ӧ
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



