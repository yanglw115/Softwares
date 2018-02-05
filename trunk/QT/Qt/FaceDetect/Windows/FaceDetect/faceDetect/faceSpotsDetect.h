#ifndef __FACE_SPOTS_DETECT_H__
#define __FACE_SPOTS_DETECT_H__

#include "faceGlobal.h"
#include "CObjectResult.h"
#include "glob.h"

bool findFaceSpots(const string &strImageName, const string &strImagePath, bool bHasFace,
                   const vectorContours &faceContours, const enumItemType type, CObjectResult &objResult);
int findBlackHeads(const string &strImageName, const cv::Mat &srcImg, cv::Mat &imgMask);
int findPimples(const string &strImageName, const cv::Mat &srcImg, cv::Mat &imgMask);

#endif // __FACE_SPOTS_DETECT_H__

