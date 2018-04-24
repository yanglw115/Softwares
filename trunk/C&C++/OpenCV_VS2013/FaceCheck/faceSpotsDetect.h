#ifndef __FACE_SPOTS_DETECT_H__
#define __FACE_SPOTS_DETECT_H__

#include "faceGlobal.h"

bool findFaceSpots(const string &strImageName, const cv::Mat &matSrc, const vectorContours &faceContours, vectorInt &vectorIntResult);
int findBlackHeads(const string &strImageName, const cv::Mat &srcImg, cv::Mat &imgMask);
int findPimples(const string &strImageName, const cv::Mat &srcImg, cv::Mat &imgMask);
int getMoistureAndOil(const string &strImageName, const cv::Mat &srcImg, cv::Mat &imgMask);

#endif // __FACE_SPOTS_DETECT_H__

