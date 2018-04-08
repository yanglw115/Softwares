
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "hm_encrypt.h"


static const int IP_Table[64] = { 
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 
	57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 
}; 
static const int IPR_Table[64] = { 
	40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 
	38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 
	34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25 
}; 
static const char S_Box[8][4][16] = { 
	14,	 4,	13,	 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7, 
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8, 
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0, 
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13, 
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10, 
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5, 
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15, 
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9, 
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8, 
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1, 
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7, 
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12, 
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15, 
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9, 
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4, 
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14, 
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9, 
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6, 
	 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14, 
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3, 
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11, 
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8, 
	 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6, 
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13, 
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1, 
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6, 
	 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2, 
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12, 
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7, 
	 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2, 
	 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8, 
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11 
}; 

static const int P_Table[32] = { 
	16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10, 
	2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25 
}; 


static const int PC1_Table[56] = { 
	57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18, 
	10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36, 
	63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22, 
	14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4 
}; 


static const int PC2_Table[48] = { 
	14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10, 
	23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2, 
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 
}; 

static const char LOOP_Table[16] = { 
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 
}; 




void ByteToBit(unsigned char *Byte, bool *Bit, int len) 
{ 
	int i = 0;
	for(i = 0; i < len; i++) 
		Bit[i] = (Byte[i/8]>>(7-i%8))&1; 
} 

void BitToByte(bool *Bit, unsigned char *Byte, int len) 
{ 
	int i ,j;
	for(i= 0; i < 8; i++) 
	{ 
		Byte[i] = 0; 
		for(j = 0; j < 8; j++) 
			Byte[i] = (Byte[i]<<1)|((char)(Bit[j+i*8])); 
	} 
} 

void Transmit(bool *In, bool *Out, const int *Table, int len) 
{ 
	int i = 0;
	for(; i < len; i++) 
		Out[i] = In[Table[i] - 1]; 
} 

void IP_Transmit(bool *Mes, bool *Cry) 
{ 
	Transmit(Mes, Cry, IP_Table, 64); 
} 

void E_function(bool *Mes, bool *Cry) 
{
	int i = 0;
	for(; i < 48; i++) 
		Cry[i] = Mes[(32+i-2*(i/6)-1)%32]; 
} 

void S_function(bool *Mes, bool *Cry) 
{ 
	int r, l; 
	int c; 
	int i, j,s;
	for(i=0, s=0, j=0; s<8; i+=6, s++, j+=4) 
	{ 
		r = Mes[i]*2 + Mes[i+5]; 
		l = Mes[i+1]*8 + Mes[i+2]*4 + Mes[i+3]*2 + Mes[i+4]; 
		c = S_Box[s][r][l]; 
        Cry[j] = ((c&8)>>3 ? 1:0); 
		Cry[j+1] = ((c&4)>>2 ? 1:0);  
		Cry[j+2] = ((c&2)>>1 ? 1:0); 
		Cry[j+3] = ((c&1) ? 1:0); 
	} 
} 

void P_Transmit(bool *Mes, bool *Cry) 
{ 
	Transmit(Mes, Cry, P_Table, 32); 
} 

void Swap(bool *LMes, bool *RMes) 
{ 
	bool *Ptemp = (bool *)malloc(sizeof(bool) * 32);
	int i = 0;
	for(i = 0; i < 32; i++) 
		Ptemp[i] = LMes[i]; 
	for(i = 0; i < 32; i++) 
		LMes[i] = RMes[i]; 
	for(i = 0; i < 32; i++) 
		RMes[i] = Ptemp[i]; 

	free(Ptemp);
} 

void IPR_Transmit(bool *Mes, bool *Cry) 
{ 
	Transmit(Mes, Cry, IPR_Table, 64); 
} 

void Function(bool *Mes, bool *Cry, bool *K) 
{ 
	int i = 0;

	bool *EBits = (bool *)malloc(sizeof(bool)*48);
	bool *SBits = (bool *)malloc(sizeof(bool)*32);
	
    E_function(Mes, EBits); 
	for(i = 0; i < 48; i++) 
		EBits[i] = EBits[i] ^ K[i]; 
    S_function(EBits,SBits); 
	P_Transmit(SBits, Cry); 

	free(EBits);
	free(SBits);
} 


void PC1_Transmit(bool *Key, bool *CDkey) 
{ 
	Transmit(Key, CDkey, PC1_Table, 56); 
} 

