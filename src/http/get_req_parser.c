#include <stdio.h>
#include <string.h>

#include "../../includes/http.h"
#include "../../includes/http_parser_utility.h"

int get_req_parser(Request *const req)
{
    Get_Req *get_req = (Get_Req *)malloc(sizeof(Get_Req));
    //check http.c parse_header() line 45 for referencing req->header.path-1 as regular expression need /<path>/<path> not <path>/<path> to match full path. 
    if (get_req != NULL && http_get_req_parser(req->header.path-1, get_req) == 0)
    {
        req->get_req_mem = get_req;

        req->header.file_name = strlen(get_req->file_name) > 0 ? get_req->file_name : NULL;
        req->header.route_name = strlen(get_req->route_name) > 0 ? get_req->route_name : NULL;
        req->header.url_path = strlen(get_req->path) > 0 ? get_req->path : NULL;
        req->header.qrs = (get_req->query != NULL) ? get_req->query : NULL;

        if(req->header.qrs!=NULL){
            get_percentile_decoder(req); 
        }

        return 0;
    }
    else
    {
        if (get_req != NULL)
            free(get_req);
    }
    return -1;
}