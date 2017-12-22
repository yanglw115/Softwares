#include "faceTamura.h"

#ifdef With_Debug
#include "opencv2/highgui.hpp"
#endif // With_Debug

#define		PI		(3.141592653589793)

using namespace cv;
using namespace std;

double getFaceCoarseness(const Mat& matSrc, Rect rect)
{
	Mat matFace(matSrc, rect);
	cvtColor(matFace, matFace, COLOR_BGR2GRAY);
	double fCoarseness = tamuraCalCoarseness(matFace);
	double fContrast = tamuraCalContrast(matFace);
	cout << "Data rect: " << rect << endl;

#ifdef With_Debug
	Mat matDebug;
	matSrc.copyTo(matDebug);
	putText(matDebug, format("%f(%f:%f)", fCoarseness + fContrast, fCoarseness, fContrast), Point(20, 50), FONT_HERSHEY_SIMPLEX, 1.8, Scalar(0, 0, 255), 3);
	static int i = 1;
	imwrite(format("%d.jpg", i++), matDebug);
	namedWindow("Æ¤·ô´Ö²Ú¶È£º", WINDOW_NORMAL);
	imshow("Æ¤·ô´Ö²Ú¶È£º", matDebug);
	waitKey();
#endif // With_Debug

	return fCoarseness + fContrast;
}

//=====================================================
/// calculate coarseness for a greylevel image
//=====================================================
double tamuraCalCoarseness(const Mat& mat, int kVal, int histBins)
{
    if(NULL == mat.data) {
		LOG(ERROR) << "Invalid input mat when tamuraCalCoarseness.";
        return 0;
    }
    if(kVal < histBins) {
		LOG(ERROR) << "Invalid input kVal or histBins tamuraCalCoarseness.";
        return 0;
    }
	double* coarHist = new double[histBins];
    // mats for average, horizonal and vertical engergy differece  with size k*k
    Mat	*AverK	=	new Mat[kVal];
    Mat	*EnKh	=	new Mat[kVal];
    Mat *EnKv	=	new Mat[kVal];

    for(int i = 0; i < kVal; ++i) {
        AverK[i]	=	Mat::zeros(mat.size(), CV_64FC1);
        EnKh[i]		=	Mat::zeros(mat.size(), CV_64FC1);
        EnKv[i]		=	Mat::zeros(mat.size(), CV_64FC1);
    }

    int	rows	=	mat.rows;
    int	cols	=	mat.cols;
    int	lenOfk	=	1;

    //step 1 : sum over different window size
    for (int i = 0; i < kVal; ++i) {
        lenOfk	*=	2;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols ; ++c) {
                double* desVal = (double*)(AverK[i].data + r * AverK[i].step.p[0] + c * AverK[i].step.p[1]);
                *desVal = localMean(mat, r, c, lenOfk);
            }
        }
    }

    // step 2:
    lenOfk = 1 ;
    for (int i = 0; i < kVal; ++i) {
        int	k2 = lenOfk;					// -__-||  k2 = 1
        lenOfk *= 2;

        for (int x = 0; x < rows; ++x) {
            for (int y = 0; y < cols; ++y) {
                int	posx1	=	x + k2;
                int posx2	=	x - k2;
                int posy1	=	y + k2;
                int posy2	=	y - k2;
                double* hPtr	=	(double*)(EnKh[i].data + x * EnKh[i].step.p[0] + y * EnKh[i].step.p[1]);
                double* vPtr	=	(double*)(EnKv[i].data + x * EnKv[i].step.p[0] + y * EnKv[i].step.p[1]);

                if (cols - 1 >= posy1 && 0 <= posy2) {
                    *hPtr	=	fabs(*(double*)(AverK[i].data + x * AverK[i].step.p[0] + posy1 * AverK[i].step.p[1])
                                     - * (double*)(AverK[i].data + x * AverK[i].step.p[0] + posy2 * AverK[i].step.p[1]));
                } else {
                    *hPtr	=	0;
                }

                if(rows - 1  >= posx1 && 0 <= posx2) {
                    *vPtr	=	fabs(*(double*)(AverK[i].data + posx1 * AverK[i].step.p[0] + y * AverK[i].step.p[1])
                                     - * (double*)(AverK[i].data + posx2 * AverK[i].step.p[0] + y * AverK[i].step.p[1]));
                } else {
                    *vPtr	=	0;
                }
            }
        }
    }

    // step 3:
    Mat bestK	=	Mat::zeros(mat.size(), CV_64FC1);
    Mat Sbest	=	Mat::zeros(mat.size(), CV_64FC1);

    double enerSum	=	0;
    for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
            double	maxE	=	-1;
            double	maxk	=	0;
            double*	des		=	(double*)(Sbest.data + i * Sbest.step.p[0] + j * Sbest.step.p[1]);
            double* desbk	=	(double*)(bestK.data + i * bestK.step.p[0] + j * bestK.step.p[1]);

            for (int k = 0; k < kVal; ++k) {
                double*	hPtr	=	(double*)(EnKh[k].data + i * EnKh[k].step.p[0] + j * EnKh[k].step.p[1]);
                double* vPtr	=	(double*)(EnKv[k].data + i * EnKv[k].step.p[0] + j * EnKv[k].step.p[1]);

                if (*hPtr > maxE) {
                    maxE	=	*hPtr;
                    maxk	=	k;
                }
                if (*vPtr > maxE) {
                    maxE	=	*vPtr;
                    maxk	=	k;
                }
            }

            double temp	=	1;

            for (int n = 0; n < maxk; ++n) {
                temp	*=	2;
            }
            enerSum	+=	temp;
            *des	=	temp;
            *desbk	=	maxk;
        }
    }
