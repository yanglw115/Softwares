#ifndef __FACE_COLOR_H__
#define __FACE_COLOR_H__

#include "faceGlobal.h"

/* ͸�ף�232/��𪣺217/��Ȼ��200/С��161/������120/��ڣ�81 */
typedef enum {
	TouBai = 234,
	BaiXi = 220,
	ZiRan = 205,
	XiaoMai = 164,
	AnChen = 122,
	YouHei = 81
} enumFaceColorValue;

typedef enum {
	Type_Color_TouBai = 0,
	Type_Color_BaiXi = 1,
	Type_Color_ZiRan = 2,
	Type_Color_XiaoMai = 3,
	Type_Color_AnChen = 4,
	Type_Color_YouHei = 5
} enumFaceColorType;

enumFaceColorType getFaceColorType(const std::string &strFile, const vectorContours &contours);

#endif // __FACE_COLOR_H__