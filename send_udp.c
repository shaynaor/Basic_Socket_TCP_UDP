#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  char* name = "Shuki";
  char serverName[10];
  int cc, fsize;
  int socket_fd;
  struct sockaddr_in  dest;
  struct hostent *hostptr;
  struct { char head; u_long body; char tail; } msgbuf;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);//Open new UDP socket IPv4.
  memset((char *) &dest,0, sizeof(dest)); //Zeroing all bits of dest struct. 
  hostptr = gethostbyname(argv[1]);//getting arg from terminal, doing nslookup and putting IP into hostptr.
  dest.sin_family = (short) AF_INET;//Defining IPv4 to dest.
  memcpy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);//Copping the address of dest to hostptr.
  dest.sin_port = htons((u_short)0x3333);//transforming to Network Byte Order.

  msgbuf.head = '<';
  msgbuf.body = htonl(getpid()); //returning the id of the main() proccess.
  msgbuf.tail = '>';

  sendto(socket_fd, name , sizeof(name) , 0 , (struct sockaddr *)&dest , sizeof(dest));//sending our name.

  /* Reciving server name and print to the screen. */
  fsize = sizeof(dest);
  cc = recvfrom(socket_fd,serverName,8,0,(struct sockaddr *)&dest,&fsize);
  printf("Server name = %s\n" , serverName); 


  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest)); // Sending message to the server.



  return 0;
}
