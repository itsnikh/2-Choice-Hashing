/* 
 * This file contains the Insert_packet(), Delete_Paceket(), Lookup_packet() functions which will respectively Insert/Update, Delete,
 * and Look for the incoming IP Packets.
 * According to 2-Choice Hashing, a Balance needs to be maintained for the 2-Choice Hashing method between the 1st Hashing and 
 * 2nd hashing part of the table.
 */

/*
 * This Include file will include all the necessary header files.
 */
#include "../include/header_server.h"

struct packet
{	
	unsigned short port;
	unsigned char ip[16];
	unsigned char time[30];
	unsigned short count;
	unsigned char data[50];
	unsigned short protocol;
	struct packet *next;

}*h[65536];

/*
 * A Global Hash table which has 65536 indexes and the hashing is done accordingly.
 * Also for the hash table, a balance needs to be maintained between both halfs of the tables, Thus, a balance variable is declared.
 */
static char balance=0;

/*
 * Below function takes input the Incoming Packet IP Address, adds all the decimal equivalent numbers and will return an Index value which follows the Modulo-Division Hashing method of the sum with a Prime Number. (here it's 79).
 */
unsigned short hash1(unsigned char *ip)
{
	unsigned short i;
	unsigned short sum=0;

	for(i=0;*(ip+i)!='\0';i++)
	   if (*(ip+i)>='0' && *(ip+i)<='9')
		sum= sum + (unsigned short)( *(ip+i) - 48);
	   else if ( *(ip+i)=='.' )
		continue;
	   else 
		*(ip + i--)='\0';

	return (sum % 79) ;
}

/*
 * Below function takes input the Incoming Packet IP Address, Ex-Or adds all the decimal equivalent numbers of the IP and will calculate sum as an Index value which follows the Psuedo Random Hashing method keeping the value of A and C as same Prime Number (here it's 97).
 * Also, sum value is incremented with 32768 because we will need to insert the details in the second half of the Hash Table.
 */
unsigned short hash2(unsigned char *ip)
{
	unsigned short i;
	unsigned short sum=0;

	for(i=0;*(ip+i)!='\0';i++)
	   if (*(ip+i)>='0' && *(ip+i)<='9')
		sum= sum ^ (unsigned short)( *(ip+i) - 48);
	   else if ( *(ip+i)=='.' )
		continue;

	sum*=97;
	sum+=97;
	sum%=65536;
	if(sum<32768) sum+=32768;
	return sum ;
}

/*
 * The entry_check() called from Lookup_packet() simply traverses through all the entries for the particular index value and returns '0' if the value is present for that index value else '1' for a negative response.
 */
unsigned char entry_check(unsigned short index, struct packet *a)
{
	struct packet *temp;

	temp=h[index];
	while(temp)
	 if(strcmp((char*)temp->ip, (char*)a->ip)==0 && temp->protocol == a->protocol)
	   return 0;
	 else
	   temp=temp->next;	
	return 1;
}

/*
 * The Lookup_Packet() will simply check whether the incoming IP Address is present in the Hash table or not! If positive, then 0 is returned else if negative then 1 is returned. 
 * Do note that the agrument is of type (void *) and the operations are carried out after the type conversion to (Struct Packet *) type.
 */
unsigned char lookup_packet(void *arg)
{
	struct packet *a= (struct packet *)arg;
	unsigned short index1 = hash1(a->ip); 
	unsigned short index2 = hash2(a->ip);
	unsigned char flag=0;

	if(h[index1]==NULL || h[index2]==NULL)
	{
		printf("NO ENTRY PRESENT!!");
		return 1;
	}
	
	if( (flag=entry_check(index1,a))==1)
	{
		if( (flag=entry_check(index2,a))==1)
			printf("NO ENTRY PRESENT!!");
	
		else return flag;	
	}
	else return flag;	
}

/*
 * The Display() will simply display all the valid Entries of the Hash Table.
 */
void display()
{
	unsigned short c;
	unsigned char flag=0;
	struct packet *temp;

	for(c=0;c<65535;c++)
	   if(h[c]!=NULL)
		flag=1;
	
	if(flag==0) 
	  printf( "\n\nHASH TABLE EMPTY\n" );

	else
	{
		printf("\n\n\t\t-----HASH TABLE-----");
		printf("\n\n");
		for(c=0;c<65535;c++)
		{
			temp=h[c];
		   	if(temp==NULL)
		     	   continue;
		   	else
		   	{
			   printf("\n\nINDEX (%02u) ", c);
			   while(temp!=NULL)
			   {		
				if(temp->protocol == TCP)
				printf("\n\nSource IP ADDR.: %s:%5d \n  Dest IP ADDR.: %s:%5d \n\t   TIME: %s       PROTOCOL: TCP\n\t   DATA: %s\n\t  COUNT: %hd \n", temp->ip, temp->port, "192.168.131.38", 5000, temp->time, temp->data, temp->count);
				else 
				printf("\n\nSource IP ADDR.: %s:%5d \n  Dest IP ADDR.: %s:%5d \n\t   TIME: %s       PROTOCOL: UDP\n\t   DATA: %s\n\t  COUNT: %hd \n", temp->ip, temp->port, "192.168.131.38", 5000, temp->time, temp->data, temp->count);
				temp=temp->next;
			   }
		   	}	     
		}
	}
}

