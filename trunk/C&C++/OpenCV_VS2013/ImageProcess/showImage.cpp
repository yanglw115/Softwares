
#include "showImage.h"
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;


void showImage(string strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("ԭʼͼ��", srcImage);
	waitKey(0); // 0�������޵ȴ�����λ��ms
}

void ircodeImage(string strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("ԭʼͼ��", srcImage);

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat dstImage;
	erode(srcImage, dstImage, element); // ͼ����и�ʴ
	imshow("��ʾ��ʴЧ��ͼ", dstImage);
	waitKey(0);
}