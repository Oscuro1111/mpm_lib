#include "../../includes/utils.h"
#include "../../includes/http.h"
#include "../../includes/string_builder.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


int percentile_decoder(char *str, char **res)
{
  static char *decoded[21][2] = {
      {"3A", ":"},
      {"2F", "/"},
      {"3F", "?"},
      {"23", "#"},
      {"5B", "["},
      {"5D", "]"},
      {"40", "@"},
      {"21", "!"},
      {"24", "$"},
      {"26", "&"},
      {"27", "'"},
      {"28", "("},
      {"29", ")"},
      {"2A", "*"},
      {"2B", "+"},
      {"2C", ","},
      {"3B", ";"},
      {"3D", "="},
      {"25", "%"},
      {"20", "+"}};

  int row, col;
  char *temp;
  if (str == NULL || res == NULL)
    return -1;

  for (row = 0, col = 0; row < 20; row++)
  {
    temp = *(*(decoded + row) + col);
    if (temp[0] == str[0] && temp[1] == str[1])
    {
      *res = *(*(decoded + row) + col + 1);
      return 0;
    }
  }

  return 0;
}

Queries *parse_query(char *buffer, uint32_t size) {

  Queries *queries = (Queries *)malloc(sizeof(Queries));

  char *__save_ptr;
  char *__save_ptr1;

  char *qry_str[MAX_QUERIES];

  const char *AND = "&";
  const char *EQUAL = "=";
  const char *PLUS = "+";

  char temp[2] = {'\0', '\0'};
  char *temp_ptr=NULL;
  short len = 0;
  uint32_t index = 0;

  char *and;
  char *equ;
  char *plus;

  and = strtok_r(buffer, AND, &__save_ptr);

  if (and == NULL) {
    return NULL;
  } else {
    goto parse_loop;
  }

  while ((and = strtok_r(NULL, AND, &__save_ptr)) != NULL &&
         (len < MAX_QUERIES)) {

  parse_loop:
    qry_str[len] = and;
    ++len;
  }

  queries->length = len;
  len = 0;

  do {
    String *string = allocate_string(0);
    uint32_t val_pos;

    index = 0;

    equ = strtok_r(qry_str[len], EQUAL, &__save_ptr1);

    CHECK_NULL(equ);

    fprintf(stderr,"%s\n",equ);

    insert_string(&string, equ);

    insert_string(&string, " ");

    equ = equ + strlen(equ) + 1;

    val_pos = string->current;
    
    while (equ[index] != '\0') {
      if (equ[index] == '+') {
        insert_string(&string, " ");
        ++index;
      } else if(equ[index]=='%'){ 
        percentile_decoder(equ+index+1,&temp_ptr);
        insert_string(&string,temp_ptr);
        index=index+3;
      }else {
        temp[0] = equ[index];
        insert_string(&string, temp);
        ++index;
      }
    }

    queries->queries[len].name  = strtok_r(string->str,"\n ",&__save_ptr1);
    queries->queries[len].value = string->str + val_pos;
    queries->queries[len]._mem  = string;

    ++len;
  } while (len < queries->length);

  return queries;
}


Queries *parse_encoded_url(Request *request, char *remain,
                           uint32_t readed_bytes) {
  char *buffer = (char *)malloc(request->header.content_length + 256);

  uint32_t readed = 0;

  if (buffer == NULL) {
    fprintf(stderr, "Unable to allocate memory for buffer\n");
    return NULL;
  }

  memset(buffer, 0, request->header.content_length+256);

  memcpy(buffer, remain, readed_bytes);
  
  readed += readed_bytes;

  while (readed < request->header.content_length) {
    readed += recv(request->clnt_sock, buffer + readed,
                   request->header.content_length + 256 - readed, 0);
     }

  //Parsing queries 
  Queries *qrs = parse_query(buffer, readed);
 
  free(buffer);
 
 
  return qrs;
}

void dispose_queries(Queries *qrs) {

  uint32_t index;

  if (qrs == NULL) {
    fprintf(stderr, "dispose_queries(): qrs==NULL unexpected value \n");
    return;
  }

  for (index = 0; index < qrs->length; index++) {
    if (qrs->queries[index]._mem != NULL) {
      free_string(qrs->queries[index]._mem);
    }
  }

  free(qrs);
}
