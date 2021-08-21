
/*


#include "../includes/error_exit.h"
#include "../includes/http.h"
#include "../includes/mpm_tcp.h"
#include "../includes/utils.h"
#include <stdlib.h>
#include <string.h>




#define RESOURCE_DIR "public"

int http_mpm_server_start(char *port, char *root_dir) {

  if (port==NULL||root_dir==NULL) {
    DieWithUserMessage("Parameter(s)", "<Server Port/Service> ,root dir");
  }

 const int MAX_SIZE = (strlen(root_dir)+ strlen(RESOURCE_DIR)+2) * sizeof(char);

   char * const resource_path =
   (cha
int http_mpm_server_start(char *port, char *root_dir) {

  if (port==NULL||root_dir==NULL) {
    DieWithUserMessage("Parameter(s)", "<Server Port/Service> ,root dir");
  }

 const int MAX_SIZE = (strlen(root_dir)+ strlen(RESOURCE_DIR)+2) * sizeof(char);

   char * const resource_path =
   (char *)malloc(MAX_SIZE);
r *)malloc(MAX_SIZE);

 if(resource_path==NULL){
   DieWithSystemMessage("Unable to allocate resources");
  }

 if(join_path(resource_path,MAX_SIZE,root_dir, RESOURCE_DIR)<0){
   DieWithUserMessage("join_path:","Unable to work");
 }

  int servSock = SetupTCPServerSocket(port);

  // serveSock is now ready to accept connection
  // Create an input stream from the socket
  for (;;) {
    // Wait for client to connect 
    int clntSock = AcceptTCPConnection(servSock);
    HandleClient(clntSock, resource_path,root_dir);
  } // Each client

  free(resource_path);
  return 0;
}


*/
