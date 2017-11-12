
#include "showImage.h"
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;


/* ���õ����ݽṹ
��ı�ʾ��
	Point_<int>��Point2i��Point�ȼ�
	Point_<float>��Point2f����ȼ�
��ɫ�ı�ʾ��
	Scalar(r,g,b,alpha);
�ߴ�ı�ʾ��
	Size_<int>��Size2i��Size�໥�ȼ�
	Size(width, heigth);
���εı�ʾ��
	Rect()���䳣�õĳ�Ա�����У�Size()���سߴ磻area()���������contains(Point)�жϵ��Ƿ��ھ����ڣ�
	inside(rect)�жϾ����Ƿ��ھ����ڣ�tl()�������Ͻ����ꣻbr()�������½����ꣻ
	�������εĽ�����Rect rect = rect1 & rect2; ������Rect rect = rect1 | rect2;
	���ο���������ƽ�ƣ�Rect rect = rect1 + point; ���ţ�Rect rect = rect1 + size;
��ɫ�ռ�ת����
	cvtColor();����ʵ��RGB��HSV��HSI����ɫ�ռ��ת����Ҳ��ת��Ϊ�Ҷ�ͼ��
	���磺cvtColor(srcImage, dstImage, COLOR_GRAY2BGR);
*/

/* ����ͼ�εĻ���
����ֱ�ߵ�line������
������Բ��ellipse����
���ƾ��ε�rectangle����
����Բ��circle����
�������Ķ���ε�fillPoly����
*/

/* Look Up Table����
	operationsOnArrays::LUT()<lut>������������ͼ��Ԫ�ز��ҡ�ɨ�������
*/

/* ��ʱ����
	getTickCount()��getTickFrequency()�����溯������CPUһ�����߹���������
*/

/* ����ͼ�������ص����෽��
	1��ָ����ʣ�C��������uchar *data = outputImage.ptr<uchar>(i);��ȡ��i�е��׵�ַ��ֱ�Ӳ���uchar���ݣ�
	2����������iterator,ָ�����أ�(*iterator)[0],(*iterator)[1],(*iterator)[2]
		Mat_<Vec3b>::iterator it = outputImage.begin<Vec3b>();
		Mat_<Vec3b>::iterator itEnd = outputImage.end<Vec3b>();
	3����̬��ַ����
		outputImage.at<Vec3b>(i, j)[0],outputImage.at<Vec3b>(i, j)[1],outputImage.at<Vec3b>(i, j)[2]
*/

/* ROI�������
	1������ROI���������ַ�ʽ��һ����ָ��rect����һ����ָ��range
*/

/* ͼ���ں�
	addWeighted()��ע���ںϵ�����ͼ������С�����һ��
*/

/* ������ɫͨ��,��ͨ����ɫ���
	split��merge
*/

/* �������ȺͶԱȶ�
    ԭ��ͷ���
*/

/* ��ʾͼ�� 
	imshow()
*/


/* �ļ���ȡ����(xml/yaml)
	FileStorage
*/

void showImage(const string &strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("ԭʼͼ��", srcImage);
	cout << "ͼ��ľ�������Ϊ��\n" << srcImage;
	waitKey(0); // 0�������޵ȴ�����λ��ms
}

/* ͼ��ĸ�ʴ������Χ����ɫ������Լ�һ���ģ�
	erode()
*/
void ircodeImage(const string &strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("ԭʼͼ��", srcImage);

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat dstImage;
	erode(srcImage, dstImage, element); // ͼ����и�ʴ
	imshow("��ʾ��ʴЧ��ͼ", dstImage);
	waitKey(0); // 0�������޵ȴ�����λ��ms
}

/* ��ֵ�˲�����С��ɫ֮��Ĳ��� 
	blur()
*/
void blurImage(const string &strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("ԭʼͼ��", srcImage);
	Mat dstImage;
	blur(srcImage, dstImage, Size(7, 7));
	imshow("��ֵ�˲�Ч��", dstImage);
	waitKey(0); // 0�������޵ȴ�����λ��ms
}

/* ��Ե��⣺������б�Ե
	Canny()
*/
void cannyImage(const string &strPath)
{
	Mat srcImage = imread(strPath.c_str());
	imshow("ԭʼͼ��", srcImage);
	Mat dstImage, edgeImage, grayImage;
	dstImage.create(srcImage.size(), srcImage.type());

	/* ת��Ϊ�Ҷ�ͼ�� */
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	imshow("ת����ĻҶ�ͼ��", grayImage);
	/* �Ƚ��� */
	blur(grayImage, edgeImage, Size(3, 3));
	/* ����canny���� */
	Canny(edgeImage, edgeImage, 3, 9, 3);
	imshow("Canny��Ե���Ч��", edgeImage);
	waitKey(0); // 0�������޵ȴ�����λ��ms
}

/* ����ͼ���ļ� */
/* bool imgwrite(const string &fileName, InputArray img, const vector<in>& params = vector<int>(0) ) */

