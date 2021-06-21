#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../includes/http.h"
#include "../../includes/utils.h"
#include <regex.h>

static MPM_MIMIE_TYPES MIME_TYPE = {
    .png = "image/png",
    .bin = "application/octet-stream",
    .bz = "application/x-bzip",
    .bmp = "image/bmp",
    .htm = "text/html",
    .doc = "application/msword",
    .docx = "application/vnd.openxmlformats-officedocument.wordprocessingml.document",
    .ico = "image/vnd.microsoft.icon",
    .bz2 = "application/x-bzip2",
    .zip = "application/zip",
    .mpeg = "video/mpeg",
    .mjs = "text/javascript",
    .odt = "application/vnd.oasis.opendocument.text",
    .pptx = "application/vnd.openxmlformats-officedocument.presentationml.presentation",
    .ppt = "application/vnd.ms-powerpoint",
    .oga = "audio/ogg",
    .svg = "image/svg+xml",
    .tar = "application/x-tar",
    .sh = "application/x-sh",
    .rar = "application/vnd.rar",
    .wav = "audio/wav",
    .xhtml = "application/xhtml+xml",
    ._7z = "application/x-7z-compressed",
    .webm = "video/webm",
    .xls = "application/vnd.ms-excel",
    .xlsx = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
    .mp3 = "audio/mpeg",
    .mp4 = "video/mp4",
    .xml = "application/xml",
    .gif = "image/gif",
    .json = "application/json",
    .js = "text/javascript",
    .txt = "text/plain",
    .css = "text/css",
    .jpeg = "image/jpeg",
    .jpg = "image/jpeg",
    .pdf = "application/pdf",
    .html = "text/html"};

#ifndef MAX_ERROR_MSG
#define MAX_ERROR_MSG 1024
#endif

static int compile_regex(regex_t *r, const char *regex_text)
{
	int status = regcomp(r, regex_text, REG_EXTENDED | REG_NEWLINE);
	if (status != 0)
	{
		char error_message[MAX_ERROR_MSG];
		regerror(status, r, error_message, MAX_ERROR_MSG);
		printf("Regex error compiling '%s': %s\n",
		       regex_text, error_message);
		return 1;
	}
	return 0;
}

int file_extention_regex(char *regexp, char *to_match, char *extention, unsigned int size)
{
	regex_t r;
	const char *regex_text;
	const char *find_text;

	/* "P" is a pointer into the string which points to the end of the
       previous match. */
	const char *p = to_match;
	/* "N_matches" is the maximum number of matches allowed. */
	const int n_matches = MAX_QUERIES;
	/* "M" contains the matches found. */
	regmatch_t m[n_matches];

	regex_text = regexp;

	compile_regex(&r, regex_text);

	while (1)
	{
		int i = 0;
		int nomatch = regexec(&r, p, n_matches, m, 0);
		if (nomatch)
		{
			regfree(&r);
			return -1;
		}
		for (i = 0; i < n_matches; i++)
		{
			int start;
			int finish;
			if (m[i].rm_so == -1)
			{
				break;
			}
			//m[i].rm_so is start  position inside the string.
			//Base is changing
			start = m[i].rm_so + (p - to_match);
			finish = m[i].rm_eo + (p - to_match);

			//capture
			if (i == 1)
			{
				snprintf(extention, size, "%.*s", (finish - start), to_match + start);
				regfree(&r);
				return 0;
			}
		}
		p += m[0].rm_eo;
	}

	regfree(&r);
	return -1;
}

int file_extention(char *str, char *store, unsigned int size)
{

	const char *regex_text;
	const char *find_text;

	int res = -1;
	regex_text = "\\.([a-zA-Z0-9_-]+)$";
	find_text = str;

	res = file_extention_regex(regex_text, find_text, store, size);

	return res;
}

const char *get_content_type(char *file_name)
{
	char extention[16];
	int error = -1;

	if (file_name == NULL)
		return NULL;

	error = file_extention(file_name, extention, 16);

	if (error == -1)
	{
		fprintf(stderr, "get_content_type():unable to find extention for :%s\n", file_name);
		return NULL;
	}

	if (strcmp(extention, "png") == 0)
	{

		return MIME_TYPE.png;
	}

	if (strcmp(extention, "jpg") == 0)
	{
		return MIME_TYPE.jpg;
	}

	if (strcmp(extention, "jpeg") == 0)
	{
		return MIME_TYPE.jpeg;
	}

	if (strcmp(extention, "pdf") == 0)
	{
		return MIME_TYPE.pdf;
	}

	if (strcmp(extention, "html") == 0)
	{
		return MIME_TYPE.html;
	}

	if (strcmp(extention, "css") == 0)
	{
		return MIME_TYPE.css;
	}

	if (strcmp(extention, "svg") == 0)
	{
		return MIME_TYPE.svg;
	}

	if (strcmp(extention, "mp4") == 0)
	{
		return MIME_TYPE.mp4;
	}

	if (strcmp(extention, "mp3") == 0)
	{
		return MIME_TYPE.mp3;
	}

	if (strcmp(extention, "txt") == 0)
	{
		return MIME_TYPE.txt;
	}

	if (strcmp(extention, "json") == 0)
	{
		return MIME_TYPE.json;
	}

	if (strcmp(extention, "js") == 0)
	{
		return MIME_TYPE.js;
	}

	if (strcmp(extention, "mjs") == 0)
	{
		return MIME_TYPE.mjs;
	}

	if (strcmp(extention, "gif") == 0)
	{
		return MIME_TYPE.gif;
	}

	if (strcmp(extention, "ppt") == 0)
	{
		return MIME_TYPE.ppt;
	}

	if (strcmp(extention, "zip") == 0)
	{
		return MIME_TYPE.zip;
	}

	if (strcmp(extention, "xml") == 0)
	{
		return MIME_TYPE.xml;
	}

	if (strcmp(extention, "doc") == 0)
	{
		return MIME_TYPE.doc;
	}

	if (strcmp(extention, "docx") == 0)
	{
		return MIME_TYPE.docx;
	}

	if (strcmp(extention, "xls") == 0)
	{
		return MIME_TYPE.xls;
	}

	if (strcmp(extention, "xlsx") == 0)
	{
		return MIME_TYPE.xlsx;
	}
	if (strcmp(extention, "sh") == 0)
	{
		return MIME_TYPE.sh;
	}

	if (strcmp(extention, "tar") == 0)
	{
		return MIME_TYPE.tar;
	}

	if (strcmp(extention, "rar") == 0)
	{
		return MIME_TYPE.rar;
	}

	if (strcmp(extention, "ico") == 0)
	{
		return MIME_TYPE.ico;
	}

	if (strcmp(extention, "odt") == 0)
	{
		return MIME_TYPE.odt;
	}


	if (strcmp(extention, "wav") == 0)
	{
		return MIME_TYPE.wav;
	}

	if (strcmp(extention, "bin") == 0)
	{
		return MIME_TYPE.bin;
	}


	return NULL;
}
