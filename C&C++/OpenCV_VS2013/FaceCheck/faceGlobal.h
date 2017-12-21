
#ifndef  __FACE_GLOBAL_H__
#define __FACE_GLOBAL_H__

#include "log.h"

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

enum enumSkinPoreType {
	TYPE_SKIN_SMOOTH = 0,
	TYPE_SKIN_NORMAL = 1,
	TYPE_SKIN_ROUGH = 2
};

enum enumFaceValueIndex {
	INDEX_VALUE_LEFT = 0,
	INDEX_VALUE_RIGHT,
	INDEX_VALUE_FOREHEAD,
	INDEX_VALUE_JAW,
	INDEX_VALUE_NOSE,
	INDEX_VALUE_BLACKHEADS,
	INDEX_VALUE_PORE_TYPE,
	INDEX_VALUE_MAX
};

enum enumFaceContourIndex {
	INDEX_CONTOUR_LEFT = 0,
	INDEX_CONTOUR_RIGHT = 1,
	INDEX_CONTOUR_FOREHEAD = 2,
	INDEX_CONTOUR_JAW = 3,
	INDEX_CONTOUR_NOSE = 4,
	INDEX_CONTOUR_FACE = 5,
};

using namespace std;

typedef vector<vector<cv::Point> > vectorContours;
typedef vector<int> vectorInt;

#endif // ! __FACE_GLOBAL_H__

