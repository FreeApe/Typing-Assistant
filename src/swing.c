#include <gtk/gtk.h>
#include <string.h>
#include <glib.h>
#include <swing.h>
#include <stdlib.h>
#include "path.h"
#include "net.h"

GtkWidget *combo_file;
GList *list = NULL;
extern GtkWidget *win;
extern GtkWidget *set_win;
extern USER_INFO local_userinfo;	
GtkWidget *stop_count;


/*****打字速度排名变量******/
char num_one[50] = "";			//"ycmhy 350"
char num_two[50] = "";		
char num_thr[50] = "";
char num_fou[50] = "";				//"ycmhy 350"
char num_fiv[50] = "";		
char num_six[50] = "";

GtkWidget *label_one_p;			//gtk_label_set_markup(GTK_LABEL(label_one_p),num_one);
GtkWidget *label_two_p;
GtkWidget *label_thr_p;
GtkWidget *label_fou_p;
GtkWidget *label_fiv_p;
GtkWidget *label_six_p;

GtkWidget *label_current_num_show;//当前用户在线人数显示
GtkWidget *username, *passwd ;
/**************************/


static gboolean drag = FALSE;   // 只在左键按下时拖动窗体
static int nX = 0;
static int nY = 0;

static gint button_press_event(GtkWidget * widget,
          GdkEventButton * event, gpointer data)
{
    if (event->button == 1)// 判断是否左键按下
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


gboolean pic_load(GtkWidget* widget,gpointer data)
{
	GtkWidget *pic;
	pic= gtk_image_new_from_file (IMG_CORRECT);
	gtk_table_attach_defaults(GTK_TABLE(widget),pic,140,160,640,660);
	pic= gtk_image_new_from_file (IMG_ERROR);
	gtk_table_attach_defaults(GTK_TABLE(widget),pic,230,250,640,660);
	pic= gtk_image_new_from_file (IMG_ACCURACY_RATE);
	gtk_table_attach_defaults(GTK_TABLE(widget),pic,320,380,640,660);
	pic= gtk_image_new_from_file (IMG_SPEED);
	gtk_table_attach_defaults(GTK_TABLE(widget),pic,450,510,640,660);
	pic= gtk_image_new_from_file (IMG_TIME);
	gtk_table_attach_defaults(GTK_TABLE(widget),pic,570,590,640,660);
	pic= gtk_image_new_from_file (IMG_CLOCK);
	gtk_table_attach_defaults(GTK_TABLE(widget),pic,660,680,640,660);

	//设置按钮背景图片
	pic= gtk_image_new_from_file (IMG_SETTING);
	gtk_table_attach_defaults(GTK_TABLE(widget),pic,980,1000,670,690);
	return TRUE;
}

//初始化entry
void entry_init()
{
	PangoFontDescription *Entryfont;
	Entryfont = pango_font_description_from_string("DejaVu Sans Mono Bold 8");
	/**************Entry1 init*****************/
	show.entry_correct = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(show.entry_correct),"0");		//初始化显示内容	
	gtk_widget_modify_font(show.entry_correct,Entryfont);		//设置字体和显示大小
	/**************Entry2 init*****************/	
	show.entry_error= gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(show.entry_error),"0");
	gtk_widget_modify_font(show.entry_error,Entryfont);
	/**************Entry3 init*****************/
	show.entry_AR= gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(show.entry_AR),"0.0");
	gtk_widget_modify_font(show.entry_AR,Entryfont);
	/**************Entry4 init*****************/
	show.entry_speed= gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(show.entry_speed),"0 w/min");
	gtk_widget_modify_font(show.entry_speed,Entryfont);
	/**************Entry5 init*****************/
	show.entry_ctime= gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(show.entry_ctime),"0");
	gtk_widget_modify_font(show.entry_ctime,Entryfont);
	/**************Entry6 init*****************/
	show.entry_clock= gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(show.entry_clock),"5min");
	gtk_widget_modify_font(show.entry_clock,Entryfont);


	gtk_entry_set_editable((GtkEntry *)show.entry_correct,FALSE);	//设置是否可编辑
	gtk_entry_set_editable((GtkEntry *)show.entry_error,FALSE);
	gtk_entry_set_editable((GtkEntry *)show.entry_AR,FALSE);
	gtk_entry_set_editable((GtkEntry *)show.entry_speed,FALSE);
	gtk_entry_set_editable((GtkEntry *)show.entry_ctime,FALSE);
	gtk_entry_set_editable((GtkEntry *)show.entry_clock,FALSE);

	gtk_widget_set_size_request(show.entry_correct, 60, PIC_WIDGET);		//设置文本框大小
	gtk_widget_set_size_request(show.entry_error, 60, PIC_WIDGET);
	gtk_widget_set_size_request(show.entry_AR, 60, PIC_WIDGET);
	gtk_widget_set_size_request(show.entry_speed, 60, PIC_WIDGET);
	gtk_widget_set_size_request(show.entry_ctime, 60, PIC_WIDGET);
	gtk_widget_set_size_request(show.entry_clock, 60, PIC_WIDGET);

}
//加载数据显示框
void view_init()
{
	int i = 0;
	PangoFontDescription *viewfont;
	//Mono:等宽字体 Bold:粗体 Italic:斜体 字体:DejaVu Sans 一行:显42个字母
	viewfont = pango_font_description_from_string("DejaVu Sans Mono Bold 20");//2	
	GdkColor error_color_bg = ERROR_COLOR_BG;
	GdkColor correct_color_bg = CORRECT_COLOR_BG;
    GdkColor entry_text = ENTRY_TEXT;
    GdkColor entry_text_bg = ENTRY_TEXT_BG;
    GdkColor mirror_text = MIRROR_TEXT;
	GdkColor mirror_text_bg = MIRROR_TEXT_BG;
//	gdk_color_parse ("green", &error_color_bg);								//准备颜色
	//偶数
	for(i=0;i<10;i++){
		if((i%2)==0){
			view[i] = gtk_text_view_new();						     //创建英文对照textview
			gtk_text_view_set_editable(GTK_TEXT_VIEW(view[i]),FALSE);//设置文本是否可编辑
			gtk_widget_modify_text(view[i],GTK_STATE_NORMAL,&mirror_text);//文本框字体颜色设置
			gtk_widget_modify_base(view[i],GTK_STATE_NORMAL,&mirror_text_bg); //文本框背景颜色
			
		}
	
	}
	//奇数
	for(i=0;i<10;i++){
		if((i%2)!=0){
			view[i] = gtk_text_view_new();							 //创建用户输入textview
			gtk_text_view_set_editable(GTK_TEXT_VIEW(view[i]),TRUE); //设置文本是否可编辑
			gtk_widget_modify_text(view[i],GTK_STATE_NORMAL,&entry_text);//文本框字体颜色设置
			gtk_widget_modify_base(view[i],GTK_STATE_NORMAL,&entry_text_bg); //文本框背景颜色
		}
	}
	//1~10
	for(i=0;i<10;i++){
		gtk_widget_modify_font(view[i],viewfont);					//文本框字体设置
		gtk_widget_set_size_request(view[i], FONT_WIDTH, FONT_HEIGHT);		//设置文本框大小
		gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (view[i]), GTK_WRAP_CHAR);//自动换行
	}
}
//构件背景图片
void update_widget_bg(GtkWidget *widget, gchar *img_file)
{
    GtkStyle *style;
    GdkPixbuf *pixbuf;
    GdkPixmap *pixmap;
	gint width,height;
        
    pixbuf = gdk_pixbuf_new_from_file(img_file, NULL);

	width = gdk_pixbuf_get_width(pixbuf);
	height = gdk_pixbuf_get_width(pixbuf);
	
    pixmap = gdk_pixmap_new(NULL, width, height, 24);
    gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, NULL, 0);

	style = gtk_style_copy(GTK_WIDGET (widget)->style);

	if (style->bg_pixmap[GTK_STATE_NORMAL])
		g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);

	style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_ACTIVE] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_PRELIGHT] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_SELECTED] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_INSENSITIVE] = g_object_ref(pixmap);

	gtk_widget_set_style(GTK_WIDGET (widget), style);
	g_object_unref(style);
}
void show_window()
{
	GtkWidget* stable;
	GtkWidget* check_time;
	GtkWidget* set_time;
	GtkTextIter iter;											
	GtkWidget *image_bg;
	GtkWidget *quit_button;
	GtkWidget *land_button;
	GtkWidget *set_button;
	
	GtkWidget *label_one;
	GtkWidget *label_two;
	GtkWidget *label_thr; 
	GtkWidget *label_fou; 
	GtkWidget *label_fiv; 
	GtkWidget *label_six; 
	
	GtkWidget *label_current_num_title;
	
	
	/****窗口基本设置*GTK_WINDOW_TOPLEVEL*POPUP*/
    show.window = gtk_window_new (GTK_WINDOW_TOPLEVEL);			//创建新窗口
    gtk_window_set_position(GTK_WINDOW(show.window),GTK_WIN_POS_CENTER);
    gtk_window_set_decorated (GTK_WINDOW(show.window), FALSE);	//包含标题栏，右边界
    //gtk_window_set_title (GTK_WINDOW(show.window), "打字高手v1.0");
	//gtk_window_set_title (GTK_WINDOW (show.window), g_locale_to_utf8("打字高手",-1,NULL,NULL,NULL));
	gtk_window_set_default_icon_from_file(IMG_ICON,NULL); 
	gtk_window_set_deletable(GTK_WINDOW(show.window),FALSE);	//禁止叉叉可以关闭
    gtk_widget_set_size_request (show.window, WINDOW_W, WINDOW_H);//设置主窗口默认打开大小
   	gtk_window_set_resizable ((GtkWindow *)show.window,FALSE);	//固定窗口显示大小
	gtk_container_set_border_width(GTK_CONTAINER(show.window),0);//窗口空白边界大小

    g_signal_connect (G_OBJECT(show.window), "destroy",
                        G_CALLBACK(gtk_main_quit), NULL);		//X可关闭
    
	//创建stable容器，并将容器添加到窗口中
	stable = gtk_table_new(WINDOW_W,WINDOW_H,TRUE);
	gtk_container_add(GTK_CONTAINER(show.window),stable);
    //背景图片
	update_widget_bg(show.window, WINDOW_BG);
	//设置鼠标捕获事件

	gtk_widget_set_events(show.window,  // 设置窗体获取鼠标事件
	       GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
	       | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK
	       | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);
	gtk_signal_connect(GTK_OBJECT(show.window), "button_press_event",
	           (GtkSignalFunc) button_press_event, show.window);       // 加入事件回调
	gtk_signal_connect(GTK_OBJECT(show.window), "motion_notify_event",
	           (GtkSignalFunc) motion_notify_event, show.window);
	gtk_signal_connect(GTK_OBJECT(show.window), "button_release_event",
	           (GtkSignalFunc) button_release_event, show.window);

	//前三名打字速度显示label和当前在线人数显示
	label_current_num_title= gtk_label_new(NULL);
	label_current_num_show= gtk_label_new("0");
	gtk_label_set_markup(GTK_LABEL(label_current_num_title),"<b>当前在线人数：</b>");
	gtk_table_attach_defaults(GTK_TABLE(stable), label_current_num_title,420,510,5,25);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_current_num_show,510,525,5,25);

	label_one = gtk_label_new("第一名：");
	label_two = gtk_label_new("第二名：");
	label_thr = gtk_label_new("第三名：");
	label_fou = gtk_label_new("第四名：");
	label_fiv = gtk_label_new("第五名：");
	label_six = gtk_label_new("第六名：");

	label_one_p = gtk_label_new("NO.1");
	label_two_p = gtk_label_new("NO.2");
	label_thr_p = gtk_label_new("NO.3");
	label_fou_p = gtk_label_new("NO.4");
	label_fiv_p = gtk_label_new("NO.5");
	label_six_p = gtk_label_new("NO.6");

	gtk_table_attach_defaults(GTK_TABLE(stable), label_one,200,260,70,90);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_two,200,260,95,115);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_thr,200,260,120,140);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_fou,400,460,70,90);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_fiv,400,460,95,115);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_six,400,460,120,140);

	gtk_table_attach_defaults(GTK_TABLE(stable), label_one_p,260,350,70,90);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_two_p,260,350,95,115);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_thr_p,260,350,120,140);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_fou_p,460,550,70,90);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_fiv_p,460,550,95,115);
	gtk_table_attach_defaults(GTK_TABLE(stable), label_six_p,460,550,120,140);

	//加载文件选择下拉组合框
	combo_file = gtk_combo_new();
	gtk_widget_set_size_request(combo_file, 150, 30);		//设置大小
	gtk_table_attach_defaults(GTK_TABLE(stable), combo_file,700,850,70,100); //设置显示位置
	
    list=g_list_append(list,"select article");
    list=g_list_append(list,"article 1");
    list=g_list_append(list,"article 2");
    list=g_list_append(list,"article 3");
	
    gtk_combo_set_popdown_strings(GTK_COMBO(combo_file),list);
	
	g_signal_connect(GTK_COMBO(combo_file)->entry,"changed",
		GTK_SIGNAL_FUNC(combox_file_changed),NULL);
	//打字时间设定选择框checkbutton 
	check_time = gtk_check_button_new_with_label("set time");
	set_time = gtk_entry_new();
	gtk_widget_set_size_request(set_time, 60, 20);
	gtk_entry_set_text(GTK_ENTRY(set_time),"5min");		//初始化显示内容	

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_time),FALSE);
	gtk_table_attach_defaults(GTK_TABLE(stable), check_time,700,780,105,125);
	gtk_table_attach_defaults(GTK_TABLE(stable), set_time,785,845,105,125);
	
	g_signal_connect(G_OBJECT(check_time),"clicked",G_CALLBACK(toggle_checkbutton_callback),NULL);
	//小图标加载
	pic_load(stable,NULL);
	//显示entry位置
	entry_init();
	gtk_table_attach_defaults(GTK_TABLE(stable), show.entry_correct,160,220,640,660);
	gtk_table_attach_defaults(GTK_TABLE(stable), show.entry_error,250,310,640,660);
	gtk_table_attach_defaults(GTK_TABLE(stable), show.entry_AR,380,440, 640,660);
	gtk_table_attach_defaults(GTK_TABLE(stable), show.entry_speed,500,560, 640,660);
	gtk_table_attach_defaults(GTK_TABLE(stable), show.entry_ctime,590,650, 640,660);
	gtk_table_attach_defaults(GTK_TABLE(stable), show.entry_clock,680,740, 640,660);
	//显示文本框textview
	view_init();
	int i = 0,y = 58, z = 1;

	for(i =0 ;i< 10 ;i++){
		buffer[i] = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view[i]));	//文本缓冲区
	}
	//文本框显示位置设置
	for(i = 0,y = 0;i < 10;i++,y++){
		if((i == 0) || (i == 1)){
			if((i%2) == 0){
				gtk_table_attach_defaults(GTK_TABLE(stable), view[i],LEFT_MARGIN, (LEFT_MARGIN+FONT_WIDTH),INIT_TOP,INIT_TOP+FONT_HEIGHT);				
			}
			else {
				gtk_table_attach_defaults(GTK_TABLE(stable), view[i],LEFT_MARGIN,(LEFT_MARGIN+FONT_WIDTH),INIT_TOP + FONT_HEIGHT,INIT_TOP + FONT_HEIGHT+FONT_HEIGHT);	
			}
		}
		else {
			if((i%2) == 0){
				gtk_table_attach_defaults(GTK_TABLE(stable), view[i],LEFT_MARGIN,(LEFT_MARGIN+FONT_WIDTH) ,(INIT_TOP)+(SPACE)*i + y*(FONT_HEIGHT),FONT_HEIGHT+(INIT_TOP)+(SPACE)*i + y*(FONT_HEIGHT));	
			}
			else {
				gtk_table_attach_defaults(GTK_TABLE(stable), view[i],LEFT_MARGIN,(LEFT_MARGIN+FONT_WIDTH),(INIT_TOP)+ (SPACE)*(i-1) + (y)*(FONT_HEIGHT),FONT_HEIGHT+(INIT_TOP)+ (SPACE)*(i-1) + (y)*(FONT_HEIGHT));	
			}
		}
	}
	//加载文件中的文本
	show_file_text(NULL,NULL);
	//默认隐藏登陆窗口
	login_window();
	hide_child_window(NULL,win);
	//创建登陆,退出按钮
	quit_button = gtk_button_new_with_label("Quit");
	land_button = gtk_button_new_with_label("Land");
	set_button = gtk_button_new_with_label("");
	stop_count = gtk_button_new_with_label("START");
	gtk_button_set_relief((gpointer)quit_button,GTK_RELIEF_NONE);//设置button透明
	gtk_button_set_relief((gpointer)land_button,GTK_RELIEF_NONE);//设置button透明
	gtk_button_set_relief((gpointer)set_button,GTK_RELIEF_NONE);//设置button透明
	gtk_button_set_relief((gpointer)stop_count,GTK_RELIEF_NONE);//设置button透明
	
	gtk_widget_set_size_request(quit_button, 35, 20);
	gtk_widget_set_size_request(land_button, 50, 20);
	gtk_widget_set_size_request(set_button, 20, 20);
	gtk_widget_set_size_request(stop_count, 60, 40);
	
	gtk_table_attach_defaults(GTK_TABLE(stable),quit_button,965,1000, 0 ,25);
	gtk_table_attach_defaults(GTK_TABLE(stable),land_button,915,965,0,25);
	gtk_table_attach_defaults(GTK_TABLE(stable),set_button,980,1000,670,690);
	gtk_table_attach_defaults(GTK_TABLE(stable),stop_count,750,820,635,665);
	
