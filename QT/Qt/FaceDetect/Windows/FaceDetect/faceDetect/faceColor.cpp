
#include "faceColor.h"

#include "opencv2/imgproc/imgproc.hpp"    
#include "faceGlobal.h"
#include "glob.h"

#include <iostream>
#include <stdio.h>
#include <math.h>

#include <QDir>
#include <QDateTime>

using namespace cv;

static const char *g_colorString[] = {
    "透白",
    "白皙",
    "自然",
    "小麦",
    "暗沉",
    "幽默"
};

MatND getHistogram(Mat &image)
{
	MatND hist;

	int channels[] = { 0 };
	int dims = 1;
	int histSize[] = { 256 };
	float granges[] = { 0, 256 };
	const float *ranges[] = { granges };
	calcHist(&image, 1, channels, Mat(), hist, dims, histSize, ranges);
	
	return hist;
}
  
Mat getHistogramImage(const string &strImageName, Mat &image, double *pColorValue)
{
	MatND hist = getHistogram(image);
	Mat showImage(256, 256, CV_8UC3, Scalar(0, 0, 0));
	double maxValue = 0;
	Point maxPoint;
	minMaxLoc(hist, 0, &maxValue, 0, &maxPoint);
    //qWarning() << strImageName << ": Color max point：" << "(" << to_string(maxPoint.x) << ", " << to_string(maxPoint.y) << ")"
        //<< ", max value: " << to_string(maxValue);
	*pColorValue = maxPoint.y;

#ifdef With_Debug_Show
	for (int i = 0; i < 256; i++) {
		float value = hist.at<float>(i);
		int intensity = saturate_cast<int>(256 - 256 * (value / maxValue));
		rectangle(showImage, Point(i, 256 - 1), Point((i + 1) - 1, intensity), Scalar(0, 0, 255));
	}
#endif // With_Debug_Show

	return showImage;
}

enumFaceColorType getFaceColorType(const string &strImageName, const cv::Mat &matSrc, bool bHasFace,
                                   const cv::Rect &rectFace, CObjectResult *pObjResult)
{
	double maxColorValue = -1;
	enumFaceColorType type = Type_Color_TouBai;

    QDir dir(".");
    dir.mkpath(QString("%1%2").arg(QString(g_strImgTmpDir)).arg(QString(strImageName.c_str()).split(".")[0]));
    QString strPathFaceColor = QString("%1%2/%3%4%5").arg(QString(g_strImgTmpDir)).arg(QString(strImageName.c_str()).split(".")[0])
            .arg("faceColor.").arg(QDateTime::currentSecsSinceEpoch()).arg(".jpg");
    Mat matOutput;
    matSrc.copyTo(matOutput);

    Mat imageColor;

    if (bHasFace) {
        vectorContours faceContours;
        vector<Point> vectorShape;
        vectorShape.resize(4);
        vectorShape[0] = Point(rectFace.x, rectFace.y);
        vectorShape[1] = Point(rectFace.x, rectFace.y + rectFace.height);
        vectorShape[2] = Point(rectFace.x + rectFace.width, rectFace.y + rectFace.height);
        vectorShape[3] = Point(rectFace.x + rectFace.width, rectFace.y);
        faceContours.push_back(vectorShape);

        Mat imageFace(matSrc.size(), CV_8UC3);
        Mat mask(matSrc.size(), CV_8UC1);
        mask = 0;
        /* 这里取的是向量数据第5索引，即整个面部的正中矩形 */
        drawContours(mask, faceContours, 0, Scalar(255), -1);
        drawContours(matOutput, faceContours, 0, Scalar(255, 255, 0), 1);

        matSrc.copyTo(imageFace, mask);
        /* 只取脸部代表颜色的关键区域 */
        Mat matImageTemp(imageFace, rectFace);
        matImageTemp.copyTo(imageColor);
        if (!imageColor.data) {
            qWarning() << strImageName.c_str() << ": Fail to load the image";
            return type;
        }
    } else {
        matSrc.copyTo(imageColor);
    }

#ifdef Use_ITA
	Mat imageTemp(imageColor.size(), CV_8UC3);
	cvtColor(imageColor, imageTemp, COLOR_BGR2Lab);
	Scalar labScalar =  mean(imageTemp);
	double L = labScalar[0] * 100 / 255;
	double B = labScalar[2] - 128;
	maxColorValue = atan((L - 50) / B) * 180 / PI;
    //qDebug() << strImageName << ": ITA: " << to_string(maxColorValue);
#else
   	Mat imageResult = getHistogramImage(strImageName, imageColor, &maxColorValue);
#endif // Use_ITA

    if (maxColorValue >= pObjResult->m_objFaceColor.m_nTouBai) {
		type = Type_Color_TouBai;
    } else if (maxColorValue >= pObjResult->m_objFaceColor.m_nBaiXi) {
		type = Type_Color_BaiXi;
    } else if (maxColorValue >= pObjResult->m_objFaceColor.m_nZiRan) {
		type = Type_Color_ZiRan;
    } else if (maxColorValue >= pObjResult->m_objFaceColor.m_nXiaoMai) {
		type = Type_Color_XiaoMai;
    } else if (maxColorValue >= pObjResult->m_objFaceColor.m_nAnChen) {
		type = Type_Color_AnChen;
	} else {
		type = Type_Color_YouHei;
	}

    imwrite(strPathFaceColor.toStdString(), matOutput);
    pObjResult->m_objFaceColor.m_strImgPath = strPathFaceColor;
    pObjResult->m_objFaceColor.m_strColorType = QString(g_colorString[type]);
    pObjResult->m_objFaceColor.m_strColorValue = QString("%1").arg(maxColorValue);

#if 0
    Mat matOutputput;
    matSrc.copyTo(matOutputput);
    putText(matOutputput, format("%s:%4f", pStrColorString, maxColorValue), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0, 0, 255), 3);
    imwrite("../../../images/pics/bak_" + strImageName, matOutputput);
#endif
#ifdef With_Debug_Show
	const char *pStrColorString = g_colorString[type];
	namedWindow("image original:", WINDOW_NORMAL);
    putText(matSrc, format("%s", pStrColorString), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0, 0, 255), 3);
    imshow("image original:", matSrc);
	namedWindow("image face pre:", WINDOW_NORMAL);
	imshow("image face pre:", imageColor);
#ifndef Use_ITA
	namedWindow("showImage", WINDOW_NORMAL);
	imshow("showImage", imageResult);
#endif // Use_ITA
	waitKey(0);
#endif // With_Debug_Show

	return type;
}
