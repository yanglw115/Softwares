#ifndef  __FACE_LANDMARK_DETECT_H__
#define __FACE_LANDMARK_DETECT_H__

#include <QString>

#include "faceGlobal.h"

bool faceDetect(const string &strImageName, const string &strImagePath);
enumTypeResult faceLandmarkDetect(const string &strImageName, const cv::Mat &matSrc, vectorContours &, cv::Rect &);

#endif // ! __FACE_LANDMARK_DETECT_H__
