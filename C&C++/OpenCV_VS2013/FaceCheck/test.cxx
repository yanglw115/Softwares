
#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"
#include "faceGlobal.h"

#include <string>
#include <sstream>

#ifdef __linux
#define SIZE_TYPE_T int
#else
#include <windows.h>
#define SIZE_TYPE_T size_t
static CureLog g_logObject;
#endif

int main(int argc, char **argv)
{
	vector<int> vectorIntResult(5);
	vectorContours vectorFace;
	cv::Rect rectFace;
	enumFaceColorType colorType = Type_Color_TouBai;
	cv::Mat matSrc;
	string strImageName("");
	bool bResult = false;
	SIZE_TYPE_T nPosition = -1;

#ifdef __linux
	const char *pStrFilePath = "images/yangliwei.jpg";
#else
	const char *pStrFilePath = "../../../images/pics/*.*";
	vector<string> vectorFiles;
	HANDLE hFile;
	WIN32_FIND_DATA pNextInfo;
	hFile = FindFirstFile(pStrFilePath, &pNextInfo);
	if (hFile == INVALID_HANDLE_VALUE) {
		LOG(WARNING) << "Cannot find image files.";
		return -1;
	}
	if (pNextInfo.cFileName[0] != '.')
		vectorFiles.push_back("../../../images/pics/" + string(pNextInfo.cFileName));

	while (FindNextFile(hFile, &pNextInfo)) {
		if (pNextInfo.cFileName[0] == '.')//����.��..
			continue;
		vectorFiles.push_back("../../../images/pics/" + string(pNextInfo.cFileName));
	}
	for (int j = 0; j < vectorFiles.size(); ++j) {
		pStrFilePath = vectorFiles[j].c_str();
#endif // __linux

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
			}
			else {
				cv::resize(matSrc, matSrc, cv::Size(1280, 1280 * matSrc.rows / matSrc.cols));
			}
		}
		bResult = faceLandmarkDetect(strImageName, matSrc, vectorFace, rectFace);
		if (!bResult) {
			LOG(ERROR) << strImageName << ": Detect face landmark failed!";
			goto End;
		}
		bResult = findFaceSpots(strImageName, matSrc, vectorFace, vectorIntResult);
		if (!bResult) {
			LOG(ERROR) << strImageName << ": Find face spots failed!";
			goto End;
		}
		colorType = getFaceColorType(strImageName, matSrc, rectFace);

	End:
		stringstream ss;
		ss << "{\"result\":" << bResult << ",\"spots\":{\"A\":" << vectorIntResult[0] << ",\"B\":" << vectorIntResult[1]
			<< ",\"C\":" << vectorIntResult[2] << ",\"D\":" << vectorIntResult[3] << ",\"E\":" << vectorIntResult[4]
			<< "},\"color\":" << colorType << "}";

		LOG(INFO) << strImageName << ": " << ss.str();
#ifndef __linux
	}
#endif

	return 0;
}