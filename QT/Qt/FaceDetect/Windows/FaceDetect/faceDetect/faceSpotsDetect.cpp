
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "faceSpotsDetect.h"

#include <QString>
#include <QtDebug>
#include <QDir>
#include <QDateTime>

using namespace cv;

#define MIN_SIZE_PIMPLES 20 // 痘痘的最小尺寸
#define MAX_SIZE_PIMPLES 250
#define MAX_SIZE_BLACKHEADS 20 // 黑头的最大尺寸

#define MAX_RATIO 2.5 // 矩形长宽比最大值
#define MIN_RATIO 0.3 // 矩形长宽比最小值

#define NUMBER_PORE_ROUGH 150
#define NUMBER_PORE_NORMAL 60

#ifndef WITH_SPOTS_AS_PIMPLES
//#define WITH_SPOTS_AS_PIMPLES // 将斑点都当作痘痘来处理，否则排除斑点，只计算痘痘
#endif // WITH_SPOTS_AS_PIMPLES

#define MIN_COLOR_PIMPLES 20
#define MIN_COLOR_DIFF_G_R 20
#define MAX_COLOR_BLACKHEADS 255

static const string stdstrPoreType[] = {"细腻", "一般", "粗大"};
static const string stdstrPoreTypePY[] = {"XiNi", "YiBan", "CuDa"};

int findPimples(const string &strImageName, const Mat &srcImg, Mat &imgMask, CObjectResult *objResult, Mat &matOut)
{
	Mat bw;
	vectorContours vectorSpots;
	/* 转换为灰度图的效果和上面取单一通道效果差不太多 */
	cvtColor(imgMask, bw, COLOR_BGR2GRAY);
	int pimplesCount = 0; // 找不到边界即设置为0

#ifdef With_Debug_Show
	Mat matDebug;
	srcImg.copyTo(matDebug);
	namedWindow("自适应阈值化之前", WINDOW_NORMAL);
	imshow("自适应阈值化之前", bw);
#else
    Q_UNUSED(srcImg);
#endif // With_Debug_Show

	/* 自适应阈值化：图像分割，去除一定范围内的像素 */
	/* bw必须是单通道的8bit图像 */
	/* 第1个参数是输入图像，第2个参数是输出图像，第3个参数是满足条件的最大像素值，第4个参数是所用算法，
		第6个参数是用来计算阈值的块大小(必须是奇数)，第7个参数是需要从加权平均值减去的一个常量 */
	adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 5); // 目前调试这里使用15是最优的，可以再调试
	//adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 13, 5);

#ifdef With_Debug_Show
	namedWindow("自适应阈值化之后", WINDOW_NORMAL);
	imshow("自适应阈值化之后", bw);
#endif // With_Debug

	/* 膨胀操作：前两个参数是输入与输出；参数3：膨胀操作的核，NULL时为3*3；参数4：锚的位置，下面代表位于中心；参数5：迭代使用dilate的次数 */
	dilate(bw, bw, Mat(), Point(-1, -1), 1);
#ifdef With_Debug_Show
	namedWindow("膨胀操作之后", WINDOW_NORMAL);
	imshow("膨胀操作之后", bw);
