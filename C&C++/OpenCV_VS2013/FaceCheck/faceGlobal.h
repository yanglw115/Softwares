
#ifndef  __FACE_GLOBAL_H__
#define __FACE_GLOBAL_H__

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

enum enumSkinPoreType {
	TYPE_SKIN_SMOOTH = 0,
	TYPE_SKIN_NORMAL = 1,
	TYPE_SKIN_ROUGH = 2
};

enum enumSkinOilType {
	TYPE_OIL_LOW = 0, /* 干燥 */
	TYPE_OIL_BALANCE = 1, /* 中性 */
	TYPE_OIL_OVER = 2, /* 油性 */
	TYPE_OIL_MIX = 3 /* 混合性 */
};

enum enumFaceValueIndex {
	INDEX_VALUE_LEFT = 0, /* 左脸痘痘 */
	INDEX_VALUE_RIGHT, /* 右脸痘痘 */
	INDEX_VALUE_FOREHEAD, /* 额头痘痘 */
	INDEX_VALUE_JAW, /* 下巴痘痘 */
	INDEX_VALUE_NOSE, /* 鼻子痘痘 */
	INDEX_VALUE_BLACKHEADS, /* 黑头(鼻子) */
	INDEX_VALUE_PORE_TYPE, /* 毛孔粗大度 */
	INDEX_VALUE_OIL_TYPE, /* 肤质，油性与水份 */
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

//#define PI 3.14
#define		PI		(3.141592653589793)

using namespace std;

typedef vector<vector<cv::Point> > vectorContours;
typedef vector<int> vectorInt;

#ifndef WIN_TEST
#include "log.h"
#else
/* 最近在使用VS2013或2017进行调试的时候，运行时总是出错，2018.1.15之前还是正常的。暂时去除GLOG模块的使用 */
#define LOG(XXX) cout << endl
#define ERROR 0
#define INFO 2 
#endif

#endif // ! __FACE_GLOBAL_H__

