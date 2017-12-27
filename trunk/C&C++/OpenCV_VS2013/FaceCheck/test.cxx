
#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"
#include "faceGlobal.h"
#include "faceTamura.h"

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
	vector<int> vectorIntResult(INDEX_VALUE_MAX); /* left_face, right_face, forehead, jaw, nose | blackheads */
	vectorContours vectorFace;
	cv::Rect rectFaceColor;
	cv::Rect rectFaceCorassness;
	double fCoarseness = 0.0;
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
		if (pNextInfo.cFileName[0] == '.')//¹ýÂË.ºÍ..
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
		LOG(INFO) << "Get valid image file path: " << pStrFilePath << "(" << to_string(matSrc.cols) << ", " << to_string(matSrc.rows) << ")";
		strImageName = string(pStrFilePath);
		nPosition = strImageName.rfind("/");
		if (-1 != nPosition) {
			strImageName = strImageName.substr(nPosition + 1);
		}

		if (1) {//(matSrc.rows > 1280 || matSrc.cols > 1280) {
			if (matSrc.rows > matSrc.cols) {
				cv::resize(matSrc, matSrc, cv::Size(1280 * matSrc.cols / matSrc.rows, 1280));
			}
			else {
				cv::resize(matSrc, matSrc, cv::Size(1280, 1280 * matSrc.rows / matSrc.cols));
			}
		}
		bResult = faceLandmarkDetect(strImageName, matSrc, vectorFace, rectFaceColor);
		if (!bResult) {
			LOG(ERROR) << strImageName << ": Detect face landmark failed!";
			goto End;
		}

		rectFaceCorassness = cv::Rect(vectorFace[INDEX_CONTOUR_FACE][0], vectorFace[INDEX_CONTOUR_FACE][2]);
		fCoarseness = getFaceCoarseness(matSrc, rectFaceCorassness);
		LOG(INFO) << strImageName << ": Skin coarseness: " << to_string(fCoarseness); 

		bResult = findFaceSpots(strImageName, matSrc, vectorFace, vectorIntResult);
		if (!bResult) {
			LOG(ERROR) << strImageName << ": Find face spots failed!";
			goto End;
		}
		colorType = getFaceColorType(strImageName, matSrc, rectFaceColor);

	End:
		stringstream ss;
		ss << "{\"result\":" << bResult << ",\"spots\":{\"A\":" << vectorIntResult[INDEX_VALUE_LEFT] << ",\"B\":" << vectorIntResult[INDEX_VALUE_RIGHT]
			<< ",\"C\":" << vectorIntResult[INDEX_VALUE_FOREHEAD] << ",\"D\":" << vectorIntResult[INDEX_VALUE_JAW] << ",\"E\":" << vectorIntResult[INDEX_VALUE_NOSE]
			<< "}, \"blackheads\":" << vectorIntResult[INDEX_VALUE_BLACKHEADS] << ",\"pore\":" << vectorIntResult[INDEX_VALUE_PORE_TYPE] << ",\"coarseness\":" << fCoarseness << ",\"color\":" << colorType << "}";

		LOG(INFO) << strImageName << ": " << ss.str();
#ifndef __linux
	}
#endif

	return 0;
}
