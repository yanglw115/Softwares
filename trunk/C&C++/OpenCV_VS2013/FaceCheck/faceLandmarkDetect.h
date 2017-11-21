#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace std;

std::vector<std::vector<cv::Point>> faceLandmarkDetect(const string strFile);