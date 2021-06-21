#include "../includes/error_exit.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


extern void PrintSocketAddress(struct sockaddr*, FILE *stream);

static const int MAXPENDING = 5;
int SetupTCPServerSocket(const char *service) {

  //Construct the server address structure
  struct addrinfo addreCriteria;
  memset(&addreCriteria, 0, sizeof(addreCriteria));

  addreCriteria.ai_family = AF_UNSPEC;

  // Accept on any address/port
  addreCriteria.ai_flags = AI_PASSIVE;

  addreCriteria.ai_socktype = SOCK_STREAM;
  addreCriteria.ai_protocol = IPPROTO_TCP;

  struct addrinfo *servAddr; // List of server address available

  int rtnVal = getaddrinfo(NULL, service, &addreCriteria, &servAddr);

  if (rtnVal != 0) {
    DieWithUserMessage("getaddrinfo() unable to work", gai_strerror(rtnVal));
  }

  int servSock = -1;

  for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
    // Create TCP socket
    servSock = socket(addr->ai_family, addr->ai_socktype,
                      addr->ai_protocol);
    if (servSock < 0) {
      continue;
    }

    // Bind to the local address and set socket to list
    if ((bind(servSock, addr->ai_addr, addr->ai_addrlen) == 0) &&
        (listen(servSock, MAXPENDING) == 0)) {
      //Print local address of socket

      struct sockaddr_storage localAddr;

      socklen_t addrSize = sizeof(localAddr);

      if(getsockname(servSock,(struct sockaddr *)&localAddr,&addrSize)<0){

        DieWithSystemMessage("getSockName() unable to work!");
      }
      fputs("Bind to:",stdout);

      PrintSocketAddress((struct sockaddr *)&localAddr,stdout);
      fputc('\n',stdout);
      break; //Bind and list successful
    }
    close(servSock);
    servSock = -1;
  }
  //Free address list allocated by getaddrinfo()
  freeaddrinfo(servAddr);
  return servSock;
}
