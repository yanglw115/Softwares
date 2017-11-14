
#include "faceColor.h"


#include "opencv2/highgui/highgui.hpp"    
#include "opencv2/imgproc/imgproc.hpp"    
#include <iostream>    
#include <stdio.h>    

using namespace std;
using namespace cv;

/* 透白：232/白皙：217/自然：200/小麦：161/暗沉：120/黝黑：81 */
typedef enum {
	透白 = 232,
	白皙 = 217,
	自然 = 200,
	小麦 = 161,
	暗沉 = 120,
	黝黑 = 81
} enumFaceColorType;

static const char *g_colorString[] = {
	"tou bai",
	"bai xi",
	"zi ran",
	"xiao mai",
	"an chen",
	"you hei"
};

MatND getHistogram(Mat &image, bool bHSV)
{
	MatND hist;
	if (!bHSV) {
		int channels[] = { 0, 0 };
		int dims = 1;
		int histSize[] = { 256 };
		float granges[] = { 0, 256 };
		const float *ranges[] = { granges };
		calcHist(&image, 1, channels, Mat(), hist, dims, histSize, ranges);
	} else {
		Mat HSMats[3];
		HSMats[0] = Mat(image.size(), CV_8UC1);
		HSMats[1] = Mat(image.size(), CV_8UC1);
		HSMats[2] = Mat(image.size(), CV_8UC1);
		split(image, HSMats);
		int channels[] = { 0, 1 };
		int dims = 2;
		int histSize[] = { 30, 32 };
		float hRanges[] = { 0, 180 };
		float sRanges[] = {0, 255};
		const float *ranges[] = { hRanges, sRanges };
		calcHist(HSMats, 2, channels, Mat(), hist, dims, histSize, ranges);
	}
	return hist;
}
  
Mat getHistogramImage(Mat &image, int *pColorValue, bool bHSV = false)
{
	MatND hist = getHistogram(image, bHSV);
	Mat showImage(256, 256, CV_8UC3, Scalar(0, 0, 0));
	int i;
	double maxValue = 0;
	Point maxPoint;
	minMaxLoc(hist, 0, &maxValue, 0, &maxPoint);
	cout << "最大值点：" << maxPoint << ", 最大值：" << maxValue;
	*pColorValue = maxPoint.y;
	for (i = 0; i < 256; i++) {
		float value = hist.at<float>(i);
		int intensity = saturate_cast<int>(256 - 256 * (value / maxValue));
		rectangle(showImage, Point(i, 256 - 1), Point((i + 1) - 1, intensity), Scalar(0, 0, 255));
	}
	return showImage;
}

int getFaceColorValue(const std::string &strFile, std::vector<std::vector<cv::Point>> contours)
{
	Mat imageSrc = imread(strFile);
	Mat imageFace(imageSrc.size(), CV_8UC3);
	Mat mask(imageSrc.size(), CV_8UC3);
	int maxColorValue;
	mask = 0;
	drawContours(mask, contours, 2, Scalar(255, 255, 255), -1);

	imageSrc.copyTo(imageFace, mask);
	/* 只取脸部代表颜色的关键区域 */
	Mat imageColor(imageFace, Rect(contours.at(2).at(0), contours.at(2).at(2)));
	if (!imageSrc.data) {
		cout << "fail to load the image" << endl;
		return 0;
	}
#if 0
	cvtColor(imageColor, imageColor, COLOR_BGR2GRAY);
	Mat imageResult = getHistogramImage(imageColor, &maxColorValue);
#else
	cvtColor(imageColor, imageColor, COLOR_BGR2HSV);
	Mat imageResult = getHistogramImage(imageColor, &maxColorValue, true);
#endif

	const char *strColorString = NULL;
	if (maxColorValue > 透白) {
		strColorString = g_colorString[0];
	} else if (maxColorValue > 白皙) {
		strColorString = g_colorString[1];
	} else if (maxColorValue > 自然) {
		strColorString = g_colorString[2];
	} else if (maxColorValue > 小麦) {
		strColorString = g_colorString[3];
	} else if (maxColorValue > 暗沉) {
		strColorString = g_colorString[4];
	} else {
		strColorString = g_colorString[5];
	}

	namedWindow("image original:", WINDOW_NORMAL);
	putText(imageSrc, format("%s", strColorString), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0, 0, 255), 3);
	imshow("image original:", imageSrc);
	namedWindow("image face pre:", WINDOW_NORMAL);
	imshow("image face pre:", imageColor);
	namedWindow("showImage", WINDOW_NORMAL);
	imshow("showImage", imageResult);
	waitKey(0);
	return maxColorValue;
}