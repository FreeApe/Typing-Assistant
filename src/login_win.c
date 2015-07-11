#include <gtk/gtk.h>
#include "swing.h"
#include "path.h"

#define WIN_W	380
#define WIN_H	290

GtkWidget *win;

static gboolean drag = FALSE;   // 只在左键按下时拖动窗体
static int nX = 0;
static int nY = 0;

static gint button_press_event(GtkWidget * widget,
          GdkEventButton * event, gpointer data)
{
    if (event->button == 1)	// 判断是否左键按下
    {
		drag = TRUE;
		nX = event->x;  	// 取得鼠标相对于窗口的位置
		nY = event->y;
    }
    return TRUE;
}

static gint button_release_event(GtkWidget * widget, GdkEventButton * event,
          gpointer data)         // 鼠标抬起事件
{
    if (event->button == 1)
             drag = FALSE;
    return TRUE;
} 

static gint motion_notify_event(GtkWidget * widget, GdkEventButton * event,
          gpointer data)         // 鼠标移动事件
{
    if (drag)
    {
         int x, y;
         GtkWidget *window = (GtkWidget *) data;
         gtk_window_get_position((GtkWindow *) window, &x, &y);         // 取窗体绝对坐标
         gtk_window_move((GtkWindow *) window, x + event->x - nX,
                 y + event->y - nY);// 移动窗体
    }
    return TRUE;
}


//login_submit_callback
void login_submit_callback(GtkWidget *widget ,gpointer data)
{
	//获取登陆用户名和密码

}

//login_window
int login_window()
{
	GtkWidget *table;
	GtkWidget *username;
	GtkWidget *passwd;
	GtkWidget *logo;
	GtkWidget *submit,*quit;
	GtkWidget *hbox;
	
	//登陆窗口初始化
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (win, WIN_W, WIN_H);//设置主窗口默认打开大小
    gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
    gtk_window_set_decorated (GTK_WINDOW(win), FALSE);	//包含标题栏，右边界
    //gtk_window_set_title (GTK_WINDOW(win), "login");
   	gtk_window_set_resizable ((GtkWindow *)win,FALSE);	//固定窗口显示大小
	gtk_container_set_border_width(GTK_CONTAINER(win),0);//窗口空白边界大小
	gtk_window_set_deletable(GTK_WINDOW(win),FALSE);	//禁止叉叉可以关闭

//    g_signal_connect (G_OBJECT(win), "destroy",
//                        G_CALLBACK(gtk_main_quit), NULL);//X可关闭
	//添加背景
	update_widget_bg(win, LOGIN_WIN);

	//创建table容器，并添加到窗口中
	table =  gtk_table_new(WIN_W,WIN_H,TRUE);
	gtk_container_add(GTK_CONTAINER(win),table);
	//创建文本框和登陆按钮
	username = gtk_entry_new();
	passwd = gtk_entry_new();
	gtk_entry_set_visibility (GTK_ENTRY(passwd),FALSE);	//密码文字不可见
	
	submit = gtk_button_new_with_label("登陆");
	quit = gtk_button_new_with_label("退出");
	//将按钮放入vbox
	hbox = gtk_hbox_new(TRUE,5);
	gtk_box_pack_start(GTK_BOX(hbox),submit,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox),quit,TRUE,TRUE,0);
	//构件大小设置
	gtk_widget_set_size_request(username, 205, 30);		//设置大小
	gtk_widget_set_size_request(passwd, 205, 30);		//设置大小
//	gtk_widget_set_size_request(hbox, 145, 40);		//设置大小
	
	//添加构件到容器中
	gtk_table_attach_defaults(GTK_TABLE(table),username,95,250, 140 ,177);
	gtk_table_attach_defaults(GTK_TABLE(table),passwd,95,250, 180 ,217);
	gtk_table_attach_defaults(GTK_TABLE(table),hbox,100,245, 240 ,280);
	g_signal_connect(G_OBJECT(quit),"clicked",G_CALLBACK(hide_child_window),GTK_WINDOW(win));
	g_signal_connect(G_OBJECT(submit),"clicked",G_CALLBACK(login_submit_callback),GTK_WINDOW(win));

	//鼠标事件捕获

	gtk_widget_set_events(win,  // 设置窗体获取鼠标事件
	       GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
	       | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK
	       | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);
	gtk_signal_connect(GTK_OBJECT(win), "button_press_event",
	           (GtkSignalFunc) button_press_event, win);       // 加入事件回调
	gtk_signal_connect(GTK_OBJECT(win), "motion_notify_event",
	           (GtkSignalFunc) motion_notify_event, win);
	gtk_signal_connect(GTK_OBJECT(win), "button_release_event",
	           (GtkSignalFunc) button_release_event, win);
	
	gtk_widget_show_all(win);

	return 0;
}
