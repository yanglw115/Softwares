#include "imageProcess.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

/* 线性滤波：方框滤波、均值滤波、高斯滤波
	方框滤波：boxFilter()
	均值滤波：blur()
	高斯滤波：GaussianBlur()
*/

/* 非线性滤波
	中值滤波: medianBlur(),它的kSize必须是大于1的奇数 
	双边滤波: bilateralFilter()
*/

/* 形态学滤波(1)
	最基本的形态学操作：膨胀(dilate)和腐蚀(erode)
	腐蚀和膨胀是对白色部分(高亮部分)而言，不是黑色部分。
	膨胀是对图像中的高亮部分进行扩张，效果图拥有比原图更大的高亮区域；腐蚀则相反；
*/

/* 形态学滤波(2)
	形态学的高级形态，往往都是建立在膨胀和腐蚀基础之上的。
	开运算：开运算其实就是先腐蚀后膨胀的过程；可以用来消除小物体，在纤细处分离物体，并且在平滑较大物体的边界的同时不明显改变其面积；
	闭运算：先膨胀后腐蚀的过程；能够排除小型黑洞(黑色区域);
	形态学梯度：膨胀图与腐蚀图之差。对二值图像进行这一操作，可将团块的边缘突出出来；可以使用形态学梯度来保留物体的边缘轮廓；
	顶帽：是原图与“开运算”的结果图之差；往往用来分离比邻近点亮一些的斑块；在一幅具有大幅背景的而微小物品比较有规律的情况下，可以使用顶冒运算进行背景提取； 
	黑帽：黑帽运算是“闭运算”与原图的结果图之差。其运算结果突出了比原图轮廓周围的区域更暗的区域；用来分离比邻近点暗的一些斑块，效果图有着非常完美的轮廓；
*/

/* 漫水填充
	用一种特定的颜色来填充连通区域，通过设置可连通像素的上下限及连通方式来达到不同的填充效果的方法。漫水填充经常被用来标记或分离图像的一部分，以便对其进一步的处理或分析。
	也可以用来从输入图像获取掩码区域，掩码会加速处理过程，或只处理掩码指定的像素点，操作的结果总是某个连续的区域。
	在OpenCV中，漫水填充是填充算法中最通用的方法。
	floodFill()：用我们指定的颜色从种子点开始填充一个连接域。连通性由像素值的接近程序来衡量。
	有两种接口，一个带mask掩膜，另一个不带；
*/

/* 图像金字塔与图片尺寸缩放
	图像金字塔主要有两种：
	高斯金字塔：向下采样，主要的图片金字塔；图像会越来越小，分辨率也越来越小；（这里的向下，可以理解从0到1,2,3......即向金字塔塔顶）
		高斯金字塔包括了一系列的低通滤波器。
	拉普拉斯金字塔：向上采样。用于对图像进行最大程度的还原，即预测残差。配合高斯金字塔一起使用；
	图像金字塔非常重要的一个应用就是图像分割。
	图像金字塔的主要api是pyrUp()和pyrDown()
	尺寸调整：resize()函数
*/

/* 阈值化
	在对各种图像进行操作处理的过程中，我们常常需要对图像中的像素作出取舍与决策；直接剔除一些低于或者高于一定值的像素。
	阈值可以认为是最简单的图像分割方法。
	Threshhold()：固定阈值操作
	AdaptiveThreshhold()：自适应阈值操作
*/

/* 图像转换
	边缘检测：Canny()
		Sobel()：使用扩展的sobel算子来计算一阶、二阶、三阶或混合图像差分；
		Laplacian()：可以计算图像经过拉普拉斯变换后的结果；
		Scharr()：
	霍夫变换：
		是图像处理中的一种特征提取技术，该过程在一个参数空间中通过计算累计结果的局部最大值得到一个符合该特定形状的集合作为霍夫变换结果。
		霍夫线变换：
			标准霍夫变换：HoughLines()
			多尺度霍夫变换：HoughLines()
			累计概率霍夫变换：HoughLinesP()
		霍夫圆变换：
			HoughCircles()
	重映射：
		remap()：比如x轴翻转等
	仿射变换：(又称仿射映射)
		wrapAffine()、getRotationMatrix2D()
	直方图均衡化：
		使图像颜色更加饱满，从0~255区间都有值而不局限于某一定区域，使频谱更加舒展，有效利用了0~255的空间；
*/

static const char *g_pWindowBoxFilter = "方框滤波效果";
static const char *g_pWindowBlurFilter = "均值滤波效果";
static const char *g_pWindowGaussianFilter = "高斯滤波效果";
static const char *g_pWindowMedianFilter = "中值滤波效果";
static const char *g_pWindowBilalteralFilter = "双边滤波效果";
static const char *g_pWindowErode = "腐蚀效果";
static const char *g_pWindowDilate = "膨胀效果";

