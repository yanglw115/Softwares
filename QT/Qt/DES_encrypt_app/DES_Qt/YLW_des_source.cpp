
#include <QtDebug>
#include <QFile>
#include <QCryptographicHash>

#include "YLW_des_source.h"

#include "YLW_VS_char_set.h"

#ifdef __cplusplus
extern "C" {

/*function prototypes*/
static void encryptDES(unsigned char m_bit[8], unsigned char k_bit[8], unsigned char e_bit[8]);
static void decryptDES(unsigned char m_bit[8] , unsigned char k_bit[8] , unsigned char e_bit[8]);

static void byteToBit(unsigned char byte[64], unsigned char bit[8]);
static void bitToByte(unsigned char bit[8], unsigned char byte[64]);
static void generateRoundKey(unsigned char oldkey[8], unsigned char newkey[16][8]);
static void boxSReplace(unsigned char s_bit[8]);
/*function prototypes*/

}
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {

/**
 * byteToBit
 * change 64byte to 64bit(8byte)
 */
void byteToBit(unsigned char byte[64], unsigned char bit[8])
{
    int i = 0;
    for (i = 0; i <= 7; ++i) {
        if (byte[0 + 8 * i] == 0x1) bit[i] = bit[i] | 0x80;
        else bit[i] = bit[i] & 0x7f;
        if (byte[1 + 8 * i] == 0x1) bit[i] = bit[i] | 0x40;
        else bit[i] = bit[i] & 0xbf;
        if (byte[2 + 8 * i] == 0x1) bit[i] = bit[i] | 0x20;
        else bit[i] = bit[i] & 0xdf;
        if (byte[3 + 8 * i] == 0x1) bit[i] = bit[i] | 0x10;
        else bit[i] = bit[i] & 0xef;
        if (byte[4 + 8 * i] == 0x1) bit[i] = bit[i] | 0x08;
        else bit[i] = bit[i] & 0xf7;
        if (byte[5 + 8 * i] == 0x1) bit[i] = bit[i] | 0x04;
        else bit[i] = bit[i] & 0xfb;
        if (byte[6 + 8 * i] == 0x1) bit[i] = bit[i] | 0x02;
        else bit[i] = bit[i] & 0xfd;
        if (byte[7 + 8 * i] == 0x1) bit[i] = bit[i] | 0x01;
        else bit[i] = bit[i] & 0xfe;
    }
} /* end of byte2bit */

/**
 * bitToByte
 * change 64bit(8byte) to 64byte
 */
void bitToByte(unsigned char bit[8], unsigned char byte[64])
{
    int i = 0 ;

    for (i = 0; i <= 63; ++i) {
        byte[i] = 0x0;
    }
    for (i = 0; i <= 7; ++i) {
        if ((bit[i] & 0x80) == 0x80) byte[i * 8 + 0] = 0x01;
        if ((bit[i] & 0x40) == 0x40) byte[i * 8 + 1] = 0x01;
        if ((bit[i] & 0x20) == 0x20) byte[i * 8 + 2] = 0x01;
        if ((bit[i] & 0x10) == 0x10) byte[i * 8 + 3] = 0x01;
        if ((bit[i] & 0x08) == 0x08) byte[i * 8 + 4] = 0x01;
        if ((bit[i] & 0x04) == 0x04) byte[i * 8 + 5] = 0x01;
        if ((bit[i] & 0x02) == 0x02) byte[i * 8 + 6] = 0x01;
        if ((bit[i] & 0x01) == 0x01) byte[i * 8 + 7] = 0x01;
    }
} /* end of bit2byte */

/**
 * generateRoundKey: 生成16轮迭代所用的轮密钥
 * PC: permutation choice
 */
void generateRoundKey(unsigned char keyOld[8], unsigned char keyNew[16][8])
{
    int i = 0, j = 0, k = 0;
    /* 56位数据置换表(可以替换成自己的) */
    int PC1[56] = { 27, 31, 38, 50, 16, 19, 58, 24,
                    44,  7, 63, 54, 40, 62, 46, 10,
                    41, 29, 15, 39, 55, 12, 30,  9,
                    42, 49, 17,  3, 22, 47,  8, 48,
                     5, 33, 11, 20, 37, 21, 60, 28,
                    23, 32, 51, 61, 13, 36, 14,  2,
                    25, 43, 59, 56,  6, 34,  1, 26 };
    /* 48位数据置换表(可以替换成自己的) */
    int PC2[48] = { 12, 22, 48, 54, 36, 53,
                    37, 26,  2, 19, 16,  3,
                    33, 43, 18, 10,  1,  6,
                    15, 38, 30, 44, 21, 25,
                    28, 20, 24, 52, 46, 42,
                    23,  9, 17, 47, 39, 40,
                     7,  8, 13,  5, 56, 14,
                    45, 31, 50, 34, 55, 29 };

    /* 每轮迭代时循环左移的位数 */
    int moveBit[16] = {2, 2, 1, 1, 2, 2, 1, 2, 1, 1, 2, 2, 1, 2, 2, 1};
    unsigned char keyOldByte[64] = {0};
    unsigned char keyOldByte1[64] = {0};
    unsigned char keyOldByte2[64] = {0};
    unsigned char keyOldC[28] = {0};
    unsigned char keyOldD[28] = {0};
    unsigned char temp;
    unsigned char keyNewByte[16][64] = {{0}};

    /** change to byte
     * 将8*8位转换为64个字节代替,这里是方便操作计算 **/
    bitToByte(keyOld, keyOldByte);

    /** 根据表PC1进行置换,并生成56位密钥 **/
    for (i = 0; i <= 55; ++i) keyOldByte1[i] = keyOldByte[PC1[i] - 1];
    /** 生成的56位密钥等分为2份 **/
    for (i = 0; i <= 27; ++i) keyOldC[i] = keyOldByte1[i];
    for (i = 28; i <= 55; ++i) keyOldD[i -28] = keyOldByte1[i];

    /** 一共16次迭代
     * 每次迭代:分别对两份28位密钥进行循环左移,左移多少位依据moveBit **/
    for (i = 0; i <= 15; ++i) {
        /* 或者移位1次,或者移位2次,这里的moveBit[i]或为1,或为2 */
        for (j = 1; j <= moveBit[i]; ++j) {
            /** 前28位密钥进行循环左移一位 **/
            temp = keyOldC[0];
            for (k = 0; k <= 26; ++k) {
                keyOldC[k] = keyOldC[k+1];
            }
            keyOldC[27] = temp;

            /** 后28位密钥进行循环左移一位 **/
            temp = keyOldD[0];
            for (k = 0; k <= 26; ++k) {
                keyOldD[k] = keyOldD[k+1];
            }
            keyOldD[27] = temp;
        }
        /* 将移位后的两个28位密钥合并 */
        for (k = 0; k <= 27; ++k) keyOldByte2[k] = keyOldC[k];
        for (k = 28; k <= 55; ++k) keyOldByte2[k] = keyOldD[k-28];
        /* 将合并后的密钥由56位压缩为48位,同时根据表PC2进行置换,由此生成第i轮密钥 */
        for (k = 0; k <= 47; ++k) keyNewByte[i][k] = keyOldByte2[PC2[k] - 1];
    }
    /** 将生成的16组密钥由byte转换为bit,便于后续计算 **/
    for (i = 0; i <= 15; ++i) byteToBit(keyNewByte[i], keyNew[i]);
}

/****************************************************
 * data encryption                                  *
 * IP: Initalization Permutation                    *
 ****************************************************/
void encryptDES(unsigned char inDataBit[8], unsigned char inKeyBit[8], unsigned char outDataBit[8])
{
    /* 初始置换表(暂时不可以替换成自己的,但可与IP1互换) */
    int IP[64] = {  40,  8, 48, 16, 56, 24, 64, 32,
                    39,  7, 47, 15, 55, 23, 63, 31,
                    38,  6, 46, 14, 54, 22, 62, 30,
                    37,  5, 45, 13, 53, 21, 61, 29,
                    36,  4, 44, 12, 52, 20, 60, 28,
                    35,  3, 43, 11, 51, 19, 59, 27,
                    34,  2, 42, 10, 50, 18, 58, 26,
                    33,  1, 41,  9, 49, 17, 57, 25
                   };

    /* 逆初始置换表 */
    int IP1[64] = {  58, 50, 42, 34, 26, 18, 10,  2,
                    60, 52, 44, 36, 28, 20, 12,  4,
                    62, 54, 46, 38, 30, 22, 14,  6,
                    64, 56, 48, 40, 32, 24, 16,  8,
                    57, 49, 41, 33, 25, 17,  9,  1,
                    59, 51, 43, 35, 27, 19, 11,  3,
                    61, 53, 45, 37, 29, 21, 13,  5,
                    63, 55, 47, 39, 31, 23, 15,  7
                 };

    /* 扩展置换表,由32位扩展为48位(可以替换成自己的) */
    int EP[48] = {   7, 19, 29, 21, 32,  2,
                    20, 28,  8, 13, 26, 30,
                    27,  3, 23, 11, 25, 31,
                     6, 15,  9,  1, 12, 24,
                    17, 18, 14, 10,  4, 16,
                     5, 22, 11, 22, 23, 10,
                    26,  3, 29,  9, 20, 14,
                    21,  2, 17, 12,  8,  1
                };

    unsigned char inDataBitPermuted[8] = {0};
    unsigned char inDataByte[64] = {0};
    unsigned char inDataBytePermuted[64] = {0};
    unsigned char keyRound16[16][8] = {{0}};
    unsigned char leftDataBit[17][8] = {{0}};
    unsigned char rightDataBit[17][8] = {{0}};
    unsigned char outDataByte[64] = {0};
    unsigned char outDataBytePermuted[64] = {0};
    unsigned char rightDataByte[64] = {0};
    unsigned char rightDataByteEPermuted[64] = {0};
    int i = 0, j = 0;

    /* 生成48位的密钥组,共16组(16行 x 6byte in keyRound16) */
    generateRoundKey(inKeyBit, keyRound16);
    /* 比特转换成字节 */
    bitToByte(inDataBit, inDataByte);
    /* 将输入数据根据IP表进行置换 */
    for (i = 0; i <= 63; ++i) inDataBytePermuted[i] = inDataByte[IP[i] - 1];
    /* 字节转换成比特 */
    byteToBit(inDataBytePermuted, inDataBitPermuted);
    /* 将置换后的原始数据分组为左右2等份,因为要进行16轮迭代,所以会最终生成16组这样的数据 */
    for (i = 0; i <= 3; ++i) leftDataBit[0][i] = inDataBitPermuted[i];
    for (i = 4; i <= 7; ++i) rightDataBit[0][i - 4] = inDataBitPermuted[i];

    /** 16轮迭代的过程
     * 迭代过程中主要对右值进行处理,左值用上一轮的右值 **/
    for (i = 1; i <= 16; ++i) {
        /* L(n) = R(n-1): 第N轮迭代的左值等于上一轮迭代的右值 */
        for (j = 0; j <= 3; ++j) leftDataBit[i][j] = rightDataBit[i-1][j];

        /* 比特转换为字节,便于后面的计算 */
        bitToByte(rightDataBit[i-1], rightDataByte);
        /* 将32位的右值进行扩展置换,便于后续与48位的密钥进行异或运算 */
        for (j = 0; j <= 47; ++j) rightDataByteEPermuted[j] = rightDataByte[EP[j] - 1];
         /* 字节转换为比特 */
        byteToBit(rightDataByteEPermuted, rightDataBit[i-1]);

        /* 右值与密钥进行异或运算后得到新的右值 */
        for (j = 0; j <= 5; ++j) rightDataBit[i-1][j] = rightDataBit[i-1][j] ^ keyRound16[i-1][j];

        /* 将计算后的48位右值通过S盒运算得到32位的右值 */
        boxSReplace(rightDataBit[i-1]);
        /* 最后将32位的右值与32位的左值进行异或运算,从而得到本轮最终的右值 */
        for (j = 0; j <= 3; ++j) {
            rightDataBit[i][j] = leftDataBit[i-1][j] ^ rightDataBit[i-1][j];
        }
    }

    /** rightDataBit + leftDataBit -> outDataBit(64bit)
     * 迭代完成之后,将32位的左右值组合在一起 **/
    for (i = 0; i <= 3; ++i) outDataBit[i] = rightDataBit[16][i];
    for (i = 4; i <= 7; ++i) outDataBit[i] = leftDataBit[16][i - 4];

    bitToByte(outDataBit, outDataByte);
    /* 根据逆置换表对加密后的数据再进行一次逆置换 */
    for (i = 0; i <= 63; ++i) outDataBytePermuted[i] = outDataByte[IP1[i] - 1];
    byteToBit(outDataBytePermuted, outDataBit);

} /** 加密结束 **/


/****************************************************
 * data uncryption                                  *
 * IP: Initalization Permutation                    *
 * NOTE: in fact, inDataBit is encryption data,     *
 * outDataBit is uncryption                         *
 ****************************************************/
void decryptDES(unsigned char inDataBit[8] , unsigned char inKeyBit[8] , unsigned char outDataBit[8])
{
    /* 初始置换表(暂时不可以替换成自己的,还不清楚原因,但可与IP1互换) */
    int IP[64] = {  40,  8, 48, 16, 56, 24, 64, 32,
                    39,  7, 47, 15, 55, 23, 63, 31,
                    38,  6, 46, 14, 54, 22, 62, 30,
                    37,  5, 45, 13, 53, 21, 61, 29,
                    36,  4, 44, 12, 52, 20, 60, 28,
                    35,  3, 43, 11, 51, 19, 59, 27,
                    34,  2, 42, 10, 50, 18, 58, 26,
                    33,  1, 41,  9, 49, 17, 57, 25
                   };

    /* 逆初始置换表 */
    int IP1[64] = { 58, 50, 42, 34, 26, 18, 10,  2,
                    60, 52, 44, 36, 28, 20, 12,  4,
                    62, 54, 46, 38, 30, 22, 14,  6,
                    64, 56, 48, 40, 32, 24, 16,  8,
                    57, 49, 41, 33, 25, 17,  9,  1,
                    59, 51, 43, 35, 27, 19, 11,  3,
                    61, 53, 45, 37, 29, 21, 13,  5,
                    63, 55, 47, 39, 31, 23, 15,  7
                 };

    /* 扩展置换表,由32位扩展为48位.(可以替换成自己的) */
    int EP[48] = {   7, 19, 29, 21, 32,  2,
                    20, 28,  8, 13, 26, 30,
                    27,  3, 23, 11, 25, 31,
                     6, 15,  9,  1, 12, 24,
                    17, 18, 14, 10,  4, 16,
                     5, 22, 11, 22, 23, 10,
                    26,  3, 29,  9, 20, 14,
                    21,  2, 17, 12,  8,  1
                };

    unsigned char inDataBitPermuted[8] = {0};
    unsigned char inDataByte[64] = {0};
    unsigned char inDataBytePermuted[64] = {0};
    unsigned char keyRound16[16][8] = {{0}};
    unsigned char leftDataBit[17][8] = {{0}};
    unsigned char rightDataBit[17][8] = {{0}};
    unsigned char outDataByte[64] = {0};
    unsigned char outDataBytePermuted[64] = {0};
    unsigned char leftDataByte[64] = {0};
    unsigned char leftDataBytePermuted[64] = {0};
    int i = 0, j = 0 ;

    /* 生成48位的密钥组,共16组(16行 x 6byte in keyRound16) */
    generateRoundKey(inKeyBit, keyRound16);

    /* 比特转换成字节 */
    bitToByte(inDataBit, inDataByte);
    /* 将输入数据根据IP表进行置换 */
    for (i = 0; i <= 63; ++i) inDataBytePermuted[i] = inDataByte[IP[i] - 1];
    /* 字节转换成比特 */
    byteToBit(inDataBytePermuted, inDataBitPermuted);
    /** 将置换后的原始数据分组为左右2等份,因为要进行16轮迭代,所以会最终生成16组这样的数据.
     * 注意这里使用一维下标的顺序与加密是反的,先从16开始 **/
    for (i = 0; i <= 3; ++i) rightDataBit[16][i] = inDataBitPermuted[i];
    for (i = 4; i <= 7; ++i) leftDataBit[16][i - 4] = inDataBitPermuted[i];

    /** 16轮迭代的过程: 与加密过程类似,但有些是反的,这样才能保证解密正确
     * 迭代过程中主要对左值进行处理,右值用上一轮的左值,这与加密过程不同或相反 **/
    for (i = 16; i >= 1; --i) {
        for (j = 0; j <= 3; ++j) rightDataBit[i - 1][j] = leftDataBit[i][j];

        bitToByte(leftDataBit[i], leftDataByte);
        /* 扩展置换 */
        for (j = 0; j <= 47; ++j) leftDataBytePermuted[j] = leftDataByte[EP[j] - 1];

        byteToBit(leftDataBytePermuted, leftDataBit[i]);
        /* 左值与密钥进行异或运算 */
        for (j = 0; j <= 5; ++j) leftDataBit[i][j] = leftDataBit[i][j] ^ keyRound16[i-1][j];
        /* 通过S盒运算将左值由48位转为32位 */
        boxSReplace(leftDataBit[i]);
        /* 通过L=L^R等到新的左值 */
        for (j = 0; j <= 3; ++j) {
            leftDataBit[i-1][j] = rightDataBit[i][j] ^ leftDataBit[i][j];
        }
    }
    /** 左值与右值再重新组合在一起 **/
    for (i = 0; i <= 3; ++i) outDataBit[i] = leftDataBit[0][i];
    for (i = 4; i <= 7; ++i) outDataBit[i] = rightDataBit[0][i - 4];

    bitToByte(outDataBit, outDataByte);
    /* 根据逆置换表对加密后的数据再进行一次置换 */
    for (i = 0; i <= 63; ++i) outDataBytePermuted[i] = outDataByte[IP1[i] - 1];
    byteToBit(outDataBytePermuted, outDataBit);

} /* 解密结束 */

/** S盒运算,将48位的数据转换为32位数据 **/
void boxSReplace(unsigned char inBit[8])
{
    /* 初始置换表(可以替换成自己的) */
    int IP[32] =
        {  6, 13, 32,  3,
          25, 31, 23, 14,
           4, 27, 21, 24,
          26,  8, 17, 20,
          28,  7, 11, 16,
          12,  9, 29, 30,
          22,  5, 18,  1,
          15, 10, 19,  2
        };
    /* S盒置换所用的16组表,每组表得到一个值,因为6位数据,最大是64,
     * 所以下面的表的容量都是64(都可以替换成自己的) */
    unsigned char s1[4][16] =
    {	{13,  3,  1,  7,  2, 11, 10,  0, 14,  6,  5, 15,  4, 12,  9,  8},
        { 4, 11,  8,  2, 14,  0, 10,  5,  3,  6, 12,  1, 15, 13,  7,  9},
        { 8, 10, 14,  3,  6,  2,  0,  1,  7,  4,  5, 15,  9, 11, 12, 13},
        {10,  6,  3, 14, 13,  7, 15,  2, 12, 11,  5,  8,  4,  1,  0,  9}
    };
    unsigned char s2[4][16] =
    {	{14,  4,  2,  9, 13,  1,  8, 15, 10,  6,  5, 12,  0, 11,  7,  3},
        {14, 10,  2, 15,  7,  8,  3,  1,  4,  6, 13,  9,  0, 12, 11,  5},
        { 3,  0, 14, 13,  5, 12,  8,  1,  7,  2,  6, 15, 11,  9,  4, 10},
        {15,  9,  7,  1, 11,  6,  0,  2,  4,  3,  8, 14,  5, 13, 10, 12},
    };
    unsigned char s3[4][16] =
    {	 {8,  2, 15,  0,  5,  4,  9, 11,  3,  7, 14, 10, 12, 13,  1,  6},
         {5, 10,  7,  0, 11, 15,  9,  8, 14,  2,  1, 12,  6,  4, 13,  3},
         {1,  4,  5,  8, 12, 15,  2, 13, 11, 14,  3,  9, 10,  6,  0,  7},
         {2,  3, 15,  4,  8,  7, 10, 13, 12,  9,  5,  6, 11, 14,  1,  0},
    };
    unsigned char s4[4][16] =
    {	 { 0, 13,  9,  6, 11,  4,  2, 15,  8, 14, 12,  7, 10,  1,  3,  5},
         {15, 13, 14, 12,  5, 11,  8,  1,  4,  9,  6, 10,  3,  0,  2,  7},
         {12,  5,  6, 14, 15, 11,  1, 10, 13,  0,  9,  8,  4,  7,  3,  2},
         { 3, 13,  0,  5, 11,  8,  4,  9, 15,  2,  1,  6, 14, 10, 12,  7},
    };
    unsigned char s5[4][16] =
    {	 { 7,  0,  5,  1,  9, 10,  2, 14,  3, 11,  8, 12, 15,  4,  6, 13},
         {14,  5,  2,  3,  6,  7,  1,  4, 11, 10, 15, 12,  8,  9,  0, 13},
         {15, 10,  7, 14, 13, 12,  9,  4,  3,  8,  2,  1,  0,  6,  5, 11},
         { 2,  0,  4, 11, 10, 12,  5,  1, 15, 14,  8, 13,  7,  3,  6,  9},
    };
    unsigned char s6[4][16] =
    {	{14,  2,  9, 10,  8,  6, 15,  0, 11,  7,  1, 12,  4, 13,  3,  5},
        { 3,  2, 10,  5,  0,  9, 11, 12, 15, 13,  8,  1, 14,  4,  7,  6},
        { 8,  4, 12, 11, 15,  1,  7, 14, 10,  0, 13,  9,  5,  6,  2,  3},
        {11, 14,  9,  7,  6, 13,  1,  5,  2,  3,  8, 10, 12, 15,  4,  0},
    };
    unsigned char s7[4][16] =
    {	 {5,  6,  0,  2, 11,  8, 14,  3, 13, 10,  7,  1, 12,  9,  4, 15},
         {13,  4,  9,  3, 12,  2,  8, 15, 10,  6,  1,  5,  0, 14, 11,  7},
         {12,  0, 11,  8,  5, 10, 13,  2, 14,  6,  9,  4, 15,  7,  3,  1},
         {15,  1,  0, 12,  4, 11,  5,  7,  8,  2, 13,  9, 14, 10,  6,  3},
    };
    unsigned char s8[4][16] =
    {	 {4, 11,  7,  3,  0, 12, 15,  6,  5, 10,  8,  1, 13,  9, 14,  2},
         {9, 15, 12, 13,  3,  7,  8,  1, 11,  0,  4,  5, 14, 10,  6,  2},
         {1,  4, 12,  3, 10,  5,  9,  7, 13, 11, 14,  8,  2,  6, 15,  0},
         {13,  1,  2,  7,  6,  3, 12,  0,  9,  4, 14,  5, 11, 10, 15,  8},
    };

    int i=0, j=0 ;
    unsigned char boxSByte[64] = {0};
    unsigned char boxSByte1[64] = {0};
    unsigned char bitTemp[8] = {0};
    unsigned char row = 0, col = 0;
    unsigned char boxSOutBit[8] = {0};
    unsigned char boxSOutBit1[8] = {0};

    /* 这里将48位的比特转换为48位的字节,便于后面的运算 */
    bitToByte(inBit, boxSByte);

    /* 从S盒的每个盒(4*16)中取出一个值来，最大值为15 */
    for (i = 0; i <= 7; ++i) {
        for (j = 0; j <= 63; ++j) boxSByte1[j] = 0x0;

        /** 一个6位数据: 如果将所有6位数据放在二组数组[4][16]中的话,
         * 比如 11 0001,其最高的两个位"11"可以代表其在[4][16]中哪一行,这里11=3,下标从0开始,所以是第4行.
         * 而0001则代表其在[4][16]中的哪一列,这里0001=1,所以是第二列 **/

        /* 对于8*6(8行6列)数据，取0和5列组成row值，其他组成col值 */
        boxSByte1[6] = boxSByte[i * 6];
        boxSByte1[7] = boxSByte[i * 6 + 5];
        /* 转换为字节之后，6和7到字节的0和1位 */
        byteToBit(boxSByte1, bitTemp);
        /* 求出行值 */
        row = bitTemp[0];

        for (j = 0; j <= 63; ++j) boxSByte1[j] = 0x0;

        boxSByte1[4] = boxSByte[i * 6 + 1];
        boxSByte1[5] = boxSByte[i * 6 + 2];
        boxSByte1[6] = boxSByte[i * 6 + 3];
        boxSByte1[7] = boxSByte[i * 6 + 4];
        byteToBit(boxSByte1, bitTemp);
        /* 求出列值 */
        col = bitTemp[0];


        /* 从S盒中找出对应的值 */
        switch (i) {
        case 0:
            boxSOutBit[i] = s1[row][col];
            break;
        case 1:
            boxSOutBit[i] = s2[row][col];
            break;
        case 2:
            boxSOutBit[i] = s3[row][col];
            break;
        case 3:
            boxSOutBit[i] = s4[row][col];
            break;
        case 4:
            boxSOutBit[i] = s5[row][col];
            break;
        case 5:
            boxSOutBit[i] = s6[row][col];
            break;
        case 6:
            boxSOutBit[i] = s7[row][col];
            break;
        case 7:
            boxSOutBit[i] = s8[row][col];
            break;
        } ;
    }

    /* 将两个4位组成一个8位,即一个字节 */
    boxSOutBit1[0] = (boxSOutBit[0] << 4) + boxSOutBit[1];
    boxSOutBit1[1] = (boxSOutBit[2] << 4) + boxSOutBit[3];
    boxSOutBit1[2] = (boxSOutBit[4] << 4) + boxSOutBit[5];
    boxSOutBit1[3] = (boxSOutBit[6] << 4) + boxSOutBit[7];

    for (i = 0; i <= 63; ++i) boxSByte[i] = 0x0;
    for (i = 0; i <= 63; ++i) boxSByte1[i] = 0x0;
    /* 比特转换为字节 */
    bitToByte(boxSOutBit1, boxSByte);
    /* 再将生成的32位数据(只取32位进行)进行一次置换 */
    for (i = 0; i <= 31; ++i) boxSByte1[i] = boxSByte[IP[i] - 1];
    for (i = 0; i <= 7; ++i) inBit[i] = 0x0;
    byteToBit(boxSByte1, inBit);
} /* S盒置换结束  */

} // extern "C"
#endif // __cplusplus


