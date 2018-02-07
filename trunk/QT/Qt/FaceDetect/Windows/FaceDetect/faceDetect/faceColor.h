#ifndef __FACE_COLOR_H__
#define __FACE_COLOR_H__

#include "faceGlobal.h"
#include "CObjectResult.h"

#define Use_ITA

#ifdef Use_ITA
typedef enum {
	TouBai = 55, // 色图测出来是78,
	BaiXi = 45, // 色图测出来是72,
	ZiRan = 25, // 色图测出来是57,
	XiaoMai = 10, // 色图测出来是48,
	AnChen = -30, // 色图测出来是3,
	YouHei = -30, // 色图测出来是-57
} enumFaceColorValue;

#else
typedef enum {
	TouBai = 234,
	BaiXi = 220,
	ZiRan = 205,
	XiaoMai = 164,
	AnChen = 122,
	YouHei = 81
} enumFaceColorValue;

#endif // Use_ITA

typedef enum {
	Type_Color_TouBai = 0,
	Type_Color_BaiXi = 1,
	Type_Color_ZiRan = 2,
	Type_Color_XiaoMai = 3,
	Type_Color_AnChen = 4,
	Type_Color_YouHei = 5
} enumFaceColorType;

enumFaceColorType getFaceColorType(const string &strImageName, const cv::Mat &matSrc, bool bHasFace,
                                   const cv::Rect &rectFace, CObjectResult *pObjResult);

#endif // __FACE_COLOR_H__
