#pragma once

#include "faceGlobal.h"

#ifdef With_Debug
bool findFaceSpots(cv::Mat &matSrc, const vectorContours &faceContours, vectorInt &vectorIntResult);
#else
bool findFaceSpots(const cv::Mat &matSrc, const vectorContours &faceContours, vectorInt &vectorIntResult);
#endif
