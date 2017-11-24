#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "faceDetect.h"

using namespace cv;
static string g_strCascadeName = "./build/data/haarcascades/haarcascade_frontalface_alt.xml";
//static string g_strCascadeName = "./build/data/haarcascades/haarcascade_frontalcatface.xml";
//static string g_strCascadeName = "./build/data/haarcascades/haarcascade_frontalcatface_extended.xml";
//static string g_strCascadeName = "./build/data/haarcascades/haarcascade_frontalface_alt_tree.xml";
//static string g_strCascadeName = "./build/data/haarcascades/haarcascade_frontalface_alt2.xml";
//static string g_strCascadeName = "./build/data/haarcascades/haarcascade_frontalface_default.xml";
//static string g_strCascadeName = "./build/data/haarcascades/haarcascade_licence_plate_rus_16stages.xml";
//static string g_strCascadeName = "./build/data/haarcascades/haarcascade_russian_plate_number.xml";
//static string g_strCascadeName = "./build/data/haarcascades/haarcascade_lefteye_2splits.xml";
//static string g_strCascadeName = "./build/data/haarcascades/haarcascade_righteye_2splits.xml";
static string g_strNestedCacadeName = "";// "./build/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

static Rect detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale, bool bTryFlip);

Rect faceDetect(const string strFile)
{
	Mat frame, image;
	CascadeClassifier cascade, nestedCascade;
	double scale;
	bool bTryFlip;
	Rect rectReturn;

	scale = 1;
	if (scale < 1)
		scale = 1;
	bTryFlip = false;

	if (!g_strNestedCacadeName.size() || !nestedCascade.load(g_strNestedCacadeName)) {
		cout << "WARNING: Could not load classifier cascade for nested objects" << endl;
	}
	if (!cascade.load(g_strCascadeName)) {
		cout << "ERROR: Could not load classifier cascade" << endl;
		return Rect();
	}

    if (strFile.size()) {
		image = imread(strFile, 1);
		if (image.empty()) {
			cout << "Could not read " << strFile << endl;
			return Rect();
		}
	}

	//resize(image, image, Size(800, 700));

	cout << "Detecting face(s) in " << strFile << endl;
	if (!image.empty())	{
		rectReturn = detectAndDraw(image, cascade, nestedCascade, scale, bTryFlip);
		waitKey(0);
	} else {
		cout << "Input image is empty!" << endl;
		return Rect();
	}
	return rectReturn;
}

