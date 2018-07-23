#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <form.h>
#include <menu.h>
#include <panel.h>
#include <ctype.h>
#include <pthread.h>


FILE *ioutfileserver;
FILE *ioutfileclient;



 
	
void exitsystem()
{
		if(ioutfileserver!=NULL)
			fclose(ioutfileserver);
		if(ioutfileclient!=NULL)
			fclose(ioutfileclient);
		//WSACleanup();
		exit(0);
}


//DWORD WINAPI 
void threadproclient(int pparam)
{
	int hsock =pparam;
	char crecvbuffer[1024];
	char cshowbuffer[1024];
	int num=0;
	if(hsock!=-1)
		printf("start:\n");
	while(1)
	{
		num=recv(hsock,crecvbuffer,1024,0);
		if(num>=0)
		{
			crecvbuffer[num]='\0';
			sprintf(cshowbuffer,"to me:%s\n",crecvbuffer);
			printf("%s",cshowbuffer);
			fwrite(cshowbuffer,sizeof(char),strlen(cshowbuffer),ioutfileclient);
			fflush(ioutfileclient);
			if(strcmp("exit",crecvbuffer)==0)
				exitsystem();
		}
	}
	//return 0;
}
//DWORD WINAPI 
void threadproserver(int pparam)
{
	int hsock =pparam;
	char crecvbuffer[1024];
	char cshowbuffer[1024];
	int num = 0;
	if(hsock!=-1)
		printf("start:\n");
	while(1)
	{
		num=recv(hsock,crecvbuffer,1024,0);
		if(num>=0)
		{
			crecvbuffer[num]='\0';
			sprintf(cshowbuffer,"to me:%s\n",crecvbuffer);
			printf("%s",cshowbuffer);
			fwrite(cshowbuffer,sizeof(char),strlen(cshowbuffer),ioutfileserver);
			fflush(ioutfileserver);
			if(strcmp("exit",crecvbuffer)==0)
				exitsystem();
		}
	}
	//return 0;	
}

int checkip(char *cip)
{
	char ipaddress[128];
	char ipnumber[4];
	int isubip=0;
	int idot = 0;
	int iresult = 0;
	int iipresult = 1;
	int i;
	memset(ipnumber,0,4);
	strncpy(ipaddress,cip,128);
	for(i=0;i<128;i++)
	{
		if(ipaddress[i]=='.')
		{
			idot++;
			isubip=0;
			if(atoi(ipnumber)>255)
				iipresult = 0;
			memset(ipnumber,0,4);
		}
		else
			ipnumber[isubip++]=ipaddress[i];
		if(idot==3&&iipresult!=0)
			iresult=1;
	}
	return iresult;
}

void createserver()
{
	int iport = 4600;
	int  m_sockserver;
	int m_server;
	struct sockaddr_in serveraddr;
	struct sockaddr_in serveraddrfrom;
	
	int ibindresult =1;
	int iwhilecount =200;
	//struct hostent *localhost;
	char *localip;
	
	char cwelcomebuffer[]="welcome to you\0";
	int len =sizeof(struct sockaddr);
	int iwhilelistencount =10;
	//DWORD nthreadid=0;
	pthread_t tid2;
	int ires;
	char csendbuffer[1024];
	char cshowbuffer[1024];
	ioutfileserver =fopen("messageserver.txt","a");

	
	m_sockserver =socket(AF_INET,SOCK_STREAM,0);
	printf("ben ji bang ding duan kou da yu1024:");
	scanf("%d",&iport);
	//localhost =gethostbyname("");
	//localip=inet_ntoa(*(struct in_addr *)*localhost->h_addr_list);

	/*设置网络地址信息*/

	serveraddr.sin_family =AF_INET;
	serveraddr.sin_port =htons(iport);/*端口*/
	serveraddr.sin_addr.s_addr =htonl(INADDR_ANY);/*地址*/


	/*绑定地址信息*/

	ibindresult =bind(m_sockserver,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr));
	/*如果端口不能被绑定，重新设置端口*/

	while(ibindresult!=0&&iwhilecount>0)
	{
		printf("shi bai chongx bangding:");
		scanf("%d",&iport);

		/*设置网络地址信息*/
		serveraddr.sin_family =AF_INET;
		serveraddr.sin_port =htons(iport);/*端口*/
		serveraddr.sin_addr.s_addr =inet_addr(localip);/*地址*/
		/*绑定地址信息*/

		ibindresult =bind(m_sockserver,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr));
		iwhilecount--;
		if(iwhilecount<=0)
		{
			printf("duankou ban ding shi bai\n");
			exit(0);
		}
	
	}
	while(iwhilelistencount>0)
	{
		printf("start listen\n");
		listen(m_sockserver,0);/*返回值判断单个监听是否超时*/
		m_server=accept(m_sockserver,(struct sockaddr*)&serveraddrfrom,&len);
		if(m_server!=-1)
		{
			/*有连接成功，发送欢迎信息*/
			send(m_server,cwelcomebuffer,sizeof(cwelcomebuffer),MSG_NOSIGNAL);
			/*启动接收信息的线程*/
			//CreateThread(NULL,0,threadproserver,(LPVOID)m_server,0,&nthreadid);
			pthread_create(&tid2,NULL,(void *)threadproserver,m_server); 
			break;
		}
		printf(".");
		iwhilelistencount--;
		if(iwhilelistencount<=0)
		{
			printf("\njian li lianjie shibai\n");
			pthread_cancel(tid2);
			exit(0);
		}
	}
	while(1)
	{
		memset(csendbuffer,0,1024);
		scanf("%s",csendbuffer);
		if(strlen(csendbuffer)>0)
		{
			ires=send(m_server,csendbuffer,strlen(csendbuffer),MSG_NOSIGNAL);
			if(ires<0)
				printf("send fail\n");
			else
			{
				sprintf(cshowbuffer,"send to :%s\n",csendbuffer);
				printf("%s",cshowbuffer);
				fwrite(cshowbuffer,sizeof(char),strlen(cshowbuffer),ioutfileserver);
			}
			if(strcmp("exit",csendbuffer)==0)
				pthread_cancel(tid2);
				exitsystem();
		}
	}
}

