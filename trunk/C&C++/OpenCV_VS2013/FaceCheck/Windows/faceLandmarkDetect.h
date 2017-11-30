#ifndef  __FACE_LANDMARK_DETECT_H__
#define __FACE_LANDMARK_DETECT_H__

#include "faceGlobal.h"

#ifdef With_Debug
bool faceLandmarkDetect(const string &strImageName, cv::Mat &, vectorContours &);
#else
bool faceLandmarkDetect(const string &strImageName, const cv::Mat &, vectorContours &);
#endif

#endif // ! __FACE_LANDMARK_DETECT_H__