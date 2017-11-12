#include "opencv2/opencv.hpp"

#include "showImage.h"
#include "dftImage.h"
#include "imageProcess.h"
#include "faceDetect.h"
#include "grabcut.h"
#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"

void main()
{
	//showImage("images/3.jpg");
	//ircodeImage("3.jpg");
	//blurImage("images/scine.jpg");
	//cannyImage("images/scine.jpg");
	//imageAddWeighted("images/img1.jpg", "images/img2.jpg");
	//imageROI("images/img2.jpg", "images/logo.jpg");
	//imageSplit("images/dota_jugg.jpg", "images/dota_logo.jpg");
	//setContrastAndBright("images/dota_jugg.jpg");
	//dftImage("images/dota_jugg.jpg");
	//imageFilter("images/AC.jpg");
#if 0
	Mat image = imread("images/skin_detect.jpg", 1);
	resize(image, image, Size(1280, 1280 * image.rows / image.cols));
	imwrite("images/test.jpg", image);
#endif

#if 0 // 人脸轮廓提取方法1。有时会有少量的脏数据
	Rect rect = faceDetect("images/test.jpg");
	cout << "Get rect: " << rect;
	grabCut("images/test.jpg", rect);
#endif

#if 1 // 人脸轮廓提取方法2。会丢失一定的数据
	std::vector<std::vector<cv::Point>> contours;
	contours = faceLandmarkDetect("images/test.jpg");
	findFaceSpots("images/test.jpg", contours);
#endif
	//faceContours("images/skin_detect.jpg");
	//imageContours("images/skin_detect.jpg");
	//getFaceShape("images/skin_detect.jpg");
	
	//faceSwap();
	cv::waitKey(0);
}

