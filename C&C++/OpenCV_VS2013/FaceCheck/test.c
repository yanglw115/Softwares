
#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"
#include "faceGrabcut.h"
#include "faceDetect.h"
#include "faceGlobal.h"

using namespace std;

int main(int argc, char **argv)
{
	const char *pStrFilePath = "images/baby.jpg";
	vector<int> vectorIntResult(5);
	vectorContours vectorFace;
	enumFaceColorType colorType = Type_Color_TouBai;
	bool bResult = false;

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
		cout << "°àµã¼ì²â½á¹ûÎª£º" << var << endl;
	}
	colorType = getFaceColorType(pStrFilePath, vectorFace);
	cout << "Face color type: " << colorType << endl;

#ifdef With_Debug
	while ('q' != cv::waitKey(0));
#endif // With_Debug

	return 0;
}