#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;

int findFaceSpots(const string &strFile, const vector<vector<cv::Point>> contours);
