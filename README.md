# 2-Choice-Hashing

>> This is a Client-Server Socket Programming which uses Select() as the blocking statement for continuous monitoring of any incoming TCP/UDP Packets.

>> The Incoming Packets are stored in a Hash Table which uses 2-Choice Hashing Technique followed by Separate Chaining method to prevent any collisions.

# Method to create Executable file
>> To Create the executable file, Go to the Make folder. Open this path in the Terminal and give command make.

>> The executable will be created with the name as "Server". 

>> In the src Folder, there are two files: Server.c and Hash.c

# Server.c 
>> File contains the Socket Related functions like: Socket(), Bind(), Listen(), Accept() for the TCP Connections while it has Socket() and recvfrom() API for the UDP Connections.

# Hash.c 
>> File contains all the functions which will be required for the modification of the Hash Table like Insert_Packet(), Delete_Packet() and Lookup_Packet().

#For Hashing, a structure is used. 

#Name of the Structure is "Packet" and the Structure Members are:

  	unsigned short port;    //To store the Port of the Incoming Packet
	unsigned char ip[16];   //To store the IP Address of the Incoming Packet
	unsigned char time[30]; //To store the Timestamp of the Incoming Packet
	unsigned short count;   //To store the occurance of the same IP Packet
	unsigned char data[50]; //To store the data received
	unsigned short protocol //To store the Protocol for the Incoming IP Packet
	struct packet *next;    


# Insert_Packet() 
>> API is used to insert/update a Hash Entry. It has a void pointer as argument which then, is later type-casted to Struct Packet format.

# Delete_Packet() 
>> API is used to delete the Hash Entry. This API also has a void poineter as argumet which then is later type-casted to struct packet format.

# lookup_packet() 
>> API will look for the particular IP Address and return accordingly. This API also has a void poineter as argumet which then is later type-casted to struct packet format.

