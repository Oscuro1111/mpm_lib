#include<stdio.h>

int join_path(char *buf,unsigned int size, char *root_dir, char *path){
  if(buf==NULL||root_dir==NULL||path==NULL){
    fprintf(stderr, "buf or root_dir or path is NULL");
    return -1;
  }

  return snprintf(buf,size,"%s/%s",root_dir,path);
}
