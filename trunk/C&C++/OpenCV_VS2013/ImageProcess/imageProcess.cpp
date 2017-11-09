#include "imageProcess.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

/* �����˲��������˲�����ֵ�˲�����˹�˲�
	�����˲���boxFilter()
	��ֵ�˲���blur()
	��˹�˲���GaussianBlur()
*/

/* �������˲�
	��ֵ�˲�: medianBlur(),����kSize�����Ǵ���1������ 
	˫���˲�: bilateralFilter()
*/

/* ��̬ѧ�˲�(1)
	���������̬ѧ����������(dilate)�͸�ʴ(erode)
	��ʴ�������Ƕ԰�ɫ����(��������)���ԣ����Ǻ�ɫ���֡�
	�����Ƕ�ͼ���еĸ������ֽ������ţ�Ч��ͼӵ�б�ԭͼ����ĸ������򣻸�ʴ���෴��
*/

/* ��̬ѧ�˲�(2)
	��̬ѧ�ĸ߼���̬���������ǽ��������ͺ͸�ʴ����֮�ϵġ�
	�����㣺��������ʵ�����ȸ�ʴ�����͵Ĺ��̣�������������С���壬����ϸ���������壬������ƽ���ϴ�����ı߽��ͬʱ�����Ըı��������
	�����㣺�����ͺ�ʴ�Ĺ��̣��ܹ��ų�С�ͺڶ�(��ɫ����);
	��̬ѧ�ݶȣ�����ͼ�븯ʴͼ֮��Զ�ֵͼ�������һ�������ɽ��ſ�ı�Եͻ������������ʹ����̬ѧ�ݶ�����������ı�Ե������
	��ñ����ԭͼ�롰�����㡱�Ľ��ͼ֮���������������ڽ�����һЩ�İ߿飻��һ�����д�������Ķ�΢С��Ʒ�Ƚ��й��ɵ�����£�����ʹ�ö�ð������б�����ȡ�� 
	��ñ����ñ�����ǡ������㡱��ԭͼ�Ľ��ͼ֮���������ͻ���˱�ԭͼ������Χ�������������������������ڽ��㰵��һЩ�߿飬Ч��ͼ���ŷǳ�������������
*/

/* ��ˮ���
	��һ���ض�����ɫ�������ͨ����ͨ�����ÿ���ͨ���ص������޼���ͨ��ʽ���ﵽ��ͬ�����Ч���ķ�������ˮ��侭����������ǻ����ͼ���һ���֣��Ա�����һ���Ĵ���������
	Ҳ��������������ͼ���ȡ���������������ٴ�����̣���ֻ��������ָ�������ص㣬�����Ľ������ĳ������������
	��OpenCV�У���ˮ���������㷨����ͨ�õķ�����
	floodFill()��������ָ������ɫ�����ӵ㿪ʼ���һ����������ͨ��������ֵ�Ľӽ�������������
	�����ֽӿڣ�һ����mask��Ĥ����һ��������
*/

/* ͼ���������ͼƬ�ߴ�����
	ͼ���������Ҫ�����֣�
	��˹�����������²�������Ҫ��ͼƬ��������ͼ���Խ��ԽС���ֱ���ҲԽ��ԽС������������£���������0��1,2,3......���������������
		��˹������������һϵ�еĵ�ͨ�˲�����
	������˹�����������ϲ��������ڶ�ͼ��������̶ȵĻ�ԭ����Ԥ��в��ϸ�˹������һ��ʹ�ã�
	ͼ��������ǳ���Ҫ��һ��Ӧ�þ���ͼ��ָ
	ͼ�����������Ҫapi��pyrUp()��pyrDown()
	�ߴ������resize()����
*/

/* ��ֵ��
	�ڶԸ���ͼ����в�������Ĺ����У����ǳ�����Ҫ��ͼ���е���������ȡ������ߣ�ֱ���޳�һЩ���ڻ��߸���һ��ֵ�����ء�
	��ֵ������Ϊ����򵥵�ͼ��ָ����
	Threshhold()���̶���ֵ����
	AdaptiveThreshhold()������Ӧ��ֵ����
*/

