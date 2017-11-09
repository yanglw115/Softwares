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
	/* ��ֻ���������ֵ�ͼ����splitͨ������ */
	split(img, bgr);
	/* ����ȡ��ɫͨ�� */
	bw = bgr[1];
	int pimplescount = 0;

	imshow("����Ӧ��ֵ��֮ǰ", bw);
	/* ����Ӧ��ֵ����ͼ��ָȥ��һ����Χ�ڵ����� */
	/* bw�����ǵ�ͨ����8bitͼ�� */
	/* ��1������������ͼ�񣬵�2�����������ͼ�񣬵�3�������������������������ֵ����4�������������㷨��
		��6������������������ֵ�Ŀ��С(����������)����7����������Ҫ�Ӽ�Ȩƽ��ֵ��ȥ��һ������ */
	adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 5);
	imshow("����Ӧ��ֵ��֮��", bw);

	/* ���Ͳ�����ǰ�������������������������3�����Ͳ����ĺˣ�NULLʱΪ3*3������4��ê��λ�ã��������λ�����ģ�����5������ʹ��dilate�Ĵ��� */
	dilate(bw, bw, Mat(), Point(-1, -1), 1);
	imshow("���Ͳ���֮��", bw);

	g_contours.clear();
	/* ��������:������8λ��ͨ��ͼ�񣬲���4��������ȡ����㼰�������� */
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
	/* ȡԭʼͼ�� */
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
			/* ����һ��ͨ������ԭͼ��С��ȵ�Mat */
			Mat mask(img.size(), CV_8UC1);
			/* ����ֵΪȫ0����ɫ����Ϊȫ�� */
			mask = 0;
			g_contours.push_back(g_points);
			/* ��һ����������Ҫ��������ͼ�񣬵ڶ������������������飬������������������������������4�������������������ɫ����5�������� */
			drawContours(mask, g_contours, 0, Scalar(255), -1);
			/* ����һ����ͨ����mask */
			Mat masked(img.size(), CV_8UC3);
			masked = Scalar(255, 255, 255);
			/* ������������ԭͼ����mask������masked�������maskֻ������������ɫֵ��1����ֻ����ԭͼ�������ݵ�masked */
			img.copyTo(masked, mask);
			/* g_imgSrcʼ�ձ��ֲ��� */
			g_imgClone = g_imgSrc.clone();
			findPimples(masked);
		}
	}

	if (g_bMouseDown)
	{
		if (g_points.size() > 2)
			/* ��img�л��ߣ�����2����㣬����3���յ㣬����4�ǻ�����ɫ */
			line(img, Point(x, y), g_points[g_points.size() - 1], Scalar(0, 255, 0));

		g_points.push_back(Point(x, y));
		/* ��갴�º���ԭͼ�ϻ�������ѡ������ */
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

	imshow("ԭͼ��", g_imgSrc);

	namedWindow("pimples dedector", WINDOW_AUTOSIZE);
	g_imgClone = g_imgSrc.clone();
	setMouseCallback("pimples dedector", onMouse, &g_imgClone);
	imshow("pimples dedector", g_imgSrc);

	while( 'q' != waitKey(0));
	return 0;
}