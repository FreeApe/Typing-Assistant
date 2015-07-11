#ifndef __SWING_H_
#define __SWING_H_

#include <gtk/gtk.h>

typedef struct{
	GtkWidget *window;				//主窗口
	GtkWidget *entry_correct;		//打字正确个数
	GtkWidget *entry_error;			//打字错误个数
	GtkWidget *entry_AR;			//打字正确率
	GtkWidget *entry_speed;			//打字速度
	GtkWidget *entry_ctime;			//打字计时显示单行文本框
	GtkWidget *entry_clock;			//打字练习定时
	
}ENTRY_SHOW;

typedef struct{
	GdkPixbuf *src_pixbuf; 		//
	GdkPixbuf *dst_pixbuf; 		//
	GdkPixmap *pixmap;			//定义内存图片缓冲区
}PICTURE_SHOW;

/*************宏定义start...********************/
#define WINDOW_W	1000
#define WINDOW_H	690

//颜色宏定义
#define MIRROR_TEXT_BG		{0, 0xFFFF, 0xFFFF, 0xFFFF}		//blue:对照文本框背景颜色
#define MIRROR_TEXT         {0, 0x0000, 0x0000, 0x0000}		//yellow:对照文本颜色

#define ENTRY_TEXT_BG		{0, 0xFFFF, 0xFFFF, 0xFFFF}		//浅蓝:输入文本框背景颜色
#define ENTRY_TEXT   		{0, 0x0000, 0x0000, 0x0000}		//yellow:输入文本颜色

#define ERROR_COLOR_BG		{0, 0xffff, 0x0000, 0x0000}		//red:错误文本框背景颜色
#define	CORRECT_COLOR_BG	{0, 0x0000, 0x0000, 0xffff}		//blue:正确文本框背景颜色

#define FONT_HEIGHT  40
#define FONT_WIDTH   836
#define LEFT_MARGIN  ((1000-FONT_WIDTH)/2)
#define INIT_TOP     170
#define SPACE        5
#define PIC_WIDGET	 20			              //笑脸等图标边长
/*************宏定义end...**********************/
////////////////////////////////////////////////
/*************定义变量start...******************/
ENTRY_SHOW show;				//单行文本框
GtkTextBuffer *buffer[10];		//多行文本框缓冲区
GtkWidget *view[10];			//多行文本框
/*************定义变量end...********************/



//加载窗口
void show_window();
//获取练习的用户的信息对话框
gint get_user_message(GtkWidget *widget,gpointer data);
//文件选择下拉组合框信号回调函数
gboolean combox_file_changed(GtkCombo *combo, gpointer data);
//checkbutton回调函数
void toggle_checkbutton_callback(GtkWidget *widget,gpointer data);
//进入关闭窗口按钮回调函数
void quit_button_enter( GtkWidget *widget,gpointer   data );
//构件背景图片
void update_widget_bg(GtkWidget *widget, gchar *img_file);
//显示子窗口
void show_child_window(GtkWidget *widget,gpointer data);
//隐藏子窗口
void hide_child_window(GtkWidget *widget,gpointer data);
//login_window
int login_window();
//设置窗口
int setting_window();
//stop count time 回调函数
int stop_count_time(GtkWidget *widget,gpointer data);

#endif

