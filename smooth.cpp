
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat image1;

	image1 = imread("C:/Lena_color.png", IMREAD_GRAYSCALE);

	if (image1.empty()) {
		cout << "Could not open or find the img" << std::endl;
		return -1;
	}

	//resize(image1, image1, Size(256, 256), 0, 0, INTER_LINEAR);

	Mat new_image = Mat::zeros(image1.size(), image1.type());

	int masksize = 3; //Default
	int sum = 0;

	cout << "Enter the marsksize (Only odd)" << std::endl;
	cin >> masksize;

	for (int y = 0; y < image1.rows; y++) {
		for (int x = 0; x < image1.cols; x++) {
			sum = 0;
			int new_y, new_x;
			for (int i = -1 * masksize / 2; i <= masksize / 2; i++) {
				for (int j = -1 * masksize / 2; j <= masksize / 2; j++) {
					new_y = y + i;
					new_x = x + j;
					//Fill all border values with (0,0)
					if (new_y < 0) new_y = 0;
					else if (new_y > image1.rows - 1) new_y = 0;
					if (new_x < 0) new_x = 0;
					else if (new_x > image1.cols - 1) new_x = 0;

					sum += image1.at<uchar>(new_y, new_x) * 1;
				}
			}

			sum = sum/(masksize*masksize);
			if (sum > 255) sum = 255;
			if (sum < 0) sum = 0;

			new_image.at<uchar>(y, x) = sum;
		}
	}

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", new_image);
	waitKey(0);

	return 0;
}