/* ͼ��ת��
	��Ե��⣺Canny()
		Sobel()��ʹ����չ��sobel����������һ�ס����ס����׻���ͼ���֣�
		Laplacian()�����Լ���ͼ�񾭹�������˹�任��Ľ����
		Scharr()��
	����任��
		��ͼ�����е�һ��������ȡ�������ù�����һ�������ռ���ͨ�������ۼƽ���ľֲ����ֵ�õ�һ�����ϸ��ض���״�ļ�����Ϊ����任�����
		�����߱任��
			��׼����任��HoughLines()
			��߶Ȼ���任��HoughLines()
			�ۼƸ��ʻ���任��HoughLinesP()
		����Բ�任��
			HoughCircles()
	��ӳ�䣺
		remap()������x�ᷭת��
	����任��(�ֳƷ���ӳ��)
		wrapAffine()��getRotationMatrix2D()
	ֱ��ͼ���⻯��
		ʹͼ����ɫ���ӱ�������0~255���䶼��ֵ����������ĳһ������ʹƵ�׸�����չ����Ч������0~255�Ŀռ䣻
*/

static const char *g_pWindowBoxFilter = "�����˲�Ч��";
static const char *g_pWindowBlurFilter = "��ֵ�˲�Ч��";
static const char *g_pWindowGaussianFilter = "��˹�˲�Ч��";
static const char *g_pWindowMedianFilter = "��ֵ�˲�Ч��";
static const char *g_pWindowBilalteralFilter = "˫���˲�Ч��";
static const char *g_pWindowErode = "��ʴЧ��";
static const char *g_pWindowDilate = "����Ч��";

static const char *g_pWindowOpen = "��̬ѧ�˲���������";
static const char *g_pWindowClose = "��̬ѧ�˲���������";
static const char *g_pWindowGradient = "��̬ѧ�˲����ݶ�����";
static const char *g_pWindowTophat = "��̬ѧ�˲�����ð����";
static const char *g_pWindowBlackhat = "��̬ѧ�˲�����ð����";
static const char *g_pWindowErodeMorph = "��̬ѧ�˲�����ʴ����";
static const char *g_pWindowDilateMorph = "��̬ѧ�˲�����������";

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
	imshow("�����˲�Ч��", dstBoxFilter);
}

static void onBlurFilter(int value)
{
	Mat dstBlurFilter;
	boxFilter(g_srcImage, dstBlurFilter, -1, Size(g_nBlurFilterValue + 1, g_nBlurFilterValue + 1));
	imshow("��ֵ�˲�Ч��", dstBlurFilter);
}

static void onGaussianFilter(int value)
{
	Mat dstGaussianFilter;
	boxFilter(g_srcImage, dstGaussianFilter, -1, Size(g_nGaussianFilterValue + 1, g_nGaussianFilterValue + 1));
	imshow("��˹�˲�Ч��", dstGaussianFilter);
}

static void onMedianFilter(int value)
{
	Mat dstMedianFilter;
	medianBlur(g_srcImage, dstMedianFilter, g_nMedianFilterValue * 2 + 1);
	imshow("��ֵ�˲�Ч��", dstMedianFilter);
}

static void onBilalteralFilter(int value)
{
	Mat dstBilalteralFilter;
	bilateralFilter(g_srcImage, dstBilalteralFilter, g_nBilalteralFilterValue, g_nBilalteralFilterValue * 2, g_nBilalteralFilterValue / 2);
	imshow("˫���˲�Ч��", dstBilalteralFilter);
}

static void onErode(int value)
{
	Mat dstErode;
	/* �Զ����ں˾���Ҳ����ʹ��Ĭ�ϵġ�Size()�ں˴�С��Point()ê�� */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nErodeValue * 2 + 1, g_nErodeValue * 2 + 1), Point(g_nErodeValue, g_nErodeValue));
	erode(g_srcImage, dstErode, element);
	imshow("��ʴЧ��", dstErode);
}

