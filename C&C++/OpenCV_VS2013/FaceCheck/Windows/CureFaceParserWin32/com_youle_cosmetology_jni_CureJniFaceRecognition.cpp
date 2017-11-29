
#include "com_youle_cosmetology_jni_CureJniFaceRecognition.h"

#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"
#include "faceGlobal.h"

#include <sstream>

using namespace std;

JNIEXPORT jstring JNICALL Java_com_youle_cosmetology_jni_CureJniFaceRecognition_recogni
  (JNIEnv *env, jobject obj, jstring strFilePath)
{
	const char *pStrFilePath = env->GetStringUTFChars(strFilePath, 0);
	cout << "Input parameter:" << pStrFilePath << endl;

	vector<int> vectorIntResult(5);
	vectorContours vectorFace;
	enumFaceColorType colorType = Type_Color_TouBai;
	bool bResult = false;

	cv::Mat matSrc = cv::imread(pStrFilePath);
	if (matSrc.empty()) {
		goto End;
	}
	if (matSrc.rows > 1280 || matSrc.cols > 1280) {
		if (matSrc.rows > matSrc.cols) {
			cv::resize(matSrc, matSrc, cv::Size(1280 * matSrc.cols / matSrc.rows, 1280));
		} else {
			cv::resize(matSrc, matSrc, cv::Size(1280, 1280 * matSrc.rows / matSrc.cols));
		}
	}
	
	bResult = faceLandmarkDetect(matSrc, vectorFace);
	if (!bResult) {
		goto End;
	}
	bResult = findFaceSpots(matSrc, vectorFace, vectorIntResult);
	if (!bResult) {
		goto End;
	}
	for (uint i = 0; i < vectorIntResult.size(); ++i) {
		//cout << "Detect result for area: " << vectorIntResult[i] << endl;
	}
	colorType = getFaceColorType(matSrc, vectorFace);
	//cout << "Face color type: " << colorType << endl;

End:	
	env->ReleaseStringUTFChars(strFilePath, 0);
	stringstream ss;
	ss << "{\"result\":" << bResult << ",\"spots\":{\"A\":" << vectorIntResult[0] <<  ",\"B\":" << vectorIntResult[1]
	   << ",\"C\":" << vectorIntResult[2] << ",\"D\":" << vectorIntResult[3] << ",\"E\":" << vectorIntResult[4] 
	   << "},\"color\":" << colorType << "}";
	
	return env->NewStringUTF(ss.str().c_str());
}