/*
 * The Update_Entry() will update the Timestamp, Counter Value and Data receive, if the Entry for the incoming IP address already exist in the Table
 */
void update_entry(struct packet *p, struct packet *a)
{
	unsigned char c;
	for(c=0;a->time[c]!='\0';c++)	
	   p->time[c]=a->time[c];

	++(p->count);
	p->port=a->port;

	for(c=0;a->data[c]!='\0';c++)
	   p->data[c]=a->data[c];
	   p->data[c]='\0';
}

/*
 * New_Entry() will create new entries of new incoming Packets
 */
void new_entry(unsigned short index, struct packet *temp, struct packet *a)
{
	struct packet *p;	
	unsigned char c;

	if(temp!=NULL)
		while(temp->next)
		temp=temp->next;

	p = (struct packet *) malloc(sizeof(struct packet));

	if(p==NULL)
	{
		printf("initial malloc fail...exiting");
		exit(-1);
	}
		
	p = memset(p, '\0', sizeof(struct packet));

	if(a->protocol == TCP)
	   p->protocol = TCP;
	else
	   p->protocol = UDP;

	for(c=0;a->ip[c]!='\0';c++)
   	   p->ip[c]=a->ip[c];
	   p->ip[c]='\0';

	p->port=a->port;

	for(c=0;a->time[c]!='\0';c++)	
	   p->time[c]=a->time[c];

	++(p->count);

	for(c=0;a->data[c]!='\0';c++)
	   p->data[c]=a->data[c];
	   p->data[c]='\0';

	if(h[index]==NULL)
	    h[index]=p;
	else
	{
		temp=h[index];
		while(temp->next)
		    temp=temp->next;
		temp->next=p;
	}
	return;
}

/*
 * Insert_Packet() will insert/update a/an new/old entry. 
 * Do note that the agrument is of type (void *) and the operations are carried out after the type conversion to (Struct Packet *) type.
 */
void insert_packet(void *arg )
{
	struct packet *a= (struct packet *)arg;
	struct packet *temp,*prev=NULL;	
	unsigned short index1 = hash1(a->ip); 
	unsigned short index2 = hash2(a->ip);
	if(h[index1]==NULL && h[index2]==NULL)
	{
		if(balance == 0 )
		{
			new_entry(index1, h[index1], a);
			balance++;
		}
		else if(balance>0)
		{
			new_entry(index2, h[index2], a);
			balance=0;
		}
		return ;
	}
	else if(entry_check(index2, a)==0)
	{
		temp=h[index2];
		while(temp)
		{
			if(strcmp((char*)temp->ip,(char*)a->ip)==0 && a->protocol == temp->protocol)
			{
				update_entry(temp, a);
				return;
			}
			else
			{
				prev=temp;
				temp=temp->next;
			}
		}
	}
	else if(entry_check(index1, a)==0)
	{
		temp=h[index1];
		while(temp)
		{
			if(strcmp((char*)temp->ip,(char*)a->ip)==0 && a->protocol == temp->protocol)
			{
				update_entry(temp, a);
				return;
			}
			else
			{
				prev=temp;
				temp=temp->next;
			}
		}
	}		
	else
	{
		if(balance!=0)
		{
		   new_entry(index2,prev, a);
		   balance=0;
		}
		else if(balance == 0)
		{
		   new_entry(index1,prev, a);
		   balance++;
		}
		return;
	}
}

/*
 * Delete_Packet() will check whether IP entry is present or not.
 * If not present, then the data cannot be deleted. 
 * Else we find the entry and then delete the IP entry. 
 */
void delete_packet(void *arg)
{
	if( lookup_packet(arg) )
     	   printf( "Cannot Delete...Entry not present\n" );
	else
	{
		struct packet *a = (struct packet *) arg;
		unsigned short index1= hash1(a->ip);
		unsigned short index2= hash2(a->ip);
		unsigned char flag=0;
		struct packet *temp, *prev;
		
		temp=h[index1];
		prev=temp;
		if(entry_check(index1, a)==0)
		{
			while(temp!=NULL)
			{		
				if(strcmp((char*)temp->ip,(char*)a->ip)==0 && temp->protocol == a->protocol)
				{
					prev->next = temp->next;
					if(h[index1] == temp)
					h[index1] = temp->next;
					temp = memset(temp, '\0', sizeof(struct packet));
					free(temp);
					temp=NULL;
					flag=1;
					break;
				}	
				else 
				{
					prev=temp;
					temp=temp->next;
				}
			}
		}

		else if( entry_check(index2,a)==0 )
		{
			temp=h[index2];
			prev=temp;
			while(temp!=NULL)
	   		{		
				if(strcmp((char*)temp->ip,(char*)a->ip)==0 && temp->protocol == a->protocol)
				{
					prev->next = temp->next;
					if(h[index2] == temp)
					h[index2] = temp->next;
					temp = memset(temp, '\0', sizeof(struct packet));
					free(temp);
					temp=NULL;
					flag=1;
					break;
				}	
				else 
				{
					prev=temp;
					temp=temp->next;
				}
			}
		}
	}
	if(flag==1)
	printf("\nENTRY DELETED\n");
	else if(flag==0)
	printf("\nENTRY NOT FOUND...CANNOT DELETE IT!!\n");
}
