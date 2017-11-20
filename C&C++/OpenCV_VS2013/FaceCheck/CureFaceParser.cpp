
#include "CureFaceParser.h"

#include <iostream>

using namespace std;

JNIEXPORT jstring JNICALL Java_CureFaceParser_getFaceParseResult
  (JNIEnv *env, jobject obj, jstring strFilePath)
{
	const char *pStrFilePath = env->GetStringUTFChars(strFilePath, 0);
	cout << "Input parameter:" << pStrFilePath << endl;
	env->ReleaseStringUTFChars(strFilePath, 0);
	return env->NewStringUTF("Hello JNI.");
}