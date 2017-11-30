#ifndef __FACE_SPOTS_DETECT_H__
#define __FACE_SPOTS_DETECT_H__

#include "faceGlobal.h"

#ifdef With_Debug
bool findFaceSpots(const string &strImageName, cv::Mat &matSrc, const vectorContours &faceContours, vectorInt &vectorIntResult);
#else
bool findFaceSpots(const string &strImageName, const cv::Mat &matSrc, const vectorContours &faceContours, vectorInt &vectorIntResult);
#endif

#endif // __FACE_SPOTS_DETECT_H__

