#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#define SERVPORT 3333 
#define BACKLOG 10 
#define MAXDATASIZE 100  

int main()
{
	int sockfd,client_fd; 
	struct sockaddr_in my_addr; 
	struct sockaddr_in remote_addr; 
	char buf[MAXDATASIZE];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("socket err\n"); exit(1);
	}
	else
		printf("socket OK\n");

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind err\n");
		exit(1);
	}
	else
		printf("bind OK\n");

	if (listen(sockfd, BACKLOG) == -1) 
	{
		perror("listen err\n");
		exit(1);
	}
	else
		printf("listen OK\n");

	while(1) 
	{
		int sin_size = sizeof(struct sockaddr_in);
		if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size)) == -1)
		{
			perror("accept err\n");
			continue;
		}
		else
			printf("accept OK\n");
		printf("received a connection from %s\n", inet_ntoa(remote_addr.sin_addr));
		
		int recvbytes =0;
		if ((recvbytes=recv(client_fd, buf, MAXDATASIZE, 0)) ==-1) 
		{
			perror("recve err\n");
			exit(1);
		}
		buf[recvbytes] = '\0';
		printf("Received: %s\n",buf);

		if (!fork())
		{
			if (send(client_fd, "<HTML><BODY>xia,Hello, you are connected!</BODY></HTML> \n", 100, 0) == -1)
				perror("send err\n");
			
			else
				printf("send OK\n");

			close(client_fd);
			exit(0);
		}
		close(client_fd);
	}
	return 0;
}