//	g_signal_connect(G_OBJECT(quit_button),"clicked",G_CALLBACK(gtk_main_quit),NULL);
//	g_signal_connect(G_OBJECT(quit_button),"enter",G_CALLBACK(quit_button_enter),show.window);
	g_signal_connect(G_OBJECT(quit_button),"clicked",G_CALLBACK(quit_button_enter),show.window);
	g_signal_connect(G_OBJECT(land_button),"clicked",G_CALLBACK(show_child_window),
			GTK_WINDOW(win));
/*	g_signal_connect(G_OBJECT(set_button),"clicked",G_CALLBACK(show_child_window),
			GTK_WINDOW(set_win));
*/
	g_signal_connect(G_OBJECT(stop_count),"clicked",G_CALLBACK(stop_count_time),
			NULL);
	
	
	/***显示窗口***/
    gtk_widget_show_all(show.window);
	//gtk_widget_hide(land_button);			//隐藏某个构件
	get_user_message(NULL,NULL);
}

//checkbutton回调函数
void toggle_checkbutton_callback(GtkWidget *widget,gpointer data)
{
	if(GTK_TOGGLE_BUTTON(widget)->active){
		//如果选中了
		//g_printf("is true\n");
	}
	else {
		//没有选中 
		//g_printf("is false\n");
	}
}
//进入关闭窗口按钮回调函数
void quit_button_enter( GtkWidget *widget,gpointer   data )
{
	//printf("enter\n");
    GdkColor entry_text_bg = ENTRY_TEXT_BG;
	gtk_widget_modify_base(widget,GTK_STATE_NORMAL,&entry_text_bg); //背景颜色

	local_userinfo.u_status = 0;
	
	//sleep(2);				//�ȴ��˳���Ϣ�������
	gtk_main_quit();	
}
//文件选择下拉组合框信号回调函数
gboolean combox_file_changed(GtkCombo *combo, gpointer data) 
{    	
	//printf("file: %s \n",gtk_entry_get_text(GTK_ENTRY(((GtkCombo*)combo_file)->entry)));
}

