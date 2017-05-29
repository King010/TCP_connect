#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

int startup(const char *_ip,int _port)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
	perror("socket");
	exit(2);
	}
//	printf("sock:%d\n",sock);
	
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(_port);
	local.sin_addr.s_addr= inet_addr(_ip);

	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		perror("bind");
		exit(3);
	}

	if(listen(sock,10)<0)
	{
		perror("listen");
		exit(4);
	}

	return sock;

}

static void usage(const char *proc)
{
	printf("%s [ip] [port]\n",proc);
}

int main(int argc,char*argv[])
{
	if(argc!=3)
	{
		usage(argv[0]);
	}
		
	int listen_sock=startup(argv[1],atoi(argv[2]));
	struct sockaddr_in remote;
	socklen_t len=sizeof(remote);
	char buf[2014];
	while(1)
	{
		int sock=accept(listen_sock,(struct sockaddr*)&remote,&len);
		if(sock<0){
		perror("accept");
		continue;
		}
		printf("client ip :%s ,port:%d \n",\
				inet_ntoa(remote.sin_addr),ntohs(remote.sin_port));
		while(1){
			ssize_t s=read(sock,buf,sizeof(buf)-1);
			if(s>0){		
				buf[s]=0;
				printf("client say #%s\n",buf);
				write(sock,buf,strlen(buf));
			}else if(read==0)
			{
				printf("client quit\n");
				break;
			}
		
		}

	}
	return 0;
}
