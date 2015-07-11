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


extern USER_INFO local_userinfo;						//�����û���Ϣ
extern USER_HEAD	user_head;							//���������û���Ϣͷ


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

	local_userinfo.u_status = 1;					//����ΪĬ�ϵ�¼
	
	//��ʾ������
	show_window();
	//����ESC�����˳�����
	g_signal_connect(G_OBJECT(show.window),"key_press_event",G_CALLBACK(ESC_quit),NULL);
	//�����ı����źŴ���
	text_signal_pro();
	//��ʱ����ʼ��
	timer_init();
	//��ȡ�㲥ip��ַ
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

	//udp��ʼ��
	udp_init(broadcast_ip);							//udp����ͨ�ų�ʼ��
	user_link_init();								//��������
	
    gtk_main ();
}

