#include<string.h>

#include "../../includes/mpm_list.h"
//disructive on string .
//pass copy of string recommened.
char *last_token(char *string,char *delimiter){

  char *temp;
  char *last;
  char *_save;

  char *prev;

  temp=strtok_r(string,delimiter,&_save);

  if(is_null(temp)){
    return  NULL;
  }else goto loop;

  while((temp=strtok_r(NULL,delimiter,&_save))!=NULL){
 loop:
    prev=temp;
  }
  return prev;
}

