
#include "faceColor.h"

#ifdef With_Debug
#include "opencv2/highgui/highgui.hpp"    
#endif // With_Debug
#include "opencv2/imgproc/imgproc.hpp"    
#include <iostream>    
#include <stdio.h>
#include <math.h>

using namespace cv;

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
  
Mat getHistogramImage(const string &strImageName, Mat &image, double *pColorValue)
{
	MatND hist = getHistogram(image);
	Mat showImage(256, 256, CV_8UC3, Scalar(0, 0, 0));
	double maxValue = 0;
	Point maxPoint;
	minMaxLoc(hist, 0, &maxValue, 0, &maxPoint);
	LOG(INFO) << strImageName << ": Color max point：" << "(" << to_string(maxPoint.x) << ", " << to_string(maxPoint.y) << ")" 
		<< ", max value: " << to_string(maxValue);
	*pColorValue = maxPoint.y;

#ifdef With_Debug
	for (int i = 0; i < 256; i++) {
		float value = hist.at<float>(i);
		int intensity = saturate_cast<int>(256 - 256 * (value / maxValue));
		rectangle(showImage, Point(i, 256 - 1), Point((i + 1) - 1, intensity), Scalar(0, 0, 255));
	}
#endif // With_Debug

	return showImage;
}

enumFaceColorType getFaceColorType(const string &strImageName, const cv::Mat &imageSrc, const cv::Rect &rectFace)
{
	double maxColorValue = -1;
	enumFaceColorType type = Type_Color_TouBai;

	vectorContours faceContours;
	vector<Point> vectorShape;
	vectorShape.resize(4);
	vectorShape[0] = Point(rectFace.x, rectFace.y);
	vectorShape[1] = Point(rectFace.x, rectFace.y + rectFace.height);
	vectorShape[2] = Point(rectFace.x + rectFace.width, rectFace.y + rectFace.height);
	vectorShape[3] = Point(rectFace.x + rectFace.width, rectFace.y);
	faceContours.push_back(vectorShape);

	Mat imageFace(imageSrc.size(), CV_8UC1);
	Mat mask(imageSrc.size(), CV_8UC1);
	mask = 0;
	/* 这里取的是向量数据第5索引，即整个面部的正中矩形 */
	drawContours(mask, faceContours, 0, Scalar(255), -1);

	imageSrc.copyTo(imageFace, mask);
	/* 只取脸部代表颜色的关键区域 */
	Mat imageColor(imageFace, rectFace);
	if (!imageColor.data) {
		LOG(ERROR) << strImageName << ": Fail to load the image";
		return type;
	}

	Mat imageResult = getHistogramImage(strImageName, imageColor, &maxColorValue);

	const char *pStrColorString = g_colorString[0];
	if (maxColorValue >= TouBai) {
		pStrColorString = g_colorString[0];
		type = Type_Color_TouBai;
	} else if (maxColorValue >= BaiXi) {
		pStrColorString = g_colorString[1];
		type = Type_Color_BaiXi;
	} else if (maxColorValue >= ZiRan) {
		pStrColorString = g_colorString[2];
		type = Type_Color_ZiRan;
	} else if (maxColorValue >= XiaoMai) {
		pStrColorString = g_colorString[3];
		type = Type_Color_XiaoMai;
	} else if (maxColorValue >= AnChen) {
		pStrColorString = g_colorString[4];
		type = Type_Color_AnChen;
	} else {
		pStrColorString = g_colorString[5];
		type = Type_Color_YouHei;
	}

#ifdef With_Debug
	namedWindow("image original:", WINDOW_NORMAL);
	putText(imageSrc, format("%s", pStrColorString), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0, 0, 255), 3);
	imshow("image original:", imageSrc);
	namedWindow("image face pre:", WINDOW_NORMAL);
	imshow("image face pre:", imageColor);
	namedWindow("showImage", WINDOW_NORMAL);
	imshow("showImage", imageResult);
	waitKey(0);
#endif // With_Debug
	return type;
}
