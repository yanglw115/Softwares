
#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"
#include "faceGlobal.h"

#include <string>
#include <sstream>

#ifdef __linux
#define SIZE_TYPE_T int
#else
#define SIZE_TYPE_T size_t
#endif

static CureLog g_logObject;

int main(int argc, char **argv)
{
#ifdef __linux
	const char *pStrFilePath = "images/yangliwei.jpg";
#else
	const char *pStrFilePath = "images/2.jpg";
#endif
	vector<int> vectorIntResult(5);
	vectorContours vectorFace;
	cv::Rect rectFace;
	enumFaceColorType colorType = Type_Color_TouBai;
	cv::Mat matSrc;
	string strImageName("");
	bool bResult = false;
	SIZE_TYPE_T nPosition = -1;

	matSrc = cv::imread(pStrFilePath);
	if (matSrc.empty()) {
		LOG(ERROR) << "Input image file path is invalid: " << pStrFilePath;
#ifdef __linux
		goto End;
#else
		return -1;
#endif
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
	bResult = faceLandmarkDetect(strImageName, matSrc, vectorFace, rectFace);
	if (!bResult) {
#ifdef __linux
		goto End;
#else
		return -1;
#endif
	}
	bResult = findFaceSpots(strImageName, matSrc, vectorFace, vectorIntResult);
	if (!bResult) {
#ifdef __linux
		goto End;
#else
		return -1;
#endif
	}
	colorType = getFaceColorType(strImageName, matSrc, rectFace);

#ifdef __linux
End:	
#endif
	stringstream ss;
	ss << "{\"result\":" << bResult << ",\"spots\":{\"A\":" << vectorIntResult[0] <<  ",\"B\":" << vectorIntResult[1]
	   << ",\"C\":" << vectorIntResult[2] << ",\"D\":" << vectorIntResult[3] << ",\"E\":" << vectorIntResult[4] 
	   << "},\"color\":" << colorType << "}";

	LOG(INFO) << strImageName << ": " << ss.str();
#ifndef __linux
#ifdef With_Debug
	while ('q' != cv::waitKey(0));
#endif // With_Debug
#endif

	return 0;
}
