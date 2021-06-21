#include "../includes/error_exit.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

extern void PrintSocketAddress(struct sockaddr*addr,FILE *stream);

int AcceptTCPConnection(int servSock){

  struct sockaddr_storage clntAddr;

  //Set length of client address structure (in-out parameter)
  socklen_t  clntAddrLen =sizeof(clntAddr);

  //Wait for client to connect
  int clntSock = accept(servSock,(struct sockaddr *)&clntAddr,&clntAddrLen);

  if(clntSock<0){
    DieWithSystemMessage("accept() unable to work!");
  }
  //clntSock connected to a client
  fputs("Handle client::",stdout);

  PrintSocketAddress((struct sockaddr *)&clntAddr,stdout);

  fputc('\n',stdout);

  return clntSock;
}

