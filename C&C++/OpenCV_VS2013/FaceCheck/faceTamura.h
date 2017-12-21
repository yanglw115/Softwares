#ifndef __FACE_TAMURA__H__
#define __FACE_TAMURA__H__

#include "faceGlobal.h"

using namespace cv; 
using namespace std;

double localMean(const Mat& mat, int x, int y, int K);
double tamuraCalContrast(const Mat& mat);
double tamuraCalCoarseness(const Mat& mat, int kVal = 5, int histBins = 3);
double getFaceCoarseness(const Mat& mat, Rect rect);
#endif // __FACE_TAMURA__H__