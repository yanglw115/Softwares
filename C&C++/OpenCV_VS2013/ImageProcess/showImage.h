
#ifndef __SHOW_IMAGE_H__
#define __SHOW_IMAGE_H__

#include <iostream>
using namespace std;

void showImage(const string &strPath);
void ircodeImage(const string &strPath);
void blurImage(const string &strPath);
void cannyImage(const string &strPath);
void imageAddWeighted(const string &strSrc1, const string &strSrc2);
void imageROI(const string &strSrc1, const string &strSrc2);
void imageSplit(const string &strSrc1, const string &strSrc2);
void setContrastAndBright(const string &strImage);

#endif //__SHOW_IMAGE_H__