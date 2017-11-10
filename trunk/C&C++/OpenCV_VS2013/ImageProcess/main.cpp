#include "opencv2/opencv.hpp"

#include "showImage.h"
#include "dftImage.h"
#include "imageProcess.h"
#include "faceDetect.h"
#include "grabcut.h"

using namespace cv;

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
	//faceDetect("images/skin_detect.jpg");
	//faceContours("images/skin_detect.jpg");
	//imageContours("images/skin_detect.jpg");
	getFaceShape("images/skin_detect.jpg");
	//grabCut("images/faceDetest_01.jpg");
	waitKey(0);
}