void createclient()
{
	int m_sockclient;
	struct sockaddr_in clientaddr;
	char cserverip[128];
	int iwhileip=10;
	int icnnres;
	//DWORD nthreadid = 0;
	pthread_t tid1;
	char csendbuffer[1024];
	char cshowbuffer[1024];
	char crecvbuffer[1024];

	int num;
	int ires;
	int iipres;
	m_sockclient=socket(AF_INET,SOCK_STREAM,0);
	printf("qing shu ru fu wuqi dizhi:");
	scanf("%s",cserverip);

	/*ip地址判断*/
	if(strlen(cserverip)==0)
		strcpy(cserverip,"127.0.0.1");/*没有输入地址，使用环回地址*/
	else
	{
		iipres=checkip(cserverip);
		while(!iipres&&iwhileip>0)
		{
			printf("qing chongxinshuru\n");
			scanf("%s",cserverip);
			iipres=checkip(cserverip);
			iwhileip--;
			if(iwhileip<=0)
			{
				printf("shu ru ci shu guo duo\n");
				exit(0);
			}
		}
	}
	ioutfileclient=fopen("messageserverclient.txt","a");
	clientaddr.sin_family=AF_INET;

	/*客户端向服务器请求的端口号，应该与服务器绑定的端口号一致*/
	clientaddr.sin_port = htons(4600);
	clientaddr.sin_addr.s_addr = inet_addr(cserverip);
	icnnres = connect(m_sockclient,(struct sockaddr*)&clientaddr,sizeof(struct sockaddr));
	if(icnnres==0)
	{
		num=recv(m_sockclient,crecvbuffer,1024,0);
		if(num>0)
		{
			printf("receive from server :%s\n",crecvbuffer);
			/*开启线程*/
			//CreateThread(NULL,0,threadproclient,(LPVOID)m_sockclient,0,&nthreadid);
			pthread_create(&tid1,NULL,(void *) threadproclient,m_sockclient); 
		}
		while(1)
		{
			memset(csendbuffer,0,1024);
			scanf("%s",csendbuffer);
			if(strlen(csendbuffer)>0)
			{
				ires=send(m_sockclient,csendbuffer,strlen(csendbuffer),MSG_NOSIGNAL);
				if(ires<0)
					printf("sent fail\n");
				else
				{
					sprintf(cshowbuffer,"send to :%s\n",csendbuffer);
					printf("%s",cshowbuffer);
					fwrite(cshowbuffer,sizeof(char),strlen(cshowbuffer),ioutfileclient);
					fflush(ioutfileclient);
				}
				if(strcmp("exit",csendbuffer)==0)
					{
						exitsystem();
						pthread_cancel(tid1);
					}
		}
	}
		}
		else
			
			printf("not right\n");
			
		
	}


int main(void)
{
	
	int isel = 0;
	/*WSADATA wsd;
	WSAStartup(MAKEWORD(2,2),&wsd);*/
	do
	{
		printf("chose\n");
		printf("p2p server 1\n");
		printf("p2p client 2\n");
		scanf("%d",&isel);
	}while(isel<0||isel>3);
	switch(isel)
	{
	case 1:
		createserver();
		break;
	case 2:
		createclient();
		break;
	}
	printf("exit\n");
	return 0;
}