CMyDES::CMyDES(QObject *parent)
    : QObject(parent)
{

}

CMyDES::~CMyDES()
{

}

QString CMyDES::md5ForFile(const QString filePath)
{
    QString strReturn = "";
    QString strPath = filePath;
    if (strPath.startsWith("file:///", Qt::CaseInsensitive)) {
        strPath = filePath.right(filePath.length() - QString("file:///").length());
    }
    if (!strPath.isEmpty() && QFile::exists(strPath)) {
        QFile file(strPath);
        if (file.open(QIODevice::ReadOnly)) {
            QCryptographicHash hash(QCryptographicHash::Md5);
            /* 此方法需要验证是否支持大文件 */
            hash.addData(&file);
            strReturn = QString(hash.result().toHex());
            file.close();
        } else {
            qWarning() << "文件打开失败，无法进行md5ForFile取值！";
        }
    }
    qDebug() << "MD5 result:" << strReturn;
    return strReturn;
}

QString CMyDES::encryptDataByDES(const QString inData, const QString inStrKey)
{
    QByteArray inDataByte = QByteArray::fromHex(inData.toLocal8Bit());
    QByteArray outData = "";
    if (inDataByte.isEmpty() || inStrKey.isEmpty()) {
        qCritical() << "明文或密钥为空!";
        return "";
    }

    if (8 != inStrKey.length()) {
        qCritical() << "密钥长度不为8!";
        return "";
    }

    if (0 != (inDataByte.length() % 8)) {
        /* 加密的数据长度必须是8字节的整数倍,这里不做容错处理 */
        qCritical() << "明文长度不为8的整数倍!" << "len: " << inDataByte.length();
        return "";
    }

    outData.resize(inDataByte.length());
    outData.fill(0);
    /* 加密过程一次只能对8个字节进行 */
    for (int i = 0; i < inDataByte.length() / 8; ++i) {
        encryptDES((unsigned char*)(inDataByte.data() + i * 8),
                   (unsigned char*)(inStrKey.toLocal8Bit().data()),
                   (unsigned char*)outData.data() + i * 8);
    }

    return outData.toHex();
}

QString CMyDES::decryptDataByDES(const QString inData, const QString inStrKey)
{
    QByteArray inDataByte = QByteArray::fromHex(inData.toLocal8Bit());
    QByteArray outData = "";
    if (inDataByte.isEmpty() || inStrKey.isEmpty()) {
        return "";
    }

    if (8 != inStrKey.length()) {
        return "";
    }

    if (0 != (inDataByte.length() % 8)) {
        /* 解密的数据长度必须是8字节的整数倍,这里不做容错处理 */
        return "";
    }

    outData.resize(inDataByte.length());
    outData.fill(0);
    /* 解密过程一次只能对8个字节进行 */
    for (int i = 0; i < inDataByte.length() / 8; ++i) {
        decryptDES((unsigned char*)(inDataByte.data() + i * 8),
                   (unsigned char*)(inStrKey.toLocal8Bit().data()),
                   (unsigned char*)outData.data() + i * 8);
    }

    return outData.toHex();
}