#endif // With_Debug

	/* 查找轮廓:必须是8位单通道图像，参数4：可以提取最外层及所有轮廓 */
	findContours(bw, vectorSpots, RETR_LIST, CHAIN_APPROX_SIMPLE);

    qDebug() << strImageName.c_str() << ": Detected contours counts: " << vectorSpots.size();
    qDebug() << "MinArea: " << objResult->m_objPimples.m_dMinAreaSize;
    qDebug() << "MaxArea: " << objResult->m_objPimples.m_dMaxAreaSize;
	double areaSize = 0.0;
	for (size_t i = 0; i < vectorSpots.size(); ++i)	{
        qDebug() << strImageName.c_str() << ": Contour area size: " << fabs(contourArea(vectorSpots[i]));
		/* 这里的值也需要调试 */
		areaSize = fabs(contourArea(vectorSpots[i]));
        if (areaSize > objResult->m_objPimples.m_dMinAreaSize && areaSize < objResult->m_objPimples.m_dMaxAreaSize) {
			//Rect minRect = boundingRect(Mat(vectorSpots[i]));
			Rect minRect = minAreaRect(Mat(vectorSpots[i])).boundingRect();
			/* 这里通过minAreaRect可能取到的矩形已经超出了图片边界，一般是最外的轮廓，所以要进行先处理 */
			if (minRect.x < 0)
				minRect.x = 0;
			if (minRect.y < 0)
				minRect.y = 0;
			if (minRect.x + minRect.width > imgMask.cols)
				minRect.width = imgMask.cols - minRect.x;
			if (minRect.y + minRect.height > imgMask.rows)
				minRect.height = imgMask.rows - minRect.y;
#ifndef WITH_SPOTS_AS_PIMPLES
			Mat maskCopy;
			imgMask.copyTo(maskCopy);
			Mat imgroi(maskCopy, minRect);

			int nShiftV = (minRect.height / 5 - 1) / 2; // adjustROI的参数所*的核是5pix*5pix
			int nShiftH = (minRect.width / 5 - 1) / 2;
			imgroi = imgroi.adjustROI(-nShiftV, -nShiftV, -nShiftH, -nShiftH); // top, bottom, left, right
			Mat imgroiHLS = imgroi.clone();
			cvtColor(imgroiHLS, imgroiHLS, COLOR_BGR2HLS);
			Scalar meanColorHLS = mean(imgroiHLS);
			Scalar meanColor = mean(imgroi);

			/* 这里根据颜色值进行一次过滤 */
			//if (color[0] < 10 & color[1] > 70 & color[2] > 50) { // HSV
            qDebug() << "mean HLS L: " << meanColorHLS[2];
            qDebug() << "RG differ: " << meanColor[2] - meanColor[1];
            qDebug() << "MinL: " << objResult->m_objPimples.m_nMinLColorValue;
            qDebug() << "MaxRG diff: " << objResult->m_objPimples.m_nMinRGDiffValue;
            if (((int)(meanColorHLS[2]) >= objResult->m_objPimples.m_nMinLColorValue)
                    && (meanColor[2] - meanColor[1] > objResult->m_objPimples.m_nMinRGDiffValue)) { // HLS的L值;RGB的G与B值差；

#endif // WITH_SPOTS_AS_PIMPLES
				double ratio = minRect.width * 1.0 / minRect.height;
                qDebug() << "Ratio: " << ratio;
                qDebug() << "MinRatio: " << objResult->m_objPimples.m_dMinRatio;
                qDebug() << "MaxRatio: " << objResult->m_objPimples.m_dMaxRatio;
                if ((ratio < objResult->m_objPimples.m_dMaxRatio) && (ratio > objResult->m_objPimples.m_dMinRatio)) {
                    Point2f center;
                    float radius = 0;
                    minEnclosingCircle(Mat(vectorSpots[i]), center, radius);
                    circle(matOut, center, (int)(radius + 1), Scalar(139, 0, 0), 2, 8);
#ifdef With_Debug_Show
					Point2f center;
					float radius = 0;
					minEnclosingCircle(Mat(vectorSpots[i]), center, radius); 

					if (1) {//(radius > 2 && radius < 50) {
						Mat matTest;
						srcImg.copyTo(matTest);
						string strSize = to_string(areaSize);
						//putText(matTest, format("c(%d:%d),s(%s)", (int)meanColorHLS[2], (int)(meanColor[2] - meanColor[1]), strSize.substr(0, 3).c_str()), cv::Point2f(20, 20), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 1);
						putText(matTest, format("c(%d:%d),s(%s)", (int)meanColorHLS[2], (int)(meanColor[2] - meanColor[1]), strSize.substr(0, 3).c_str()), cv::Point2f(20, 20), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
						rectangle(matTest, minRect, Scalar(0, 255, 0));
                        circle(matTest, center, (int)(radius + 1), Scalar(139, 0, 0), 2, 8);
						namedWindow("当前斑点：", WINDOW_NORMAL);
						imshow("当前斑点：", matTest);
						waitKey();
					}
#endif // With_Debug		
					pimplesCount++;
				}
#ifndef WITH_SPOTS_AS_PIMPLES
			}
#endif // WITH_SPOTS_AS_PIMPLES
		}
	}

#ifdef With_Debug_Show
	putText(matDebug, format("%d", pimplesCount), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.8, Scalar(0, 0, 255), 3);
	namedWindow("痘痘检测结果：", WINDOW_NORMAL);
	imshow("痘痘检测结果：", matDebug);
	waitKey();
#endif // With_Debug

	return pimplesCount;
}


