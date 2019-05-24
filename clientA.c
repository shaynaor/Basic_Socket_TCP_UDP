
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
#define SIM_LENGTH 10 
#define IP_ADDRESS "127.0.0.1" 
#define PORT 13000

int main(void)
{ 
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value; 
  char* name = "clientA";

  printf("ClientA is alive and establishing socket connection.\n");
  
  /* Opens TCP socket with IPv4 */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  /* Checks if the socket opened correctly - else print error massage. */
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
      
  /* Zeroing client name. */
  memset((char *)&cli_name,0, sizeof(cli_name)); 
  cli_name.sin_family = AF_INET; 
  cli_name.sin_addr.s_addr = inet_addr(IP_ADDRESS); 
  cli_name.sin_port = htons(PORT);

  /* Creating connection with the server - if connection fails print error message. */
  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }

    int counter = 0;
    int msg_from_b = 0;
    char ch = 'A';

    write(sock, &ch ,1);//write the first message from clientA.

    while(counter < 5){
        read(sock, &msg_from_b ,4);

        if(msg_from_b == 8){
            write(sock,&ch ,1);
            printf("ClientB: %d\n" , msg_from_b);
            msg_from_b = 0;
        }
        counter++;
    }

  /* Prints exitinig message and closing socket. */
  printf("Exiting now.\n");

  close(sock); 
  exit(0); 

} 