void PC2_Transmit(bool *CDkey, bool *SubKey) 
{ 
	Transmit(CDkey, SubKey, PC2_Table, 48); 
} 

void LOOP_LS(bool *CDkey, int Turn) 
{ 
	int t = 0, i;
	for(t = 0; t < LOOP_Table[Turn]; t++) 
	{ 
		bool bit = CDkey[0]; 
		for(i =0; i < 27; i++) 
			CDkey[i] = CDkey[i+1]; 
		CDkey[27] = bit; 
	} 
} 

void CreateSubKey(bool *Key, bool **SubKey) 
{ 
    
	bool *CDKey = (bool *)malloc(sizeof(bool) * 56);
	bool *CKey = (bool *)malloc(sizeof(bool) * 28);
	bool *DKey = (bool *)malloc(sizeof(bool) * 28);
	
	int i = 0;
	PC1_Transmit(Key, CDKey); 
	for(i = 0; i < 28; i++) 
	{ 
		CKey[i] = CDKey[i]; 
		DKey[i] = CDKey[i+28]; 
	} 
	int turn = 0;
	for(turn = 0; turn < 16; turn++) 
	{ 
		LOOP_LS(CKey, turn); 
		LOOP_LS(DKey, turn); 
		for(i = 0; i < 56; i++) 
			if(i < 28) 
				CDKey[i] = CKey[i]; 
			else 
				CDKey[i] = DKey[i-28]; 
		PC2_Transmit(CDKey, SubKey[turn]); 
	}
    
	free(CDKey);
	free(CKey);
	free(DKey);
} 

void DES_Encryption(bool *Mes, bool *Key, bool *Cry) 
{ 
    
	bool *LRCry = (bool*) malloc(sizeof(bool)*64); 
	bool *LCry = (bool*) malloc(sizeof(bool)*32);
	bool *RCry = (bool*) malloc(sizeof(bool)*32);
	bool *FCry = (bool*) malloc(sizeof(bool)*32); 
	bool **SubKey =  (bool**)malloc(sizeof(bool *)*16);

	int p =0;
	for(p = 0; p < 16; p++) 
	{
		SubKey[p] = (bool*) malloc(sizeof(bool)*48); 
	}

    /*

    bool LRCry[64];
    bool LCry[32];
    bool RCry[32];
    bool FCry[32];
    bool SubKey[16][48];
*/	
	CreateSubKey(Key, SubKey); 
    IP_Transmit(Mes, LRCry); 

	int i, turn;
    for(i = 0; i < 32; i++) 
	{ 
		LCry[i] = LRCry[i]; 
		RCry[i] = LRCry[i+32]; 
	} 
	for(turn = 0; turn < 16; turn++) 
	{ 
		Function(RCry, FCry, SubKey[turn]); 
		for(i = 0; i < 32; i++) 
			LCry[i] = LCry[i] ^ FCry[i]; 
        Swap(LCry, RCry); 
	} 
	for(i = 0; i < 64; i++) 
		if(i < 32) 
			LRCry[i] = RCry[i]; 
		else 
			LRCry[i] = LCry[i-32]; 
	IPR_Transmit(LRCry, Cry); 

/*
    */
	free(LRCry);
	free(LCry);
	free(RCry);
	free(FCry);
	for(p = 0; p < 16; p++) 
	{
		free(SubKey[p]); 
	}
	free(SubKey);
} 

void DES_Decryption(bool *Cry, bool *Key, bool *Mes) 
{ 
    
	bool *LRMes = (bool*) malloc(sizeof(bool)*64); 
	bool *LMes = (bool*) malloc(sizeof(bool)*32);
	bool *RMes = (bool*) malloc(sizeof(bool)*32);
	bool *FMes = (bool*) malloc(sizeof(bool)*32); 

	bool **SubKey =  (bool**)malloc(sizeof(bool *)*16); 


	int p, i, turn;
	for(p = 0; p < 16; p++) 
	{

		SubKey[p] = (bool*) malloc(sizeof(bool)*48); 
	}
	CreateSubKey(Key, SubKey); 
    IP_Transmit(Cry, LRMes); 
    for(i = 0; i < 32; i++) 
	{ 
		LMes[i] = LRMes[i]; 
		RMes[i] = LRMes[i+32]; 
	} 
	for(turn = 0; turn < 16; turn++) 
	{ 
		Function(RMes, FMes, SubKey[15-turn]); 
		for(i = 0; i < 32; i++) 
			LMes[i] = LMes[i] ^ FMes[i]; 
        Swap(LMes, RMes); 
	} 
	for(i = 0; i < 64; i++) 
		if(i < 32) 
			LRMes[i] = RMes[i]; 
		else 
			LRMes[i] = LMes[i-32]; 
	IPR_Transmit(LRMes, Mes); 

	free(LRMes);
	free(LMes);
	free(RMes);
	free(FMes);
    
	for(p = 0; p < 16; p++) 
	{
		free(SubKey[p]); 
	}
	free(SubKey);
} 

