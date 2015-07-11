#ifndef __NET_H_
#define __NET_H_

#include <net/if.h>
#include <sys/ioctl.h>

//打字用户信息
typedef	struct	_USER_INFO{			
	char u_name[20];				//用户名
	char u_passwd[20];				//用户密码
	char u_head[50];				//用户头像
	int  u_speed;				    //用户当前打字速度
	char u_ip[20];					//用户ip地址
	int  u_status;					//用户当前登录状态(默认为未登录)
	struct _USER_INFO *next;		
	struct _USER_INFO *prev;
}USER_INFO;

//所有用户信息头
typedef struct _USER_HEAD{			
	int user_num;					//当前登录用户的数量
	struct _USER_INFO *link_head;	//用户信息头的第一个节点
}USER_HEAD;


//链表初始化:默认第一个为空信息用户
void user_link_init();
//查询链表，是否存在该用户在链表中:0 不存在  1 存在
int select_user_link(USER_INFO *u_info,USER_INFO *head);
//删除链表用户
int delete_user_link(USER_INFO *u_info,USER_INFO *head);
//添加链表用户:总是添加到head的后面
void add_user_link(USER_INFO *u_info,USER_INFO *head);
//更新链表用户
int update_user_link(USER_INFO *u_info,USER_INFO *head);
//删除或添加或更新用户:【flag =0 删除】【 flag =1添加】 【flag =2 更新】
void change_user_link(USER_INFO *u_info,USER_INFO *head,int flags);
//遍历链表
void user_link_travel(USER_INFO * head);
//按打字速度从小到大重新排序
USER_INFO * user_link_sort(USER_INFO * src_head);
//获取打字速度排名为n的用户信息
USER_INFO * get_user_rank(USER_INFO *head, int n);
//删除用户status > 2的用户(即已经离线的用户)
void delete_offline_user_by_status(USER_INFO *head);
//用户statua++
void add_online_user_status_by_status(USER_INFO *head,const char *localuser);

//udp 初始化
int udp_init(char *b_ip);
//获取本机ip地址
int getlocalip(char* outip);


#endif