static const char *g_pWindowOpen = "形态学滤波：开运算";
static const char *g_pWindowClose = "形态学滤波：闭运算";
static const char *g_pWindowGradient = "形态学滤波：梯度运算";
static const char *g_pWindowTophat = "形态学滤波：顶冒运算";
static const char *g_pWindowBlackhat = "形态学滤波：黑冒运算";
static const char *g_pWindowErodeMorph = "形态学滤波：腐蚀运算";
static const char *g_pWindowDilateMorph = "形态学滤波：膨胀运算";

static int g_nBoxFilterValue = 3;
static int g_nBlurFilterValue = 3;
static int g_nGaussianFilterValue = 3;
static int g_nMedianFilterValue = 3;
static int g_nBilalteralFilterValue = 3;
static int g_nErodeValue = 3;
static int g_nDilateValue = 3;

static int g_nOpenValue = 3;
static int g_nCloseValue = 3;
static int g_nGradientValue = 3;
static int g_nTophatValue = 3;
static int g_nBlackhatValue = 3;
static int g_nErodeMorphValue = 3;
static int g_nDilateMorphValue = 3;

static Mat g_srcImage;

static void onBoxFilter(int value)
{
	Mat dstBoxFilter;
	boxFilter(g_srcImage, dstBoxFilter, -1, Size(g_nBoxFilterValue + 1, g_nBoxFilterValue + 1));
	imshow("方框滤波效果", dstBoxFilter);
}

static void onBlurFilter(int value)
{
	Mat dstBlurFilter;
	boxFilter(g_srcImage, dstBlurFilter, -1, Size(g_nBlurFilterValue + 1, g_nBlurFilterValue + 1));
	imshow("均值滤波效果", dstBlurFilter);
}

static void onGaussianFilter(int value)
{
	Mat dstGaussianFilter;
	boxFilter(g_srcImage, dstGaussianFilter, -1, Size(g_nGaussianFilterValue + 1, g_nGaussianFilterValue + 1));
	imshow("高斯滤波效果", dstGaussianFilter);
}

static void onMedianFilter(int value)
{
	Mat dstMedianFilter;
	medianBlur(g_srcImage, dstMedianFilter, g_nMedianFilterValue * 2 + 1);
	imshow("中值滤波效果", dstMedianFilter);
}

static void onBilalteralFilter(int value)
{
	Mat dstBilalteralFilter;
	bilateralFilter(g_srcImage, dstBilalteralFilter, g_nBilalteralFilterValue, g_nBilalteralFilterValue * 2, g_nBilalteralFilterValue / 2);
	imshow("双边滤波效果", dstBilalteralFilter);
}

static void onErode(int value)
{
	Mat dstErode;
	/* 自定义内核矩阵，也可以使用默认的。Size()内核大小，Point()锚点 */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nErodeValue * 2 + 1, g_nErodeValue * 2 + 1), Point(g_nErodeValue, g_nErodeValue));
	erode(g_srcImage, dstErode, element);
	imshow("腐蚀效果", dstErode);
}

static void onDilate(int value)
{
	Mat dstDilate;
	/* 自定义内核矩阵，也可以使用默认的。Size()内核大小，Point()锚点 */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nDilateValue * 2 + 1, g_nDilateValue * 2 + 1), Point(g_nDilateValue, g_nDilateValue));
	dilate(g_srcImage, dstDilate, element);
	imshow("膨胀效果", dstDilate);
}

/* 形态学运算 -------------------------------------------------------------------*/

static void onOpen(int value)
{
	Mat dstImage;
	/* 自定义内核矩阵，也可以使用默认的。Size()内核大小，Point()锚点 */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nOpenValue * 2 + 1, g_nOpenValue * 2 + 1), Point(g_nOpenValue, g_nOpenValue));
	morphologyEx(g_srcImage, dstImage, MORPH_OPEN, element);
	imshow(g_pWindowOpen, dstImage);
}

static void onClose(int value)
{
	Mat dstImage;
	/* 自定义内核矩阵，也可以使用默认的。Size()内核大小，Point()锚点 */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nCloseValue * 2 + 1, g_nCloseValue * 2 + 1), Point(g_nCloseValue, g_nCloseValue));
	morphologyEx(g_srcImage, dstImage, MORPH_CLOSE, element);
	imshow(g_pWindowClose, dstImage);
}

static void onGradient(int value)
{
	Mat dstImage;
	/* 自定义内核矩阵，也可以使用默认的。Size()内核大小，Point()锚点 */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nGradientValue * 2 + 1, g_nGradientValue * 2 + 1), Point(g_nGradientValue, g_nGradientValue));
	morphologyEx(g_srcImage, dstImage, MORPH_GRADIENT, element);
	imshow(g_pWindowGradient, dstImage);
}

static void onTophat(int value)
{
	Mat dstImage;
	/* 自定义内核矩阵，也可以使用默认的。Size()内核大小，Point()锚点 */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nTophatValue * 2 + 1, g_nTophatValue * 2 + 1), Point(g_nTophatValue, g_nTophatValue));
	morphologyEx(g_srcImage, dstImage, MORPH_TOPHAT, element);
	imshow(g_pWindowTophat, dstImage);
}

