
#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"
#include "faceGrabcut.h"
#include "faceDetect.h"
#include "faceGlobal.h"

using namespace std;

int main(int argc, char **argv)
{
	int nSpots = 0;
	double nColorValue = 0;
	const char *pStrFilePath = "images/zhuchong.jpg";
	bool bResult = false;

	vector<int> vectorIntResult(5);
	vectorContours vectorFace;

	//cv::Rect rect = faceDetect(pStrFilePath);
	//grabCut(pStrFilePath, rect);

	bResult = faceLandmarkDetect(pStrFilePath, vectorFace);
	if (!bResult) {
		return -1;
	}
	bResult = findFaceSpots(pStrFilePath, vectorFace, vectorIntResult);
	if (!bResult) {
		return -1;
	}
	for each (auto var in vectorIntResult)	{
		cout << "班点检测结果为：" << var << endl;
	}
	//nColorValue = getFaceColorValue(pStrFilePath, vectorFace);

#ifdef With_Debug
	while ('q' != cv::waitKey(0));
#endif // With_Debug

	return 0;
}