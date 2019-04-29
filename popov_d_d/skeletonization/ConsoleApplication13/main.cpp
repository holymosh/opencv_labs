#include "pch.h"
#include <Windows.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv/cv.h>
#include "skeletonization.h"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;

int radius = 0;
int radius_max = 10;

int iterations = 6;
int iterations_max = 10;

int block = 11;
int block_max = 100;

int pixels = 8;
int pixels_max = 55;

int filter_height = 10, filter_height_max = 200, filter_width = 10, filter_width_max = 200, sigma1 = 0, sigma1_max = 500
    , sigma2 = 0, sigma2_max = 500;

auto main(int argc, char** argv) -> int
{
	IplImage* inputImage;


	const char* filename = argc == 2
		                       ? argv[1]
		                       : "‪image.jpg";
	inputImage = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* inputImage1;
	inputImage1 = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
	printf("[i] image: %s\n", filename);
	if (!inputImage)
	{
		printf("%s",filename, "image not found\n");
		exit(0);
	}
	CvSize size(inputImage->width, inputImage->height);
	IplImage* improvedImage = cvCreateImage(size, IPL_DEPTH_8U, 3);

	improveImageQuality(inputImage1, improvedImage);
	cvShowImage("improved", improvedImage);
	resizeWindow("improved", 400, 400);
	IplImage* sum = cvCloneImage(improvedImage);
	IplImage* dst = cvCreateImage(CvSize(improvedImage->width, improvedImage->height), IPL_DEPTH_8U, 1);

	IplConvKernel* Kern =
		cvCreateStructuringElementEx(radius * 2 + 1, radius * 2 + 1, radius, radius, CV_SHAPE_ELLIPSE);
	cvDilate(improvedImage, sum, Kern, iterations);
	cvErode(sum, sum, Kern, iterations);
	cvReleaseStructuringElement(&Kern);
	IplImage* smoothImage = cvCloneImage(sum);
	cvSmooth(sum, smoothImage, CV_BILATERAL, filter_height, filter_width, sigma1, sigma2);
	cvShowImage("image", smoothImage);
	resizeWindow("image", 400, 400);
	IplImage* gray = cvCreateImage(CvSize(improvedImage->width, improvedImage->height), IPL_DEPTH_8U, 1);
	cvCvtColor(smoothImage, gray, CV_RGB2GRAY);
	IplImage* bin = cvCreateImage(cvSize(inputImage->width, inputImage->height), IPL_DEPTH_8U, 1);
	cvAdaptiveThreshold(gray, bin, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, block, pixels);
	cvShowImage("skelet", bin);
	resizeWindow("skelet", 400, 400);
	cvCanny(gray, dst, 200, 250, 3);

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;
	IplImage* imageInRange = cvCreateImage(cvSize(inputImage->width, inputImage->height), IPL_DEPTH_8U, 1);
	IplImage* dst1 = cvCloneImage(improvedImage);
	for (CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next)
	{
		cvDrawContours(dst1, seq0, CV_RGB(0, 0, 250), CV_RGB(255, 216, 0), 0, 1, 8);
	}
	cvWaitKey();
	cvReleaseImage(&imageInRange);
	cvReleaseImage(&dst1);

	cvReleaseImage(&gray);
	cvReleaseImage(&bin);
	cvReleaseImage(&smoothImage);
	printf("All done\n");
	return 0;
}