static void onBlackhat(int value)
{
	Mat dstImage;
	/* 自定义内核矩阵，也可以使用默认的。Size()内核大小，Point()锚点 */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nBlackhatValue * 2 + 1, g_nBlackhatValue * 2 + 1), Point(g_nBlackhatValue, g_nBlackhatValue));
	morphologyEx(g_srcImage, dstImage, MORPH_BLACKHAT, element);
	imshow(g_pWindowBlackhat, dstImage);
}

static void onDilateMorph(int value)
{
	Mat dstImage;
	/* 自定义内核矩阵，也可以使用默认的。Size()内核大小，Point()锚点 */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nDilateMorphValue * 2 + 1, g_nDilateMorphValue * 2 + 1), Point(g_nDilateMorphValue, g_nDilateMorphValue));
	morphologyEx(g_srcImage, dstImage, MORPH_DILATE, element);
	imshow(g_pWindowDilateMorph, dstImage);
}

static void onErodeMorph(int value)
{
	Mat dstImage;
	/* 自定义内核矩阵，也可以使用默认的。Size()内核大小，Point()锚点 */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nErodeMorphValue * 2 + 1, g_nErodeMorphValue * 2 + 1), Point(g_nErodeMorphValue, g_nErodeMorphValue));
	morphologyEx(g_srcImage, dstImage, MORPH_ERODE, element);
	imshow(g_pWindowErodeMorph, dstImage);
}

void imageFilter(const string strFile)
{
	g_srcImage = imread(strFile.c_str());
	imshow("原始图像", g_srcImage);

#if 0
	namedWindow(g_pWindowBoxFilter);
	namedWindow(g_pWindowBlurFilter);
	namedWindow(g_pWindowGaussianFilter);
	namedWindow(g_pWindowMedianFilter);
	namedWindow(g_pWindowBilalteralFilter);
	namedWindow(g_pWindowErode);
	namedWindow(g_pWindowDilate);

	cvCreateTrackbar("内核大小: ", g_pWindowBoxFilter, &g_nBoxFilterValue, 40, onBoxFilter);
	cvCreateTrackbar("内核大小: ", g_pWindowBlurFilter, &g_nBlurFilterValue, 40, onBlurFilter);
	cvCreateTrackbar("内核大小: ", g_pWindowGaussianFilter, &g_nGaussianFilterValue, 40, onGaussianFilter);
	cvCreateTrackbar("参数值: ", g_pWindowMedianFilter, &g_nMedianFilterValue, 40, onMedianFilter);
	cvCreateTrackbar("参数值: ", g_pWindowBilalteralFilter, &g_nBilalteralFilterValue, 40, onBilalteralFilter);
	cvCreateTrackbar("内核大小: ", g_pWindowErode, &g_nErodeValue, 40, onErode);
	cvCreateTrackbar("内核大小: ", g_pWindowDilate, &g_nDilateValue, 40, onDilate);

	onBoxFilter(0);
	onBlurFilter(0);
	onGaussianFilter(0);
	onMedianFilter(0);
	onBilalteralFilter(0);
	onErode(0);
	onDilate(0);
#endif

/* 形态学运算 -------------------------------------------------------------------*/
#if 0
	namedWindow(g_pWindowOpen);
	namedWindow(g_pWindowClose);
	namedWindow(g_pWindowGradient);
	namedWindow(g_pWindowTophat);
	namedWindow(g_pWindowBlackhat);
	namedWindow(g_pWindowErodeMorph);
	namedWindow(g_pWindowDilateMorph);

	cvCreateTrackbar("内核大小: ", g_pWindowOpen, &g_nOpenValue, 100, onOpen);
	cvCreateTrackbar("内核大小: ", g_pWindowClose, &g_nCloseValue, 100, onClose);
	cvCreateTrackbar("内核大小: ", g_pWindowGradient, &g_nGradientValue, 100, onGradient);
	cvCreateTrackbar("内核大小: ", g_pWindowTophat, &g_nTophatValue, 100, onTophat);
	cvCreateTrackbar("内核大小: ", g_pWindowBlackhat, &g_nBlackhatValue, 100, onBlackhat);
	cvCreateTrackbar("内核大小: ", g_pWindowErodeMorph, &g_nErodeMorphValue, 100, onErodeMorph);
	cvCreateTrackbar("内核大小: ", g_pWindowDilateMorph, &g_nDilateMorphValue, 100, onDilateMorph);

	onOpen(0);
	onClose(0);
	onGradient(0);
	onTophat(0);
	onBlackhat(0);
	onErodeMorph(0);
	onDilateMorph(0);

#endif

/* 漫水填充 -------------------------------------------------------------------*/
#if 1
	imageFloodFill(strFile);
#endif
	waitKey();
}

/* 漫水填充 -------------------------------------------------------------------*/
Mat imageSrc, imageDst, imageMask, imageGray;

void imageFloodFill(const string &strFile)
{
	
}

