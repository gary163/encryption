/* encryption extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_encryption.h"
#include <string.h>
#include "yiencrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include "hmac_sha1.h"
#include "base64.h" 
#include <time.h>

/* {{{ void yi_decode()
 */
PHP_FUNCTION(yi_decode)
{
	char *in     = NULL;
	size_t lenIn = 0;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(in, lenIn)
	ZEND_PARSE_PARAMETERS_END();

    char buff[lenIn+1];
    strcpy(buff,in);

    unsigned char *out = decode(buff);
    if(out == NULL){
       return; 
    } 
    RETVAL_STRING(out);

    if(out != NULL){
        free(out);
        out = NULL;
    }
    return;
}

/* }}} */

/* {{{ string yi_encode( [ string $var ] )
 */
PHP_FUNCTION(yi_encode)
{
	char *in     = NULL;
    size_t lenIn = 0 ;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(in,lenIn)
	ZEND_PARSE_PARAMETERS_END();

    unsigned char *out = encode(in);
    RETVAL_STRING(out);
    if(out != NULL){
        free(out);
        out = NULL;
    }
	return;
}


const unsigned char BYTEMAP[256] = 
{
    0xC7,0xD0,0xBB,0xC8,0xA2,0x08,0xF3,0xD1,0x73,0xF4,0x48,0x2D,0xAE,0x96,0x35,0xB5,
    0x0D,0x36,0xC4,0x39,0x74,0xBF,0x23,0x16,0x14,0x06,0xEB,0x04,0x3E,0x9B,0x0B,0xD4,
    0x12,0x5C,0x61,0x63,0x8B,0xBC,0xF6,0xA5,0xE1,0x65,0xD8,0x07,0xF0,0x13,0xF5,0x5A,
    0xF2,0x20,0x6B,0x4A,0x24,0x59,0x89,0x64,0xD7,0x42,0x6A,0x5E,0x3D,0x0A,0x77,0xE0,
    0x80,0x27,0xB8,0xC5,0x8C,0x0E,0xFA,0x8A,0xD5,0x29,0x56,0x57,0x6C,0x53,0x67,0x41,
    0x00,0x1A,0x4D,0xCE,0x83,0xB0,0x22,0x28,0x3F,0x26,0x46,0x4F,0x6F,0x2B,0xE8,0xE9,
    0x2F,0x40,0x5F,0x44,0x8E,0x6E,0x45,0x7E,0xAB,0x2C,0x70,0x1F,0xB4,0xAC,0x9D,0x91,
    0x72,0x3A,0xF1,0x97,0x95,0x49,0x84,0xE5,0xE3,0x79,0x8F,0x51,0x10,0x8D,0xA8,0x82,
    0xC6,0xDD,0xFF,0xFC,0xE4,0xCF,0xB3,0x09,0x5D,0xEA,0x9C,0x34,0xF9,0x17,0x9F,0xDA,
    0xDC,0xA7,0x50,0x4B,0x94,0xC0,0x92,0x4C,0x11,0x5B,0x78,0xD9,0xB1,0xED,0x19,0xD2,
    0x87,0x15,0x05,0x3C,0x85,0x2E,0xFB,0xEE,0x47,0x3B,0xEF,0x37,0x7F,0xD3,0xA4,0xF8,
    0x93,0xAF,0x69,0x71,0x31,0x75,0xA0,0xAA,0xBA,0x7C,0x38,0x02,0xB7,0xDE,0x21,0x0C,
    0x81,0x01,0xFD,0xE7,0x1D,0xCC,0xBD,0x1B,0x7A,0x2A,0xAD,0x66,0xBE,0x55,0x33,0xCD,
    0x03,0xB2,0x1E,0x4E,0xB9,0xE6,0xC2,0xF7,0xCB,0x7D,0xC9,0x62,0xC3,0xA6,0x88,0xDB,
    0xA1,0x1C,0xB6,0x32,0x99,0x7B,0x6D,0x9A,0x30,0xD6,0xA9,0x25,0xA3,0x76,0x9E,0x86,
    0x90,0xCA,0xE2,0x58,0xC1,0x18,0x52,0xFE,0xDF,0x68,0x98,0x54,0xEC,0x60,0x43,0x0F
};

const char *KEY = "icUeFXB7mLNlLIlscaSVjg==";


