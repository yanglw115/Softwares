#ifndef __FACE_TAMURA__H__
#define __FACE_TAMURA__H__

#include "faceGlobal.h"

double localMean(const cv::Mat& mat, int x, int y, int K);
double tamuraCalContrast(const cv::Mat& mat);
double tamuraCalCoarseness(const cv::Mat& mat, int kVal = 5, int histBins = 3);
double getFaceCoarseness(const cv::Mat& mat, cv::Rect rect);
double tamuraCalDirectionality(const cv::Mat& mat);
#endif // __FACE_TAMURA__H__