/*encrypt:flag=true decrypt:flag=false*/
void HM_DES(unsigned char *inData, unsigned char *inKey, unsigned char *out, bool flag)
{
	if ( inData == NULL || inData == NULL || out == NULL ) return;

	bool *DataBits	= (bool*) malloc(sizeof(bool)*64);  
    bool *KeyBits	= (bool*) malloc(sizeof(bool)*64);
    bool *CTBits	= (bool*) malloc(sizeof(bool)*64);
    
			
    ByteToBit(inKey, KeyBits, 64);	
	
	if ( flag )
	{
		ByteToBit(inData, DataBits, 64);	
		DES_Encryption(DataBits, KeyBits, CTBits);
		BitToByte(CTBits, out, 64);	
	}
	else
	{
		ByteToBit(inData, CTBits, 64);	
		DES_Decryption(CTBits, KeyBits, DataBits); 
		BitToByte(DataBits, out, 64);
	}  
	free(DataBits);
	free (KeyBits);
	free (CTBits);
}

void HM_3DES(unsigned char *inData, int inDataLen, unsigned char *inKey, int inKeyLen, unsigned char *out, bool flag)
{
	if ( inKeyLen != 16 || inData == NULL || inKey == NULL || out == NULL ) return;

	int nCount = inDataLen / 8 + ((inDataLen % 8 == 0) ? 0 : 1);

	int i ;
	for (i = 0; i < nCount; ++i )
	{
		HM_DES(inData + i * 8, inKey, out + i * 8, flag);
		HM_DES(out + i * 8, inKey + 8, out + i * 8, !flag);
		HM_DES(out + i * 8, inKey, out + i * 8, flag);
	}
}

void HM_CreateKey(int inKeyLen, unsigned char *out)
{
   
	if ( inKeyLen != 8 && inKeyLen != 16 ) return;

	unsigned char cTable[17] = { "0123456789ABCDEF" };

	unsigned char tmp1, tmp2;
	int i ;
	for ( i = 0; i < inKeyLen * 2; ++i )
	{	
		tmp1 = rand() % 16;	
		
		out[i] = cTable[tmp1];
/*
		if ( i % 2 == 1 )
		{
			out[i/2] = cTable[tmp2] * 16 + cTable[tmp1];
		}
		else
		{
			tmp2 = tmp1;
		}
*/
	}
}

void HM_CreateMasterKey(unsigned char *out)
{
	unsigned char key1[16] = { 0 };
	unsigned char key2[16] = { 0 };
	HM_CreateKey(16, key1);
	HM_CreateKey(16, key2);

	HM_XOR(key1, key2, out, 16);
}

void HM_XOR(unsigned char *input1,unsigned char *input2,unsigned char *output,int len)
{
	if ( input1 == NULL || input2 == NULL || output == NULL )
	{
		return;
	}

	while ( len != 0 ) 
	{
		*output++ = *input1++ ^ *input2++;
		len--;
	}
}

/*
*@brief: 根据输入数据计算MAC，初始IV向量默认为"x00x00x00x00x00x00x00x00"
*@param: inData 输入数据
*@param: inLen 输入数据长度
*@param: inKey 密钥
*@param: out 计算出来的MAC
*@调用自定义HM_XOR和HM_DES函数
*/
void HM_MAC(unsigned char *inData, int inLen, unsigned char *inKey, unsigned char *out)
{
	int i, n, iNum;
	
	unsigned char data[512]		= { 0 };
	unsigned char tmp[20]		= { 0 };
	unsigned char xorData[20]	= { 0 };
	unsigned char desData[20]	= { 0 };
	
	memcpy(data, inData, inLen);
	iNum = inLen % 8;
	if ( iNum == 0 )
	{
		n = inLen / 8;
	}
	else 
	{
		n = inLen / 8 + 1;
	}
	
	for ( i = 0; i < n; ++i )
	{
		memcpy(tmp, data + i * 8, 8);
		
		HM_XOR(desData, tmp, xorData, 8);
		HM_DES(xorData, inKey, desData, 1);
	}

	HM_DES(desData, inKey+8, xorData, 0);
	HM_DES(xorData, inKey, desData, 1);
	
	memcpy(out, desData, 8);	
}

