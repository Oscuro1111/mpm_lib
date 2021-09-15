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
#include <stdio.h>

/*
  3kb buffer for header reciver
*/

int32_t recive_header_data(char buff[3072], uint16_t *new_line, uint16_t *header_size, FILE *fp)
{
	uint16_t total_recived = 0,
			 check = 0;

	char ch;

	if (fp == NULL)
	{
		log_str("Unable to open clnt_sock,fdopen() return NULL!");
		return -1;
	}

	log_str("Getting header");
	
	for (; total_recived < 3072;)
	{
		ch = getc(fp);
		if (ch == '\r' || ch == '\n' || ch == '\r' || ch == '\n')
		{
			check++;
			*new_line=*new_line+1;	
		}
		else
		{
			check = 0;
		}

		buff[total_recived] = ch;
		total_recived++;

		if (check == 4)
		{
			*header_size = total_recived;
			return 0;
		}
	}

	log_str("unexpected error :header size exceeded 3kb limit!");
	return -1;
}