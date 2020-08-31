#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

void nearestInter(Mat &src, Mat &dst, float scaleY, float scaleX);
void bilinearInter(Mat &src, Mat &dst, float scaleY, float scaleX);

int main(){
	float scaleX = 3.0;
	float scaleY = 2.0;

	Mat input, output1, output2, output3;

	//Loadtheimage
	input = imread("C:/Lena_color.png", 1);

	if (input.empty()) {
		cout << "Could not open or find the img" << std::endl;
		return -1;
	}

	output1 = Mat::zeros(input.rows*scaleY, input.cols*scaleX, input.type());
	output2 = Mat::zeros(input.rows*scaleY, input.cols*scaleX, input.type());
	output3 = Mat::zeros(input.rows*scaleY, input.cols*scaleX, input.type());

	namedWindow("src Image", 1);
	imshow("src Image", input);

	resize(input, output3, Size(), scaleX, scaleY, INTER_LINEAR);
	imshow("New Image - linear", output3);

	nearestInter(input, output1, scaleY, scaleX);
	imshow("New Image - neares", output1);

	nearestInter(input, output2, scaleY, scaleX);
	imshow("New Image - bilinear", output2);


	waitKey(0);

	return 0;
}

void nearestInter(Mat &src, Mat &dst, float scaleY, float scaleX) {
	int px, py;
	for (int y = 0; y < dst.rows; y++) {
		for (int x = 0; x < dst.cols; x++) {
			px = x / scaleX;
			py = y / scaleY;
			dst.at<Vec3b>(y, x) = src.at<Vec3b>(py, px);
		}
	}
}

void bilinearInter(Mat &src, Mat &dst, float scaleY, float scaleX) {
	double px, py;
	double fx1, fy1, fx2, fy2;
	for (int y = 0; y < dst.rows; y++) {
		for (int x = 0; x < dst.cols; x++) {
			px = x / scaleX;
			py = y / scaleY;

			//(1-s)A +sB
			fx1 = ((double)x / (double)scaleX) - (double)px;
			fx2 = 1 - fx1;
			fy1 = ((double)y / (double)scaleY) - (double)py;
			fy2 = 1 - fy1;

			if (src.channels() == 1) {
				uchar p1 = src.at<uchar>(py, px);
				uchar p2 = src.at<uchar>(py, px + 1);
				uchar p3 = src.at<uchar>(py + 1, px);
				uchar p4 = src.at<uchar>(py + 1, px + 1);

				dst.at<uchar>(y, x) = fx2 * fy2*p1 + fx1 * fy2*p2 + fx2 * fy1*p3 + fx1 * fy1*p4;
			}

			else if (src.channels() == 3) {
				Vec3b p1 = src.at<Vec3b>(py, px);
				Vec3b p2 = src.at<Vec3b>(py, px + 1);
				Vec3b p3 = src.at<Vec3b>(py + 1, px);
				Vec3b p4 = src.at<Vec3b>(py + 1, px + 1);

				dst.at<Vec3b>(y, x) = fx2 * fy2*p1 + fx1 * fy2*p2 + fx2 * fy1*p3 + fx1 * fy1*p4;
			}
		}
	}
}
