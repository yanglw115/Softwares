#ifndef __GRABCUT_H__
#define __GRABCUT_H__

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;

int grabCut(const string strFile, cv::Rect rect);
#endif 