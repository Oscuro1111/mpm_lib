
/*
    NOTE:Testing _recive_header()
*/

#include "../includes/http.h"

#include "../includes/utils.h"

// log string.
void log_str(char *str) { fprintf(stderr, "\n%s\n", str); }

// log numeric values.

void log_num(long int num) { fprintf(stderr, "\n%ld\n", num); }




void __recive_header_test() {

  /*
        #define MAX 2048

       recived=reciv(sock,buffer+offset,limit,0);
       offset+=recived;

       header_size!=0

       if(offset==0){
        return -1;
       }

       if(offset>0&&offset>50){

       //header by finding /r/n/r/n
       //parse content
       }
  */


  char *header =
      "POST /HTTP/1.1\r\n"
      "Host: localhost:3001\r\n"
      "Referer: http://localhost:3001/form.html\r\n"
      "Origin: http://localhost:3001\r\n"
      "Content-Type: application/x-www-form-urlencoded\r\n"
      "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
      "Upgrade-Insecure-Requests: 1\r\n"
      "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/605.1.15\r\n"
      "(KHTML, like Gecko) Version/13.0 Safari/605.1.15 Midori/6\r\n"
      "Accept-Encoding: gzip, deflate\r\n"
      "Accept-Language: en-US\r\n"
      "Connection: Keep-Alive\r\n"
      "Content-Length: 45\r\n"
      "\r\n\r\n"
    "username=Ben&password=12345&date=1%2F1%2F2021";

  //parse(header,request);
  return;
}

int main() {
  printf("runing test\n");
  // runing test
  __recive_header_test();
  printf("Testing done\n");
}
