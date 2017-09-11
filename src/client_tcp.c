#include "../include/header_client.h"

void main()
{
	short fd;
	struct sockaddr_in c;
	char buf[100]=TEXT;

	if((fd=socket(AF_INET,SOCK_STREAM,0)) == 0)
	{
		printf("Error in Socket!! \n\n");
		exit(-1);
	}

	c.sin_family=AF_INET;
	c.sin_addr.s_addr = inet_addr(IP);
	c.sin_port = htons(PORT);

	if((connect(fd,(struct sockaddr *)&c, sizeof(c)))<0)
	{
		printf("Error in Connect!! \n\n");
		exit(-1);
	}
	
	printf("Enter the data to be sent:");
	scanf("%[^\n]s",buf);

	if ((write(fd, buf,sizeof(buf) ))>0)
	   printf("Data Written Successfully \n");	
	close(fd);
}
