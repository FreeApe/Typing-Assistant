#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "net.h"

#define SA	 			struct sockaddr
#define PORT			15000					//端口号

USER_HEAD	user_head;							//公用所有用户信息头
struct sockaddr_in user_staddr;					//本地地址结构体
USER_INFO local_userinfo;						//本地用户信息


extern gdouble sp;						//打字速度
/*****打字速度排名变量******/
extern char num_one[50];				//"ycmhy 350"
extern char num_two[50];		
extern char num_thr[50];
extern char num_fou[50];				//"ycmhy 350"
extern char num_fiv[50];		
extern char num_six[50];


extern GtkWidget *label_one_p;			//gtk_label_set_markup(GTK_LABEL(label_one_p),num_one);
extern GtkWidget *label_two_p;
extern GtkWidget *label_thr_p;
extern GtkWidget *label_fou_p;			//gtk_label_set_markup(GTK_LABEL(label_one_p),num_one);
extern GtkWidget *label_fiv_p;
extern GtkWidget *label_six_p;

extern GtkWidget *label_current_num_show;	//当前用户在线人数显示

/**************************/


//广播用户信息线程子函数:本机用户信息
void * thread_send_userinfo(void *arg)
{
	pthread_detach(pthread_self());
	int sockfd = (int)arg;
	while(local_userinfo.u_status){
		sleep(1);
		local_userinfo.u_speed = (unsigned int)sp;				//获取本地用户打字速度
		sendto(sockfd, &local_userinfo, sizeof(local_userinfo), 0, (SA*)&user_staddr, sizeof(struct sockaddr_in));
	}	
}
//广播显示用户数据线程子函数:打字前三名用户信息
void *thread_show_userinfo(void *arg)
{
	char online_num [50] = "";
	pthread_detach(pthread_self());
	USER_INFO *temp = NULL;

	int sockfd = (int)arg;
	while(1){
		sleep(1);
		//printf("------------------------------------------------\n");
		//显示在线人数
		sprintf(online_num,"<span foreground='white'>%d</span>",user_head.user_num-1);
		gtk_label_set_markup(GTK_LABEL(label_current_num_show),online_num);
		//显示打字速度前三名的信息
		if((temp = get_user_rank(user_head.link_head,1)) != NULL){//获取打字速度排名为1的用户信息
			sprintf(num_one,"<b><span foreground='red'>%s %d</span></b>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_one_p),num_one);
		}
		else gtk_label_set_markup(GTK_LABEL(label_one_p),"NO.1");
		if((temp = get_user_rank(user_head.link_head,2)) != NULL){//获取打字速度排名为2的用户信息
			sprintf(num_two,"<span foreground='green'>%s %d</span>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_two_p),num_two);
		}
		else gtk_label_set_markup(GTK_LABEL(label_two_p),"NO.2");
		if((temp = get_user_rank(user_head.link_head,3)) != NULL){//获取打字速度排名为3的用户信息
			sprintf(num_thr,"<span foreground='blue'>%s %d</span>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_thr_p),num_thr);
		}
		else gtk_label_set_markup(GTK_LABEL(label_thr_p),"NO.3");
		if((temp = get_user_rank(user_head.link_head,4)) != NULL){//获取打字速度排名为3的用户信息
			sprintf(num_fou,"<span foreground='blue'>%s %d</span>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_fou_p),num_fou);
		}
		else gtk_label_set_markup(GTK_LABEL(label_fou_p),"NO.4");
		if((temp = get_user_rank(user_head.link_head,5)) != NULL){//获取打字速度排名为3的用户信息
			sprintf(num_fiv,"<span foreground='blue'>%s %d</span>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_fiv_p),num_fiv);
		}
		else gtk_label_set_markup(GTK_LABEL(label_fiv_p),"NO.5");
		if((temp = get_user_rank(user_head.link_head,6)) != NULL){//获取打字速度排名为3的用户信息
			sprintf(num_six,"<span foreground='blue'>%s %d</span>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_six_p),num_six);
		}
		else gtk_label_set_markup(GTK_LABEL(label_six_p),"NO.6");
		//printf("===============================================\n");
	}
	pthread_exit(0);
}
//广播接收线程子函数: 接收用户信息和服务端广播信息
void * thread_recv_userinfo(void *arg)
{
	pthread_detach(pthread_self());

	struct sockaddr_in staddr;
	socklen_t len = sizeof(struct sockaddr_in);
	int sockfd = (int)arg;
	USER_INFO userinfo;

	while(1){
		recvfrom(sockfd, &userinfo, sizeof(userinfo), 0, (SA*)&staddr, &len);
		inet_ntop(AF_INET, (void*)&staddr.sin_addr, userinfo.u_ip, len);
		//如果用户登录就处理数据
		if(userinfo.u_status){
			if(select_user_link(&userinfo,user_head.link_head)){//链表中存在该用户,则更新数据
				change_user_link(&userinfo,user_head.link_head,2);
			}
			else {							//新加入用户，添加到链表
				change_user_link(&userinfo,user_head.link_head,1);
			}
		}	
		else{//用户已经离线
			if(select_user_link(&userinfo,user_head.link_head)){//链表中存在该用户，则删除该用户
				change_user_link(&userinfo,user_head.link_head,0);
			}
			else ;
		}
		//printf("======================start========================\n");
		//user_link_travel(user_head.link_head);
		//printf("\r[%-8s(%-13s)] say: %d\n", userinfo.u_name, userinfo.u_ip, userinfo.u_speed);
		//printf("======================end======================\n");
	}
	pthread_exit(0);
}

//用户状态管理线程
void *thread_user_status_pro(void *arg)
{
	pthread_detach(pthread_self());

	int sockfd = (int)arg;
	while(1){
		sleep(2);
		add_online_user_status_by_status(user_head.link_head,local_userinfo.u_name);
		delete_offline_user_by_status(user_head.link_head);
	}
	pthread_exit(0);
}
//链表初始化:默认第一个为空信息用户
void user_link_init()
{
	USER_INFO *p = NULL, *new = NULL, *head = NULL;
	new = (USER_INFO *)malloc(sizeof(USER_INFO));
	new->next = NULL;
	strcpy(new->u_name,"root");				//第一个用户为root用户
	strcpy(new->u_passwd,"111111");			//密码
	new->u_speed = 0;						//speed
	if(p != NULL){
		p->next = new;
		new->prev = p;
	}
	else {
		new->prev = NULL;
		head = new;
	}
	p = new;

	user_head.link_head = head;
	user_head.user_num = 1;
}
//查询链表，是否存在该用户(u_name)在链表中:0 不存在  1 存在
int select_user_link(USER_INFO *u_info,USER_INFO *head)
{
	while(head != NULL && (strcmp(head->u_name,u_info->u_name)) != 0){
		head = head->next;
	}
	if(head == NULL){
		printf("Select :is a new user.\n");return 0;
	}
	return 1;
}
//删除所有用户信息:除root外
int delete_all_user(USER_INFO *head)
{
	USER_INFO * temp;
	if(head->next == NULL) return 0;			//保留root用户
	while(head->next != NULL){
		head = head->next;
		temp = head;
		
		temp->prev->next = temp->next;
		if(temp->next != NULL)temp->next->prev = temp->prev;	
		free(temp);
	}
	return 1;
}
//删除链表用户
int delete_user_link(USER_INFO *u_info,USER_INFO *head)
{	
	while(head != NULL && (strcmp(head->u_name,u_info->u_name)) != 0){
		head = head->next;
	}
	if(head == NULL){
		printf("Delete :The user is not login or in use.\n");return 0;
	}
	
	head->prev->next = head->next;
	if(head->next != NULL)head->next->prev = head->prev;

	free(head);
	user_head.user_num -= 1;
	return 1;
}
//添加链表用户:总是添加到head的后面(或者按照打字速度大小来插入)
void add_user_link(USER_INFO *u_info,USER_INFO *head)
{
	USER_INFO *new = NULL;
	new = (USER_INFO *)malloc(sizeof(USER_INFO));
	strcpy(new->u_name,u_info->u_name);
	strcpy(new->u_passwd,u_info->u_passwd);
	strcpy(new->u_head,u_info->u_head);
	new->u_speed = u_info->u_speed;
	new->u_status = u_info->u_status;
	
	if(head->next == NULL){				//链表中只有一个用户
		head->next = new;
		new->next = NULL;
		new->prev = head;
	}
	else {	
		head->next->prev = new;
		new->next = head->next;
		new->prev = head;
		head->next = new;
	}
	user_head.user_num += 1;
}
//更新链表用户
int update_user_link(USER_INFO *u_info,USER_INFO *head)
{
	//printf("=%s===%s===[%d]=\n",u_info->u_name,u_info->u_passwd,u_info->u_status);
	while(head != NULL && (strcmp(head->u_name,u_info->u_name)) != 0){
		head = head->next;
	}
	if(head == NULL){
		printf("Update :The user is not login or in use.\n");return 0;
	}

	strcpy(head->u_name,u_info->u_name);
	strcpy(head->u_passwd,u_info->u_passwd);
	strcpy(head->u_head,u_info->u_head);
	head->u_speed = u_info->u_speed;
	head->u_status = u_info->u_status;

	return 1;
}
//删除或添加或更新用户:【flag =0 删除】【 flag =1添加】 【flag =2 更新】
void change_user_link(USER_INFO *u_info,USER_INFO *head,int flags)
{
	switch(flags){
		case 0 :delete_user_link(u_info,head);break;
		case 1 :add_user_link(u_info,head);break;
		case 2 :update_user_link(u_info,head);break;
		default : printf("change_user_link [flags] is error!\n");break;
	}
}
//遍历链表
void user_link_travel(USER_INFO * head)
{
	while(head){
		printf("Travel====%s====%d==%d=\n",head->u_name,head->u_speed,head->u_status);
		head = head->next;
	}	
	printf("\n");
}
//按打字速度从小到大重新排序
USER_INFO * user_link_sort(USER_INFO * src_head)
{
    USER_INFO *head=(USER_INFO*)malloc(sizeof(USER_INFO));//构建一个头节点，用来插入传入链表的元素
    head->next=NULL;
    USER_INFO *p=head;
    while(src_head->next == NULL){			//只有一个空用户
		printf("There is only one user: root\n");	
		return NULL;
	}	
	
	src_head=src_head->next;				//指向传入链表的第一个有效元素
    while(src_head!= NULL)
    {
	    while(head->next!=NULL && p->next!=NULL && src_head->u_speed < p->next->u_speed){	//对于构建的链表，除头节点外其他元素存在，遍历到最后一个节点前,
             p = p->next;                     										//指针停在要插入节点的上一节点 	        
	    }
        USER_INFO *temp=(USER_INFO*)malloc(sizeof(USER_INFO));
		
        temp->u_speed = src_head->u_speed;
        temp->u_status= src_head->u_status;
		strcpy(temp->u_name,src_head->u_name);
		strcpy(temp->u_passwd,src_head->u_passwd);
		strcpy(temp->u_head,src_head->u_head);
		strcpy(temp->u_ip,src_head->u_ip);

        temp->next=p->next;					//找到应插入节点，头插法插入
        p->next=temp;
        p=head;								//p重新指向头节点
        src_head=src_head->next;			//取传入链表的下一节点
    }
    return head;							//返回排好序的链表头
}
//获取打字速度排名为n的用户信息
USER_INFO * get_user_rank(USER_INFO *head, int n)
{
	USER_INFO *d_head;
	if((head->next == NULL) || n<= 0){
		//printf("get_user_rank is NULL or 'n' is too low\n");
		return NULL;
	}
	d_head = user_link_sort(head);
	while((d_head != NULL) && (n--)){
		d_head = d_head->next;
	}
	
	return d_head;					//head为NULL即到了尾部
}

//删除用户status > 2的用户(即已经离线的用户)
void delete_offline_user_by_status(USER_INFO *head)
{
	USER_INFO *temp;
	
	if(head->next == NULL){
		return ;
	}
	head = head->next;
	while(head){
		temp = head;
		if(temp->u_status > 3){
			temp->prev->next = temp->next;
			if(temp->next != NULL)temp->next->prev = temp->prev;
			free(temp);	
			user_head.user_num -= 1;
		}
		head = head->next;
	}

	return ;	
}

//用户statua++
void add_online_user_status_by_status(USER_INFO *head,const char *localuser)
{
	if(head->next == NULL){
		return ;
	}	
	head = head->next;
	while(head){
		if(strcmp(head->u_name,localuser) != 0){
			head->u_status++;	//本地用户不用自增
								//printf("add u_status = %d\n",head->u_status);
		}
		else ;					//printf("no add user_status\n");
		head = head->next;
	}
}

//udp 初始化
int udp_init(char *b_ip)
{
	int sockfd;
	pthread_t tid , tid1 ,tid2, tid3;
	socklen_t len = sizeof(struct sockaddr_in);
	
	//直接从流中读入数据或直接向流中写入数据，而没有缓冲区。
	setvbuf(stdout, NULL, _IONBF, 0);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
		perror("sockfd");
		return -1;
	}
	// 设置地址复用，防止bind时出现Address Already in use的情况出现。
	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	bzero(&user_staddr, sizeof(user_staddr));
	user_staddr.sin_family = AF_INET;
	user_staddr.sin_port = htons(PORT);		
	user_staddr.sin_addr.s_addr = inet_addr(b_ip);
	if(bind(sockfd, (SA*)&user_staddr, sizeof(SA)) < 0){
		perror("bind");
		return -1;
	}

	pthread_create(&tid,  NULL, thread_recv_userinfo, (void*)sockfd);
	pthread_create(&tid1, NULL, thread_send_userinfo, (void*)sockfd);
	pthread_create(&tid2, NULL, thread_show_userinfo, (void*)sockfd);
	pthread_create(&tid3, NULL, thread_user_status_pro,(void*)sockfd);
	return 0;
}

//获取本机ip地址
int getlocalip(char* outip)
{
	int i=0;
	int sockfd;
	struct ifconf ifconf;
	char buf[512];
	struct ifreq *ifreq;
	char* ip;
	//鍒濆鍖杋fconf
	ifconf.ifc_len = 512;
	ifconf.ifc_buf = buf;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		return -1;
	}
	ioctl(sockfd, SIOCGIFCONF, &ifconf);    //鑾峰彇鎵�鏈夋帴鍙ｄ俊鎭�
	close(sockfd);
	//鎺ヤ笅鏉ヤ竴涓竴涓殑鑾峰彇IP鍦板潃
	ifreq = (struct ifreq*)buf;
	for(i=(ifconf.ifc_len/sizeof(struct ifreq));i>0;i--){
		ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);

		if(strcmp(ip,"127.0.0.1")==0){ //鎺掗櫎127.0.0.1锛岀户缁笅涓�涓�
			ifreq++;
			continue;
		}
		strcpy(outip,ip);
		return 0;
	}

	return -1;
}