//
void login_callback_fun(GtkWidget * widget ,gpointer data)
{
	strcpy(local_userinfo.u_name,(char *)gtk_entry_get_text(GTK_ENTRY(username)));
	strcpy(local_userinfo.u_passwd,(char *)gtk_entry_get_text(GTK_ENTRY(passwd)));
    gtk_widget_destroy((GtkWidget *)data);
}
//获取练习的用户的信息对话框
gint get_user_message(GtkWidget *widget,gpointer data)
{
	GtkWidget *login_b;
	GtkWidget *user_getm_dialog;

	GtkWidget *userlabel,*passwdlabel,*msg_label;
    GtkWidget *table;
	gint res;
	
    user_getm_dialog = gtk_dialog_new_with_buttons("user message",data,
                                         GTK_RESPONSE_DELETE_EVENT ,NULL);
    //创建4个条目，告诉用户输入的数据
	login_b = gtk_button_new_with_label("login");
	g_signal_connect(G_OBJECT(login_b),"clicked",G_CALLBACK(login_callback_fun),
			user_getm_dialog);
    username = gtk_entry_new();
    passwd = gtk_entry_new();
	userlabel = gtk_label_new("英文姓名");
	passwdlabel = gtk_label_new("密码设置");
#if 0	
    //设置条目的缺省值,即默认初始显示值
    gtk_entry_set_text(GTK_ENTRY(username),g_get_user_name());
    gtk_entry_set_text(GTK_ENTRY(passwd),g_get_real_name());
#endif
    //表组装
    table = gtk_table_new(6,6,TRUE);
	msg_label = gtk_label_new("必须输入英文姓名和初始密码才能进入练习");
   	gtk_table_attach_defaults(GTK_TABLE(table),msg_label,0,6,0,1);

    gtk_table_attach_defaults(GTK_TABLE(table),userlabel,0,2,2,3);
    gtk_table_attach_defaults(GTK_TABLE(table),username,2,6,2,3);

    gtk_table_attach_defaults(GTK_TABLE(table),passwdlabel,0,2,3,4);	
    gtk_table_attach_defaults(GTK_TABLE(table),passwd,2,6,3,4);
   	gtk_table_attach_defaults(GTK_TABLE(table),login_b,2,4,5,6);
	//设置行列间隔和边距
    gtk_table_set_row_spacings(GTK_TABLE(table),5);
    gtk_table_set_col_spacings(GTK_TABLE(table),5);
    gtk_container_set_border_width(GTK_CONTAINER(table),5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(user_getm_dialog)->vbox),table);
    gtk_widget_show_all(user_getm_dialog);

    //运行和输出对话框
    res = gtk_dialog_run(GTK_DIALOG(user_getm_dialog));
    switch(res){
        case GTK_RESPONSE_NONE:
            /*-1
             * 对话框被窗口管理器销毁或通过gtk_widget_destroy()有其他程序销毁。
             * 如果构件没有设置一个相应标识，会返回它。
             */
            break;
        case GTK_RESPONSE_REJECT:
            break;
        case GTK_RESPONSE_ACCEPT:
            break;
        case GTK_RESPONSE_DELETE_EVENT:
		    //printf("-4\n");
            exit(0);
            break;
        case GTK_RESPONSE_OK:
            break;
        case GTK_RESPONSE_CANCEL:
            break;
        case GTK_RESPONSE_CLOSE:
            break;
        case GTK_RESPONSE_YES:
            break;
        case GTK_RESPONSE_NO:
            break;
        case GTK_RESPONSE_APPLY:
            break;
        case GTK_RESPONSE_HELP:
            break;
        default:
            //g_print("something wrong!\n");
            break;
    }
	
    gtk_widget_destroy(user_getm_dialog);
	return 0;

}
