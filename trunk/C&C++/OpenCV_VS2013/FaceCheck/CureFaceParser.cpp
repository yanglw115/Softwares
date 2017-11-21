
#include "CureFaceParser.h"

#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"

#include <iostream>

using namespace std;

JNIEXPORT jstring JNICALL Java_CureFaceParser_getFaceParseResult
  (JNIEnv *env, jobject obj, jstring strFilePath)
{
	const char *pStrFilePath = env->GetStringUTFChars(strFilePath, 0);
	cout << "Input parameter:" << pStrFilePath << endl;

	int nSpots = 0;
	double nColorValue = 0;

	std::vector<std::vector<cv::Point>> vector1 = faceLandmarkDetect(pStrFilePath);
	nSpots = findFaceSpots(pStrFilePath, vector1);
	nColorValue = getFaceColorValue(pStrFilePath, vector1);
	
	env->ReleaseStringUTFChars(strFilePath, 0);
	return env->NewStringUTF("\"{\"result\": \"0\", \"spots\": \"10\", \"color\": \"110.1\"}.");
}