static void onDilate(int value)
{
	Mat dstDilate;
	/* �Զ����ں˾���Ҳ����ʹ��Ĭ�ϵġ�Size()�ں˴�С��Point()ê�� */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nDilateValue * 2 + 1, g_nDilateValue * 2 + 1), Point(g_nDilateValue, g_nDilateValue));
	dilate(g_srcImage, dstDilate, element);
	imshow("����Ч��", dstDilate);
}

/* ��̬ѧ���� -------------------------------------------------------------------*/

static void onOpen(int value)
{
	Mat dstImage;
	/* �Զ����ں˾���Ҳ����ʹ��Ĭ�ϵġ�Size()�ں˴�С��Point()ê�� */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nOpenValue * 2 + 1, g_nOpenValue * 2 + 1), Point(g_nOpenValue, g_nOpenValue));
	morphologyEx(g_srcImage, dstImage, MORPH_OPEN, element);
	imshow(g_pWindowOpen, dstImage);
}

static void onClose(int value)
{
	Mat dstImage;
	/* �Զ����ں˾���Ҳ����ʹ��Ĭ�ϵġ�Size()�ں˴�С��Point()ê�� */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nCloseValue * 2 + 1, g_nCloseValue * 2 + 1), Point(g_nCloseValue, g_nCloseValue));
	morphologyEx(g_srcImage, dstImage, MORPH_CLOSE, element);
	imshow(g_pWindowClose, dstImage);
}

static void onGradient(int value)
{
	Mat dstImage;
	/* �Զ����ں˾���Ҳ����ʹ��Ĭ�ϵġ�Size()�ں˴�С��Point()ê�� */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nGradientValue * 2 + 1, g_nGradientValue * 2 + 1), Point(g_nGradientValue, g_nGradientValue));
	morphologyEx(g_srcImage, dstImage, MORPH_GRADIENT, element);
	imshow(g_pWindowGradient, dstImage);
}

static void onTophat(int value)
{
	Mat dstImage;
	/* �Զ����ں˾���Ҳ����ʹ��Ĭ�ϵġ�Size()�ں˴�С��Point()ê�� */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nTophatValue * 2 + 1, g_nTophatValue * 2 + 1), Point(g_nTophatValue, g_nTophatValue));
	morphologyEx(g_srcImage, dstImage, MORPH_TOPHAT, element);
	imshow(g_pWindowTophat, dstImage);
}

static void onBlackhat(int value)
{
	Mat dstImage;
	/* �Զ����ں˾���Ҳ����ʹ��Ĭ�ϵġ�Size()�ں˴�С��Point()ê�� */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nBlackhatValue * 2 + 1, g_nBlackhatValue * 2 + 1), Point(g_nBlackhatValue, g_nBlackhatValue));
	morphologyEx(g_srcImage, dstImage, MORPH_BLACKHAT, element);
	imshow(g_pWindowBlackhat, dstImage);
}

static void onDilateMorph(int value)
{
	Mat dstImage;
	/* �Զ����ں˾���Ҳ����ʹ��Ĭ�ϵġ�Size()�ں˴�С��Point()ê�� */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nDilateMorphValue * 2 + 1, g_nDilateMorphValue * 2 + 1), Point(g_nDilateMorphValue, g_nDilateMorphValue));
	morphologyEx(g_srcImage, dstImage, MORPH_DILATE, element);
	imshow(g_pWindowDilateMorph, dstImage);
}

static void onErodeMorph(int value)
{
	Mat dstImage;
	/* �Զ����ں˾���Ҳ����ʹ��Ĭ�ϵġ�Size()�ں˴�С��Point()ê�� */
	Mat element = getStructuringElement(MORPH_RECT, Size(g_nErodeMorphValue * 2 + 1, g_nErodeMorphValue * 2 + 1), Point(g_nErodeMorphValue, g_nErodeMorphValue));
	morphologyEx(g_srcImage, dstImage, MORPH_ERODE, element);
	imshow(g_pWindowErodeMorph, dstImage);
}