Rect detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale, bool bTryFlip)
{
	double t = 0;
	vector<Rect> faces, faces2;
	Rect rectReturn;
	const static Scalar colors[] =
	{
		Scalar(255, 0, 0),
		Scalar(255, 128, 0),
		Scalar(255, 255, 0),
		Scalar(0, 255, 0),
		Scalar(0, 128, 255),
		Scalar(0, 255, 255),
		Scalar(0, 0, 255),
		Scalar(255, 0, 255)
	};
	Mat gray, smallImg;
	/* 先转换为灰度图 */
	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;
	/* 图像缩放 */
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
	/* 均衡化直方图 */
	equalizeHist(smallImg, smallImg);
	t = (double)getTickCount();
	/* 使用级联分类器进行人脸的识别 */
	cascade.detectMultiScale(smallImg, faces,
		1.1, 2, 0
		//|CASCADE_FIND_BIGGEST_OBJECT
		//|CASCADE_DO_ROUGH_SEARCH
		| CASCADE_SCALE_IMAGE,
		Size(30, 30));
	if (bTryFlip) {
		flip(smallImg, smallImg, 1);
		cascade.detectMultiScale(smallImg, faces2,
			1.1, 2, 0
			//|CASCADE_FIND_BIGGEST_OBJECT
			//|CASCADE_DO_ROUGH_SEARCH
			| CASCADE_SCALE_IMAGE,
			Size(30, 30));
		for (vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r) {
			faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
		}
	}
	t = (double)getTickCount() - t;
	cout << "detection time = " <<  t * 1000 / getTickFrequency() << "ms" << endl;
	for (size_t i = 0; i < faces.size(); i++) {
		Rect r = faces[i];
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i % 8];
		int radius;
		double aspect_ratio = (double)r.width / r.height;
		if (0) {//(0.75 < aspect_ratio && aspect_ratio < 1.3) {
			center.x = cvRound((r.x + r.width*0.5)*scale);
			center.y = cvRound((r.y + r.height*0.5)*scale);
			radius = cvRound((r.width + r.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		} else {
			rectReturn =  Rect(cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
				cvPoint(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)));
			rectangle(img, rectReturn,
				color, 3, 8, 0);
		}
		if (nestedCascade.empty())
			continue;
		smallImgROI = smallImg(r);
		nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CASCADE_FIND_BIGGEST_OBJECT
			//|CASCADE_DO_ROUGH_SEARCH
			//|CASCADE_DO_CANNY_PRUNING
			| CASCADE_SCALE_IMAGE,
			Size(30, 30));
		for (size_t j = 0; j < nestedObjects.size(); j++) {
			Rect nr = nestedObjects[j];
			center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
			center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
			radius = cvRound((nr.width + nr.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
	}
	namedWindow("result", WINDOW_NORMAL);
	imshow("result", img);
	waitKey();
	return rectReturn;
}

void faceContours(const string strFile)
{
	Mat srcImage = imread(strFile, 0);
	resize(srcImage, srcImage, Size(800, 700));
	imshow("原始图像", srcImage);

	Mat dstImage = Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC3);
	srcImage = srcImage > 119;
	imshow("取阈值后的原始图：", srcImage);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;


	//findContours(srcImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	findContours(srcImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);

	int index = 0;
	for (; index >= 0; index = hierarchy[index][0]) {
		//Scalar color(rand() & 255, rand() & 255, rand() & 255);
		Scalar color(0, 255, 0);
		//drawContours(dstImage, contours, index, color, FILLED, 8, hierarchy);
		drawContours(dstImage, contours, index, color, 1, 8, hierarchy);
	}

	imshow("找到轮廓之后的图", dstImage);
}

/*******************************************/

// Function Headers
static void detectAndDisplay(Mat frame);

// Global variables
// Copy this file from opencv/data/haarscascades to target folder
string face_cascade_name = "./build/data/haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
int filenumber; // Number of file to be saved
string filename;

// Function main
/* 也达不到想要的效果，还是一个比较大的轮廓，不仅包括面部 */
int getFaceShape(const string strFile)
{
	// Load the cascade
	if (!face_cascade.load(face_cascade_name)){
		printf("--(!)Error loading\n");
		return (-1);
	}

	// Read the image file
	Mat frame = imread(strFile);
	resize(frame, frame, Size(600, 800));

	// Apply the classifier to the frame
	if (!frame.empty()){
		detectAndDisplay(frame);
	}
	else{
		printf(" --(!) No captured frame -- Break!");
		//break;
	}

	while (1) {
		int c = waitKey(10);

		if (27 == char(c)){
			break;
		}
	}

	return 0;
}

// Function detectAndDisplay
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;
	Mat crop;
	Mat res;
	Mat gray;
	string text;
	stringstream sstm;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	// Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// Set Region of Interest
	cv::Rect roi_b;
	cv::Rect roi_c;

	size_t ic = 0; // ic is index of current element
	int ac = 0; // ac is area of current element

	size_t ib = 0; // ib is index of biggest element
	int ab = 0; // ab is area of biggest element

	for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)

	{
		roi_c.x = faces[ic].x;
		roi_c.y = faces[ic].y;
		roi_c.width = (faces[ic].width);
		roi_c.height = (faces[ic].height);

		ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)

		roi_b.x = faces[ib].x;
		roi_b.y = faces[ib].y;
		roi_b.width = (faces[ib].width);
		roi_b.height = (faces[ib].height);

		ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element

		if (ac > ab)
		{
			ib = ic;
			roi_b.x = faces[ib].x;
			roi_b.y = faces[ib].y;
			roi_b.width = (faces[ib].width);
			roi_b.height = (faces[ib].height);
		}

		crop = frame(roi_b);
		resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR); // This will be needed later while saving images
		cvtColor(crop, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale

		// Form a filename
		filename = "";
		stringstream ssfn;
		ssfn << filenumber << ".jpg";
		filename = ssfn.str();
		filenumber++;

		imwrite(filename, gray);

		Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window
		Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
		rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
	}

	// Show image
	sstm << "Crop area size: " << roi_b.width << "x" << roi_b.height << " Filename: " << filename;
	text = sstm.str();

	putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
	imshow("original", frame);

	if (!crop.empty())
	{
		imshow("detected", crop);
	}
	else
		destroyWindow("detected");
}