#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/trace.hpp>
using namespace cv;
using namespace cv::dnn;
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;
/* Find best class for the blob (i. e. class with maximal probability) */
static void getMaxClass(const Mat &probBlob, int *classId, double *classProb)
{
	Mat probMat = probBlob.reshape(1, 1); //reshape the blob to 1x1000 matrix
	Point classNumber;
	minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
	*classId = classNumber.x;
}
static std::vector<String> readClassNames(const char *filename = "synset_words.txt")
{
	std::vector<String> classNames;
	std::ifstream fp(filename);
	if (!fp.is_open())
	{
		std::cerr << "File with classes labels not found: " << filename << std::endl;
		exit(-1);
	}
	std::string name;
	while (!fp.eof())
	{
		std::getline(fp, name);
		if (name.length())
			classNames.push_back(name.substr(name.find(' ') + 1));
	}
	fp.close();
	return classNames;
}
int main(int argc, char **argv)
{
	CV_TRACE_FUNCTION();
	/* 下面的模型可以使用,误差个人觉得在接受的范围内 */
	/* 详细参考：https://data.vision.ee.ethz.ch/cvl/rrothe/imdb-wiki/ 这里的CNN使用的是VGG-16 架构*/
	//String modelTxt = "build/data/caffe/IMDB_age_gender_models/age.prototxt";
	//String modelBin = "build/data/caffe/IMDB_age_gender_models/dex_imdb_wiki.caffemodel";

	//String modelTxt = "build/data/caffe/IMDB_age_gender_models/age_iccv2015.prototxt";
	//String modelBin = "build/data/caffe/IMDB_age_gender_models/dex_chalearn_age_iccv2015.caffemodel";

	//String modelTxt = "build/data/caffe/IMDB_age_gender_models/gender.prototxt";
	//String modelBin = "build/data/caffe/IMDB_age_gender_models/gender.caffemodel";

	/* 下面的模型image参数需要设置为 Size(227, 227) 该模型用的数据可能比较小,精度远没有上面的高 */
	// 参考:https://gist.github.com/GilLevi/c9e99062283c719c03de 和 http://www.openu.ac.il/home/hassner/projects/cnn_agegender/
	// https://github.com/GilLevi/AgeGenderDeepLearning/blob/master/AgeGenderDemo.ipynb
	/*	age_list=['(0, 2)','(4, 6)','(8, 12)','(15, 20)','(25, 32)','(38, 43)','(48, 53)','(60, 100)']
		gender_list=['Male','Female'] */
	//String modelTxt = "build/data/caffe/cnn_age_gender_models_and_data/deploy_age.prototxt";
	//String modelBin = "build/data/caffe/cnn_age_gender_models_and_data/age_net.caffemodel";

	String modelTxt = "build/data/caffe/cnn_age_gender_models_and_data/deploy_gender.prototxt";
	String modelBin = "build/data/caffe/cnn_age_gender_models_and_data/gender_net.caffemodel";
	String imageFile = (argc > 1) ? argv[1] : "images/baby.jpg";
	Net net;
	try {
		net = dnn::readNetFromCaffe(modelTxt, modelBin);
	}
	catch (cv::Exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		if (net.empty())
		{
			std::cerr << "Can't load network by using the following files: " << std::endl;
			std::cerr << "prototxt:   " << modelTxt << std::endl;
			std::cerr << "caffemodel: " << modelBin << std::endl;
			std::cerr << "bvlc_googlenet.caffemodel can be downloaded here:" << std::endl;
			std::cerr << "http://dl.caffe.berkeleyvision.org/bvlc_googlenet.caffemodel" << std::endl;
			exit(-1);
		}
	}
	Mat img = imread(imageFile);
	if (img.empty())
	{
		std::cerr << "Can't read image from the file: " << imageFile << std::endl;
		exit(-1);
	}
	//GoogLeNet accepts only 224x224 BGR-images,这里也适用VGG-16 
	//Mat inputBlob = blobFromImage(img, 1.0f, Size(224, 224), Scalar(104, 117, 123), false);   //Convert Mat to batch of images
	Mat inputBlob = blobFromImage(img, 1.0f, Size(227, 227), Scalar(104, 117, 123), false, false);   //Convert Mat to batch of images

	Mat prob;
	cv::TickMeter t;
	for (int i = 0; i < 10; i++)
	{
		CV_TRACE_REGION("forward");
		net.setInput(inputBlob, "data");        //set the network input
		//net.setInput(img, "data");        //set the network input
		t.start();
		prob = net.forward("prob");                          //compute output
		t.stop();
	}
	int classId;
	double classProb;
	getMaxClass(prob, &classId, &classProb);//find the best class
	std::vector<String> classNames = readClassNames("build/data/caffe/cnn_age_gender_models_and_data/synset_words_gender.txt");
	std::cout << "Best class: #" << classId << " '" << classNames.at(classId) << "'" << std::endl;
	std::cout << "Probability: " << classProb * 100 << "%" << std::endl;
	std::cout << "Time: " << (double)t.getTimeMilli() / t.getCounter() << " ms (average from " << t.getCounter() << " iterations)" << std::endl;
	waitKey(0);
	return 0;
} //main