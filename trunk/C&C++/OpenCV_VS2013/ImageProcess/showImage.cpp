
#include "showImage.h"
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;


void showImage(string strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("原始图像", srcImage);
	waitKey(0); // 0代表无限等待，单位是ms
}

void ircodeImage(string strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("原始图像", srcImage);

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat dstImage;
	erode(srcImage, dstImage, element); // 图像进行腐蚀
	imshow("显示腐蚀效果图", dstImage);
	waitKey(0);
}