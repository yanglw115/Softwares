#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

double getFaceColorValue(const std::string &strFile, std::vector<std::vector<cv::Point>> contours);
