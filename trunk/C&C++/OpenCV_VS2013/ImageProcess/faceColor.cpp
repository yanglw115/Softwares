
#include "faceColor.h"


#include "opencv2/highgui/highgui.hpp"    
#include "opencv2/imgproc/imgproc.hpp"    
#include <iostream>    
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

/* ͸�ף�232/��𪣺217/��Ȼ��200/С��161/������120/��ڣ�81 */
typedef enum {
	͸�� = 234,
	��� = 220,
	��Ȼ = 205,
	С�� = 164,
	���� = 122,
	��� = 81
} enumFaceColorType;

static const char *g_colorString[] = {
	"tou bai",
	"bai xi",
	"zi ran",
	"xiao mai",
	"an chen",
	"you hei"
};

MatND getHistogram(Mat &image)
{
	MatND hist;

	int channels[] = { 0 };
	int dims = 1;
	int histSize[] = { 256 };
	float granges[] = { 0, 256 };
	const float *ranges[] = { granges };
	calcHist(&image, 1, channels, Mat(), hist, dims, histSize, ranges);
	
	return hist;
}
  
Mat getHistogramImage(Mat &image, double *pColorValue)
{
	MatND hist = getHistogram(image);
	Mat showImage(256, 256, CV_8UC3, Scalar(0, 0, 0));
	int i;
	double maxValue = 0;
	Point maxPoint;
	minMaxLoc(hist, 0, &maxValue, 0, &maxPoint);
	cout << "���ֵ�㣺" << maxPoint << ", ���ֵ��" << maxValue;
	*pColorValue = maxPoint.y;
	for (i = 0; i < 256; i++) {
		float value = hist.at<float>(i);
		int intensity = saturate_cast<int>(256 - 256 * (value / maxValue));
		rectangle(showImage, Point(i, 256 - 1), Point((i + 1) - 1, intensity), Scalar(0, 0, 255));
	}
	return showImage;
}

double getFaceColorValue(const std::string &strFile, std::vector<std::vector<cv::Point>> contours)
{
	double maxColorValue;

	Mat imageSrc = imread(strFile, 0);

	Mat imageFace(imageSrc.size(), CV_8UC1);
	Mat mask(imageSrc.size(), CV_8UC1);
	mask = 0;
	drawContours(mask, contours, 2, Scalar(255), -1);

	imageSrc.copyTo(imageFace, mask);
	/* ֻȡ����������ɫ�Ĺؼ����� */
	Mat imageColor(imageFace, Rect(contours.at(2).at(0), contours.at(2).at(2)));
	if (!imageColor.data) {
		cout << "fail to load the image" << endl;
		return 0;
	}
	//imshow("Face image: ", imageColor);
	//waitKey();

	Mat imageResult = getHistogramImage(imageColor, &maxColorValue);

	const char *strColorString = NULL;
	if (maxColorValue > ͸��) {
		strColorString = g_colorString[0];
	} else if (maxColorValue > ���) {
		strColorString = g_colorString[1];
	} else if (maxColorValue > ��Ȼ) {
		strColorString = g_colorString[2];
	} else if (maxColorValue > С��) {
		strColorString = g_colorString[3];
	} else if (maxColorValue > ����) {
		strColorString = g_colorString[4];
	} else {
		strColorString = g_colorString[5];
	}

	namedWindow("image original:", WINDOW_NORMAL);
	putText(imageSrc, format("%s", strColorString), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0, 0, 255), 3);
	imshow("image original:", imageSrc);
	//namedWindow("image face pre:", WINDOW_NORMAL);
	//imshow("image face pre:", imageColor);
	namedWindow("showImage", WINDOW_NORMAL);
	imshow("showImage", imageResult);
	waitKey(0);
	return maxColorValue;
}