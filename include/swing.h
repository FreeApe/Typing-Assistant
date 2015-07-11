#ifndef __SWING_H_
#define __SWING_H_

#include <gtk/gtk.h>

typedef struct{
	GtkWidget *window;				//������
	GtkWidget *entry_correct;		//������ȷ����
	GtkWidget *entry_error;			//���ִ������
	GtkWidget *entry_AR;			//������ȷ��
	GtkWidget *entry_speed;			//�����ٶ�
	GtkWidget *entry_ctime;			//���ּ�ʱ��ʾ�����ı���
	GtkWidget *entry_clock;			//������ϰ��ʱ
	
}ENTRY_SHOW;

typedef struct{
	GdkPixbuf *src_pixbuf; 		//
	GdkPixbuf *dst_pixbuf; 		//
	GdkPixmap *pixmap;			//�����ڴ�ͼƬ������
}PICTURE_SHOW;

/*************�궨��start...********************/
#define WINDOW_W	1000
#define WINDOW_H	690

//��ɫ�궨��
#define MIRROR_TEXT_BG		{0, 0xFFFF, 0xFFFF, 0xFFFF}		//blue:�����ı��򱳾���ɫ
#define MIRROR_TEXT         {0, 0x0000, 0x0000, 0x0000}		//yellow:�����ı���ɫ

#define ENTRY_TEXT_BG		{0, 0xFFFF, 0xFFFF, 0xFFFF}		//ǳ��:�����ı��򱳾���ɫ
#define ENTRY_TEXT   		{0, 0x0000, 0x0000, 0x0000}		//yellow:�����ı���ɫ

#define ERROR_COLOR_BG		{0, 0xffff, 0x0000, 0x0000}		//red:�����ı��򱳾���ɫ
#define	CORRECT_COLOR_BG	{0, 0x0000, 0x0000, 0xffff}		//blue:��ȷ�ı��򱳾���ɫ

#define FONT_HEIGHT  40
#define FONT_WIDTH   836
#define LEFT_MARGIN  ((1000-FONT_WIDTH)/2)
#define INIT_TOP     170
#define SPACE        5
#define PIC_WIDGET	 20			              //Ц����ͼ��߳�
/*************�궨��end...**********************/
////////////////////////////////////////////////
/*************�������start...******************/
ENTRY_SHOW show;				//�����ı���
GtkTextBuffer *buffer[10];		//�����ı��򻺳���
GtkWidget *view[10];			//�����ı���
/*************�������end...********************/



//���ش���
void show_window();
//��ȡ��ϰ���û�����Ϣ�Ի���
gint get_user_message(GtkWidget *widget,gpointer data);
//�ļ�ѡ��������Ͽ��źŻص�����
gboolean combox_file_changed(GtkCombo *combo, gpointer data);
//checkbutton�ص�����
void toggle_checkbutton_callback(GtkWidget *widget,gpointer data);
//����رմ��ڰ�ť�ص�����
void quit_button_enter( GtkWidget *widget,gpointer   data );
//��������ͼƬ
void update_widget_bg(GtkWidget *widget, gchar *img_file);
//��ʾ�Ӵ���
void show_child_window(GtkWidget *widget,gpointer data);
//�����Ӵ���
void hide_child_window(GtkWidget *widget,gpointer data);
//login_window
int login_window();
//���ô���
int setting_window();
//stop count time �ص�����
int stop_count_time(GtkWidget *widget,gpointer data);

#endif

