#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../../includes/http.h"
#include "../../includes/utils.h"

uint32_t parse_header_line(char *header_option, char *option[256]) {

  const char *DELIMITR = " ";
  uint32_t len = 0;
  char *_state;

  char *str = strtok_r(header_option, DELIMITR,&_state);

  option[len] = str;

  len++;

  while ((str = strtok_r(NULL, DELIMITR,&_state))) {
    option[len] = str;
    len++;
  }

  option[len] = NULL;
  return len;
}

//NOTE:
//ISSUE: SEGFAULT aftersevreal request
int parse_header(char **header_options, size_t size, Header_t *header) {

  uint16_t i = 0;
  uint16_t index = 0;
  char *str;
  uint8_t len;

  while (((str = header_options[index++]) != NULL)) {

    char *temp[MAX_COOKIES];

    len = parse_header_line(str, temp);

    if (strcmp(temp[0], "GET") == 0 || strcmp(temp[0], "POST") == 0) {
      header->method = temp[0];
      header->path = temp[1] + 1; // /path/to/file -> path/to/file
      header->protocol = temp[2];
      continue;
    }

    if (strcmp("Accept:", temp[0]) == 0) {
      header->accept = temp[1];
      continue;
    }

    if (strcmp("Accept-Language:", temp[0]) == 0) {
      header->accept_language = temp[1];
      continue;
    }

    if (strcmp("Connection:", temp[0]) == 0) {
      header->connection = temp[1];
      continue;
    }

    if (strcmp("Accept-Encoding:", temp[0]) == 0) {
      header->accept_encoding = temp[1];
      continue;
    }

    if(strcmp("Cookie:",temp[0])==0){
  
      for(;i<len-1&&i<MAX_COOKIES;i++)
      header->cookies[i]=temp[i+1];

      header->cookies[i] = NULL;
}

    if (strcmp("Host:", temp[0]) == 0) {
      header->host = temp[1];
      continue;
    }

    if (strcmp("Content-Type:", temp[0]) == 0) {
      if (strcmp("multipart/form-data;", temp[1]) == 0) {

        header->content_type = strtok(temp[1], ";");
        header->boundary = temp[2] + 9;

      } else if (strcmp("application/x-www-form-urlencoded", temp[1]) == 0) {
        header->content_type = temp[1];
      }

      continue;
    }

    if (strcmp("Content-Length:", temp[0]) == 0) {
      header->content_length = atoi(temp[1]);
      continue;
    }
  }

  return 0;
}


//BUG:MAIN CENTER(malloc) heap_corruption
int parse(const char *buffer, size_t size, uint32_t num_header_option,
          Request *request) {

  char *copy_buffer = (char *)malloc(size + 256*1000);//Gardian memory
  const char *lineBreak = "\r\n";

  char *_state;

  uint16_t options = 0;

  log_str("header options");
  log_num(num_header_option);

  char **header_options = (char **)malloc(sizeof(char *) * num_header_option*100); 
  strncpy(copy_buffer, buffer, size);

  //------------------------------------------------------------
  char *str = strtok_r(copy_buffer, lineBreak,&_state);

  header_options[options] = str;

  options++;

  while ((str = strtok_r(NULL, lineBreak,&_state))) {
    header_options[options] = str;
    options++;
  }

  //--------------------------------------------------------
  header_options[options] = NULL;

  request->mem_buffer = copy_buffer;

  log_str("Before parser\n");
  //Bug: make crash after several requests
  parse_header(header_options, options, &request->header);

  log_str("after header parser");

  if(header_options)
  free(header_options);

  log_str("done free");
   header_options=NULL;
  return 0;
}

int readChannel(char *buf, size_t size, FILE *file, uint32_t *new_lines) {

  if (buf == NULL || file == NULL)
    return -1;

  uint32_t i = 0;
  char ch;
  uint8_t end = 0;
  char check[4] = {'\r', '\n', '\r', '\n'};

  while ((ch = getc(file)) != EOF) {
    fprintf(stderr, "%c", ch);
    if (ch == check[end]) {
      end++;
      goto save;
    } else {
      end = 0;
      buf[i++] = ch;
      continue;
    }
  save:
    *new_lines += 1;
    buf[i++] = ch;
    if (end == 3) {
      goto headerEnd;
    }
  }

headerEnd:
  buf[i] = '\0';
  return i;
}

void set_responseHeader(Response *res, int res_option, const char *value) {

  switch (res_option) {
  case STATUS:
    strcpy(res->status, value);
    break;
  case COOKIES:
     strncpy(res->cookies,value,2048);
     break;
  case CONTENT_TYPE:
    strcpy(res->content_type, value);
    break;
  case CONTENT_LENGTH:
    strcpy(res->content_length, value);
    break;
  case ACCEPT_RANGES:
    strcpy(res->accept_ranges, value);
    break;
  case CONNECTION:
    strcpy(res->connection, value);
    break;
  default:
    fprintf(stderr, "\nInvalid response  header option\n");
    break;
  }
}

void response(Response *http_response, char *body, uint length, uint size) {

  const uint32_t RES_SIZE = size * length + sizeof(Response) + 1024;

  fprintf(stderr, "\nRES_SIZE=%d\n", RES_SIZE);
  // Warning: NULL is not checked
  char *response = (char *)malloc(RES_SIZE);

  uint offset = 0;

  memset(response, 0, RES_SIZE);

  if (strcmp("200", http_response->status) == 0) {
    offset +=
        snprintf(response + offset, RES_SIZE - offset, "%s", "HTTP/1.1 200 OK");
    response[offset++] = '\r';
    response[offset++] = '\n';
  }

  if (strlen(http_response->content_length) > 0) {
    offset += snprintf(response + offset, RES_SIZE - offset, "%s: %s",
                       "Content-Length", http_response->content_length);
    response[offset++] = '\r';
    response[offset++] = '\n';
  }

  if (strlen(http_response->content_type) > 0) {
    offset += snprintf(response + offset, RES_SIZE - offset, "%s: %s",
                       "Content-Type", http_response->content_type);
    response[offset++] = '\r';
    response[offset++] = '\n';
  }

  if (strlen(http_response->accept_ranges) > 0) {

    offset += snprintf(response + offset, RES_SIZE - offset, "%s: %s",
                       "Accept-Ranges", http_response->accept_ranges);
    response[offset++] = '\r';
    response[offset++] = '\n';
  }

  if (strlen(http_response->connection) > 0) {
    offset += snprintf(response + offset, RES_SIZE - offset, "%s: %s",
                       "Connection", http_response->connection);
    response[offset++] = '\r';
    response[offset++] = '\n';
  
  }

  if(strlen(http_response->cookies)>0){
   offset += snprintf(response + offset, RES_SIZE - offset, "%s: %s",
                       "Set-Cookie", http_response->cookies);
    response[offset++] = '\r';
    response[offset++] = '\n';
  
  }

  if (body != NULL) {
    response[offset++] = '\r';
    response[offset++] = '\n';

    memcpy(response+offset,body,length);
    offset+=length;
  }
  
  send(http_response->sockfd, response, offset, 0);
  free(response);
  fprintf(stderr,"Done response\n");
}
