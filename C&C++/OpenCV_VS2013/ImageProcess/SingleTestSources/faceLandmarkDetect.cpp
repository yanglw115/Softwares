
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
#include <Windows.h>  

using namespace dlib;
using namespace std;
//using namespace cv;

class 高精度计时
{
public:
	高精度计时(void);
	~高精度计时(void);//析构函数  

private:
	LARGE_INTEGER 开始时间;

	LARGE_INTEGER 结束时间;

	LARGE_INTEGER CPU频率;

public:
	double 间隔;

public:
	void 开始();
	void 结束();
};



高精度计时::高精度计时(void)
{
	QueryPerformanceFrequency(&CPU频率);
}

高精度计时::~高精度计时(void)
{
}

void 高精度计时::开始()
{
	QueryPerformanceCounter(&开始时间);
}

void 高精度计时::结束()
{
	QueryPerformanceCounter(&结束时间);

	间隔 = ((double)结束时间.QuadPart - (double)开始时间.QuadPart) / (double)CPU频率.QuadPart;

}

// ----------------------------------------------------------------------------------------
std::vector<cv::Point> faceLandmarkDetect(const string strFile)
#if 1
{
	std::vector<std::vector<cv::Point>> contours;
	double t;
	try
	{
		// This example takes in a shape model file and then a list of images to
		// process.  We will take these filenames in as command line arguments.
		// Dlib comes with example images in the examples/faces folder so give
		// those as arguments to this program.
		if (0)
		{
			cout << "Call this program like this:" << endl;
			cout << "./face_landmark_detection_ex shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
			cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
			cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
			return 0;
		}

		// We need a face detector.  We will use this to get bounding boxes for
		// each face in an image.
		frontal_face_detector detector = get_frontal_face_detector();
		// And we also need a shape_predictor.  This is the tool that will predict face
		// landmark positions given an image and face bounding box.  Here we are just
		// loading the model from the shape_predictor_68_face_landmarks.dat file you gave
		// as a command line argument.
		t = (double)cv::getTickCount();
		
		shape_predictor sp;
		deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
		t = (double)cv::getTickCount() - t;
		cout << "提取market所用时间：" << t * 1000 / cv::getTickFrequency() << "ms" << endl;

		t = (double)cv::getTickCount();

		image_window win, win_faces;
		// Loop over all the images provided on the command line.
		//for (int i = 2; i < argc; ++i)
		if (1)
		{
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
			std::vector<cv::Point> vectShape;
			vectShape.resize(19);

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
				for (int i = 0; i < 17; ++i) {
					vectShape[i].x = shape.part(i).x();
					vectShape[i].y = shape.part(i).y();
				}

				vectShape[17].x = shape.part(24).x();
				vectShape[17].y = shape.part(24).y() - 20;
				vectShape[18].x = shape.part(19).x();
				vectShape[18].y = shape.part(19).y() - 20;

				cout << "shape: " << vectShape << endl;

			}
#if 1
			cv::Mat matTest = cv::imread(strFile, 1);
			cv::imshow("orig: ", matTest);
			t = (double)cv::getTickCount();
			cout << "rows: " << matTest.rows << endl;
			cout << "cols: " << matTest.cols << endl;
#if 0
			for (int X = 0; X < matTest.rows;  ++X) {
				for (int Y = 0; Y < matTest.cols; ++Y) {
					//cout << "(x, y): " << X << ", " << Y << endl;

					int j = 18;
					for (int i = 0; i < 19; ++i) {
						if ((((vectShape[i].y < X) && (vectShape[j].y >= X)) || ((vectShape[j].y < X) && (vectShape[i].y >= X))) 
							&& (vectShape[i].x + (X - vectShape[i].y) / (vectShape[j].y - vectShape[i].y) * (vectShape[j].x - vectShape[i].x) < Y)) {
							matTest.at<int>(X, Y) = (0, 0, 0);

						}
					}
				}
			}
#else
			/* 使用轮廓+mask的方法将图抠出来 */
			cv::Mat test;
			matTest.copyTo(test);
			cv::cvtColor(test, test, cv::COLOR_RGB2GRAY);
			
			contours.push_back(vectShape);
					
			//cv::drawContours(matTest, contours, 0, cv::Scalar(0, 0, 0), 3);
			cv::drawContours(matTest, contours, 0, cv::Scalar(0, 0, 0), CV_FILLED);
#endif

			t = (double)cv::getTickCount() - t;
			cout << "Set image所用时间：" << t * 1000 / cv::getTickFrequency() << "ms" << endl;
			cv::namedWindow("re", cv::WINDOW_NORMAL);
			cv::imshow("re", matTest);
			cv::waitKey();
#endif

			// Now let's view our face poses on the screen.
			win.clear_overlay();
			win.set_image(img);
			win.add_overlay(render_face_detections(shapes));

			// We can also extract copies of each face that are cropped, rotated upright,
			// and scaled to a standard size as shown here:
			dlib::array<array2d<rgb_pixel> > face_chips;
			extract_image_chips(img, get_face_chip_details(shapes), face_chips);
			win_faces.set_image(tile_images(face_chips));

			cout << "Hit enter to process the next image..." << endl;
			cin.get();
		}
	}
	catch (exception& e)
	{
		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
	}
	return 0;
}
#else
{


		高精度计时 时间;
		try
		{
			时间.开始();
			cv::VideoCapture cap(0);
			if (!cap.isOpened())
			{
				cerr << "Unable to connect to camera" << endl;
				return 1;
			}

			//image_window win;    

			// Load face detection and pose estimation models.    
			frontal_face_detector detector = get_frontal_face_detector();
			shape_predictor pose_model;
			deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;
			时间.结束();
			float 初始化时间 = 时间.间隔;
			DWORD k = ::GetTickCount(); //获取毫秒级数目  
										// Grab and process frames until the main window is closed by the user.    
			while (cv::waitKey(30) != 27)
			{
				// Grab a frame    
				cv::Mat temp;
				cap >> temp;

				cv::putText(temp, "Initial Time/ms : " + to_string(初始化时间 * 1000), cv::Point(20, 380), 5, 2, cvScalar(0, 0, 255));


				cv_image<bgr_pixel> cimg(temp);
				// Detect faces     
				//  DWORD k = ::GetTickCount(); //获取毫秒级数目  

				std::vector<rectangle> faces = detector(cimg);
				时间.结束();

				cout << "检测人脸部分耗时:" << 时间.间隔 * 1000 << endl;
				cv::putText(temp, "Face Detector/ms : " + to_string(时间.间隔 * 1000), cv::Point(20, 420), 5, 2, cvScalar(0, 0, 255));

				//  cout << (::GetTickCount() - k) << endl;  
				// Find the pose of each face.    
				std::vector<full_object_detection> shapes;
				//  k = ::GetTickCount();  
				时间.开始();
				for (unsigned long i = 0; i < faces.size(); ++i)
					shapes.push_back(pose_model(cimg, faces[i]));

				if (!shapes.empty()) {
					for (int j = 0; j < shapes.size(); j++) {
						for (int i = 0; i < 68; i++) {
							circle(temp, cvPoint(shapes[j].part(i).x(), shapes[j].part(i).y()), 2, cvScalar(255, 0, 0), -1);
							//  shapes[0].part(i).x();//68个    
						}
					}

				}
				时间.结束();
				cout << "剩余部分的总时间:" << 时间.间隔 * 1000 << endl << endl << endl;
				cv::putText(temp, "The rest/ms: " + to_string(时间.间隔 * 1000), cv::Point(20, 460), 5, 2, cv::Scalar(0, 0, 255));
				//Display it all on the screen    
				imshow("Dlib特征点", temp);

			}
		}
		catch (serialization_error& e)
		{
			cout << "You need dlib's default face landmarking model file to run this example." << endl;
			cout << "You can get it from the following URL: " << endl;
			cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
			cout << endl << e.what() << endl;
			system("pause");
		}
		catch (exception& e)
		{
			cout << e.what() << endl;
		}

}
#endif
// ----------------------------------------------------------------------------------------

