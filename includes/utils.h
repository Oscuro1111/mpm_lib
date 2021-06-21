#ifndef __MPM__UTILS

#define __MPM__UTILS
#include<stdint.h>

void int_to_str(char buf[16], int value);

/*
  join two path.
  return -1 for error
  length of path is return if successfull
 */
int join_path(char *buf , unsigned int size,char *root_dir,char *path);


//disructive on string .
//pass copy of string recommened.
char *last_token(char *string,char *delimiter);

int join_str(char *result_buf,uint32_t buf_size, const char *str_start,
             const char *str_end) ;

  void log_str(char *);
  void log_num(long int);
#endif