void imageFilter(const string strFile)
{
	g_srcImage = imread(strFile.c_str());
	imshow("ԭʼͼ��", g_srcImage);

#if 0
	namedWindow(g_pWindowBoxFilter);
	namedWindow(g_pWindowBlurFilter);
	namedWindow(g_pWindowGaussianFilter);
	namedWindow(g_pWindowMedianFilter);
	namedWindow(g_pWindowBilalteralFilter);
	namedWindow(g_pWindowErode);
	namedWindow(g_pWindowDilate);

	cvCreateTrackbar("�ں˴�С: ", g_pWindowBoxFilter, &g_nBoxFilterValue, 40, onBoxFilter);
	cvCreateTrackbar("�ں˴�С: ", g_pWindowBlurFilter, &g_nBlurFilterValue, 40, onBlurFilter);
	cvCreateTrackbar("�ں˴�С: ", g_pWindowGaussianFilter, &g_nGaussianFilterValue, 40, onGaussianFilter);
	cvCreateTrackbar("����ֵ: ", g_pWindowMedianFilter, &g_nMedianFilterValue, 40, onMedianFilter);
	cvCreateTrackbar("����ֵ: ", g_pWindowBilalteralFilter, &g_nBilalteralFilterValue, 40, onBilalteralFilter);
	cvCreateTrackbar("�ں˴�С: ", g_pWindowErode, &g_nErodeValue, 40, onErode);
	cvCreateTrackbar("�ں˴�С: ", g_pWindowDilate, &g_nDilateValue, 40, onDilate);

	onBoxFilter(0);
	onBlurFilter(0);
	onGaussianFilter(0);
	onMedianFilter(0);
	onBilalteralFilter(0);
	onErode(0);
	onDilate(0);
#endif

/* ��̬ѧ���� -------------------------------------------------------------------*/
#if 0
	namedWindow(g_pWindowOpen);
	namedWindow(g_pWindowClose);
	namedWindow(g_pWindowGradient);
	namedWindow(g_pWindowTophat);
	namedWindow(g_pWindowBlackhat);
	namedWindow(g_pWindowErodeMorph);
	namedWindow(g_pWindowDilateMorph);

	cvCreateTrackbar("�ں˴�С: ", g_pWindowOpen, &g_nOpenValue, 100, onOpen);
	cvCreateTrackbar("�ں˴�С: ", g_pWindowClose, &g_nCloseValue, 100, onClose);
	cvCreateTrackbar("�ں˴�С: ", g_pWindowGradient, &g_nGradientValue, 100, onGradient);
	cvCreateTrackbar("�ں˴�С: ", g_pWindowTophat, &g_nTophatValue, 100, onTophat);
	cvCreateTrackbar("�ں˴�С: ", g_pWindowBlackhat, &g_nBlackhatValue, 100, onBlackhat);
	cvCreateTrackbar("�ں˴�С: ", g_pWindowErodeMorph, &g_nErodeMorphValue, 100, onErodeMorph);
	cvCreateTrackbar("�ں˴�С: ", g_pWindowDilateMorph, &g_nDilateMorphValue, 100, onDilateMorph);

	onOpen(0);
	onClose(0);
	onGradient(0);
	onTophat(0);
	onBlackhat(0);
	onErodeMorph(0);
	onDilateMorph(0);

#endif

/* ��ˮ��� -------------------------------------------------------------------*/
#if 1
	imageFloodFill(strFile);
#endif
	waitKey();
}

/* ��ˮ��� -------------------------------------------------------------------*/
Mat imageSrc, imageDst, imageMask, imageGray;

void imageFloodFill(const string &strFile)
{
	
}

