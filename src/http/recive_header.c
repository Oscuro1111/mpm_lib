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

/*
  3kb buffer for header reciver
*/
uint32_t _recive_header(char buff[3072], int clnt_sock,uint16_t *new_line,uint16_t *header_size)
{

	uint16_t recived = 0,
		 index,
		 current = 0,
		 total_recived = 0,
		 check = 0;

	uint32_t remain=0;
	const uint8_t MAX_BUF = 50;
	char buffer[64];


	char match[] = {'\r', '\n', '\r', '\n', '\0'};


      log_str("Getting header");
	if (clnt_sock < 0)
	{
		fprintf(stderr, "Client socket is unvalid!\n");
		return -1;
	}

	while ((recived = recv(clnt_sock, buffer, MAX_BUF, 0)))
	{
		total_recived += recived;

		for (index = 0; index < recived; index++)
		{
			if (buffer[index] == match[check])
			{
				if (buffer[index] == '\n')
				{

					*new_line=*new_line+1;
				}
				check++;
				buff[current + index] = buffer[index];
			}
			else
			{
				if(buffer[index]=='\n'){
					*new_line=*new_line+1;
				}

				check=0;
				buff[current + index] = buffer[index];
			}
			if (check == 4)
			{
				*header_size = current + index;
				 remain = total_recived - *header_size;
				log_str("Done header");
				return remain;
			}
		}

		current += index;
	}

	*header_size=0;
	*new_line = 0;

	log_str("Ending header receiving work");
	return 0;
}