char * encode(char *in)
{
    if(in == NULL || strlen(in) == 0){
        return NULL; 
    }
    size_t lenIn = strlen(in);
    char buff[2] = {0}; //seed 十六进制
    size_t lenBuff = sizeof(buff);

    srand((unsigned)time(NULL));
    unsigned char seed = rand() % 256;
    //seed = 177;
    unsigned char inBuff[lenIn+1];
    int i;
    for(i=0;i<lenIn;i++){
        unsigned char ascii = (unsigned char )in[i];
        unsigned char pos = 0;
        int j;
        for(j=0;j<256;j++){
            if(BYTEMAP[j] == ascii){
                pos = j;
                break; 
            } 
        } 
        inBuff[i] = pos ^ seed;
    }
    inBuff[lenIn] = 0;
    sprintf(buff,"%x",seed);

    //sha1 for in
    unsigned char digest[21] = {0};
    size_t lenDigest = sizeof(digest);
    hmac_sha1(KEY,strlen(KEY),in,lenIn,digest,&lenDigest);
    char * base64Digest = (char *)malloc(BASE64_ENCODE_OUT_SIZE(lenDigest));
    size_t lenBase64Digest = base64_encode(digest, lenDigest, base64Digest);

    unsigned char *base64In = (unsigned char*)malloc(BASE64_ENCODE_OUT_SIZE(lenIn));
    int lenBase64In = base64_encode(inBuff,lenIn,base64In);

    size_t lenTotal = lenBase64Digest + lenBase64In + lenBuff + 2 + 1;//2为2个&的长度,1为最后一个字符\0
    unsigned char *out = (unsigned char *)malloc(lenTotal);
    sprintf(out,"%s&%s&%s",buff,base64Digest,base64In);

    free(base64Digest); 
    free(base64In);
    return out;
}

char * decode(char in[])
{
    if(strlen(in) == 0 || in == NULL){
        return NULL; 
    }
    const char *delims = "&";
    unsigned char *s = NULL;
    unsigned char *sig = NULL;
    unsigned char *str = NULL;
    unsigned char *result = NULL;

    result = strtok(in,delims);
    int i = 0;
    while(result != NULL){
        if(i == 0) s = result;
        if(i == 1) sig = result;
        if(i == 2) str = result;
        result = strtok(NULL,delims);
        i ++; 
    }
    if(s == NULL || sig == NULL || str == NULL){
        return NULL;
    }

    unsigned char seed = 0; 
    sscanf(s,"%x",&seed);
    unsigned char *destr = (unsigned char*)malloc(BASE64_DECODE_OUT_SIZE(strlen(str)));
    size_t lenData = base64_decode(str, strlen(str), destr);

    unsigned char data[lenData+1];
    for(i=0; i<lenData; i++){
        unsigned char pos = (unsigned char) destr[i] ^ seed;
        data[i] =BYTEMAP[pos]; 
    }
    data[lenData] = 0; 
    free(destr); 

    unsigned char digest[21] = {0};
    size_t lenDigest = sizeof(digest);
    hmac_sha1(KEY,strlen(KEY),data,strlen(data),digest,&lenDigest);
    unsigned char *dsig = (unsigned char*) malloc(BASE64_DECODE_OUT_SIZE(strlen(sig)));
    base64_decode(sig,strlen(sig),dsig);
    if(strcmp(dsig,digest) != 0){
        free(dsig); 
        return NULL; 
    }
    free(dsig); 

    unsigned char *out = (unsigned char*) malloc(strlen(data)+1); 
    strcpy(out,data);
    return out;
}

/* }}}*/

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(encryption)
{
#if defined(ZTS) && defined(COMPILE_DL_ENCRYPTION)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(encryption)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "encryption support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_yi_decode, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_yi_encode, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ encryption_functions[]
 */
static const zend_function_entry encryption_functions[] = {
	PHP_FE(yi_decode,		arginfo_yi_decode)
	PHP_FE(yi_encode,		arginfo_yi_encode)
	PHP_FE_END
};
/* }}} */

/* {{{ encryption_module_entry
 */
zend_module_entry encryption_module_entry = {
	STANDARD_MODULE_HEADER,
	"encryption",					/* Extension name */
	encryption_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(encryption),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(encryption),			/* PHP_MINFO - Module info */
	PHP_ENCRYPTION_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ENCRYPTION
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(encryption)
#endif

