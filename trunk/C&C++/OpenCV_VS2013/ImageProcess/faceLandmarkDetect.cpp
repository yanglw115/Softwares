
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
#include <opencv2/opencv.hpp>    
#include <opencv2/objdetect.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/objdetect.hpp>

using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------
std::vector<std::vector<cv::Point>> faceLandmarkDetect(const string strFile)
{
	std::vector<std::vector<cv::Point>> contours;
	double t;
	try
	{
		t = (double)cv::getTickCount();
		
		frontal_face_detector detector = get_frontal_face_detector();
		shape_predictor sp;
		deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
		t = (double)cv::getTickCount() - t;
		cout << "提取market所用时间：" << t * 1000 / cv::getTickFrequency() << "ms" << endl;

		t = (double)cv::getTickCount();
	
		cout << "processing image " << strFile << endl;
		array2d<rgb_pixel> img;
		t = (double)cv::getTickCount();
		load_image(img, strFile);

		t = (double)cv::getTickCount() - t;
		cout << "load image所用时间：" << t * 1000 / cv::getTickFrequency() << "ms" << endl;
		// Make the image larger so we can detect small faces.
		//pyramid_up(img); 手动去掉

		// Now tell the face detector to give us a list of bounding boxes
		// around all the faces in the image.
		t = (double)cv::getTickCount();
		std::vector<dlib::rectangle> dets = detector(img);

		t = (double)cv::getTickCount() - t;
		cout << "Detect image所用时间：" << t * 1000 / cv::getTickFrequency() << "ms" << endl;

		cout << "Number of faces detected: " << dets.size() << endl;

		// Now we will go ask the shape_predictor to tell us the pose of
		// each face we detected.
		std::vector<full_object_detection> shapes;
		std::vector<cv::Point> vectorShape;

		for (unsigned long j = 0; j < dets.size(); ++j)
		{
			full_object_detection shape = sp(img, dets[j]);
			cout << "number of parts: " << shape.num_parts() << endl;
			cout << "pixel position of first part:  " << shape.part(0) << endl;
			cout << "pixel position of second part: " << shape.part(1) << endl;
			// You get the idea, you can get all the face part locations if
			// you want them.  Here we just store them in shapes so we can
			// put them on the screen.
			shapes.push_back(shape);

			vectorShape.resize(13);
			vectorShape[0] = cv::Point(shape.part(36).x() - 20, shape.part(36).y() + 40);
			vectorShape[1] = cv::Point(shape.part(41).x(), shape.part(41).y() + 20);
			vectorShape[2] = cv::Point(shape.part(40).x(), shape.part(40).y() + 20);
			vectorShape[3] = cv::Point(shape.part(39).x(), shape.part(39).y() + 20);
			vectorShape[4] = cv::Point((shape.part(39).x() + shape.part(40).x()) / 2, shape.part(1).y());
			vectorShape[5] = cv::Point(shape.part(40).x(), shape.part(2).y());
			vectorShape[6] = cv::Point(shape.part(41).x(), shape.part(4).y());
			vectorShape[7] = cv::Point(shape.part(5).x() + 20, shape.part(5).y());
			vectorShape[8] = cv::Point(shape.part(4).x() + 20, shape.part(4).y());
			vectorShape[9] = cv::Point(shape.part(3).x() + 20, shape.part(3).y());
			vectorShape[10] = cv::Point(shape.part(2).x() + 20, shape.part(2).y());
			vectorShape[11] = cv::Point(shape.part(1).x() + 20, shape.part(1).y());
			vectorShape[12] = cv::Point(shape.part(0).x() + 20, shape.part(0).y());

			contours.push_back(vectorShape);


			vectorShape.resize(13);
			vectorShape[0] = cv::Point(shape.part(45).x() - 20, shape.part(45).y() + 40);
			vectorShape[1] = cv::Point(shape.part(46).x(), shape.part(46).y() + 20);
			vectorShape[2] = cv::Point(shape.part(47).x(), shape.part(47).y() + 20);
			vectorShape[3] = cv::Point(shape.part(42).x(), shape.part(42).y() + 20);
			vectorShape[4] = cv::Point((shape.part(42).x() + shape.part(47).x()) / 2, shape.part(15).y());
			vectorShape[5] = cv::Point(shape.part(47).x() - 20, shape.part(14).y());
			vectorShape[6] = cv::Point(shape.part(11).x() - 40, shape.part(12).y());
			vectorShape[7] = cv::Point(shape.part(11).x() - 20, shape.part(11).y());
			vectorShape[8] = cv::Point(shape.part(12).x() - 20, shape.part(12).y());
			vectorShape[9] = cv::Point(shape.part(13).x() - 20, shape.part(13).y());
			vectorShape[10] = cv::Point(shape.part(14).x() - 20, shape.part(14).y());
			vectorShape[11] = cv::Point(shape.part(15).x() - 20, shape.part(15).y());
			vectorShape[12] = cv::Point(shape.part(16).x() - 20, shape.part(16).y());

			contours.push_back(vectorShape);

			vectorShape.resize(4);
			vectorShape[0] = cv::Point(shape.part(4).x(), shape.part(4).y());
			vectorShape[1] = cv::Point(shape.part(12).x(), shape.part(4).y());
			vectorShape[2] = cv::Point(shape.part(12).x(), shape.part(46).y());
			vectorShape[3] = cv::Point(shape.part(4).x(), shape.part(46).y());

			contours.push_back(vectorShape);

		}

		cv::Mat matTest = cv::imread(strFile, 1);
		//cv::imshow("orig: ", matTest);
		t = (double)cv::getTickCount();
		cout << "rows: " << matTest.rows << endl;
		cout << "cols: " << matTest.cols << endl;

		/* 使用轮廓+mask的方法将图抠出来 */
		cv::Mat test;
		matTest.copyTo(test);
		cv::cvtColor(test, test, cv::COLOR_RGB2GRAY);
					
		//cv::drawContours(matTest, contours, 0, cv::Scalar(0, 0, 0), 3);
		cv::drawContours(matTest, contours, -1, cv::Scalar(0, 0, 0), CV_FILLED);


		t = (double)cv::getTickCount() - t;
		cout << "Set image所用时间：" << t * 1000 / cv::getTickFrequency() << "ms" << endl;
	}
	catch (exception& e)
	{
		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
	}
	return contours;
}
// ----------------------------------------------------------------------------------------

