#include<stdio.h>
#include<netdb.h>
#include<arpa/inet.h>

void PrintSocketAddress(const struct sockaddr *address, FILE *stream){

  if(address ==NULL||stream ==NULL){ /*  */
    fprintf(stderr,"address or stream cannot be NULL!");
    return;
  }

  //pointer to binary address
  void *numericAddress;
  //storage for printable address
  char addrBuffer[INET6_ADDRSTRLEN];

  in_port_t  port;

  switch(address->sa_family){

  case AF_INET:
    numericAddress = &((struct sockaddr_in *) address)->sin_addr;
    port = ntohs(((struct sockaddr_in *) address)->sin_port);
    break;

  case AF_INET6:
    numericAddress = &((struct sockaddr_in6*)address)->sin6_addr;
    port =ntohs(((struct sockaddr_in6*)address)->sin6_port);
    break;

  default:
    fputs("[Unknown type]",stream);
    return;
  }


  //Convert Binary or numeric to printable address
  if(inet_ntop(address->sa_family,numericAddress,addrBuffer,sizeof(addrBuffer))==NULL){

    fputs("Invalid address",stream);
  }else{

    fprintf(stream,"\nAddress:%s\n",addrBuffer);
    if(port!=0){
      fprintf(stream,"port:%u\n",port);
    }
  }

}
