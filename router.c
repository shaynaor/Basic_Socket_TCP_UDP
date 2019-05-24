#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <time.h>

#define SIM_LENGTH 10 
#define PORTA 13000 
#define PORTB 14000 

void clean_up(int cond, int *sock)
{ printf("Exiting now.\n");
  close(*sock); 
  exit(cond);
} 

int main(void)
{ 
  
  int sockA ,sockB; 
  int connect_sockA ,connect_sockB; 
  struct sockaddr_in serv_nameA ,serv_nameB ; 
  socklen_t len; 
  int count;

  /* Opens TCP socketA with IPv4 */
  sockA = socket(AF_INET, SOCK_STREAM, 0);
  /* Checks if the socket opened correctly - else print error massage. */  
  if (sockA < 0)
    { perror ("Error opening channel");
      clean_up(1, &sockA);
    }

    /* Opens TCP socketB with IPv4 */
  sockB = socket(AF_INET, SOCK_STREAM, 0);
  /* Checks if the socket opened correctly - else print error massage. */  
  if (sockB < 0)
    { perror ("Error opening channel");
      clean_up(1, &sockB);
    }

  /* Zeroing server_nameA. */
  memset((char *)&serv_nameA,0, sizeof(serv_nameA)); 
  serv_nameA.sin_family = AF_INET;                                   
  serv_nameA.sin_port = htons(PORTA); 

   /* Zeroing server_nameB. */
  memset((char *)&serv_nameB,0, sizeof(serv_nameB)); 
  serv_nameB.sin_family = AF_INET;                                   
  serv_nameB.sin_port = htons(PORTB); 

  /* Binds the adrres of the server to the socketA - if the biding fails prints error message. */
  if (bind(sockA, (struct sockaddr *)&serv_nameA, sizeof(serv_nameA)) < 0)
    { perror ("Error naming channel");
      clean_up(1, &sockA);
    }

    /* Binds the adrres of the server to the socketB - if the biding fails prints error message. */
  if (bind(sockB, (struct sockaddr *)&serv_nameB, sizeof(serv_nameB)) < 0)
    { perror ("Error naming channel");
      clean_up(1, &sockB);
    }





/* Waiting for socket connection from clientB. */   
  printf("Server is alive and waiting for socket connection from clientB.\n");
  listen(sockB, 1);

   /* Accepting connection from clientB. */
  len = sizeof(serv_nameB);
  connect_sockB = accept(sockB, (struct sockaddr *)&serv_nameB, &len);





  /* Waiting for socket connection from clientA. */   
  printf("Server is alive and waiting for socket connection from clientA.\n");
  listen(sockA, 1);

  /* Accepting connection from clientA. */
  len = sizeof(serv_nameA);
  connect_sockA = accept(sockA, (struct sockaddr *)&serv_nameA, &len);


    int msg_b = 0;//clientB sinding int 8
    char msg_a = '$';//clientA sending char A
    int r = 0;
    int counter = 0;

    srand(time(NULL)); // Initialization, should only be called once.
    /* Runs at most 10 times. */
    while(counter < 10){
        /* If first iteration or the random number bigger than 3 read the message from clientA */
        if(counter == 0 || r > 3){
          read(connect_sockA,&msg_a,1);
        }
         r = rand() % 10; //random number between 1 to 10.
        if(msg_a == 'A'){
            /* If r > 3 sending A to clientB */
            if(r > 3){
                write(connect_sockB,&msg_a , 1);
                msg_a = '$';
            }
            counter++;
        }
        /* If random number bigger than 3 read the message from clientB.  */
        if(r > 3){
          read(connect_sockB,&msg_b,4);
        }
       

        /* If msg_b == 8 sending 8 to clientA */
        if(msg_b == 8){
            write(connect_sockA,&msg_b , 4);
            msg_b = 0;
            counter++;
        }
      counter++;
    }
  
  /* Closing socketA. */
  close(connect_sockA); 
  clean_up(0, &sockA); 

    /* Closing socketB. */
  close(connect_sockB); 
  clean_up(0, &sockB); 

}
