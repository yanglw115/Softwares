#ifndef _YG_DES_SOURCE_H_
#define _YG_DES_SOURCE_H_

/***************************************************************
 *            Name : DES(Data Ecryption Standard) C Function   *
 *         Version : 0.2                                       *
 *            Type : Freeware                                  *
 *      OS/Dev tool: Dos/Turbo C & Linux/GNU cc                *
 *      Progeammer : Atan                                      *
 *            Date : 1998-03-03                                *
 ***************************************************************
 *                        [ Useage ]                           *
 * encryptDES(*Uncryption data,*password,*Encryption data)     *
 * decryptDES(*Encryption date,*password,*Uncryption data)     *
 *                        [  NOTE  ]                           *
 * 1.Uncryption is must = 8byte(64bit)                         *
 * 2.Encryption is must = 8byte(64bit)                         *
 * 3.Password(key) is must = 8byte(64bit)                      *
 * 4.Please put a head file include follow function prototypes *
 * 5.If you have a big file or data block , you can call endes *
 *   and undes function many times by 8 byte data.             *
 * 6.Good luck!                                                *
 ***************************************************************/

#ifdef __cplusplus
extern "C" {

/*function prototypes*/
void encryptDES(unsigned char m_bit[8], unsigned char k_bit[8], unsigned char e_bit[8]);
void decryptDES(unsigned char m_bit[8] , unsigned char k_bit[8] , unsigned char e_bit[8]);

static void byteToBit(unsigned char byte[64], unsigned char bit[8]);
static void bitToByte(unsigned char bit[8], unsigned char byte[64]);
static void generateRoundKey(unsigned char oldkey[8], unsigned char newkey[16][8]);
static void boxSReplace(unsigned char s_bit[8]);
/*function prototypes*/

}
#endif // __cplusplus

#endif // _YG_DES_SOURCE_H_
