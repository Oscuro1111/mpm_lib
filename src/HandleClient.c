#include "../includes/error_exit.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


void HandleTCPClient(int clntSock) {

  char buffer[BUFSIZE];
  unsigned long len;
  char msg[BUFSIZE];

  ssize_t numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);

  if (numBytesRcvd < 0) {
    DieWithSystemMessage("recv() failed");
  }

  while (numBytesRcvd > 0) {
  
    buffer[numBytesRcvd] = '\0';
    snprintf(msg, BUFSIZE, "Message recived: %s\n", buffer);

    len = strlen(msg);

    ssize_t numBytesSent = send(clntSock, msg, len, 0);

    if (numBytesSent < 0) {
      DieWithSystemMessage("Unabel to send data to client!");
    } else if (numBytesSent != len) {
      DieWithUserMessage("send()", "sent unexpected number of bytes");
    }

    numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);

    fprintf(stderr, "\n%s\n", buffer);

    if (numBytesRcvd < 0 && (errno != EAGAIN) && (errno != EWOULDBLOCK)) {
      DieWithSystemMessage("recv  unable to execute!");
    }
  }

  close(clntSock);
}