int findBlackHeads(const string &strImageName, const Mat &srcImg, Mat &imgMask, CObjectResult *objResult, Mat &matOut)
{
	Mat bw;
	vectorContours vectorSpots;
	cvtColor(imgMask, bw, COLOR_BGR2GRAY);
	int nBlackHeads = 0;

#ifdef With_Debug_Show
	Mat matDebug;
	srcImg.copyTo(matDebug);
	namedWindow("自适应阈值化之前", WINDOW_NORMAL);
	imshow("自适应阈值化之前", bw);
#else
    Q_UNUSED(srcImg);
#endif // With_Debug_Show

	/* 自适应阈值化：图像分割，去除一定范围内的像素 */
	/* bw必须是单通道的8bit图像 */
	/* 第1个参数是输入图像，第2个参数是输出图像，第3个参数是满足条件的最大像素值，第4个参数是所用算法，
	第6个参数是用来计算阈值的块大小(必须是奇数)，第7个参数是需要从加权平均值减去的一个常量 */
	adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 5); // 目前调试这里使用15是最优的，可以再调试
	//adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 13, 5);

#ifdef With_Debug_Show
	namedWindow("自适应阈值化之后", WINDOW_NORMAL);
	imshow("自适应阈值化之后", bw);
#endif // With_Debug_Show

	/* 膨胀操作：前两个参数是输入与输出；参数3：膨胀操作的核，NULL时为3*3；参数4：锚的位置，下面代表位于中心；参数5：迭代使用dilate的次数 */
	dilate(bw, bw, Mat(), Point(-1, -1), 1);
#ifdef With_Debug_Show
	namedWindow("膨胀操作之后", WINDOW_NORMAL);
	imshow("膨胀操作之后", bw);
