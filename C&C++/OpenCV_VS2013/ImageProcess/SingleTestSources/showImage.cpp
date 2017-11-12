
#include "showImage.h"
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;


/* 常用的数据结构
点的表示：
	Point_<int>、Point2i、Point等价
	Point_<float>、Point2f互相等价
颜色的表示：
	Scalar(r,g,b,alpha);
尺寸的表示：
	Size_<int>、Size2i、Size相互等价
	Size(width, heigth);
矩形的表示：
	Rect()，其常用的成员函数有：Size()返回尺寸；area()返回面积；contains(Point)判断点是否在矩形内；
	inside(rect)判断矩形是否在矩形内；tl()返回左上角坐标；br()返回右下角坐标；
	求两矩形的交集：Rect rect = rect1 & rect2; 并集：Rect rect = rect1 | rect2;
	矩形可这样进行平移：Rect rect = rect1 + point; 缩放：Rect rect = rect1 + size;
颜色空间转换：
	cvtColor();可以实现RGB向HSV、HSI等颜色空间的转换，也可转换为灰度图像；
	例如：cvtColor(srcImage, dstImage, COLOR_GRAY2BGR);
*/

/* 基本图形的绘制
绘制直线的line函数：
绘制椭圆的ellipse函数
绘制矩形的rectangle函数
绘制圆的circle函数
绘制填充的多边形的fillPoly函数
*/

/* Look Up Table操作
	operationsOnArrays::LUT()<lut>用于批量进行图像元素查找、扫描与操作
*/

/* 计时函数
	getTickCount()和getTickFrequency()，后面函数代表CPU一秒所走过的周期数
*/

/* 访问图像中像素的三类方法
	1、指针访问，C操作符；uchar *data = outputImage.ptr<uchar>(i);获取第i行的首地址，直接操作uchar数据；
	2、迭代器：iterator,指向像素，(*iterator)[0],(*iterator)[1],(*iterator)[2]
		Mat_<Vec3b>::iterator it = outputImage.begin<Vec3b>();
		Mat_<Vec3b>::iterator itEnd = outputImage.end<Vec3b>();
	3、动态地址计算
		outputImage.at<Vec3b>(i, j)[0],outputImage.at<Vec3b>(i, j)[1],outputImage.at<Vec3b>(i, j)[2]
*/

/* ROI区域操作
	1、定义ROI区域有两种方式，一种是指定rect，另一种是指定range
*/

/* 图像融合
	addWeighted()，注意融合的两个图像必须大小和深度一致
*/

/* 分离颜色通道,多通道颜色混合
	split、merge
*/

/* 调整亮度和对比度
    原理和方法
*/

/* 显示图像 
	imshow()
*/


/* 文件读取操作(xml/yaml)
	FileStorage
*/

void showImage(const string &strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("原始图像", srcImage);
	cout << "图像的矩阵数据为：\n" << srcImage;
	waitKey(0); // 0代表无限等待，单位是ms
}

/* 图像的腐蚀：把周围的颜色变成与自己一样的？
	erode()
*/
void ircodeImage(const string &strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("原始图像", srcImage);

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat dstImage;
	erode(srcImage, dstImage, element); // 图像进行腐蚀
	imshow("显示腐蚀效果图", dstImage);
	waitKey(0); // 0代表无限等待，单位是ms
}

/* 均值滤波：减小颜色之间的差异 
	blur()
*/
void blurImage(const string &strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("原始图像", srcImage);
	Mat dstImage;
	blur(srcImage, dstImage, Size(7, 7));
	imshow("均值滤波效果", dstImage);
	waitKey(0); // 0代表无限等待，单位是ms
}

/* 边缘检测：检测所有边缘
	Canny()
*/
void cannyImage(const string &strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("原始图像", srcImage);
	Mat dstImage, edgeImage, grayImage;
	dstImage.create(srcImage.size(), srcImage.type());

	/* 转换为灰度图像 */
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	imshow("转换后的灰度图像", grayImage);
	/* 先降噪 */
	blur(grayImage, edgeImage, Size(3, 3));
	/* 运行canny算子 */
	Canny(edgeImage, edgeImage, 3, 9, 3);
	imshow("Canny边缘检测效果", edgeImage);
	waitKey(0); // 0代表无限等待，单位是ms
}

/* 生成图像文件 */
/* bool imgwrite(const string &fileName, InputArray img, const vector<in>& params = vector<int>(0) ) */

/* 图像融合
	addWeighted()
*/
void imageAddWeighted(const string &strSrc1, const string &strSrc2)
{
	Mat srcImage1 = imread(strSrc1.c_str());
	Mat srcImage2 = imread(strSrc2.c_str());
	Mat dstImage;
	imshow("原始图像1：", srcImage1);
	imshow("原始图像2：", srcImage2);
	addWeighted(srcImage1, 0.5, srcImage2, 0.5, 0.0, dstImage);
	imshow("融合之后的图像：", dstImage);
	waitKey(0); // 0代表无限等待，单位是ms
}

