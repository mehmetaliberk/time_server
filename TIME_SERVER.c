#include<stdio.h>	//This header file contains declarations used in most input and output and is typically included in all C programs.
#include<string.h>    // for strlen
#include<sys/socket.h>//The header file socket.h includes a number of definitions of structures needed for sockets. 
#include<arpa/inet.h> // for inet_addr
#include<unistd.h>    // for write
#include<time.h>      //The time.h header file contains definitions of functions to get and manipulate date and time information. 
#define PORT_NUMBER 60008 //our school numbers (2018510008,2019510022)
 //We replaced the last 3 digits of the lower school number with the last 3 digits of 60000.
int main(int argc, char *argv[])
{
    time_t t = time(NULL);//time_t represents the clock time as an integer which is a part of the calendar time. 
    struct tm *tm = localtime(&t);// struct tm holds the date and time which contains:
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char *message;
     
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    //Address Family - AF_INET (this is IP version 4)
    //Type - SOCK_STREAM (this means connection oriented TCP protocol)
    //Protocol - 0 (Specifies a particular protocol to be used with the socket...
       //Specifying a protocol of 0 causes socket() to use an unspecified default protocol) 
       //[ or IPPROTO_IP This is IP protocol]
    if (socket_desc == -1)
    {
        puts("Could not create socket");
        return 1;
    }
     
    server.sin_family = AF_INET;  //IPv4 Internet protocols
    server.sin_addr.s_addr = INADDR_ANY; //IPv4 local host address
    server.sin_port = htons(PORT_NUMBER); // server will listen to 60008 port
     
    // Bind
    if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    //This call results in a stream socket with the TCP protocol providing the underlying communication. 
    {
        puts("Binding failed");
        return 1;
    }
    puts("Socket is binded");
     
    // Listen
    listen(socket_desc, 3);
     
    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    
    c = sizeof(struct sockaddr_in);
    //int sock = *((int *)socket_desc);
    while( new_socket = 
           accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c) )//Accept and incoming connection
    {
        puts("Connection accepted");
        message = "Hello client, I received your connection. See you\n\n";
        write(new_socket, message, strlen(message));
        char client_input[200];//The char array where we hold the inputs 
        while(1){
        memset(client_input, 0, sizeof client_input);//memset() is used to fill that struct with zero values to initiate it.

        
     if (recv(new_socket, client_input, sizeof(client_input), 0) > 0)//returns 0 only when you request a 0-byte buffer or the other peer has gracefully disconnected. 
     {
    
     if(strcmp(client_input, "GET_TIME\r\n") == 0)//if the input is GET TIME
     {
       	char str[100];
  	size_t len = strftime(str, sizeof(str), "%H:%M:%02S\n", tm);
  	//Copies into ptr the content of format, expanding its format specifiers into the corresponding values that represent the time described in timeptr, with a limit of maxsize characters.Copies into ptr the content of format, expanding its format specifiers into the corresponding values that represent the time described in timeptr, with a limit of maxsize characters.
        write(new_socket, str, strlen(str));//print function
      

     }
     else if(strcmp(client_input, "GET_DATE\r\n") == 0)//if the input is GET_DATE
     {
       	char str2[100];
  	size_t len = strftime(str2, sizeof(str2), "%d.%m.%Y\n", tm);
        write(new_socket, str2, strlen(str2));

     }
    else  if(strcmp(client_input, "GET_TIME_DATE\r\n") == 0)//if the input is GET_TIME_DATE
    {
        char str3[100];
  	size_t len = strftime(str3, sizeof(str3), "%H:%M:%02S,%d.%m.%Y\n", tm);
  	write(new_socket, str3, strlen(str3)); 

     }
    else  if(strcmp(client_input, "GET_TIME_ZONE\r\n") == 0)//if the input is GET_TIME_ZONE
    {
        char str4[100];
  	size_t len = strftime(str4, sizeof(str4), "%Z:00\n", tm);
  	write(new_socket, str4, strlen(str4)); 

     }
      else  if(strcmp(client_input, "GET_DAY_OF_WEEK\r\n") == 0)//if the input is GET_DAY_OF_WEEK
      {
        char str5[100];
  	size_t len = strftime(str5, sizeof(str5), "%A\n", tm);
  	write(new_socket, str5, strlen(str5)); 

     }
     else  if(strcmp(client_input, "CLOSE_SERVER\r\n") == 0)//if the input is CLOSE_SERVER
     {
        message = "GOOD BYE\n";
  	write(new_socket, message, strlen(message)); 
  	close(socket_desc);
    	close(new_socket); 

     }
     else	//if the input is wrong
     {
      message = "INCORRECT REQUEST\n";
      write(new_socket, message, strlen(message)); 
     }

     }
    }    
            bzero(client_input,5000);
            //The bzero() function erases the data in the n bytes of the memory starting at the location pointed to by s, by writing zeros (bytes containing '\0') to that area.
    
    }


    return 0;
}

