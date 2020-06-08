#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("C:/Girl_in_front_of_a_green_background.jpg");
	Mat back = imread("C:/sky.jpg");
	Mat dst = Mat::zeros(src.size(), src.type());

	Mat chromaKey = Mat::zeros(src.size(), src.type());// 0 y / 1 Cr / 2 Cb
	resize(back, back, src.size(), 0, 0); // resize background image

	uchar r, g, b;
	int delta = 128;//for 8-bit images

	// Processing in YCbCr
	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			r = saturate_cast<uchar>(src.at<Vec3b>(y, x)[2]);
			g = saturate_cast<uchar>(src.at<Vec3b>(y, x)[1]);
			b = saturate_cast<uchar>(src.at<Vec3b>(y, x)[0]);

			chromaKey.at<Vec3b>(y, x)[0] = saturate_cast<uchar>(0.299*r + 0.587*g + 0.114*b);
			chromaKey.at<Vec3b>(y, x)[1] = saturate_cast<uchar>((r - chromaKey.at<Vec3b>(y, x)[0])*0.713 + delta);
			chromaKey.at<Vec3b>(y, x)[2] = saturate_cast<uchar>((b - chromaKey.at<Vec3b>(y, x)[0])*0.564 + delta);
		}
	}

	//Draw a histogram.
	vector<Mat> bgr_planes;
	split(chromaKey, bgr_planes);

	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;

	Mat cr_hist, cb_hist;

	calcHist(&bgr_planes[1], 1, 0, Mat(), cr_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), cb_hist, 1, &histSize, &histRange, uniform, accumulate);

	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	normalize(cr_hist, cr_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(cb_hist, cb_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	float cr_key = 0;
	float cb_key = 0;

	//find keys
	for (int i = 1; i < histSize; i++)
	{
		if (cr_hist.at<float>(cr_key) < cr_hist.at<float>(i)) {
			cr_key = i;
		}
		if (cb_hist.at<float>(cb_key) < cb_hist.at<float>(i)) {
			cb_key = i;
		}
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(cr_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(cr_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(cb_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(cb_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}
	cout << cr_key << "," << cb_key << endl;
	namedWindow("calcHist Demo", WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);

	Mat alpha = Mat::zeros(src.size(), CV_32FC1);
	Mat distance = Mat::zeros(src.size(), CV_32FC1);

	//threshold
	float inner = 28;
	float outer = 54;

	//Mapping according to threshold
	for (int y = 0; y < chromaKey.rows; y++) {
		for (int x = 0; x < chromaKey.cols; x++) {
			distance.at<float>(y, x) = sqrt(pow(chromaKey.at<Vec3b>(y, x)[1] - cr_key, 2) + pow(chromaKey.at<Vec3b>(y, x)[2] - cb_key, 2));
			if (distance.at<float>(y, x) < inner) {
				alpha.at<float>(y, x) = 0;
			}
			else if (distance.at<float>(y, x) > outer) {
				alpha.at<float>(y, x) = 1;
			}
			else {
				alpha.at<float>(y, x) = (distance.at<float>(y, x) - inner) / (outer - inner);
			}
		}
	}
	//inear blending between foreground and background images
	for (int y = 0; y < alpha.rows; y++) {
		for (int x = 0; x < alpha.cols; x++) {
			for (int c = 0; c < 3; c++) {
				dst.at<Vec3b>(y, x)[c] = ((1 - alpha.at<float>(y, x)) * back.at<Vec3b>(y, x)[c]) + (alpha.at<float>(y, x) * src.at<Vec3b>(y, x)[c]);
			}
		}
	}

	namedWindow("New Image", 1);
	imshow("New Image", dst);
	waitKey(0);
	return 0;
}