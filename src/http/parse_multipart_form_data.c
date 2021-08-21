#include "../../includes/http.h"
#include "../../includes/utils.h"
#include "../../includes/fs_c.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdint.h>

uint32_t parse_header_by_deli(char *header_option, char *option[256],
                              char *delimiter)
{

  const char *DELIMITR = delimiter;
  uint32_t len = 0;
  char *_state;

  char *str = strtok_r(header_option, DELIMITR, &_state);

  if(str==NULL){
    return 0;
  }

  option[len] = str;

  len++;

  while ((str = strtok_r(NULL, DELIMITR, &_state)))
  {
    option[len] = str;
    len++;
  }

  option[len] = NULL;
  return len;
}

int join_str(char *result_buf, uint32_t buf_size, const char *str_start,
             const char *str_end)
{

  return snprintf(result_buf, buf_size, "%s%s", str_start, str_end);
}

void parse_file_info_header(char *line, Multipart_Form_Data *multipart_form)
{

  const char *delimiter = ";";
  char *_state;
  char *temp = strtok_r(line, delimiter, &_state);
  char *options[4];
  char *prev;

  while ((temp = strtok_r(NULL, delimiter, &_state)) != NULL)
  {

    parse_header_by_deli(temp, options, "=");

    if (strcmp(" name", options[0]) == 0)
    {

      multipart_form->name = options[1];

      continue;
    }
    else if (strcmp(" filename", options[0]) == 0)
    {

      char *file_name[1];

      // file_name="name of file" removing \" for getting file name
      parse_header_by_deli(options[1], file_name, "\"");

      multipart_form->file_name = file_name[0];

      continue;
    }
  }
  return;
}

void free_multipart_form(Multipart_Form *multipart)
{
  if (multipart == NULL)
  {
    return;
  }
  if (multipart->counter < MAX_NUM_FILES_SUPPORT)
  {

    for (uint32_t form = 0; form < multipart->counter; form++)
    {
      if (multipart->forms[form].file_data != NULL)
      {
        free(multipart->forms[form].file_data);
      }
    }
  }
}

char readBuffer(char *buffer, const uint32_t size, uint8_t reset)
{
  static uint32_t offset;
  static char *current_buffer;
  static uint32_t max_size;

  if (buffer != current_buffer || reset == 0)
  {
    offset = 0;
    current_buffer = buffer;
    max_size = size;
  }

  return offset <= max_size ? current_buffer[offset++] : EOF;
}

int readHeader(char *buf, size_t size, char *buffer, uint32_t buffer_max_size,
               uint32_t *new_lines)
{

  if (buf == NULL || buffer == NULL)
    return -1;
  uint32_t i = 0;
  char ch;
  uint8_t end = 0;
  char check[4] = {'\r', '\n', '\r', '\n'};

  for (uint32_t index = 0; index < buffer_max_size; index++)
  {

    ch = buffer[index];
    fprintf(stderr, "%c", ch);

    if (ch == check[end++])
    {
      goto save;
    }
    else
    {
      end = 0;
      buf[i++] = ch;
      continue;
    }

  save:
    *new_lines += 1;
    buf[i++] = ch;
    if (end == 3)
    {
      goto headerEnd;
    }
  }

headerEnd:
  buf[i] = '\0';

  return i;
}

int32_t recive_header(Request *request, char *rem, uint32_t size)
{

  char buffer[2048]; // 2kb data

  char header[1024];

  uint32_t readed = 0;
  uint32_t offset = 0;
  uint32_t new_lines = 0;
  uint32_t header_size = 0;


read_data:
   readed = recv(request->clnt_sock, buffer + offset, 2048 - offset, 0);

   /*
      Must have readed  50bytes total before reciving zero bytes  as header is more then 50bytes longer . 
   */
   if(readed==0){
    fprintf(stderr,"False connection\n");
     return -1;
  }

  if (readed < 50)
  {
    offset += readed;
    goto read_data;
  }

  offset += readed;

  if ((header_size = readHeader(header, 1024, buffer, 2048, &new_lines)) == 0)
  {
    return -1;
  }

  if ((offset - header_size) >= size)
  {
    return -1;
  }

  memcpy(rem, (buffer + header_size), offset - header_size);

  // Parsing header
  parse(header, header_size, new_lines, request);

  //remain
  return offset - header_size;
}

