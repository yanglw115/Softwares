
#include "dftImage.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void dftImage(const string &strFile)
{
	/* 以灰度模式读取原始图像 */
	Mat srcImage = imread(strFile.c_str(), 0);
	if (!srcImage.data) {
		cout << "Read image file failed, please check file: " << strFile.c_str();
		return;
	}
	imshow("原始图像", srcImage);
	/* 获取进行傅里叶变换最佳尺寸，以得到最快的傅里叶转换速度 */
	int rows = getOptimalDFTSize(srcImage.rows);
	int cols = getOptimalDFTSize(srcImage.cols);

	/* 将添加的像素初始化为0 */
	Mat padded;
	copyMakeBorder(srcImage, padded, 0, rows - srcImage.rows, 0, cols - srcImage.cols, BORDER_CONSTANT, Scalar::all(0));

	/*  为傅里叶变换的结果(实部和虚部)分配存储空间 */
	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	Mat complexI;
	merge(planes, 2, complexI);

	/* 进行离散傅里叶变换 */
	dft(complexI, complexI);

	/* 将复数转换为幅值 */
	split(complexI, planes);
	//planes[0] = Re(DFT(I), );
	magnitude(planes[0], planes[1], planes[0]);
	Mat magnitudeImage = planes[0];

	/* 进行对数尺度缩放 */
	magnitudeImage += Scalar::all(1);
	log(magnitudeImage, magnitudeImage); //求自然对数

	/* 剪切和重分布幅度图象限 */
	magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));

	/* 重新排列傅里叶图像中的象限，使得原点位于图像中心 */
	int cx = magnitudeImage.cols / 2;
	int cy = magnitudeImage.rows / 2;
	Mat q0(magnitudeImage, Rect(0, 0, cx, cy));
	Mat q1(magnitudeImage, Rect(cx, 0, cx, cy));
	Mat q2(magnitudeImage, Rect(0, cy, cx, cy));
	Mat q3(magnitudeImage, Rect(cx, cy, cx, cy));

	/* 交换象限，左上与右下 */
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	/* 交换象限，右上与左下 */
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	/* 归一化处理 */
	normalize(magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX);
	imshow("频谱增值", magnitudeImage);
	
	waitKey();
}