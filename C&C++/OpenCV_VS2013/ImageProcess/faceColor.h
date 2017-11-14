#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

int getFaceColorValue(const std::string &strFile, std::vector<std::vector<cv::Point>> contours);