char *recive(char *root_dir, Request *request,
             uint32_t *file_size, char *remain, uint32_t remain_bytes)
{
  char *ret_path;

  static uint32_t num = 0;

  char path[1024];
  char location[1024];
  char cachefile_name[16];
  char path_to_cache[1024];

  char buf[2048];
  uint32_t i = 0;
  uint32_t bytes_readed = 0;
  uint32_t total_bytes_to_read =
      request->header.content_length - remain_bytes;

  int_to_str(cachefile_name, ++num);

  join_str(location, 1024, "temp/cache", cachefile_name);

  join_str(path_to_cache, 1024, location, ".txt");

  join_path(path, 1024, root_dir, path_to_cache);

  FILE *cache_file = fopen(path, "a");

  if (cache_file == NULL)
  {
    fprintf(stderr, "\nunable to open or creae file\n");
    return NULL;
  }

  fwrite(remain, sizeof(char), remain_bytes, cache_file);

  while (i <= total_bytes_to_read)
  {

    bytes_readed = recv(request->clnt_sock, buf, 1536, 0);
    i += bytes_readed;
    fwrite(buf, sizeof(char), bytes_readed, cache_file);
  }

  *file_size = i;

  fclose(cache_file);

  ret_path = (char *)malloc(1024);

  memcpy(ret_path, path, strlen(path) + 2);

  return ret_path;
}

void parse_multipart_form(char *path, char *root_dir, Request *request,
                          uint32_t file_size)
{

  FS_File fs_file;

  Multipart_Form form;

  FILE *channel = fopen(path, "r");

  int f_size = file_size;
  char tchar;
  char check[4] = {'\r', '\n', '\r', '\n'};
  uint16_t end = 0;
  int32_t i = 0;
  char buf[1024];

  char *temp;
  char *next;
  char *_state;

  char *content_type;
  char res_path[1024];

  const char *DELIMITER = "\r\n";
  if (channel == NULL)
  {
    return;
  }

  while ((tchar = getc(channel)) != EOF)
  {

    if (check[end] == tchar && end < 4)
    {

      fprintf(stderr, "%c", tchar);
      buf[i++] = tchar;
      ++end;
      if (end == 4)
      {
        goto store_file;
      }
      continue;
    }
    else
    {
      if (end > 0)
        end = 0;

      fprintf(stderr, "%c", tchar);

      buf[i] = tchar;
      i++;
      continue;
    }
  }

store_file:

  temp = strtok_r(buf, DELIMITER, &_state);
  next = strtok_r(NULL, DELIMITER, &_state);
  content_type = strtok_r(NULL, DELIMITER, &_state);

  parse_file_info_header(next, &form.forms[0]);

  form.forms[0].content_type = content_type + strlen("Content-Type: ");

  snprintf(res_path, 1024, "%s/%s/%s", root_dir, "temp",
           strtok_r(form.forms[0].file_name, "\"", &_state));

  FILE *file = fopen(res_path, "a");

  if (file == NULL)
  {
    fprintf(stderr, "%s:unable to open file", res_path);
    return;
  }

  f_size -= i;

  f_size = f_size - strlen(request->header.boundary) - 7;

  for (i = 0; i < f_size; i++)
  {
    tchar = getc(channel);
    fputc(tchar, file);
  }

  memcpy(request->header.multipart_form.file_path, res_path, strlen(res_path));

  memcpy(request->header.multipart_form.file_name,
         form.forms[0].file_name,
         strlen(form.forms[0].file_name));

  request->header.multipart_form.is_any_file = 1;

  //Path to file root/dir
  snprintf(res_path, 1024, "%s/%s", root_dir, "temp");

  fs_file.name = form.forms[0].file_name;
  fs_file.path = res_path;

  f_size = 0;

  f_size = fs_file_size(&fs_file);

  if (f_size == -1)
    fprintf(stderr, "%s: Unabel to find size of file!", res_path);

  request->header.multipart_form.file_size = f_size;

  fclose(channel);

  fclose(file);

   //Delete cache file.
   if (unlink(path) == -1)
   {
     fprintf(stderr, "%s unable to delete this file\n", path);
   }
}
