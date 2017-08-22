//refer:http://blog.csdn.net/giantpoplar/article/details/47657303

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

int main(){  
		
	int sock_fd,client_fd; /*sock_fd：监听socket；client_fd：数据传输socket */  
	struct sockaddr_in ser_addr; /* 本机地址信息 */  
	struct sockaddr_in cli_addr; /* 客户端地址信息 */  
	int MAX_MSG_SIZE = 1000;
	char msg[MAX_MSG_SIZE];/* 缓冲区*/  
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);/*创建连接的SOCKET */  
	if(sock_fd < 0)  
	{/*创建失败 */  
		fprintf( stderr, "socker Error:%s\n", strerror(errno));  
		exit(1);  
	}  

	/* 初始化服务器地址*/  
	addrlen = sizeof(struct sockaddr_in);  
	bzero(&ser_addr, addrlen);  
	ser_addr.sin_family = AF_INET;  
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
	int SERVER_PORT = 6666;
	ser_addr.sin_port = htons(SERVER_PORT);  

	if(bind(ser_sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in))<0){ /*绑定失败 */  
		fprintf(stderr,"Bind Error:%s\n", strerror(errno));  
		exit(1);  
	}  

	/*侦听客户端请求*/  
	int BACKLOG = 50;
	if(listen(sock_fd, BACKLOG) < 0){  
		fprintf(stderr, "Listen Error:%s\n", strerror(errno));  
		close(sock_fd);  
		exit(1);  
	}  

	while(1){/* 等待接收客户连接请求*/  
		cli_fd = accept(sock_fd, (struct sockaddr*) &cli_addr, &addrlen);  
		if(cli_fd <= 0){  
			fprintf(stderr, "Accept Error:%s\n", strerror(errno));  
		}else{/*开始服务*/  
			recv(cli_addr, msg, MAX_MSG_SIZE, 0); /* 接受数据*/  
			printf("received a connection from %sn", inet_ntoa(cli_addr.sin_addr));  
			printf("%s\n", msg);/*在屏幕上打印出来 */  
			strcpy(msg, "hi,I am server!");  
			send(cli_addr, msg, sizeof(msg),0); /*发送的数据*/  
			close(cli_addr);  
		}
	}  
	
	close(sock_fd);  
	return 0;  
}  