/* ROI */
void imageROI(const string &strSrc1, const string &strSrc2)
{
	Mat srcImage1 = imread(strSrc1.c_str());
	Mat srcImage2 = imread(strSrc2.c_str());

	/* 选取感兴趣区域的引用：有两种方法，一种是矩形区域，另一种是range区域 */
	Mat imageROI = srcImage1(Rect(200, 250, srcImage2.cols, srcImage2.rows));
	imshow("原始图像", srcImage1);

	/* 加载掩膜 */
	Mat mask = imread(strSrc2.c_str(), 0); /* 灰度图 */
	/* 将掩膜复制到ROI */
	//srcImage2.copyTo(imageROI, mask); /* 这里的图像掩膜好像效果不太明显 */
	srcImage2.copyTo(imageROI);

	/* 这里也可以使用图像叠加相关函数操作(注意两个融合的图像大小和深度必须一致) */
	//Mat dstImage;
	//addWeighted(imageROI, 0.0, srcImage2, 1.0, 0, imageROI); /* 注意这里叠加后的输出结果也在感兴趣的区域 */

	imshow("利用ROI实现图像叠加的效果", srcImage1);
	waitKey();
}

/* 分离颜色通道,多通道颜色混合
	split、merge
*/
void imageSplit(const string &strSrc1, const string &strSrc2)
{
	vector<Mat> channels;
	channels.resize(3);
	//Mat channels[3]; /* 这里也可以使用数组 */

	/* 这里一定要注意下面将图像转换成了灰度图,否则后面addWeigthed的时候会因为通道不一样而程序崩溃 */
	Mat srcImage2 = imread(strSrc2.c_str(), 0);

	Mat srcImage1 = imread(strSrc1.c_str());
	/* 颜色通道分离 */
	split(srcImage1, channels);

	Mat imageBlueChannel = channels.at(0);
	Mat imageGreenChannel = channels.at(1);
	Mat imageRedChannel = channels.at(2);

	addWeighted(imageBlueChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)), 0.5, srcImage2, 0.5, 0.0, imageBlueChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)));
	merge(channels, srcImage1);
	imshow("融合蓝色通道效果", srcImage1);

	/*------------------- 这里需要重新读取图像数据------------------ */
	srcImage1 = imread(strSrc1.c_str());
	/* 颜色通道分离 */
	split(srcImage1, channels);

	imageBlueChannel = channels.at(0);
	imageGreenChannel = channels.at(1);
	imageRedChannel = channels.at(2);

	addWeighted(imageGreenChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)), 0.5, srcImage2, 0.5, 0.0, imageGreenChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)));
	merge(channels, srcImage1);
	imshow("融合绿色通道效果", srcImage1);

	/*------------------- 这里需要重新读取图像数据------------------ */
	srcImage1 = imread(strSrc1.c_str());
	/* 颜色通道分离 */
	split(srcImage1, channels);

	imageBlueChannel = channels.at(0);
	imageGreenChannel = channels.at(1);
	imageRedChannel = channels.at(2);

	addWeighted(imageRedChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)), 0.5, srcImage2, 0.5, 0.0, imageRedChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)));
	merge(channels, srcImage1);
	imshow("融合红色通道效果", srcImage1);

	waitKey();
}

/* 调整亮度和对比度
	原理和方法
*/
int g_nContrastValue = 100;
int g_nBrightValue = 60;
Mat g_srcImage, g_dstImage;
const char *pWindowName = "效果图像";
const char *pWindowNameOrig = "原始图像";

static void onContrastAndBright(int value)
{
	for (int i = 0; i < g_dstImage.rows; ++i) {
		for (int j = 0; j < g_dstImage.cols; ++j) {
			for (int k = 0; k < 3; ++k) {
				g_dstImage.at<Vec3b>(i, j)[k] = saturate_cast<uchar>(g_srcImage.at<Vec3b>(i, j)[k] * g_nContrastValue / 300 + g_nBrightValue);
			}
		}
	}
	imshow(pWindowName, g_dstImage);
}

void setContrastAndBright(const string &strImage)
{
	g_srcImage = imread(strImage);
	g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());

	/* 创建窗口 */
	namedWindow(pWindowName);
	namedWindow(pWindowNameOrig);

	/* 创建轨迹条 */
	/* 这里传入的g_n****等最终与轨迹条进行了绑定,其值随轨迹变化而变化 */
	cvCreateTrackbar("对比度: ", pWindowName, &g_nContrastValue, 300, onContrastAndBright); //300是最大值
	cvCreateTrackbar("亮度", pWindowName, &g_nBrightValue, 200, onContrastAndBright); // 200是最大值
	onContrastAndBright(g_nContrastValue);

	imshow(pWindowNameOrig, g_srcImage);

	while (char(waitKey(0) != 'q'));
}