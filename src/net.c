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
#define PORT			15000					//¶Ë¿ÚºÅ

USER_HEAD	user_head;							//¹«ÓÃËùÓĞÓÃ»§ĞÅÏ¢Í·
struct sockaddr_in user_staddr;					//±¾µØµØÖ·½á¹¹Ìå
USER_INFO local_userinfo;						//±¾µØÓÃ»§ĞÅÏ¢


extern gdouble sp;						//´ò×ÖËÙ¶È
/*****´ò×ÖËÙ¶ÈÅÅÃû±äÁ¿******/
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

extern GtkWidget *label_current_num_show;	//µ±Ç°ÓÃ»§ÔÚÏßÈËÊıÏÔÊ¾

/**************************/


//¹ã²¥ÓÃ»§ĞÅÏ¢Ïß³Ì×Óº¯Êı:±¾»úÓÃ»§ĞÅÏ¢
void * thread_send_userinfo(void *arg)
{
	pthread_detach(pthread_self());
	int sockfd = (int)arg;
	while(local_userinfo.u_status){
		sleep(1);
		local_userinfo.u_speed = (unsigned int)sp;				//»ñÈ¡±¾µØÓÃ»§´ò×ÖËÙ¶È
		sendto(sockfd, &local_userinfo, sizeof(local_userinfo), 0, (SA*)&user_staddr, sizeof(struct sockaddr_in));
	}	
}
//¹ã²¥ÏÔÊ¾ÓÃ»§Êı¾İÏß³Ì×Óº¯Êı:´ò×ÖÇ°ÈıÃûÓÃ»§ĞÅÏ¢
void *thread_show_userinfo(void *arg)
{
	char online_num [50] = "";
	pthread_detach(pthread_self());
	USER_INFO *temp = NULL;

	int sockfd = (int)arg;
	while(1){
		sleep(1);
		//printf("------------------------------------------------\n");
		//ÏÔÊ¾ÔÚÏßÈËÊı
		sprintf(online_num,"<span foreground='white'>%d</span>",user_head.user_num-1);
		gtk_label_set_markup(GTK_LABEL(label_current_num_show),online_num);
		//ÏÔÊ¾´ò×ÖËÙ¶ÈÇ°ÈıÃûµÄĞÅÏ¢
		if((temp = get_user_rank(user_head.link_head,1)) != NULL){//»ñÈ¡´ò×ÖËÙ¶ÈÅÅÃûÎª1µÄÓÃ»§ĞÅÏ¢
			sprintf(num_one,"<b><span foreground='red'>%s %d</span></b>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_one_p),num_one);
		}
		else gtk_label_set_markup(GTK_LABEL(label_one_p),"NO.1");
		if((temp = get_user_rank(user_head.link_head,2)) != NULL){//»ñÈ¡´ò×ÖËÙ¶ÈÅÅÃûÎª2µÄÓÃ»§ĞÅÏ¢
			sprintf(num_two,"<span foreground='green'>%s %d</span>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_two_p),num_two);
		}
		else gtk_label_set_markup(GTK_LABEL(label_two_p),"NO.2");
		if((temp = get_user_rank(user_head.link_head,3)) != NULL){//»ñÈ¡´ò×ÖËÙ¶ÈÅÅÃûÎª3µÄÓÃ»§ĞÅÏ¢
			sprintf(num_thr,"<span foreground='blue'>%s %d</span>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_thr_p),num_thr);
		}
		else gtk_label_set_markup(GTK_LABEL(label_thr_p),"NO.3");
		if((temp = get_user_rank(user_head.link_head,4)) != NULL){//»ñÈ¡´ò×ÖËÙ¶ÈÅÅÃûÎª3µÄÓÃ»§ĞÅÏ¢
			sprintf(num_fou,"<span foreground='blue'>%s %d</span>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_fou_p),num_fou);
		}
		else gtk_label_set_markup(GTK_LABEL(label_fou_p),"NO.4");
		if((temp = get_user_rank(user_head.link_head,5)) != NULL){//»ñÈ¡´ò×ÖËÙ¶ÈÅÅÃûÎª3µÄÓÃ»§ĞÅÏ¢
			sprintf(num_fiv,"<span foreground='blue'>%s %d</span>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_fiv_p),num_fiv);
		}
		else gtk_label_set_markup(GTK_LABEL(label_fiv_p),"NO.5");
		if((temp = get_user_rank(user_head.link_head,6)) != NULL){//»ñÈ¡´ò×ÖËÙ¶ÈÅÅÃûÎª3µÄÓÃ»§ĞÅÏ¢
			sprintf(num_six,"<span foreground='blue'>%s %d</span>",temp->u_name,temp->u_speed);
			gtk_label_set_markup(GTK_LABEL(label_six_p),num_six);
		}
		else gtk_label_set_markup(GTK_LABEL(label_six_p),"NO.6");
		//printf("===============================================\n");
	}
	pthread_exit(0);
}
//¹ã²¥½ÓÊÕÏß³Ì×Óº¯Êı: ½ÓÊÕÓÃ»§ĞÅÏ¢ºÍ·şÎñ¶Ë¹ã²¥ĞÅÏ¢
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
		//Èç¹ûÓÃ»§µÇÂ¼¾Í´¦ÀíÊı¾İ
		if(userinfo.u_status){
			if(select_user_link(&userinfo,user_head.link_head)){//Á´±íÖĞ´æÔÚ¸ÃÓÃ»§,Ôò¸üĞÂÊı¾İ
				change_user_link(&userinfo,user_head.link_head,2);
			}
			else {							//ĞÂ¼ÓÈëÓÃ»§£¬Ìí¼Óµ½Á´±í
				change_user_link(&userinfo,user_head.link_head,1);
			}
		}	
		else{//ÓÃ»§ÒÑ¾­ÀëÏß
			if(select_user_link(&userinfo,user_head.link_head)){//Á´±íÖĞ´æÔÚ¸ÃÓÃ»§£¬ÔòÉ¾³ı¸ÃÓÃ»§
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

//ÓÃ»§×´Ì¬¹ÜÀíÏß³Ì
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
//Á´±í³õÊ¼»¯:Ä¬ÈÏµÚÒ»¸öÎª¿ÕĞÅÏ¢ÓÃ»§
void user_link_init()
{
	USER_INFO *p = NULL, *new = NULL, *head = NULL;
	new = (USER_INFO *)malloc(sizeof(USER_INFO));
	new->next = NULL;
	strcpy(new->u_name,"root");				//µÚÒ»¸öÓÃ»§ÎªrootÓÃ»§
	strcpy(new->u_passwd,"111111");			//ÃÜÂë
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
//²éÑ¯Á´±í£¬ÊÇ·ñ´æÔÚ¸ÃÓÃ»§(u_name)ÔÚÁ´±íÖĞ:0 ²»´æÔÚ  1 ´æÔÚ
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
//É¾³ıËùÓĞÓÃ»§ĞÅÏ¢:³ırootÍâ
int delete_all_user(USER_INFO *head)
{
	USER_INFO * temp;
	if(head->next == NULL) return 0;			//±£ÁôrootÓÃ»§
	while(head->next != NULL){
		head = head->next;
		temp = head;
		
		temp->prev->next = temp->next;
		if(temp->next != NULL)temp->next->prev = temp->prev;	
		free(temp);
	}
	return 1;
}
//É¾³ıÁ´±íÓÃ»§
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
//Ìí¼ÓÁ´±íÓÃ»§:×ÜÊÇÌí¼Óµ½headµÄºóÃæ(»òÕß°´ÕÕ´ò×ÖËÙ¶È´óĞ¡À´²åÈë)
void add_user_link(USER_INFO *u_info,USER_INFO *head)
{
	USER_INFO *new = NULL;
	new = (USER_INFO *)malloc(sizeof(USER_INFO));
	strcpy(new->u_name,u_info->u_name);
	strcpy(new->u_passwd,u_info->u_passwd);
	strcpy(new->u_head,u_info->u_head);
	new->u_speed = u_info->u_speed;
	new->u_status = u_info->u_status;
	
	if(head->next == NULL){				//Á´±íÖĞÖ»ÓĞÒ»¸öÓÃ»§
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
//¸üĞÂÁ´±íÓÃ»§
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
//É¾³ı»òÌí¼Ó»ò¸üĞÂÓÃ»§:¡¾flag =0 É¾³ı¡¿¡¾ flag =1Ìí¼Ó¡¿ ¡¾flag =2 ¸üĞÂ¡¿
void change_user_link(USER_INFO *u_info,USER_INFO *head,int flags)
{
	switch(flags){
		case 0 :delete_user_link(u_info,head);break;
		case 1 :add_user_link(u_info,head);break;
		case 2 :update_user_link(u_info,head);break;
		default : printf("change_user_link [flags] is error!\n");break;
	}
}
//±éÀúÁ´±í
void user_link_travel(USER_INFO * head)
{
	while(head){
		printf("Travel====%s====%d==%d=\n",head->u_name,head->u_speed,head->u_status);
		head = head->next;
	}	
	printf("\n");
}
//°´´ò×ÖËÙ¶È´ÓĞ¡µ½´óÖØĞÂÅÅĞò
USER_INFO * user_link_sort(USER_INFO * src_head)
{
    USER_INFO *head=(USER_INFO*)malloc(sizeof(USER_INFO));//¹¹½¨Ò»¸öÍ·½Úµã£¬ÓÃÀ´²åÈë´«ÈëÁ´±íµÄÔªËØ
    head->next=NULL;
    USER_INFO *p=head;
    while(src_head->next == NULL){			//Ö»ÓĞÒ»¸ö¿ÕÓÃ»§
		printf("There is only one user: root\n");	
		return NULL;
	}	
	
	src_head=src_head->next;				//Ö¸Ïò´«ÈëÁ´±íµÄµÚÒ»¸öÓĞĞ§ÔªËØ
    while(src_head!= NULL)
    {
	    while(head->next!=NULL && p->next!=NULL && src_head->u_speed < p->next->u_speed){	//¶ÔÓÚ¹¹½¨µÄÁ´±í£¬³ıÍ·½ÚµãÍâÆäËûÔªËØ´æÔÚ£¬±éÀúµ½×îºóÒ»¸ö½ÚµãÇ°,
             p = p->next;                     										//Ö¸ÕëÍ£ÔÚÒª²åÈë½ÚµãµÄÉÏÒ»½Úµã 	        
	    }
        USER_INFO *temp=(USER_INFO*)malloc(sizeof(USER_INFO));
		
        temp->u_speed = src_head->u_speed;
        temp->u_status= src_head->u_status;
		strcpy(temp->u_name,src_head->u_name);
		strcpy(temp->u_passwd,src_head->u_passwd);
		strcpy(temp->u_head,src_head->u_head);
		strcpy(temp->u_ip,src_head->u_ip);

        temp->next=p->next;					//ÕÒµ½Ó¦²åÈë½Úµã£¬Í·²å·¨²åÈë
        p->next=temp;
        p=head;								//pÖØĞÂÖ¸ÏòÍ·½Úµã
        src_head=src_head->next;			//È¡´«ÈëÁ´±íµÄÏÂÒ»½Úµã
    }
    return head;							//·µ»ØÅÅºÃĞòµÄÁ´±íÍ·
}
//»ñÈ¡´ò×ÖËÙ¶ÈÅÅÃûÎªnµÄÓÃ»§ĞÅÏ¢
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
	
	return d_head;					//headÎªNULL¼´µ½ÁËÎ²²¿
}

//É¾³ıÓÃ»§status > 2µÄÓÃ»§(¼´ÒÑ¾­ÀëÏßµÄÓÃ»§)
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

//ÓÃ»§statua++
void add_online_user_status_by_status(USER_INFO *head,const char *localuser)
{
	if(head->next == NULL){
		return ;
	}	
	head = head->next;
	while(head){
		if(strcmp(head->u_name,localuser) != 0){
			head->u_status++;	//±¾µØÓÃ»§²»ÓÃ×ÔÔö
								//printf("add u_status = %d\n",head->u_status);
		}
		else ;					//printf("no add user_status\n");
		head = head->next;
	}
}

//udp ³õÊ¼»¯
int udp_init(char *b_ip)
{
	int sockfd;
	pthread_t tid , tid1 ,tid2, tid3;
	socklen_t len = sizeof(struct sockaddr_in);
	
	//Ö±½Ó´ÓÁ÷ÖĞ¶ÁÈëÊı¾İ»òÖ±½ÓÏòÁ÷ÖĞĞ´ÈëÊı¾İ£¬¶øÃ»ÓĞ»º³åÇø¡£
	setvbuf(stdout, NULL, _IONBF, 0);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
		perror("sockfd");
		return -1;
	}
	// ÉèÖÃµØÖ·¸´ÓÃ£¬·ÀÖ¹bindÊ±³öÏÖAddress Already in useµÄÇé¿ö³öÏÖ¡£
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

//»ñÈ¡±¾»úipµØÖ·
int getlocalip(char* outip)
{
	int i=0;
	int sockfd;
	struct ifconf ifconf;
	char buf[512];
	struct ifreq *ifreq;
	char* ip;
	//åˆå§‹åŒ–ifconf
	ifconf.ifc_len = 512;
	ifconf.ifc_buf = buf;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		return -1;
	}
	ioctl(sockfd, SIOCGIFCONF, &ifconf);    //è·å–æ‰€æœ‰æ¥å£ä¿¡æ¯
	close(sockfd);
	//æ¥ä¸‹æ¥ä¸€ä¸ªä¸€ä¸ªçš„è·å–IPåœ°å€
	ifreq = (struct ifreq*)buf;
	for(i=(ifconf.ifc_len/sizeof(struct ifreq));i>0;i--){
		ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);

		if(strcmp(ip,"127.0.0.1")==0){ //æ’é™¤127.0.0.1ï¼Œç»§ç»­ä¸‹ä¸€ä¸ª
			ifreq++;
			continue;
		}
		strcpy(outip,ip);
		return 0;
	}

	return -1;
}

