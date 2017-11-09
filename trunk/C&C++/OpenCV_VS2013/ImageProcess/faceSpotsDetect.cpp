#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

Mat g_imgSrc, g_imgClone;
bool g_bMouseDown;
vector<vector<Point> > g_contours;
vector<Point> g_points;

bool findPimples(Mat img)
{
	Mat bw, bgr[3];
	/* 将只有轮廓部分的图进行split通道分离 */
	split(img, bgr);
	/* 这里取绿色通道 */
	bw = bgr[1];
	int pimplescount = 0;

	imshow("自适应阈值化之前", bw);
	/* 自适应阈值化：图像分割，去除一定范围内的像素 */
	/* bw必须是单通道的8bit图像 */
	/* 第1个参数是输入图像，第2个参数是输出图像，第3个参数是满足条件的最大像素值，第4个参数是所用算法，
		第6个参数是用来计算阈值的块大小(必须是奇数)，第7个参数是需要从加权平均值减去的一个常量 */
	adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 5);
	imshow("自适应阈值化之后", bw);

	/* 膨胀操作：前两个参数是输入与输出；参数3：膨胀操作的核，NULL时为3*3；参数4：锚的位置，下面代表位于中心；参数5：迭代使用dilate的次数 */
	dilate(bw, bw, Mat(), Point(-1, -1), 1);
	imshow("膨胀操作之后", bw);

	g_contours.clear();
	/* 查找轮廓:必须是8位单通道图像，参数4：可以提取最外层及所有轮廓 */
	findContours(bw, g_contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	for (size_t i = 0; i< g_contours.size(); i++)
	{

		if (contourArea(g_contours[i]) > 20 & contourArea(g_contours[i]) < 150)
		{
			Rect minRect = boundingRect(Mat(g_contours[i]));
			Mat imgroi(img, minRect);

			cvtColor(imgroi, imgroi, COLOR_BGR2HSV);
			Scalar color = mean(imgroi);
			cvtColor(imgroi, imgroi, COLOR_HSV2BGR);

			if (color[0] < 10 & color[1] > 70 & color[2] > 50)
			{
				Point2f center, vtx[4];
				float radius = 0;
				minEnclosingCircle(Mat(g_contours[i]), center, radius);

				if (radius < 20)
				{
					rectangle(img, minRect, Scalar(0, 255, 0));
					pimplescount++;
				}
			}
		}
	}
	putText(img, format("%d", pimplescount), Point(50, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 0), 2);

	imshow("pimples dedector", img);

	return true;
}


void onMouse(int event, int x, int y, int flags, void* userdata)
{
	/* 取原始图像 */
	Mat img = *((Mat *)userdata);

	if (event == EVENT_LBUTTONDOWN)
	{
		g_bMouseDown = true;
		g_contours.clear();
		g_points.clear();
	}

	if (event == EVENT_LBUTTONUP)
	{
		g_bMouseDown = false;
		if (g_points.size() > 2)
		{
			/* 创建一个通道并与原图大小相等的Mat */
			Mat mask(img.size(), CV_8UC1);
			/* 矩阵赋值为全0，颜色表现为全黑 */
			mask = 0;
			g_contours.push_back(g_points);
			/* 第一个参数是需要画轮廓的图像，第二个参数代表轮廓数组，第三个参数代表所用数组索引，第4个参数代表轮廓填充颜色，第5个参数是 */
			drawContours(mask, g_contours, 0, Scalar(255), -1);
			/* 创建一个三通道的mask */
			Mat masked(img.size(), CV_8UC3);
			masked = Scalar(255, 255, 255);
			/* 将画了轮廓的原图按照mask拷贝到masked；这里的mask只有轮廓部分颜色值是1，即只拷贝原图这块的内容到masked */
			img.copyTo(masked, mask);
			/* g_imgSrc始终保持不变 */
			g_imgClone = g_imgSrc.clone();
			findPimples(masked);
		}
	}

	if (g_bMouseDown)
	{
		if (g_points.size() > 2)
			/* 在img中画线：参数2是起点，参数3是终点，参数4是画线颜色 */
			line(img, Point(x, y), g_points[g_points.size() - 1], Scalar(0, 255, 0));

		g_points.push_back(Point(x, y));
		/* 鼠标按下后在原图上画轮廓，选定区域 */
		imshow("pimples dedector", img);
	}
}


int main(int argc, const char** argv)
{
	g_imgSrc = imread("images/faceWithSpots.jpg");
	if (g_imgSrc.empty())
	{
		return -1;
	}

	imshow("原图：", g_imgSrc);

	namedWindow("pimples dedector", WINDOW_AUTOSIZE);
	g_imgClone = g_imgSrc.clone();
	setMouseCallback("pimples dedector", onMouse, &g_imgClone);
	imshow("pimples dedector", g_imgSrc);

	while( 'q' != waitKey(0));
	return 0;
}