#ifndef __HM_ENCRYPT_H__
#define __HM_ENCRYPT_H__

#ifndef bool
#define bool	char
#endif

#ifdef __cplusplus
extern "C" {
#endif

void ByteToBit(unsigned char *Byte, bool *Bit, int len);	

void BitToByte(bool *Bit, unsigned char *Byte, int len);	

void Transmit(bool *In, bool *Out, const int *Table, int len);
 
void IP_Transmit(bool *Mes, bool *Cry);

void E_function(bool *Mes, bool *Cry);	

void S_function(bool *Mes, bool *Cry);

void P_Transmit(bool *Mes, bool *Cry);	

void Swap(bool *LMes, bool *RMes);

void IPR_Transmit(bool *Mes, bool *Cry);

void Function(bool *Mes, bool *Cry, bool *K);

void PC1_Transmit(bool *Key, bool *CDkey);	

void PC2_Transmit(bool *CDkey, bool *SubKey);

void LOOP_LS(bool *CDkey, int Turn);	

void CreateSubKey(bool *Key, bool **SubKey);	

void DES_Encryption(bool *Mes, bool *Key, bool *Cry);
 
void DES_Decryption(bool *Cry, bool *Key, bool *Mes);	

int ASCIIToBCD(const unsigned char *inAsc, int inLen, unsigned char *outBcd);
int BCDToASCII(unsigned char *pbDest, unsigned char *pbSrc, int nLen);

/*tangyuan 20130701*/
void HM_XOR(unsigned char *input1,unsigned char *input2,unsigned char *output,int len);
void HM_DES(unsigned char *inData, unsigned char *inKey, unsigned char *out, bool flag);
void HM_3DES(unsigned char *inData, int inDataLen, unsigned char *inKey, 
			 int inKeyLen, unsigned char *out, bool flag);
void HM_CreateKey(int inKeyLen, unsigned char *out);
void HM_CreateMasterKey(unsigned char *out);
void HM_MAC(unsigned char *inData, int inLen, unsigned char *inKey, unsigned char *out);
void HM_Pinblock(unsigned char *password, int passwordLen, unsigned char *cardNo, 
				 int cardNoLen, unsigned char *key, unsigned char *out);

int S_PinToCCBS(char *cardno,char *pinkey_lmk,char *src_pin,char *nodeid,char *dst_pin);
int S_TmkGen(char *mcc ,char *termed,char *tmk_lmk);
int S_WkeyGen(char *tmk_lmk,char *pinkey_tmk,char *mackey_tmk,char *pinkey_lmk,char *mackey_lmk);
int  MacCheck(int opflag, char *macbuf ,int macbuflen,char *Mackey_lmk,char *mac);
int S_PinToText(char *cardno, char *pinkey_lmk, char *src_pin ,char *dst_pin, int *pinlen);

#ifdef __cplusplus
}
#endif

#endif
