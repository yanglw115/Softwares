
#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	int nSpots = 0;
	double nColorValue = 0;
	const char *pStrFilePath = "liudehua.jpg";

	std::vector<std::vector<cv::Point>> vector1 = faceLandmarkDetect(pStrFilePath);
	nSpots = findFaceSpots(pStrFilePath, vector1);
	nColorValue = getFaceColorValue(pStrFilePath, vector1);

	return 0;
}