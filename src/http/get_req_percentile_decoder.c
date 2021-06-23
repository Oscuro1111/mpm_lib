#include <string.h>
#include "../../includes/http.h"
#include <stdlib.h>
#include <stdint.h>

void parse_percentile(String **string,char *value){

	 char temp[2]= {'\0','\0'};

	        char *temp_str=NULL;

		uint32_t index = 0;

		if(string==NULL||value==NULL){
			fprintf(stderr,"parse_percentile():string or value cannot be  NULL\n");
			return;
		}	
		//value
		while(value[index]!='\0'){
			if(value[index]=='%'){
				percentile_decoder(value+index+1,&temp_str);
				insert_string(string,temp_str);
				index=index+3;			
			}else if (value[index]=='+')
			{
				insert_string(string," ");
				++index;
			}else{
				temp[0] = value[index];
				insert_string(string,temp);
				++index;
			}
		}
	
}

int get_percentile_decoder(Request *req){
	uint16_t i ;
	uint32_t index;

	char *_save=NULL;

	char *temp_str=NULL;
	uint32_t value_pos;
	
	if(req!=NULL&&req->header.qrs!=NULL)
	{

	for(i=0;i<req->header.qrs->length;i++){
		char *name = req->header.qrs->queries[i].name;
		char *value= req->header.qrs->queries[i].value;

		String *string = allocate_string(0);

		if(string==NULL){
			fprintf(stderr,"allocate_string():unable to allocate memory String struct.\n");
			return -1;
		}	
		parse_percentile(&string,name);

		insert_string(&string," ");

		value_pos = string->current;

		parse_percentile(&string,value);
		
		string->str[value_pos-1]='\0';
		
		req->header.qrs->queries[i].name = string->str;
		
		req->header.qrs->queries[i].value=string->str+value_pos;

		free_string(req->header.qrs->queries[i]._mem);

		req->header.qrs->queries[i]._mem = string;

	 }
     }else return -1;
	return 0;
}