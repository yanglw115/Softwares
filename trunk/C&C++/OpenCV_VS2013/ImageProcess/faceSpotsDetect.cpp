#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static bool findPimples(Mat srcImg, Mat img, std::vector<std::vector<cv::Point>> contours)
{
	Mat bw, bgr[3];
	/* 将只有轮廓部分的图进行split通道分离 */
	split(img, bgr);
	/* 这里取绿色通道 */
	//bw = bgr[1];
	/* 转换为灰度图的效果和上面取单一通道效果差不太多 */
	cvtColor(img, bw, COLOR_BGR2GRAY);
	int pimplescount = 0;

	namedWindow("自适应阈值化之前", WINDOW_NORMAL);
	imshow("自适应阈值化之前", bw);
	/* 自适应阈值化：图像分割，去除一定范围内的像素 */
	/* bw必须是单通道的8bit图像 */
	/* 第1个参数是输入图像，第2个参数是输出图像，第3个参数是满足条件的最大像素值，第4个参数是所用算法，
		第6个参数是用来计算阈值的块大小(必须是奇数)，第7个参数是需要从加权平均值减去的一个常量 */
	adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 5); //好像这里使用15是最优的，可以再调试
	//adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 13, 5);
	namedWindow("自适应阈值化之后", WINDOW_NORMAL);
	imshow("自适应阈值化之后", bw);

	/* 膨胀操作：前两个参数是输入与输出；参数3：膨胀操作的核，NULL时为3*3；参数4：锚的位置，下面代表位于中心；参数5：迭代使用dilate的次数 */
	dilate(bw, bw, Mat(), Point(-1, -1), 1);
	namedWindow("膨胀操作之后", WINDOW_NORMAL);
	imshow("膨胀操作之后", bw);

	contours.clear();
	/* 查找轮廓:必须是8位单通道图像，参数4：可以提取最外层及所有轮廓 */
	findContours(bw, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	cout << "检测到的边界个数：" << contours.size() << endl;
	for (size_t i = 0; i< contours.size(); i++)
	{
	    cout << "边界大小：" << contourArea(contours[i]) << endl;
		/* 这里的值也需要调试 */
		if (contourArea(contours[i]) > 0 && contourArea(contours[i]) < 150)
		{
			Rect minRect = boundingRect(Mat(contours[i]));
			Mat imgroi(img, minRect);

			cvtColor(imgroi, imgroi, COLOR_BGR2HSV);
			Scalar color = mean(imgroi);
			cvtColor(imgroi, imgroi, COLOR_HSV2BGR);

			if (1)//(color[0] < 10 & color[1] > 70 & color[2] > 50)
			{
				Point2f center, vtx[4];
				float radius = 0;
				minEnclosingCircle(Mat(contours[i]), center, radius);

				/* 这里的值需要最终调试 */
				if (radius > 3)
				{
					rectangle(srcImg, minRect, Scalar(0, 255, 0));
					pimplescount++;
				}
			}
		}
	}
	putText(srcImg, format("%d", pimplescount), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.8, Scalar(0, 0, 255), 3);

	namedWindow("pimples dedector", WINDOW_NORMAL);
	imshow("pimples dedector", srcImg);
	waitKey();

	return true;
}

int findFaceSpots(const string &strFile, const std::vector<std::vector<cv::Point>> contours)
{
	Mat imgSrc = imread(strFile);
	if (imgSrc.empty()) {
		return -1;
	}

	namedWindow("原图：", WINDOW_NORMAL);
	imshow("原图：", imgSrc);

	/* 创建一个通道并与原图大小相等的Mat */
	Mat mask(imgSrc.size(), CV_8UC1);
	/* 矩阵赋值为全0，颜色表现为全黑 */
	mask = 0;
	/* 第一个参数是需要画轮廓的图像，第二个参数代表轮廓数组，第三个参数代表所用数组索引，第4个参数代表轮廓填充颜色，第5个参数是 */
	drawContours(mask, contours, -1, Scalar(255), -1);
	/* 创建一个三通道的mask */
	Mat masked(imgSrc.size(), CV_8UC3);
	masked = Scalar(255, 255, 255);
	/* 将画了轮廓的原图按照mask拷贝到masked；这里的mask只有轮廓部分颜色值是1，即只拷贝原图这块的内容到masked */
	imgSrc.copyTo(masked, mask);
	/* imgSrc始终保持不变 */
	findPimples(imgSrc, masked, contours);

	while( 'q' != waitKey(0));
	return 0;
}