#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void main()
{
	Mat srcImage = imread("faceDetest_01.jpg");
	imshow("Ô­Ê¼Í¼Ïñ", srcImage);
	waitKey(0);
}


