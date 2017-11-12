#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static bool findPimples(Mat srcImg, Mat img, std::vector<std::vector<cv::Point>> contours)
{
	Mat bw, bgr[3];
	/* ��ֻ���������ֵ�ͼ����splitͨ������ */
	split(img, bgr);
	/* ����ȡ��ɫͨ�� */
	//bw = bgr[1];
	/* ת��Ϊ�Ҷ�ͼ��Ч��������ȡ��һͨ��Ч���̫�� */
	cvtColor(img, bw, COLOR_BGR2GRAY);
	int pimplescount = 0;

	namedWindow("����Ӧ��ֵ��֮ǰ", WINDOW_NORMAL);
	imshow("����Ӧ��ֵ��֮ǰ", bw);
	/* ����Ӧ��ֵ����ͼ��ָȥ��һ����Χ�ڵ����� */
	/* bw�����ǵ�ͨ����8bitͼ�� */
	/* ��1������������ͼ�񣬵�2�����������ͼ�񣬵�3�������������������������ֵ����4�������������㷨��
		��6������������������ֵ�Ŀ��С(����������)����7����������Ҫ�Ӽ�Ȩƽ��ֵ��ȥ��һ������ */
	adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 5); //��������ʹ��15�����ŵģ������ٵ���
	//adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 13, 5);
	namedWindow("����Ӧ��ֵ��֮��", WINDOW_NORMAL);
	imshow("����Ӧ��ֵ��֮��", bw);

	/* ���Ͳ�����ǰ�������������������������3�����Ͳ����ĺˣ�NULLʱΪ3*3������4��ê��λ�ã��������λ�����ģ�����5������ʹ��dilate�Ĵ��� */
	dilate(bw, bw, Mat(), Point(-1, -1), 1);
	namedWindow("���Ͳ���֮��", WINDOW_NORMAL);
	imshow("���Ͳ���֮��", bw);

	contours.clear();
	/* ��������:������8λ��ͨ��ͼ�񣬲���4��������ȡ����㼰�������� */
	findContours(bw, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	cout << "��⵽�ı߽������" << contours.size() << endl;
	for (size_t i = 0; i< contours.size(); i++)
	{
	    cout << "�߽��С��" << contourArea(contours[i]) << endl;
		/* �����ֵҲ��Ҫ���� */
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

				/* �����ֵ��Ҫ���յ��� */
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

	namedWindow("ԭͼ��", WINDOW_NORMAL);
	imshow("ԭͼ��", imgSrc);

	/* ����һ��ͨ������ԭͼ��С��ȵ�Mat */
	Mat mask(imgSrc.size(), CV_8UC1);
	/* ����ֵΪȫ0����ɫ����Ϊȫ�� */
	mask = 0;
	/* ��һ����������Ҫ��������ͼ�񣬵ڶ������������������飬������������������������������4�������������������ɫ����5�������� */
	drawContours(mask, contours, -1, Scalar(255), -1);
	/* ����һ����ͨ����mask */
	Mat masked(imgSrc.size(), CV_8UC3);
	masked = Scalar(255, 255, 255);
	/* ������������ԭͼ����mask������masked�������maskֻ������������ɫֵ��1����ֻ����ԭͼ�������ݵ�masked */
	imgSrc.copyTo(masked, mask);
	/* imgSrcʼ�ձ��ֲ��� */
	findPimples(imgSrc, masked, contours);

	while( 'q' != waitKey(0));
	return 0;
}