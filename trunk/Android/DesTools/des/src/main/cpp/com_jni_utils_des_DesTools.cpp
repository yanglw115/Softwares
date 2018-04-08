
#include "com_jni_utils_des_DesTools.h"
#include "jw_des.h"

static unsigned char *DES_KEY = (unsigned char *)"~^3*H(@K";

JNIEXPORT jstring JNICALL Java_com_jni_utils_des_DesTools_encrypt
  (JNIEnv *env, jobject obj, jstring strInput)
{
    char buffer[128] = {0};
    const char *pStrInput = env->GetStringUTFChars(strInput, 0);
    des_encryption((unsigned char *)pStrInput, DES_KEY, (unsigned char *)buffer);
    return env->NewStringUTF(buffer) ;
}

JNIEXPORT jstring JNICALL Java_com_jni_utils_des_DesTools_decrypt
  (JNIEnv *env, jobject obj, jstring strInput)
{
    char buffer[128] = {0};
    const char *pStrInput = env->GetStringUTFChars(strInput, 0);
    /*  decrypt implement */
    des_decryption((unsigned char *)pStrInput, DES_KEY, (unsigned  char *)buffer);
    return env->NewStringUTF(buffer) ;
}