/* ͼ���ں�
	addWeighted()
*/
void imageAddWeighted(const string &strSrc1, const string &strSrc2)
{
	Mat srcImage1 = imread(strSrc1.c_str());
	Mat srcImage2 = imread(strSrc2.c_str());
	Mat dstImage;
	imshow("ԭʼͼ��1��", srcImage1);
	imshow("ԭʼͼ��2��", srcImage2);
	addWeighted(srcImage1, 0.5, srcImage2, 0.5, 0.0, dstImage);
	imshow("�ں�֮���ͼ��", dstImage);
	waitKey(0); // 0�������޵ȴ�����λ��ms
}

/* ROI */
void imageROI(const string &strSrc1, const string &strSrc2)
{
	Mat srcImage1 = imread(strSrc1.c_str());
	Mat srcImage2 = imread(strSrc2.c_str());

	/* ѡȡ����Ȥ��������ã������ַ�����һ���Ǿ���������һ����range���� */
	Mat imageROI = srcImage1(Rect(200, 250, srcImage2.cols, srcImage2.rows));
	imshow("ԭʼͼ��", srcImage1);

	/* ������Ĥ */
	Mat mask = imread(strSrc2.c_str(), 0); /* �Ҷ�ͼ */
	/* ����Ĥ���Ƶ�ROI */
	//srcImage2.copyTo(imageROI, mask); /* �����ͼ����Ĥ����Ч����̫���� */
	srcImage2.copyTo(imageROI);

	/* ����Ҳ����ʹ��ͼ�������غ�������(ע�������ںϵ�ͼ���С����ȱ���һ��) */
	//Mat dstImage;
	//addWeighted(imageROI, 0.0, srcImage2, 1.0, 0, imageROI); /* ע��������Ӻ��������Ҳ�ڸ���Ȥ������ */

	imshow("����ROIʵ��ͼ����ӵ�Ч��", srcImage1);
	waitKey();
}

/* ������ɫͨ��,��ͨ����ɫ���
	split��merge
*/
void imageSplit(const string &strSrc1, const string &strSrc2)
{
	vector<Mat> channels;
	channels.resize(3);
	//Mat channels[3]; /* ����Ҳ����ʹ������ */

	/* ����һ��Ҫע�����潫ͼ��ת�����˻Ҷ�ͼ,�������addWeigthed��ʱ�����Ϊͨ����һ����������� */
	Mat srcImage2 = imread(strSrc2.c_str(), 0);

	Mat srcImage1 = imread(strSrc1.c_str());
	/* ��ɫͨ������ */
	split(srcImage1, channels);

	Mat imageBlueChannel = channels.at(0);
	Mat imageGreenChannel = channels.at(1);
	Mat imageRedChannel = channels.at(2);

	addWeighted(imageBlueChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)), 0.5, srcImage2, 0.5, 0.0, imageBlueChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)));
	merge(channels, srcImage1);
	imshow("�ں���ɫͨ��Ч��", srcImage1);

	/*------------------- ������Ҫ���¶�ȡͼ������------------------ */
	srcImage1 = imread(strSrc1.c_str());
	/* ��ɫͨ������ */
	split(srcImage1, channels);

	imageBlueChannel = channels.at(0);
	imageGreenChannel = channels.at(1);
	imageRedChannel = channels.at(2);

	addWeighted(imageGreenChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)), 0.5, srcImage2, 0.5, 0.0, imageGreenChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)));
	merge(channels, srcImage1);
	imshow("�ں���ɫͨ��Ч��", srcImage1);

	/*------------------- ������Ҫ���¶�ȡͼ������------------------ */
	srcImage1 = imread(strSrc1.c_str());
	/* ��ɫͨ������ */
	split(srcImage1, channels);

	imageBlueChannel = channels.at(0);
	imageGreenChannel = channels.at(1);
	imageRedChannel = channels.at(2);

	addWeighted(imageRedChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)), 0.5, srcImage2, 0.5, 0.0, imageRedChannel(Rect(550, 200, srcImage2.cols, srcImage2.rows)));
	merge(channels, srcImage1);
	imshow("�ںϺ�ɫͨ��Ч��", srcImage1);

	waitKey();
}

/* �������ȺͶԱȶ�
	ԭ��ͷ���
*/
int g_nContrastValue = 100;
int g_nBrightValue = 60;
Mat g_srcImage, g_dstImage;
const char *pWindowName = "Ч��ͼ��";
const char *pWindowNameOrig = "ԭʼͼ��";

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

	/* �������� */
	namedWindow(pWindowName);
	namedWindow(pWindowNameOrig);

	/* �����켣�� */
	/* ���ﴫ���g_n****��������켣�������˰�,��ֵ��켣�仯���仯 */
	cvCreateTrackbar("�Աȶ�: ", pWindowName, &g_nContrastValue, 300, onContrastAndBright); //300�����ֵ
	cvCreateTrackbar("����", pWindowName, &g_nBrightValue, 200, onContrastAndBright); // 200�����ֵ
	onContrastAndBright(g_nContrastValue);

	imshow(pWindowNameOrig, g_srcImage);

	while (char(waitKey(0) != 'q'));
}