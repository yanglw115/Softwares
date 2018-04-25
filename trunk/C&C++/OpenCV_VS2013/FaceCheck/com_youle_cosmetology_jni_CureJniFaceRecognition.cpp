
#include "com_youle_cosmetology_jni_CureJniFaceRecognition.h"

#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"
#include "faceGlobal.h"

#include <string>
#include <sstream>

static CureLog g_logObject;

#ifdef __linux
#define SIZE_TYPE_T int
#else
#define SIZE_TYPE_T size_t
#endif

JNIEXPORT jstring JNICALL Java_com_youle_cosmetology_jni_CureJniFaceRecognition_recogni
  (JNIEnv *env, jobject obj, jstring strFilePath)
{
	vector<int> vectorIntResult(INDEX_VALUE_MAX); /* left_face, right_face, forehead, jaw, nose | blackheads */
	vectorContours vectorFace;
	cv::Rect rectFaceColor;
	enumFaceColorType colorType = Type_Color_TouBai;
	cv::Mat matSrc;
	string strImageName("");
	bool bResult = false;
	SIZE_TYPE_T nPosition = -1;

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
	LOG(INFO) << "Get valid image file path: " << pStrFilePath << "(" << to_string(matSrc.cols) << ", " << to_string(matSrc.rows) << ")";
	strImageName = string(pStrFilePath);
	nPosition = strImageName.rfind("/");
	if (-1 != nPosition) {
		strImageName = strImageName.substr(nPosition + 1);
	}
	
	/* 不管图片尺寸，全部转换为以1280为基准的图像进行处理 */
	if (1) {//(matSrc.rows > 1280 || matSrc.cols > 1280) {
		if (matSrc.rows > matSrc.cols) {
			cv::resize(matSrc, matSrc, cv::Size(1280 * matSrc.cols / matSrc.rows, 1280));
		} else {
			cv::resize(matSrc, matSrc, cv::Size(1280, 1280 * matSrc.rows / matSrc.cols));
		}
	}
	bResult = faceLandmarkDetect(strImageName, matSrc, vectorFace, rectFaceColor);
	if (!bResult) {
			LOG(ERROR) << strImageName << ": Detect face landmark failed!";
		goto End;
	}
	bResult = findFaceSpots(strImageName, matSrc, vectorFace, vectorIntResult);
	if (!bResult) {
			LOG(ERROR) << strImageName << ": Find face spots failed!";
		goto End;
	}
	colorType = getFaceColorType(strImageName, matSrc, rectFaceColor);

End:	
	env->ReleaseStringUTFChars(strFilePath, 0);
	stringstream ss;
	ss << "{\"result\":" << bResult;
	ss << ",\"spots\":{\"A\":" << vectorIntResult[INDEX_VALUE_LEFT] << ",\"B\":" << vectorIntResult[INDEX_VALUE_RIGHT]
		<< ",\"C\":" << vectorIntResult[INDEX_VALUE_FOREHEAD] << ",\"D\":" << vectorIntResult[INDEX_VALUE_JAW] << ",\"E\":" << vectorIntResult[INDEX_VALUE_NOSE] << "}";
	ss << ",\"blackheads\":" << vectorIntResult[INDEX_VALUE_BLACKHEADS];
	ss << ",\"pore\":" << vectorIntResult[INDEX_VALUE_PORE_TYPE];
	ss << ",\"oil\":" << vectorIntResult[INDEX_VALUE_OIL_TYPE];
	ss << ",\"color\":" << colorType;
	ss << "}";

	LOG(INFO) << strImageName << ": " << ss.str();
	return env->NewStringUTF(ss.str().c_str());
}
