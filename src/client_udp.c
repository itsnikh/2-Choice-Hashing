/*
 * For a Client-Server Socket Program, this Program will be the Client which will send the UDP Packet to the assigned IP and Port.
 * Here we have included the header file as Header_Client.h which is present in the include folder.
 * This Program creates a UDP Socket and sends the data taken during Run time to the Server.
 */

#include "../include/header_client.h" 

int main()
{
	short fd;
	struct sockaddr_in c;
	char buf[100]=TEXT;

	if((fd=socket(AF_INET,SOCK_DGRAM,0)) == 0)
	{
		printf("Error in Socket!! \n\n");
		exit(-1);
	}

	c.sin_family=AF_INET;
	c.sin_addr.s_addr = inet_addr(IP);
	c.sin_port = htons(PORT);

	printf("Enter the data to be sent:");
	scanf("%[^\n]s",buf);

	if ((sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&c, sizeof(c) ))>0)
	   printf("Data Written Successfully \n");	
	close(fd);
	return 0;
}
