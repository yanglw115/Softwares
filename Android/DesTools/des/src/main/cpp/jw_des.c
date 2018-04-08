#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hm_encrypt.h"
#include "jw_des.h"


static int des_charcheck(unsigned char *pSrc);
/********************************************************************************
    Function name:des_encryption
	param:
	   unsigned char* pSrc:need des enc's string (ascii)
	   unsigned char* pKey:enc key (enc and dec need the same key)(ascii)
	   unsigned char* pBuf:the data after des enc (output param)(ascii)
********************************************************************************/
int des_encryption(unsigned char* pSrc, unsigned char* pKey, unsigned char* pBuf)  //des 加密
{
	if (NULL == pSrc || NULL == pKey || NULL == pBuf)
	{
		return PARAM_NULL;
	}

	int i = 0;
	int InputdateLen = strlen(pSrc);
	int keyLen = strlen(pKey);
    int iLoop = 0;
    int iDes = 0;
    unsigned char oSet[8] = {0};
    unsigned char* Dst = NULL;
	unsigned char oBuf[64] = { 0 };
    
    if (keyLen != 8)
    {
        return KEY_LEN_ERROR;
    }

    iLoop = InputdateLen / 8;
    iDes = InputdateLen % 8;

    if (0 != iDes)
    {
        InputdateLen = (iLoop+1)*8;
    }

    Dst = malloc(InputdateLen + 1);

    if (Dst == NULL)
    {
        return MALLOC_ERROR;
    }
//	memset(BCD_Dst, 0, BCD_Len + 1);

	//ascii string to BCD
//	ASCIIToBCD(pSrc, InputdateLen, BCD_Src);
//	ASCIIToBCD(pKey, KEY_MAX_LEN, BCD_Key);

	//des enc
	for (i = 0; i < iLoop; i++)
	{
		memset(oBuf, 0, 64);
        HM_DES(pSrc + i * 8, pKey, oBuf, 1);
		memcpy(Dst + i * 8, oBuf, 8);
	}

    if (0 != iDes)
    {
		memset(oBuf, 0, 64);
        memset(oSet, InputdateLen - strlen(pSrc), 8);
        memcpy(oSet, pSrc + iLoop * 8, iDes);        
        HM_DES(oSet, pKey, oBuf, 1);
		memcpy(Dst+ iLoop * 8, oBuf, 8);        
    }
    
	//end hex value in BCD_dst
    BCDToASCII(pBuf, Dst, InputdateLen);
//	free(BCD_Dst);
    free(Dst);
	return 0;
}

static int des_charcheck(unsigned char *pSrc)
{
	int InputdateLen = strlen(pSrc);
	int iLoop = 0;
	
	for(;iLoop < InputdateLen; iLoop++)
	{
		if (0 == isxdigit(pSrc[iLoop]))
		{
			return DECODE_NOT_HEX;
		}
	}
	
	return 0;
}

/********************************************************************************
      Function name:des_encryption
      param:
        unsigned char* pSrc:need des dec's string (ascii)
        unsigned char* pKey:dec key (enc and dec need the same key)(ascii)
        unsigned char* pBuf:the data after des dec (output param)(ascii)
********************************************************************************/
int des_decryption(unsigned char* pSrc, unsigned char* pKey, unsigned char* pBuf)  //des 解密
{
	if (NULL == pSrc || NULL == pKey || NULL == pBuf)
	{
		return PARAM_NULL;
	}

	int keyLen = strlen(pKey);
	if (keyLen != 8)
	{
		return KEY_LEN_ERROR;
	}
	
	if (des_charcheck(pSrc))
	{
		return DECODE_NOT_HEX;
	}
	
	int i = 0;
	int InputdateLen = strlen(pSrc);
	int BCD_Len = 0;
	
	if (InputdateLen % MAX_LEN_DES_ENC != 0)
	{
		//after_fill_count = InputdateLen / MAX_LEN_DES_ENC + 1; //question
		InputdateLen = (InputdateLen / MAX_LEN_DES_ENC + 1) * 16;
	}
	BCD_Len = InputdateLen / 2;
	

	// param must Multiple of 16

	unsigned char oBuf[64] = { 0 };
	unsigned char BCD_Key[16] = { 0 };
	unsigned char* BCD_Src = malloc(BCD_Len + 1);
	unsigned char* BCD_Dst = malloc(BCD_Len + 1);
	if (NULL == BCD_Src || NULL == BCD_Dst)
	{
		return MALLOC_ERROR;
	}
	memset(BCD_Src, 0, BCD_Len + 1);
	memset(BCD_Dst, 0, BCD_Len + 1);

	//ascii string to BCD
	ASCIIToBCD(pSrc, InputdateLen, BCD_Src);
//	ASCIIToBCD(pKey, KEY_MAX_LEN, BCD_Key);

	int loop_times = BCD_Len / 8;
	//des enc
	for (i = 0; i < loop_times; i++)
	{
		memset(oBuf, 0, 64);
		HM_DES(BCD_Src + i * 8, pKey, oBuf, 0);
		memcpy(BCD_Dst + i * 8, oBuf, 8); 
	}
	//end hex value in BCD_dst
//	BCDToASCII(pBuf, BCD_Dst, BCD_Len);
    for (i = 0; i < BCD_Len; ++i) {
        if (BCD_Dst[i] < 8) {
            BCD_Dst[i] = 0;
        }
    }
    strncpy(pBuf, BCD_Dst, strlen(BCD_Dst));
	free(BCD_Dst);
	free(BCD_Src);
	return 0;
}
