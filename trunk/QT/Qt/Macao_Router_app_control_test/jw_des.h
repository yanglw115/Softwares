#ifndef __HEAD__DES__
#define __HEAD__DES__

#define KEY_MAX_LEN     16
#define MAX_LEN_DES_ENC 16

/*return value*/
#define PARAM_NULL    50
#define KEY_LEN_ERROR 51
#define MALLOC_ERROR  52
#define DECODE_NOT_HEX 53

/*end return value*/

#ifdef __cplusplus
extern "C" {
#endif

int des_encryption(unsigned char* pSrc,unsigned char* pKey, unsigned char* pBuf);  //des 加密
int des_decryption(unsigned char* pSrc,unsigned char* pKey, unsigned char* pBuf);  //des 解密


#ifdef __cplusplus
}
#endif

#endif