//	disMat( bestK , "k.txt" ) ;

    double	coarseness	=	enerSum / (rows * cols);

    // out put the histogram
    vector<double> histogram(histBins , 0) ;
    double interval	=	(double)kVal / histBins ;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double* dataPtr	=	(double*)(bestK.data + i * bestK.step.p[0] + j * bestK.step.p[1]);
            int		index	=	cvFloor((*dataPtr) / interval);

            histogram[index] +=	1 ;
        }
    }

    int		histMaxBin	=	0;
    double histMaxVal	=	-1;

    for (int i = 0; i < histBins; ++i) {
        if (histogram[i] > histMaxVal) {
            histMaxVal	=	histogram[i];
            histMaxBin	=	i;
        }
    }

    for (int i = 0; i < histBins; ++i) {
        histogram[i]	/=	histMaxVal;
        coarHist[i]		=	histogram[i];
    }

    if(NULL != AverK) {
        delete []AverK;
        AverK	=	NULL;
    }
    if (NULL != EnKh) {
        delete []EnKh ;
        EnKh	=	NULL;
    }
    if (NULL != EnKv) {
        delete []EnKv ;
        EnKv	=	NULL;
    }

	//LOG(INFO) << "Calculate coarseness result: " << to_string(coarseness);
    return coarseness;
}


//=======================================================
/// calculate contrast for a grey level image
//=======================================================
double tamuraCalContrast(const Mat& mat)
{
    if (NULL == mat.data) {
		LOG(ERROR) << "Invalid input mat when tamuraCalContrast.";
        return 0;
    }
    if (mat.channels() != 1) {
		LOG(ERROR) << "Invalid input mat when tamuraCalContrast, it must be a grey level image..";
        return 0;
    }

    vector<double> greyLevelNum(256 , 0);
    vector<double>::iterator iter;
    int rows	=	mat.rows;
    int cols	=	mat.cols;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int	greyVal = (int) * (mat.data + i * mat.step.p[0] + j * mat.step.p[1]);
            greyLevelNum[greyVal]	+=	1;
        }
    }

    double	averVal		=	0;
    int		greyIndex	=	0;
    for (iter = greyLevelNum.begin(); iter != greyLevelNum.end(); ++iter) {
        *iter	/=	rows * cols;
        averVal	+=	(*iter) * greyIndex;
        greyIndex++;
    }

    double	u4			=	0;
    double	stdDev		=	0;

    greyIndex	=	0;
    for (iter = greyLevelNum.begin(); iter != greyLevelNum.end(); ++iter) {
        double	temp	=	1;

        for (int i = 0; i < 4; ++i) {
            temp	*=	(greyIndex - averVal);
        }
        temp	*=	(*iter);

        u4		+=	temp;
        stdDev	+=	(greyIndex - averVal) * (greyIndex - averVal) * (*iter);
        greyIndex++;
    }

    double alpha	=	u4 / (stdDev * stdDev);

    return	sqrt(stdDev) / (sqrt(sqrt(alpha)));
}

//=======================================================
///	 calculate K * K local mean regarding ( x ,y ) as the centre
///  K = 2^ n
//========================================================
double localMean(const Mat& mat, int x, int y, int K)
{
    if (NULL == mat.data) {
        return 0;
    }

    int	rows	=	mat.rows;
    int cols	=	mat.cols;
    int k		=	K / 2;

    if (rows < x || cols < y) {
        return -1;
    }
    if (k < 1) {
        return	(double) * (mat.data + x * mat.step.p[0] + y * mat.step.p[1]);
    }

    int	xStart	=	max(0 , x - k);
    int xEnd	=	min(rows - 1  , x + k - 1);
    int yStart	=	max(0 , y - k);
    int yEnd	=	min(cols - 1 , y + k - 1);

    if (2 == K && (0 == x || 0 == y)) {
        return	(double) * (mat.data + x * mat.step.p[0] + y * mat.step.p[1]);
    }

    Mat roiMat(mat, Range(xStart , xEnd + 1), Range(yStart, yEnd + 1));
    int roiRows	=	roiMat.rows;
    int roiCols	=	roiMat.cols;
    double sum	=	0;

    for (int i = 0; i < roiRows; ++i) {
        for (int j = 0; j < roiCols; ++j) {
            sum	+=	(double) * (roiMat.data + i * roiMat.step.p[0] + j * roiMat.step.p[1]);
        }
    }

    return sum / (roiRows * roiCols);                // MY god , dead here!!!!  sum / roiRows * roiCols ;
}