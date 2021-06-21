#include "../../includes/utils.h"
#include<stdio.h>
#include<stdint.h>

//log string.
void log_str(char *str){
  fprintf(stderr,"\n%s\n",str);
}


// log numeric values.
void log_num(long int num){

  fprintf(stderr,"\n%ld\n",num);
}
