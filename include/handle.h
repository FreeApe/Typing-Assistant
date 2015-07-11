#ifndef __HANDLE_H_
#define __HANDLE_H_

#define LINE_NUMBER			52

//��ʾ�ļ��е��ı��������ı�����
int show_file_text(char *dir,char *fname);
//�����ı����е��źŴ���
void text_signal_pro();
//��ʼ�����ı�
void start_enter_text_callback(GtkTextBuffer *widget,gpointer  *data);
//ɾ���ı��ص�����
void delete_backspace_text_callback(GtkTextBuffer *widget,
    gpointer  *data);

//��ȡ�����ı�������Ӧλ�õ��ַ�
gchar mirror_textview_get_char(gint pos);
//�ж������ַ��Ƿ����
int char_is_equal(gchar ch1,gchar ch2);
//�ı��ַ�������ɫ:
void change_textview_bg(GtkTextBuffer *buf,gboolean tf);
//�������tag
void create_buffer_tag();
//ɾ����ǰ���ı��ַ� ch1:��ǰ�����һ���ַ���ch2:��ǰ�����ж�Ӧ���ַ�
void delete_current_line_textchar(gchar ch1,gchar ch2);
//�ı��������ɫ:��ȷ���ɫ���������ɫ
void change_textview_font_color(GtkTextBuffer *buf,GtkTextBuffer *buf1,gint view_f,gboolean tf);


//��ʱ����ʼ��
void timer_init();
//stop count time �ص�����
int stop_count_time(GtkWidget *widget,gpointer data);
//��ʱ������ص�����:����TRUEʱ����ʱ��������FALSEʱ��ʱ��ֹͣ
gboolean timeout_callback(gpointer *data);
//�κ�ʱ�̰���ESC���˳��źŻص�����
gint ESC_quit(GtkWidget* widget,GdkEventKey* event,gpointer data);
//ȥ����ǰ�ı��������ڵ�һ���ַ�
static void remove_textview_enter_keypress(GtkWidget *widget, gpointer  data);

#endif

