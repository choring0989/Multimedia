
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include <cmath>

using namespace cv;
using namespace std;

void createFilter(int kernel_n, double sigma);
double **gKernel;

int main(int argc, char** argv)
{
	Mat image1;

	image1 = imread("C:/Lena_color.png", IMREAD_GRAYSCALE);

	if (image1.empty()) {
		cout << "Could not open or find the img" << std::endl;
		return -1;
	}

	Mat new_image = Mat::zeros(image1.size(), image1.type());

	int masksize = 3; //Default
	double sum = 0;
	double sigma = 1.0;

	cout << "Enter the Kernel-marsksize (Only odd)" << std::endl;
	cin >> masksize;
	cout << "Enter the sigma" << std::endl;
	cin >> sigma;

	createFilter(masksize, sigma);

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

					sum += image1.at<uchar>(new_y, new_x) * gKernel[masksize / 2 + i][masksize / 2 + j];
				}
			}
			new_image.at<uchar>(y, x) = sum;
		}
	}

	namedWindow("origin window", WINDOW_AUTOSIZE);
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("origin window", image1);
	imshow("Display window", new_image);
	waitKey(0);

	for (int i = 0; i < masksize; i++)
		delete[] gKernel[i];
	delete[] gKernel;

	return 0;
}

/*I customized it by referring to the site code below.
*https://www.programming-techniques.com/2013/02/gaussian-filter-generation-using-cc.html
*/
void createFilter(int kernel_n, double sigma)
{
	int point = kernel_n / 2;
	double r, s = point * sigma * sigma;

	// sum is for normalization
	double sum = 0.0;

	gKernel = new double*[kernel_n];
	for (int i = 0; i < kernel_n; i++)
		gKernel[i] = new double[kernel_n];

	// generate n*n kernel
	for (int x = -point; x <= point; x++)
	{
		for (int y = -point; y <= point; y++)
		{
			r = sqrt(x * x + y * y);
			gKernel[x + point][y + point] = (exp(-(r*r)/s)) / (3.14159*s);
			sum += gKernel[x + point][y + point];
		}
	}

	// normalize the Kernel
	for (int i = 0; i < kernel_n; ++i)
		for (int j = 0; j < kernel_n; ++j)
			gKernel[i][j] /= sum;
}