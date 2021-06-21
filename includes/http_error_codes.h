#ifndef __MPM__HTTP__EORROR_CODES
#define __MPM__HTTP__ERROR_CODES

typedef struct http_error_codes{

const char *const CODE_503 ;

const char *const CODE_404 ;

const char *const CODE_501 ;

const char *const CODE_502;

const char *const CODE_505;

const char *const CODE_504;

const char *const CODE_506 ;

}MPM_HTTP_ERROR; 

MPM_HTTP_ERROR HTTP_ERROR_CODES ={
	.CODE_503="503",
	.CODE_404="404",
	.CODE_501="501",
        .CODE_502="502",
	.CODE_505="505",
	.CODE_504="504",
	.CODE_506="506",
};
#endif