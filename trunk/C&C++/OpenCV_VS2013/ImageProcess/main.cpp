#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void main()
{
	Mat srcImage = imread("faceDetest_01.jpg");
	imshow("ԭʼͼ��", srcImage);
	waitKey(0);
}


