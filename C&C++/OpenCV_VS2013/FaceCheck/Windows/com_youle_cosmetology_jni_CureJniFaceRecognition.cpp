
#include "com_youle_cosmetology_jni_CureJniFaceRecognition.h"

#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"
#include "faceGlobal.h"

#include <string>
#include <sstream>

static CureLog g_logObject;

JNIEXPORT jstring JNICALL Java_com_youle_cosmetology_jni_CureJniFaceRecognition_recogni
  (JNIEnv *env, jobject obj, jstring strFilePath)
{
	vector<int> vectorIntResult(5);
	vectorContours vectorFace;
	enumFaceColorType colorType = Type_Color_TouBai;
	cv::Mat matSrc;
	string strImageName("");
	bool bResult = false;
	int nPosition = -1;

	const char *pStrFilePath = env->GetStringUTFChars(strFilePath, 0);
	if (!pStrFilePath) {
		LOG(ERROR) << "Get file path from java error!(NULL)";
		goto End;
	}

	matSrc = cv::imread(pStrFilePath);
	if (matSrc.empty()) {
		LOG(ERROR) << "Input image file path is invalid: " << pStrFilePath;
		goto End;
	}
	LOG(INFO) << "Get valid image file path: " << pStrFilePath;
	strImageName = string(pStrFilePath);
	nPosition = strImageName.rfind("/");
	if (-1 != nPosition) {
		strImageName = strImageName.substr(nPosition + 1);
	}
	
	if (matSrc.rows > 1280 || matSrc.cols > 1280) {
		if (matSrc.rows > matSrc.cols) {
			cv::resize(matSrc, matSrc, cv::Size(1280 * matSrc.cols / matSrc.rows, 1280));
		} else {
			cv::resize(matSrc, matSrc, cv::Size(1280, 1280 * matSrc.rows / matSrc.cols));
		}
	}
	
	bResult = faceLandmarkDetect(strImageName, matSrc, vectorFace);
	if (!bResult) {
		goto End;
	}
	bResult = findFaceSpots(strImageName, matSrc, vectorFace, vectorIntResult);
	if (!bResult) {
		goto End;
	}
	colorType = getFaceColorType(strImageName, matSrc, vectorFace);

End:	
	env->ReleaseStringUTFChars(strFilePath, 0);
	stringstream ss;
	ss << "{\"result\":" << bResult << ",\"spots\":{\"A\":" << vectorIntResult[0] <<  ",\"B\":" << vectorIntResult[1]
	   << ",\"C\":" << vectorIntResult[2] << ",\"D\":" << vectorIntResult[3] << ",\"E\":" << vectorIntResult[4] 
	   << "},\"color\":" << colorType << "}";

	LOG(INFO) << strImageName << ": " << ss.str();
	return env->NewStringUTF(ss.str().c_str());
}
