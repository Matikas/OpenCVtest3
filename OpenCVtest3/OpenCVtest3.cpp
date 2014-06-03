// OpenCVtest3.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main( int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Not enough parameters" << endl;
		return -1;
	}

	Mat img =  imread(argv[1], CV_LOAD_IMAGE_COLOR);

	//show the original image
	namedWindow("Raw");				//sukuriamas langas, kuriame atsivaizduos foto
	imshow("Raw", img);									//parodoma nuotrauka
	Size imgSize(850, 600);
	//converting the original image into grayscale
	Mat imgGrayScale = cvCreateImage(imgSize, 8, 1); 

	cvtColor(img,imgGrayScale,CV_BGR2GRAY);

	//thresholding the grayscale image to get better results
	threshold(imgGrayScale,imgGrayScale,240,255,CV_THRESH_BINARY_INV);  

	CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours

	//finding all contours in the image
	vector<vector<cv::Point>> contours;
	findContours(imgGrayScale, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1, cvPoint(0,0));

	vector<vector<cv::Point>> result( contours.size() );
	//iterating through each contour
	Mat drawing = Mat::zeros( img.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
		//obtain a sequence of points of contour, pointed by the variable 'contour'
		approxPolyDP(Mat(contours[i]), result[i], 10, false);
		int cap = contours[i].capacity();
		//if there are 4 vertices in the contour(It should be a quadrilateral)

		cout<< "vector size: " << result[i].size() << endl;
		if(contours[i].capacity() == 4)// && result[i].size() == 4
		{
			Scalar color = Scalar( 10, 10, 255 );
			drawContours( img, result, i, color, 5, 8, vector<Vec4i>(), 0, Point() );
		}
	}

	//show the image in which identified shapes are marked   
	namedWindow("Tracked");	
	imshow("Tracked", img);

	namedWindow("Binary");
	imshow("Binary",imgGrayScale);

	cvWaitKey(0); //wait for a key press

	//cleaning up
	cvDestroyAllWindows(); 
	cvReleaseMemStorage(&storage);

	return 0;
}
