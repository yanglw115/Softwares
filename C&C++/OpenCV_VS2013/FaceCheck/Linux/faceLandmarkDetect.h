#ifndef  __FACE_LANDMARK_DETECT_H__
#define __FACE_LANDMARK_DETECT_H__

#include "faceGlobal.h"

bool faceLandmarkDetect(const string &strImageName, const cv::Mat &, vectorContours &, cv::Rect &);

#endif // ! __FACE_LANDMARK_DETECT_H__