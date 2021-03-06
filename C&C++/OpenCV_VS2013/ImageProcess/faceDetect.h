#ifndef __FACE_DETECT_H__
#define __FACE_DETECT_H__

#include <opencv2/opencv.hpp>
using namespace std;

cv::Rect faceDetect(const string strFile);
void faceContours(const string strFile);
int getFaceShape(const string strFile);

#endif // __FACE_DETECT_H__