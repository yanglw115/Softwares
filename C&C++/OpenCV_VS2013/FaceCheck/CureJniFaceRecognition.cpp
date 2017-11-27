
#include "CureJniFaceRecognition.h"

#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"
#include "faceGlobal.h"

#include <sstream>

using namespace std;

JNIEXPORT jstring JNICALL Java_CureJniFaceRecognition_recogni
  (JNIEnv *env, jobject obj, jstring strFilePath)
{
	const char *pStrFilePath = env->GetStringUTFChars(strFilePath, 0);
	cout << "Input parameter:" << pStrFilePath << endl;

	vector<int> vectorIntResult(5);
	vectorContours vectorFace;
	enumFaceColorType colorType = Type_Color_TouBai;
	bool bResult = false;


	bResult = faceLandmarkDetect(pStrFilePath, vectorFace);
	if (!bResult) {
		goto End;
	}
	bResult = findFaceSpots(pStrFilePath, vectorFace, vectorIntResult);
	if (!bResult) {
		goto End;
	}
	for (int i = 0; i < vectorIntResult.size(); ++i) {
		cout << "Detect result for area: " << vectorIntResult[i] << endl;
	}
	colorType = getFaceColorType(pStrFilePath, vectorFace);
	cout << "Face color type: " << colorType << endl;

End:	
	env->ReleaseStringUTFChars(strFilePath, 0);
	stringstream ss;
	ss << "{\"result\":" << bResult << ",\"spots\":{\"A\":" << vectorIntResult[0] <<  ",\"B\":" << vectorIntResult[1]
	   << ",\"C\":" << vectorIntResult[2] << ",\"D\":" << vectorIntResult[3] << ",\"E\":" << vectorIntResult[4] 
	   << "},\"color\":" << colorType << "}";
	
	return env->NewStringUTF(ss.str().c_str());
}
