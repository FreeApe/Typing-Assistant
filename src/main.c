#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "swing.h"
#include "data_show.h"
#include "handle.h"
//#include "mysqlconnect.h"
#include "net.h"


extern USER_INFO local_userinfo;						//本地用户信息
extern USER_HEAD	user_head;							//公用所有用户信息头


/*****************************************************************/
int main(int argc,char **argv)
{
	char broadcast_ip[20] ="";
	char temp_ip [20]="";

	gtk_init (&argc, &argv);
#if 0
	if(argc != 3){
		printf("usage :./type_assis <yourname> <password>\n");
		return -1;
	}

	strncpy(local_userinfo.u_name, argv[1], 20);
	strncpy(local_userinfo.u_passwd,argv[2],20);
#endif

	local_userinfo.u_status = 1;					//设置为默认登录
	
	//显示主窗口
	show_window();
	//接收ESC按键退出程序
	g_signal_connect(G_OBJECT(show.window),"key_press_event",G_CALLBACK(ESC_quit),NULL);
	//打字文本框信号处理
	text_signal_pro();
	//定时器初始化
	timer_init();
	//获取广播ip地址
	if(getlocalip(temp_ip) == 0){
		char *p;
		printf("the local ip is %s\n",temp_ip);
		p = strtok(temp_ip, ".");
		strcat(broadcast_ip,p);
		strcat(broadcast_ip,".");
		p = strtok(NULL, ".");
		strcat(broadcast_ip,p);
		strcat(broadcast_ip,".");
		p = strtok(NULL,".");
		strcat(broadcast_ip,p);
		strcat(broadcast_ip,".255");
		printf("the broadcast ip is %s\n",broadcast_ip);
	}else return ;

	//udp初始化
	udp_init(broadcast_ip);							//udp网络通信初始化
	user_link_init();								//创建链表
	
    gtk_main ();
}