#endif // With_Debug_Show

	/* 查找轮廓:必须是8位单通道图像，参数4：可以提取最外层及所有轮廓 */
	findContours(bw, vectorSpots, RETR_LIST, CHAIN_APPROX_SIMPLE);

    qDebug() << strImageName.c_str() << ": Detected contours counts：" << vectorSpots.size();
	double areaSize = 0.0;
	for (size_t i = 0; i < vectorSpots.size(); ++i) {
        //qDebug() << strImageName << ": Contour area size: " << to_string(fabs(contourArea(vectorSpots[i])));
		/* 这里的值也需要调试 */
		areaSize = fabs(contourArea(vectorSpots[i]));
        if (areaSize < objResult->m_objBlackheads.m_dMaxAreaSize) {
			//Rect minRect = boundingRect(Mat(vectorSpots[i]));
			Rect minRect = minAreaRect(Mat(vectorSpots[i])).boundingRect();
			/* 这里通过minAreaRect可能取到的矩形已经超出了图片边界，一般是最外的轮廓，所以要进行先处理 */
			if (minRect.x < 0)
				minRect.x = 0;
			if (minRect.y < 0)
				minRect.y = 0;
			if (minRect.x + minRect.width > imgMask.cols)
				minRect.width = imgMask.cols - minRect.x;
			if (minRect.y + minRect.height > imgMask.rows)
				minRect.height = imgMask.rows - minRect.y;

			Mat maskCopy;
			imgMask.copyTo(maskCopy);
			Mat imgroi(maskCopy, minRect);

			//cvtColor(imgroi, imgroi, COLOR_BGR2HSV);
			Scalar color = mean(imgroi);

			/* 这里根据颜色值进行一次过滤 */
            if ((int)(color[0] + color[1] + color[2]) / 3 <= objResult->m_objBlackheads.m_nMaxColor) {
				double ratio = minRect.width * 1.0 / minRect.height;
                if ((ratio < objResult->m_objBlackheads.m_dMaxRatio) && (ratio > objResult->m_objBlackheads.m_dMinRatio)) {
                    Point2f center;
                    float radius = 0;
                    minEnclosingCircle(Mat(vectorSpots[i]), center, radius);
                    circle(matOut, center, (int)(radius + 1), Scalar(139, 0, 0), 2, 8);
#ifdef With_Debug_Show
					Point2f center;
					float radius = 0;
					minEnclosingCircle(Mat(vectorSpots[i]), center, radius);

					if (1) {//(radius > 2 && radius < 50) {
						Mat matTest;
						srcImg.copyTo(matTest);
						string strSize = to_string(areaSize);
						//putText(matTest, format("color(%d:%d:%d), areaSize(%s)", color[0], color[1], color[2], strSize.substr(0, 3).c_str()), cv::Point2f(center.x, center.y - radius), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
						putText(matTest, format("color(%d:%d:%d-%d), areaSize(%s)", (int)color[0], (int)color[1], (int)color[2], (int)(color[0] + color[1] + color[2]) / 3, strSize.substr(0, 3).c_str()), cv::Point2f(20, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
						rectangle(matTest, minRect, Scalar(0, 255, 0));
                        //circle(matTest, center, (int)(radius + 1), Scalar(139, 0, 0), 2, 8);
						namedWindow("当前黑头：", WINDOW_NORMAL);
						imshow("当前黑头：", matTest);
						waitKey();
					}
#endif // With_Debug		
					nBlackHeads++;
				}
			}
		}
	}

#ifdef With_Debug_Show
	putText(matDebug, format("%d", nBlackHeads), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.8, Scalar(0, 0, 255), 3);
	namedWindow("黑头检测结果：", WINDOW_NORMAL);
	imshow("黑头检测结果：", matDebug);
	waitKey();
#endif // With_Debug_Show

	return nBlackHeads;
}

bool findFaceSpots(const string &strImageName, const Mat &matSrc, bool bHasFace,
                   const vectorContours &faceContours, const enumItemType type, CObjectResult *objResult)
{
	int nPimples = -1;
	int nBlackHeadsNose = 0;
	int nBlackHeadsFace = 0;
    vector<int> vectorIntResult;
    vectorIntResult.resize(INDEX_VALUE_MAX);

    qDebug() << objResult->m_objPimples.m_dMaxAreaSize;
    qDebug() << objResult->m_objPimples.m_dMinAreaSize;

    if (!bHasFace && (TYPE_ALL == type)) {
        qCritical() << "Cannot detect all items without face.";
        return false;
    }

    //stringstream ss;
    //ss << g_strImgTmpDir << strImageName << "pimples." << QDateTime::currentSecsSinceEpoch() << ".jpg";
    QDir dir(".");
    dir.mkpath(QString("%1").arg(QString(g_tmpDirPimples)));//.arg(QString(strImageName.c_str()).split(".")[0]));
    dir.mkpath(QString("%1").arg(QString(g_tmpDirBlackheads)));
    dir.mkpath(QString("%1").arg(QString(g_tmpDirPore)));
    QString strImagePimple = QString("%1%2_%3%4").arg(QString(g_tmpDirPimples)).arg(QString(strImageName.c_str()).split(".")[0])
            /*.arg("pimples.")*/.arg(QDateTime::currentSecsSinceEpoch()).arg(".jpg");
    QString strImageBlackheads = QString("%1%2_%3%4").arg(QString(g_tmpDirBlackheads)).arg(QString(strImageName.c_str()).split(".")[0])
            /*.arg("blackheads.")*/.arg(QDateTime::currentSecsSinceEpoch()).arg(".jpg");
    QString strImagePore = QString("%1%2_%3%4").arg(QString(g_tmpDirPore)).arg(QString(strImageName.c_str()).split(".")[0])
            /*.arg("pore.")*/.arg(QDateTime::currentSecsSinceEpoch()).arg(".jpg");
    Mat matPimples, matBlackheads, matPore;
    matSrc.copyTo(matPimples);
    matSrc.copyTo(matBlackheads);
    matSrc.copyTo(matPore);

    if (bHasFace) {
        for (int i = 0; i <= INDEX_CONTOUR_NOSE; ++i) {
            /* 创建一个通道并与原图大小相等的Mat */
            Mat mask(matSrc.size(), CV_8UC1);
            /* 矩阵赋值为全0，颜色表现为全黑 */
            mask = 0;
            /* 第一个参数是需要画轮廓的图像，第二个参数代表轮廓数组，第三个参数代表所用数组索引，第4个参数代表轮廓填充颜色，第5个参数是 */
            drawContours(mask, faceContours, i, Scalar(255), -1);
            /* 创建一个三通道的mask */
            Mat masked(matSrc.size(), CV_8UC3);
            masked = Scalar(255, 255, 255);
            /* 将画了轮廓的原图按照mask拷贝到masked；这里的mask只有轮廓部分颜色值是1，即只拷贝原图这块的内容到masked */
            matSrc.copyTo(masked, mask);
            /* imgSrc始终保持不变 */
            drawContours(matPimples, faceContours, i, Scalar(0, 255, 0), 1);
            nPimples = findPimples(strImageName, matSrc, masked, objResult, matPimples);
            qDebug() << "Detect pimples: " << nPimples;
            vectorIntResult[i] = nPimples;

            if (INDEX_CONTOUR_LEFT == i) {
                drawContours(matPore, faceContours, i, Scalar(0, 255, 0), 1);
                nBlackHeadsFace = findBlackHeads(strImageName, matSrc, masked, objResult, matPore);
                vectorIntResult[INDEX_VALUE_PORE_LEFT] = nBlackHeadsFace;
            } else if (INDEX_CONTOUR_RIGHT == i) {
                drawContours(matPore, faceContours, i, Scalar(0, 255, 0), 1);
                nBlackHeadsFace = findBlackHeads(strImageName, matSrc, masked, objResult, matPore);
                vectorIntResult[INDEX_VALUE_PORE_RIGHT] = nBlackHeadsFace;
                nBlackHeadsFace += vectorIntResult[INDEX_VALUE_PORE_LEFT];
            } else if (INDEX_CONTOUR_NOSE == i) {
                drawContours(matBlackheads, faceContours, i, Scalar(0, 255, 0), 1);
                nBlackHeadsNose = findBlackHeads(strImageName, matSrc, masked, objResult, matBlackheads);
                vectorIntResult[INDEX_VALUE_BLACKHEADS] = nBlackHeadsNose;
            }

        }

        if (nBlackHeadsFace >= NUMBER_PORE_ROUGH) {
            vectorIntResult[INDEX_VALUE_PORE_TYPE] = TYPE_SKIN_ROUGH;
        } else if (nBlackHeadsFace >= NUMBER_PORE_NORMAL) {
            vectorIntResult[INDEX_VALUE_PORE_TYPE] = TYPE_SKIN_NORMAL;
        } else {
            vectorIntResult[INDEX_VALUE_PORE_TYPE] = TYPE_SKIN_SMOOTH;
        }

        if (type & TYPE_PIMPLES) {
            imwrite(strImagePimple.toStdString(), matPimples);
            objResult->m_objPimples.m_strImgPath = strImagePimple;
            objResult->m_objPimples.m_strLeft = QString("%1").arg(vectorIntResult[INDEX_VALUE_LEFT]);
            objResult->m_objPimples.m_strRight = QString("%1").arg(vectorIntResult[INDEX_VALUE_RIGHT]);
            objResult->m_objPimples.m_strForehead = QString("%1").arg(vectorIntResult[INDEX_VALUE_FOREHEAD]);
            objResult->m_objPimples.m_strJaw = QString("%1").arg(vectorIntResult[INDEX_VALUE_JAW]);
            objResult->m_objPimples.m_strNose = QString("%1").arg(vectorIntResult[INDEX_VALUE_NOSE]);
        }

        if (type & TYPE_BLACKHEADS) {
            imwrite(strImageBlackheads.toStdString(), matBlackheads);
            objResult->m_objBlackheads.m_strImgPath = strImageBlackheads;
            objResult->m_objBlackheads.m_strCounts = QString("%1").arg(vectorIntResult[INDEX_VALUE_BLACKHEADS]);
        }

        if (type & TYPE_PORE) {
            putText(matPore, format("%s:%d", stdstrPoreTypePY[vectorIntResult[INDEX_VALUE_PORE_TYPE]].c_str(), nBlackHeadsFace),
                    Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0, 0, 255), 3);
            imwrite(strImagePore.toStdString(), matPore);
            objResult->m_objPore.m_strImgPath = strImagePore;
            objResult->m_objPore.m_strLeft = QString("%1").arg(vectorIntResult[INDEX_VALUE_PORE_LEFT]);
            objResult->m_objPore.m_strRight = QString("%1").arg(vectorIntResult[INDEX_VALUE_PORE_RIGHT]);
            objResult->m_objPore.m_strPoreType = QString("%1").arg(stdstrPoreType[vectorIntResult[INDEX_VALUE_PORE_TYPE]].c_str());
        }

    } else {
        Mat matMask;
        matSrc.copyTo(matMask);

        switch (type) {
        case TYPE_PIMPLES:
            nPimples = findPimples(strImageName, matSrc, matMask, objResult, matPimples);
            objResult->m_objPimples.m_strImgPath = strImagePimple;
            objResult->m_objPimples.m_strLeft = QString("%1").arg(nPimples);
            imwrite(strImagePimple.toStdString(), matPimples);
            break;
        case TYPE_BLACKHEADS:
            nBlackHeadsNose = findBlackHeads(strImageName, matSrc, matMask, objResult, matBlackheads);
            objResult->m_objBlackheads.m_strImgPath = strImageBlackheads;
            objResult->m_objBlackheads.m_strCounts = QString("%1").arg(nBlackHeadsNose);
            imwrite(strImageBlackheads.toStdString(), matBlackheads);
            break;
        case TYPE_PORE:
            nBlackHeadsFace = findBlackHeads(strImageName, matSrc, matMask, objResult, matPore);
            objResult->m_objPore.m_strImgPath = strImagePore;
            objResult->m_objPore.m_strLeft = QString("%1").arg(nBlackHeadsFace);
            objResult->m_objPore.m_strRight = QString("");
            objResult->m_objPore.m_strPoreType = QString("无面部检测结果");
            putText(matPore, format("%s:%d", stdstrPoreTypePY[vectorIntResult[INDEX_VALUE_PORE_TYPE]].c_str(), nBlackHeadsFace), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0, 0, 255), 3);
            imwrite(strImagePore.toStdString(), matPore);
            break;
        default:
            break;
        }
    }

#ifdef WITH_SPOTS_AS_PIMPLES
	/* 这里暂时对额头和下巴的脏数据进行简单处理 */
	if (vectorIntResult[INDEX_CONTOUR_FOREHEAD] > 3) {
		vectorIntResult[INDEX_CONTOUR_FOREHEAD] = 0;
	}
	if (vectorIntResult[INDEX_CONTOUR_JAW] > 3) {
		vectorIntResult[INDEX_CONTOUR_JAW] = 0;
	}
#endif // WITH_SPOTS_AS_PIMPLES

#ifdef With_Debug_Show
	Mat matDebug;
	const string strPoreTypes[] = {"smooth", "normal", "rough"};
	matSrc.copyTo(matDebug);
	putText(matDebug, format("%s(%d)", strPoreTypes[vectorIntResult[INDEX_VALUE_PORE_TYPE]].c_str(), nBlackHeadsFace), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.8, Scalar(0, 0, 255), 3);
	namedWindow("毛孔检测结果：", WINDOW_NORMAL);
	imshow("毛孔检测结果：", matDebug);
	waitKey();
#endif // With_Debug_Show

	return true;
}