int ASCIIToBCD(const unsigned char *inAsc, int inLen, unsigned char *outBcd)
{
	if ( outBcd == NULL || inAsc == NULL )
	{
		return 0;
	}
	
	char *p = (char *)malloc(sizeof(char) * (inLen + 1));
	memset(p, 0, inLen + 1);
	int	i = 0;
	int j = 0;
	
	for (i = 0; i < inLen; ++i) 
	{
		if( (inAsc[i] >= '0') && (inAsc[i] <= '9') )
		{
			p[i] = inAsc[i] - '0';
		}
		else if( (inAsc[i] >= 'a') && (inAsc[i] <= 'f') )
		{
			p[i] = inAsc[i] - 'a' + 10;
		}
		else if( (inAsc[i] >= 'A') && (inAsc[i] <= 'F') )
		{
			p[i] = inAsc[i] - 'A' + 10;
		}		
		else 
		{
			p[i] = inAsc[i] - '0';
		}
	}
	
	j = (inLen + inLen % 2) / 2;
	if ( inLen % 2 ) 
	{
		p[inLen + 1] = 0;
	}
	
	for ( i = 0; i < j; ++i ) 
	{
		outBcd[i] = (p[2 * i] & 0x0f) << 4;
		outBcd[i] |= (p[2 * i + 1] & 0x0f);
	}
	
	free (p);
	
	return i;
}

int BCDToASCII(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
	if (NULL == pbDest || NULL == pbSrc || nLen == 0)
	{
		return -1;
	}
	char	ddl, ddh;
	int i;

	for (i = 0; i<nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i * 2] = ddh;
		pbDest[i * 2 + 1] = ddl;
	}

	pbDest[nLen * 2] = '\0';
	return 0;
}

void HM_Pinblock(unsigned char *password, int passwordLen, unsigned char *cardNo, 
				 int cardNoLen, unsigned char *key, unsigned char *out)
{
#if 0
	int i;
	unsigned char tmp[16];
	unsigned char pin[8] = { 0 };
	unsigned char pan[8] = { 0 };
	unsigned char data[8] = { 0 };

	memset(tmp, 0x0F, 16);
	tmp[0] = '0';
	tmp[1] = passwordLen + '0';
	for ( i = 0; i < passwordLen; ++i )
	{
		tmp[i + 2] = password[i];
	}
	ASCIIToBCD(tmp, 16, pin);

	memset(tmp, '0', 16);
	if ( cardNoLen <= 12 )
	{
		memcpy(tmp + 4, cardNo, cardNoLen);	
	}
	else
	{
		memcpy(tmp + 4, cardNo + (cardNoLen - 13), 12);
	}
	ASCIIToBCD(tmp, 16, pan);

	HM_XOR(pin, pan, data, 8);
	DUMPDATA(pin, 8);
	DUMPDATA(pan, 8);
	DUMPDATA(data, 8);
	HM_3DES(data, 8, key, 16, out, 1);	
#endif

}
#if 0
int HM_Pinblock_ISO_f3(uint8_t *passwd, int passwd_len,
                            uint8_t card_no,  int card_no_len, 
                            uint8_t *key, uint8_t *out)
{
    int i = 0;
    uint8_t pin[8] = {0};
    uint8_t pan[8] = {0};
    uint8_t data[8] = {0};
    uint8_t tmp[16];
    
    if (passwd_len < 4 || passwd_len > 12)
    {
        return E_INVALID;
    }
    
    memset(tmp, 0xf, 16);
    memcpy(tmp+1, passwd, passwd_len);
    ASCIIToBCD(tmp, 16, pin);
    pin[0] = 3 << 4;
    pin[0] |= (passwd_len & 0xf);

    memset(tmp, '0', 16);
    if (card_no_len <= 12)
    {
        printf("===%d\n", card_no_len);
        memcpy(tmp + 4, card_no, card_no_len);	
    }
    else
    {
        memcpy(tmp + 4, card_no + (card_no_len - 13), 12);
    }
    ASCIIToBCD(tmp, 16, pan);

    HM_XOR(pin, pan, data, 8);

    HM_3DES(data, 8, key, 16, out, 1);
    
}
#endif


/*********************************************************/ 
