#ifndef __FS_C
#define __FS_C

#include <stdint.h>
#include <stdio.h>
#include "./mpm_list.h"
#ifndef IS_NULL
    #define IS_NULL(exp) if(exp==NULL) return 1
#endif

typedef struct fs_c{
	char *name;
	char *path;
	char *buffer;
  uint8_t is_alloc;
	uint32_t size;
	int fd;
	FILE *file;
}FS_File;


int fs_read_file(FS_File *);

int fs_delete_file(FS_File *);

int fs_write_file(FS_File *);

int fs_file_size(FS_File *);

//Check for null values in it.
int check_fs_file(FS_File *file);

//Note: fs_close_file must called before leaving scope or ending 
//close open file stream or  fd and free any allocated memory buffer
void fs_close_file(FS_File *);

  typedef struct fs_c_dir {
    char *name;
    char *path;
    uint32_t num_files;
    uint32_t num_dirs;
    int fd;
  } FS_Dir;

  int fs_read_dir(FS_Dir *,List *);
  int fs_delete_dir(FS_Dir *);
  int fs_append_file_dir(FS_Dir *, char *file_path);
  int fs_delete_file_dir(FS_Dir *, char *file_path);

  void fs_close_dir(FS_Dir *);

#endif
