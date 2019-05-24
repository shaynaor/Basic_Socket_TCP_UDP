#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/* Prints the the connection type, port and IP address. */
void printsin(struct sockaddr_in *sin, char *pname, char* msg){
  struct in_addr ipAddr = sin->sin_addr;
  char str[INET_ADDRSTRLEN];
  inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
  printf("%s\n" , pname);
  printf("%s" , msg);
  printf(" ip = ");
  printf("%s", str);
  printf(", port = ");
  printf("%u\n", ntohs(sin->sin_port));
}

int main(int argc, char *argv[])
{
  char* name = "Nagar";
  int socket_fd, cc, fsize;
  struct sockaddr_in  s_in, from;
  struct { char head; u_long  body; char tail;} msg;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);//Open new UDP socket IPv4.

  memset((char *) &s_in,0, sizeof(s_in)); //Zeroing all bits of s_in struct.

  s_in.sin_family = (short)AF_INET;//Defining IPv4 to dest.
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY = is used when you don't need to bind a socket to a specific IP. to Network Byte Order.
  s_in.sin_port = htons((u_short)0x3333);//transforming to Network Byte Order.

  printsin( &s_in, "RECV_UDP", "Local socket is:"); 
  fflush(stdout);

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));//binding the port to the ip address.

  char clientName[10];
  /* Reciving client name and print to the screen. */
  fsize = sizeof(from);
  cc = recvfrom(socket_fd,clientName,8,0,(struct sockaddr *)&from,&fsize);
  printf("Client name = %s\n" , clientName); 
  fflush(stdout);//clearing the buffer.

  

  /* Rcives message from client prints it to the screen. */
  while(1) {
    sendto(socket_fd, name , sizeof(name) , 0 , (struct sockaddr *)&from , sizeof(from));//sending our name.
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
    printsin( &from, "recv_udp: ", "Packet from:");
    printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); 
    fflush(stdout);//clearing the buffer.
  }
  
  return 0;
}

