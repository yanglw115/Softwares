
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

const Scalar RED = Scalar(0, 0, 255);
const Scalar PINK = Scalar(230, 130, 255);
const Scalar BLUE = Scalar(255, 0, 0);
const Scalar LIGHTBLUE = Scalar(255, 255, 160);
const Scalar GREEN = Scalar(0, 255, 0);
const int BGD_KEY = EVENT_FLAG_CTRLKEY;
const int FGD_KEY = EVENT_FLAG_SHIFTKEY;

int grabCut(const string strFile, Rect rectInput)
{
	Mat image = imread(strFile, 1);
	if (image.empty())
	{
		cout << "\n Durn, couldn't read image filename " << strFile << endl;
		return 1;
	}

	const string winName = "image";
	namedWindow(winName, WINDOW_NORMAL);
	Mat mask, bgdModel, fgdModel;
	mask.create(image.size(), CV_8UC1);

	mask.setTo(GC_BGD);
	(mask(rectInput)).setTo(Scalar(GC_PR_FGD));
	grabCut(image, mask, rectInput, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK);
	
	Mat res;
	Mat binMask;
	vector<Point> bgdPxls, fgdPxls, prBgdPxls, prFgdPxls;
	const int radius = 2;
	const int thickness = -1;

	if (mask.empty() || mask.type() != CV_8UC1)
		CV_Error(Error::StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)");
	if (binMask.empty() || binMask.rows != mask.rows || binMask.cols != mask.cols)
		binMask.create(mask.size(), CV_8UC1);
	binMask = mask & 1;

	image.copyTo(res, binMask);

	vector<Point>::const_iterator it;
	for (it = bgdPxls.begin(); it != bgdPxls.end(); ++it)
		circle(res, *it, radius, BLUE, thickness);
	for (it = fgdPxls.begin(); it != fgdPxls.end(); ++it)
		circle(res, *it, radius, RED, thickness);
	for (it = prBgdPxls.begin(); it != prBgdPxls.end(); ++it)
		circle(res, *it, radius, LIGHTBLUE, thickness);
	for (it = prFgdPxls.begin(); it != prFgdPxls.end(); ++it)
		circle(res, *it, radius, PINK, thickness);

	imshow("grub", res);
	waitKey();

	destroyWindow(winName);
	return 0;
}