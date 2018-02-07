
#include "faceLandmarkDetect.h"

// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

This example program shows how to find frontal human faces in an image and
estimate their pose.  The pose takes the form of 68 landmarks.  These are
points on the face such as the corners of the mouth, along the eyebrows, on
the eyes, and so forth.

The face detector we use is made using the classic Histogram of Oriented
Gradients (HOG) feature combined with a linear classifier, an image pyramid,
and sliding window detection scheme.  The pose estimator was created by
using dlib's implementation of the paper:
One Millisecond Face Alignment with an Ensemble of Regression Trees by
Vahid Kazemi and Josephine Sullivan, CVPR 2014
and was trained on the iBUG 300-W face landmark dataset (see
https://ibug.doc.ic.ac.uk/resources/facial-point-annotations/):
C. Sagonas, E. Antonakos, G, Tzimiropoulos, S. Zafeiriou, M. Pantic.
300 faces In-the-wild challenge: Database and results.
Image and Vision Computing (IMAVIS), Special Issue on Facial Landmark Localisation "In-The-Wild". 2016.
You can get the trained model file from:
http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2.
Note that the license for the iBUG 300-W dataset excludes commercial use.
So you should contact Imperial College London to find out if it's OK for
you to use this model file in a commercial product.

Also, note that you can train your own models using dlib's machine learning
tools.  See train_shape_predictor_ex.cpp to see an example.

Finally, note that the face detector is fastest when compiled with at least
SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
chip then you should enable at least SSE2 instructions.  If you are using
cmake to compile this program you can enable them by using one of the
following commands when you create the build project:
cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
This will set the appropriate compiler options for GCC, clang, Visual
Studio, or the Intel compiler.  If you are using another compiler then you
need to consult your compiler's manual to determine how to enable these
instructions.  Note that AVX is the fastest but requires a CPU from at least
2011.  SSE4 is the next fastest and is supported by most current machines.
*/

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>

#include <dlib/opencv.h>    
#include <opencv2/objdetect.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/objdetect.hpp>

#include "faceLandmarkDetect.h"

#include <windows.h>
#include <io.h>
#include <QtDebug>

using namespace dlib;

static bool g_bShapePredictorInited = false;
static shape_predictor g_sp;

const static string g_strCascadeName = ".\\resources\\data\\cascades\\lbpcascades\\lbpcascade_frontalface_improved.xml";
const static string g_strFaceLandmarks = ".\\resources\\data\\shape_predictor_68_face_landmarks.dat";
HANDLE  g_hMutex = NULL;
#define PTHREAD_MUTEX_LOCK() WaitForSingleObject(g_hMutex, INFINITE)
#define PTHREAD_MUTEX_UNLOCK() ReleaseMutex(g_hMutex)
#define INT64_T int64
#define ACCESS_F _access

static frontal_face_detector g_detector;
static cv::CascadeClassifier g_cascade;

bool faceDetect(const string &strImageName, const string &strImagePath)
{
    /* 这里加锁需要在一开始就进行，特别是要在全局对象使用前开始，否则会出混乱  */
    PTHREAD_MUTEX_LOCK();

    INT64_T tt;
    if (!g_bShapePredictorInited) {
        if (ACCESS_F(g_strFaceLandmarks.c_str(), 04)) {
            qWarning() << "File is not exist: " << QString(g_strFaceLandmarks.c_str());
            /* 这里改用goto    语句的话，gcc会报怨，并且关闭警告不太好，其实提示作用挺好 */
            PTHREAD_MUTEX_UNLOCK();
            return false;
        }
        if (ACCESS_F(g_strCascadeName.c_str(), 04)) {
            qWarning() << "File is not exist: " << QString(g_strCascadeName.c_str());
            PTHREAD_MUTEX_UNLOCK();
            return false;
        }

        /* 加载面部预测器，文件比较大，所以在共享库中全局共享 */
        tt = cv::getTickCount();
        deserialize(g_strFaceLandmarks) >> g_sp;
        if (!g_cascade.load(g_strCascadeName)) {
            qWarning() << "Load cascadde file failed!";
            PTHREAD_MUTEX_UNLOCK();
            return false;
        }
        g_bShapePredictorInited = true;
        tt = cv::getTickCount() - tt;
        qDebug() << "Init shape predictor finish, use time: " << tt * 1000 / (INT64_T)cv::getTickFrequency() << "ms";
    }

    // Make the image larger so we can detect small faces.
    //pyramid_up(img); 手动去掉

    /* 检测所有可能存在的人脸 */
    tt = cv::getTickCount();

    cv::Mat matSrc = cv::imread(strImagePath);
    if (matSrc.empty()) {
        qCritical() << "Image file is not supported: " << QString(strImagePath.c_str());
        return false;
    }
    std::vector<dlib::rectangle> dets;
    dets.resize(1);
    std::vector<cv::Rect> rectFaces;
    cv::Mat imgGray;
    cv::cvtColor(matSrc, imgGray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(imgGray, imgGray);
    g_cascade.detectMultiScale(imgGray, rectFaces, 1.1, 2, 0 | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(50, 50));

    if (rectFaces.size() <= 0) {
        qWarning() << QString(strImageName.c_str()) << ": Cannot detect face from image.";
        PTHREAD_MUTEX_UNLOCK();
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------------------
enumTypeResult faceLandmarkDetect(const string &strImageName, const cv::Mat &matSrc, vectorContours &faceContours, cv::Rect &rectOutput)
{
    /* 这里加锁需要在一开始就进行，特别是要在全局对象使用前开始，否则会出混乱  */
	PTHREAD_MUTEX_LOCK();

	INT64_T tt;
	if (!g_bShapePredictorInited) {
		if (ACCESS_F(g_strFaceLandmarks.c_str(), 04)) {
            qWarning() << "File is not exist: " << QString(g_strFaceLandmarks.c_str());
			/* 这里改用goto    语句的话，gcc会报怨，并且关闭警告不太好，其实提示作用挺好 */
			PTHREAD_MUTEX_UNLOCK();
            return TYPE_ERROR;
		}
		if (ACCESS_F(g_strCascadeName.c_str(), 04)) {
            qWarning() << "File is not exist: " << QString(g_strCascadeName.c_str());
			PTHREAD_MUTEX_UNLOCK();
            return TYPE_ERROR;
		}

		/* 加载面部预测器，文件比较大，所以在共享库中全局共享 */
		tt = cv::getTickCount();
		deserialize(g_strFaceLandmarks) >> g_sp;
		if (!g_cascade.load(g_strCascadeName)) {
            qWarning() << "Load cascadde file failed!";
			PTHREAD_MUTEX_UNLOCK();
            return TYPE_ERROR;
		}
		g_bShapePredictorInited = true;
		tt = cv::getTickCount() - tt;
        qDebug() << "Init shape predictor finish, use time: " << tt * 1000 / (INT64_T)cv::getTickFrequency() << "ms";
	}

	// Make the image larger so we can detect small faces.
	//pyramid_up(img); 手动去掉

    if (matSrc.empty()) {
        qCritical() << "Image file is not supported: " << strImageName.c_str();
        return TYPE_ERROR;
    }

	/* 检测所有可能存在的人脸 */
	tt = cv::getTickCount();

	std::vector<dlib::rectangle> dets;
	dets.resize(1);
	std::vector<cv::Rect> rectFaces;
	cv::Mat imgGray;
	cv::cvtColor(matSrc, imgGray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(imgGray, imgGray);
	g_cascade.detectMultiScale(imgGray, rectFaces, 1.1, 2, 0 | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(50, 50));

	if (rectFaces.size() <= 0) {
        qWarning() << QString(strImageName.c_str()) << ": Cannot detect face from image.";
		PTHREAD_MUTEX_UNLOCK();
        return TYPE_FALSE;
	}

	int indexRect = 0;
	cv::Rect rectMax;
	for (uint i = 0; i < rectFaces.size(); ++i) {
		if ((rectMax.width * rectMax.height) < (rectFaces[i].width * rectFaces[i].height)) {
			indexRect = i;
			rectMax = rectFaces[i];
		}
	}
    qDebug() << QString(strImageName.c_str()) << ": Max face rect: " << "(" << rectFaces[indexRect].width << ", " << rectFaces[indexRect].height << ")";

    dets[0].set_left(rectMax.x);
    dets[0].set_top(rectMax.y);
    dets[0].set_right(rectMax.x + rectMax.width);
    dets[0].set_bottom(rectMax.y + rectMax.height);

	tt = cv::getTickCount() - tt;
    qDebug() << QString(strImageName.c_str()) << ": Detect face use time: " << tt * 1000 / (INT64_T)cv::getTickFrequency() << "ms";
	
	if (dets.size() <= 0) {
		PTHREAD_MUTEX_UNLOCK();
        return TYPE_FALSE;
	}

	/* 获取面部形状，这里用的是68点进行描述。并且认为只有一个人脸被检测到，也只取第1个检测到的人脸数据 */
	std::vector<cv::Point> vectorShape;

	tt = cv::getTickCount();

	full_object_detection shape = g_sp(cv_image<rgb_pixel>(matSrc), dets[0]);

	tt = cv::getTickCount() - tt;
    qDebug() << QString(strImageName.c_str()) << ": Shape predictor use time: " << tt * 1000 / (INT64_T)cv::getTickFrequency() << "ms";

	if (shape.num_parts() < 68) {
        qWarning() << QString(strImageName.c_str()) << ": Get face shape points failed, shape number parts: " << shape.num_parts();
		PTHREAD_MUTEX_UNLOCK();
        return TYPE_FALSE;
	}

#ifdef With_Debug_Show
	cv::Mat matDebug;
	matSrc.copyTo(matDebug);

	for (uint i = 0; i < shape.num_parts(); ++i) {
        cv::circle(matDebug, cv::Point(shape.part(i).x(), shape.part(i).y()), 5, cv::Scalar(0, 255, 255), 2, cv::FILLED);
	}
	cv::namedWindow("68点区域", cv::WINDOW_NORMAL);
	cv::imshow("68点区域", matDebug);
	cv::waitKey();
#endif // With_Debug_Show
	faceContours.clear();
	tt = cv::getTickCount();

	/* 左脸(本人自己角度，非观察者角度) */
	vectorShape.push_back(cv::Point(shape.part(46).x(), shape.part(46).y() + 20));
	vectorShape.push_back(cv::Point(shape.part(42).x(), shape.part(46).y() + 20));
	//vectorShape.push_back(cv::Point((shape.part(42).x() + shape.part(47).x()) / 2, shape.part(15).y()));
	//vectorShape.push_back(cv::Point(shape.part(47).x() - 20, shape.part(14).y()));
	vectorShape.push_back(cv::Point(shape.part(11).x() - 30, shape.part(12).y()));
	vectorShape.push_back(cv::Point(shape.part(11).x() - 10, shape.part(11).y()));
	vectorShape.push_back(cv::Point(shape.part(12).x() - 10, shape.part(12).y()));
	vectorShape.push_back(cv::Point(shape.part(13).x() - 10, shape.part(13).y()));
	vectorShape.push_back(cv::Point(shape.part(14).x() - 10, shape.part(14).y()));
	vectorShape.push_back(cv::Point(shape.part(15).x() - 10, shape.part(15).y()));
	vectorShape.push_back(cv::Point(shape.part(16).x() - 10, shape.part(16).y()));
	faceContours.push_back(vectorShape);

	/* 右脸 */
	vectorShape.clear();
	vectorShape.push_back(cv::Point(shape.part(41).x(), shape.part(41).y() + 20));
	vectorShape.push_back(cv::Point(shape.part(39).x(), shape.part(41).y() + 20));
	vectorShape.push_back(cv::Point((shape.part(39).x() + shape.part(40).x()) / 2, (shape.part(2).y() + shape.part(1).y()) / 2));
	vectorShape.push_back(cv::Point(shape.part(40).x(), shape.part(2).y()));
	vectorShape.push_back(cv::Point(shape.part(41).x() > (shape.part(5).x() + 10)? shape.part(41).x(): (shape.part(5).x() + 10), shape.part(4).y()));
	vectorShape.push_back(cv::Point(shape.part(5).x() + 10, shape.part(5).y()));
	vectorShape.push_back(cv::Point(shape.part(4).x() + 10, shape.part(4).y()));
	vectorShape.push_back(cv::Point(shape.part(3).x() + 10, shape.part(3).y()));
	vectorShape.push_back(cv::Point(shape.part(2).x() + 10, shape.part(2).y()));
	vectorShape.push_back(cv::Point(shape.part(1).x() + 10, shape.part(1).y()));
	vectorShape.push_back(cv::Point(shape.part(0).x() + 10, shape.part(0).y()));
	faceContours.push_back(vectorShape);

	/* 上额 */
	vectorShape.clear();
	vectorShape.push_back(cv::Point(shape.part(18).x(), shape.part(19).y() - 120));
	vectorShape.push_back(cv::Point(shape.part(17).x(), shape.part(19).y() - 20));
	vectorShape.push_back(cv::Point(shape.part(25).x(), shape.part(19).y() - 20));
	vectorShape.push_back(cv::Point(shape.part(24).x(), shape.part(19).y() - 120));
	faceContours.push_back(vectorShape);

	/* 下额 */
	vectorShape.clear();
	vectorShape.push_back(cv::Point(shape.part(5).x(), shape.part(5).y()));
	vectorShape.push_back(cv::Point(shape.part(6).x(), shape.part(6).y()));
	vectorShape.push_back(cv::Point(shape.part(7).x(), shape.part(7).y()));
	vectorShape.push_back(cv::Point(shape.part(8).x(), shape.part(8).y()));
	vectorShape.push_back(cv::Point(shape.part(9).x(), shape.part(9).y()));
	vectorShape.push_back(cv::Point(shape.part(10).x(), shape.part(10).y()));
	vectorShape.push_back(cv::Point(shape.part(11).x(), shape.part(11).y()));
	faceContours.push_back(vectorShape);

	/* 鼻子 */
	vectorShape.clear();
	vectorShape.push_back(cv::Point(shape.part(27).x(), shape.part(27).y()));
	vectorShape.push_back(cv::Point(shape.part(31).x(), shape.part(30).y()));
	vectorShape.push_back(cv::Point(shape.part(35).x(), shape.part(30).y()));
	faceContours.push_back(vectorShape);

	/* 取皮肤的一块矩形区域，用于粗糙度检测 */
	vectorShape.clear();
	vectorShape.push_back(cv::Point(shape.part(4).x(), shape.part(3).y()));
	vectorShape.push_back(cv::Point(shape.part(4).x(), shape.part(1).y()));
	vectorShape.push_back(cv::Point(shape.part(40).x(), shape.part(1).y()));
	vectorShape.push_back(cv::Point(shape.part(40).x(), shape.part(3).y()));
	faceContours.push_back(vectorShape);

	/* 整个人脸正中部分，用于取面部肤色(同下面类似，只是这个取的比较全面；虽然有鼻子等数据，但是最终用的是统计图最大值部分) */
	/* 
		vectorShape.resize(4);
		vectorShape[0] = cv::Point(shape.part(4).x(), shape.part(4).y());
		vectorShape[1] = cv::Point(shape.part(12).x(), shape.part(4).y());
		vectorShape[2] = cv::Point(shape.part(12).x(), shape.part(46).y());
		vectorShape[3] = cv::Point(shape.part(4).x(), shape.part(46).y());
		faceContours.push_back(vectorShape);
	*/
	// 这里还是使用rect保存数据，不然调试显示的时候不太直观
	rectOutput = cv::Rect(cv::Point(shape.part(4).x(), shape.part(4).y()), cv::Point(shape.part(12).x(), shape.part(46).y()));

	tt = cv::getTickCount() - tt;
    qDebug() << QString(strImageName.c_str()) << ": Get face every parts use time: " << tt * 1000 / (INT64_T)cv::getTickFrequency() << "ms";

	PTHREAD_MUTEX_UNLOCK();

#ifdef With_Debug_Show
	cv::Mat matTest;
	matSrc.copyTo(matTest);
					
	//cv::drawContours(matTest, contours, 0, cv::Scalar(0, 0, 0), 3);
	cv::drawContours(matTest, faceContours, -1, cv::Scalar(0, 0, 0), cv::LINE_8); // CV_FILLED
	cv::namedWindow("选取的脸部区域", cv::WINDOW_NORMAL);
	cv::imshow("选取的脸部区域", matTest);
	cv::waitKey();
#endif
    return TYPE_OK;
}
// ----------------------------------------------------------------------------------------

