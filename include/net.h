#ifndef __NET_H_
#define __NET_H_

#include <net/if.h>
#include <sys/ioctl.h>

//�����û���Ϣ
typedef	struct	_USER_INFO{			
	char u_name[20];				//�û���
	char u_passwd[20];				//�û�����
	char u_head[50];				//�û�ͷ��
	int  u_speed;				    //�û���ǰ�����ٶ�
	char u_ip[20];					//�û�ip��ַ
	int  u_status;					//�û���ǰ��¼״̬(Ĭ��Ϊδ��¼)
	struct _USER_INFO *next;		
	struct _USER_INFO *prev;
}USER_INFO;

//�����û���Ϣͷ
typedef struct _USER_HEAD{			
	int user_num;					//��ǰ��¼�û�������
	struct _USER_INFO *link_head;	//�û���Ϣͷ�ĵ�һ���ڵ�
}USER_HEAD;


//�����ʼ��:Ĭ�ϵ�һ��Ϊ����Ϣ�û�
void user_link_init();
//��ѯ�����Ƿ���ڸ��û���������:0 ������  1 ����
int select_user_link(USER_INFO *u_info,USER_INFO *head);
//ɾ�������û�
int delete_user_link(USER_INFO *u_info,USER_INFO *head);
//��������û�:������ӵ�head�ĺ���
void add_user_link(USER_INFO *u_info,USER_INFO *head);
//���������û�
int update_user_link(USER_INFO *u_info,USER_INFO *head);
//ɾ������ӻ�����û�:��flag =0 ɾ������ flag =1��ӡ� ��flag =2 ���¡�
void change_user_link(USER_INFO *u_info,USER_INFO *head,int flags);
//��������
void user_link_travel(USER_INFO * head);
//�������ٶȴ�С������������
USER_INFO * user_link_sort(USER_INFO * src_head);
//��ȡ�����ٶ�����Ϊn���û���Ϣ
USER_INFO * get_user_rank(USER_INFO *head, int n);
//ɾ���û�status > 2���û�(���Ѿ����ߵ��û�)
void delete_offline_user_by_status(USER_INFO *head);
//�û�statua++
void add_online_user_status_by_status(USER_INFO *head,const char *localuser);

//udp ��ʼ��
int udp_init(char *b_ip);
//��ȡ����ip��ַ
int getlocalip(char* outip);


#endif

