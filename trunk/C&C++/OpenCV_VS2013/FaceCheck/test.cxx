
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
//static CureLog g_logObject;
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
	cv::Mat matTest;
	string strImageName("");
	bool bResult = false;
	SIZE_TYPE_T nPosition = -1;

#ifdef __linux
	const char *pStrFilePath = "images/yangliwei.jpg";
#else
	const char *pStrFilePath = "../../../images/pics/";
	vector<string> vectorFiles;
	HANDLE hFile;
	WIN32_FIND_DATA pNextInfo;
	hFile = FindFirstFile((string(pStrFilePath) + string("*.*")).c_str(), &pNextInfo);
	if (hFile == INVALID_HANDLE_VALUE) {
		LOG(WARNING) << "Cannot find image files.";
		return -1;
	}
	if (pNextInfo.cFileName[0] != '.')
		vectorFiles.push_back(string(pStrFilePath) + string(pNextInfo.cFileName));

	while (FindNextFile(hFile, &pNextInfo)) {
		if (pNextInfo.cFileName[0] == '.')//¹ýÂË.ºÍ..
			continue;
		vectorFiles.push_back(string(pStrFilePath) + string(pNextInfo.cFileName));
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

#if 1
		//findPimples(strImageName, matSrc, matSrc);
		//matTest = matSrc.clone();
		//cv::resize(matTest, matTest, cv::Size(1024, 768));
		//cv::imwrite(string(strImageName + ".jpg").c_str(), matTest);
		getMoistureAndOil(strImageName, matSrc, matSrc);

#else
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

		bResult = findFaceSpots(strImageName, matSrc, vectorFace, vectorIntResult);
		if (!bResult) {
			LOG(ERROR) << strImageName << ": Find face spots failed!";
			goto End;
		}

		rectFaceCorassness = cv::Rect(vectorFace[INDEX_CONTOUR_FACE][0], vectorFace[INDEX_CONTOUR_FACE][2]);
		fCoarseness = getFaceCoarseness(matSrc, rectFaceCorassness);
		LOG(INFO) << strImageName << ": Skin coarseness: " << to_string(fCoarseness);

		colorType = getFaceColorType(strImageName, matSrc, rectFaceColor);
#endif
	End:
		stringstream ss;
		ss << "{\"result\":" << bResult;
		ss << ",\"spots\":{\"A\":" << vectorIntResult[INDEX_VALUE_LEFT] << ",\"B\":" << vectorIntResult[INDEX_VALUE_RIGHT]
			<< ",\"C\":" << vectorIntResult[INDEX_VALUE_FOREHEAD] << ",\"D\":" << vectorIntResult[INDEX_VALUE_JAW] << ",\"E\":" << vectorIntResult[INDEX_VALUE_NOSE] << "}";
		ss << ",\"blackheads\":" << vectorIntResult[INDEX_VALUE_BLACKHEADS];
		ss << ",\"pore\":" << vectorIntResult[INDEX_VALUE_PORE_TYPE];
		ss << ",\"coarseness\":" << fCoarseness;
		ss << ",\"color\":" << colorType;
		ss << "}";

		LOG(INFO) << strImageName << ": " << ss.str();
#ifndef __linux
	}
#endif

	return 0;
}
