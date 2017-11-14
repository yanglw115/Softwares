#include "opencv2/opencv.hpp"

#include "showImage.h"
#include "dftImage.h"
#include "imageProcess.h"
#include "faceDetect.h"
#include "grabcut.h"
#include "faceLandmarkDetect.h"
#include "faceSpotsDetect.h"
#include "faceColor.h"

//const char *g_pStrImageFile = "images/baby.jpg";
//const char *g_pStrImageFile = "images/black.JPG";
const char *g_pStrImageFile = "images/zhuchong.jpg";
//const char *g_pStrImageFile = "images/yangliwei.jpg";
//const char *g_pStrImageFile = "images/zhouchen.jpg";
//const char *g_pStrImageFile = "images/10329771ea74d4765c6a40e981e5d925.jpg";


//const char *g_pStrImageFile = "images/透白.png";
//const char *g_pStrImageFile = "images/白皙.png";
//const char *g_pStrImageFile = "images/自然.png";
//const char *g_pStrImageFile = "images/小麦.png";
//const char *g_pStrImageFile = "images/暗沉.png";
//const char *g_pStrImageFile = "images/黝黑.png";

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
	//faceContours("images/skin_detect.jpg");
	//imageContours("images/skin_detect.jpg");
	//getFaceShape("images/skin_detect.jpg");
	//faceSwap();
#if 0
	cv::Mat image = cv::imread("images/IMG_20171113_113010.jpg", 1);
	resize(image, image, cv::Size(1280, 1280 * image.rows / image.cols));
	imwrite("images/zhouchen.jpg", image);
#endif

#if 0 // 人脸轮廓提取方法1。有时会有少量的脏数据
	Rect rect = faceDetect("images/test.jpg");
	cout << "Get rect: " << rect;
	grabCut("images/test.jpg", rect);
#endif

	std::vector<std::vector<cv::Point>> contours;
//face spots detected.
#if 1 // 人脸轮廓提取方法2。会丢失一定的数据
	contours = faceLandmarkDetect(g_pStrImageFile);
	//findFaceSpots(g_pStrImageFile, contours);
#endif

// face color calc
#if 1
	/* RGB 透白：232/白皙：217/自然：200/小麦：161/暗沉：120/黝黑：81 */
	int colorValue = getFaceColorValue(g_pStrImageFile, contours);
#endif
	cv::waitKey(0);
}

