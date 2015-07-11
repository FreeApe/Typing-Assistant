#ifndef __HANDLE_H_
#define __HANDLE_H_

#define LINE_NUMBER			52

//显示文件中的文本到对照文本框中
int show_file_text(char *dir,char *fname);
//输入文本框中的信号处理
void text_signal_pro();
//开始输入文本
void start_enter_text_callback(GtkTextBuffer *widget,gpointer  *data);
//删除文本回调函数
void delete_backspace_text_callback(GtkTextBuffer *widget,
    gpointer  *data);

//获取对照文本框中相应位置的字符
gchar mirror_textview_get_char(gint pos);
//判断两个字符是否相等
int char_is_equal(gchar ch1,gchar ch2);
//改变字符背景颜色:
void change_textview_bg(GtkTextBuffer *buf,gboolean tf);
//创建标记tag
void create_buffer_tag();
//删除当前行文本字符 ch1:当前行最后一个字符，ch2:当前对照行对应的字符
void delete_current_line_textchar(gchar ch1,gchar ch2);
//改变字体的颜色:正确则灰色，错误则红色
void change_textview_font_color(GtkTextBuffer *buf,GtkTextBuffer *buf1,gint view_f,gboolean tf);


//定时器初始化
void timer_init();
//stop count time 回调函数
int stop_count_time(GtkWidget *widget,gpointer data);
//定时器溢出回调函数:返回TRUE时，定时器继续，FALSE时定时器停止
gboolean timeout_callback(gpointer *data);
//任何时刻按下ESC键退出信号回调函数
gint ESC_quit(GtkWidget* widget,GdkEventKey* event,gpointer data);
//去除当前文本框光标所在的一个字符
static void remove_textview_enter_keypress(GtkWidget *widget, gpointer  data);

#endif

