#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

int main(int, char**){
	Mat img0, img1, target;
	img0 = imread("D:\scene1.row3.col2.png");
	img1 = imread("D:\scene1.row3.col3.png");
	cvtColor(img0, img0, COLOR_BGR2GRAY);
	cvtColor(img1, img1, COLOR_BGR2GRAY);

	target = Mat::zeros(img0.rows, img0.cols, img0.type());

	int wSize = 4; //window size
	float sum, min;
	int d;

	for (int i = 0; i < img0.rows - wSize; i++) {
		for (int j = 0; j < img0.cols - wSize; j++) {
			sum = 0;
			min = -1;
			for (int p = 0; p < img0.cols - wSize - j; p++) {
				if (p > wSize*wSize) break;
				for (int s = 0; s < wSize; s++) {
					for (int t = 0; t < wSize; t++) {
						sum = sum + abs(img1.at<uchar>(i + s, j + t) - img0.at<uchar>(i + s, j + t + p));
					}
				}
				sum = sum / (wSize*wSize);
				if (min > sum || min == -1) {
					min = sum;
					d = p;
				}
			}
			target.at<uchar>(i, j) = saturate_cast<uchar>(d * 10);
		}
	}

	namedWindow("Left image", WINDOW_AUTOSIZE);
	imshow("Left image", img0);
	namedWindow("Right image", WINDOW_AUTOSIZE);
	imshow("Right image", img1);
	namedWindow("Result image", WINDOW_AUTOSIZE);
	imshow("Result image", target);

	waitKey(0);
	return 0;
}
