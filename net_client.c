
#include <sys/types.h>
#include <unistd.h>    
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


#define SIM_LENGTH 10 
//#define IP_ADDRESS "192.168.14.93" 
#define PORT 1337 


/*
PART B:
  2) When the ip address in not defined the client cannot connect to the server.
  5) After changint the IP address we sees that the server sendes numbers to the clients.
  6) We see a message in the terminal "Connection refused". 
  We think that the client sends a SYN to the server port but because the server isnt lisening to the port the
      client doesnt recive an ACK and the client time out.  
  
*/


int main(int argc, char* argv[])
{ 
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value; 
  /*nslookup args */
  char* hostname;
  struct addrinfo* res;
  struct sockaddr_in* saddr;
  char* hostaddr;



  /* If we didnt recives two arguments from the terminal print error message and exit. */
  if (argc != 2) {
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }

  hostname = argv[1]; // putting the second argument into hostname.
  /* If there was an error resolving the host name print error message and exit. */
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }

  /* Putting the IP address into struct and putting IP in decimal plus dots format into hostaddr. */
  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);




  printf("Client is alive and establishing socket connection.\n");
  
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
      

  memset((char *)&cli_name,0, sizeof(cli_name)); 
  cli_name.sin_family = AF_INET; 
  cli_name.sin_addr.s_addr = inet_addr(hostaddr); 
  cli_name.sin_port = htons(PORT);


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }


  for (count = 1; count <= SIM_LENGTH; count++)
    { read(sock, &value, 4);
      printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n");

  close(sock); 
  exit(0); 

} 
