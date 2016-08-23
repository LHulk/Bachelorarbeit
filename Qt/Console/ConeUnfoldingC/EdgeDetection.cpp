#include "EdgeDetection.h"


using cv::Mat;
using cv::Size;
using cv::Scalar;
using cv::Point;
using cv::Vec3f;


void EdgeDetection::canny(const cv::Mat& greyIn, cv::Mat& cannyEdge, cv::Mat& orientation, int minThresh, int maxThresh, int kernel, double sigma)
{
    Mat blured;
	cv::GaussianBlur(greyIn, blured, Size(kernel, kernel), sigma, sigma);

	//canny edges
    cv::Canny(blured, cannyEdge, minThresh, maxThresh, kernel);

	//sobel orientation
	Mat gradX, gradY;
	cv::Sobel(blured, gradX, CV_16S, 1, 0, kernel); //dx = 1, dy = 0
	cv::Sobel(blured, gradY, CV_16S, 0, 1, kernel); //dx = 0, dy = 1

	orientation = Mat::zeros(gradX.rows, gradX.cols, CV_32F); //no CV_16F????
	gradX.convertTo(gradX, CV_32F);
	gradY.convertTo(gradY, CV_32F);
    cv::phase(gradX, gradY, orientation); //values are in radians

    cv::GaussianBlur(orientation, orientation, Size(kernel, kernel), sigma, sigma);

	Mat orientationVis;
	cv::normalize(orientation, orientationVis, 0, 255, cv::NORM_MINMAX, CV_8U);

    /*cv::imshow("grad dir", orientationVis);
    Mat orientationVis2(orientation.rows, orientation.cols, CV_8UC3);
	for(int r = 0; r < orientation.rows; r++)
	{
		for(int c = 0; c < orientation.cols; c++)
		{
            if(cannyEdge.at<uchar>(r,c) > 100)
			{
                double angle = orientation.at<float>(r,c); //-pi to +pi (angle+pi) from 0 to 2pi, abs from 0 to pi
                angle = ((angle+CV_PI)/(2*CV_PI))*179;
                orientationVis2.at<cv::Vec3b>(r,c) = cv::Vec3b(static_cast<int>(angle), 255, 255);
			}
			else
			{
				orientationVis2.at<cv::Vec3b>(r,c) = cv::Vec3b(0, 255, 0);
			}
		}
    }*/
}

void EdgeDetection::sobel(const cv::Mat& greyIn, cv::Mat& magnitude, cv::Mat& orientation, int kernel, double sigma)
{
    if(sigma > 0)
        cv::GaussianBlur(greyIn, greyIn, Size(kernel, kernel), sigma, sigma);

    //sobel orientation
    Mat gradX, gradY;
    cv::Sobel(greyIn, gradX, CV_16S, 1, 0, kernel); //dx = 1, dy = 0
    cv::Sobel(greyIn, gradY, CV_16S, 0, 1, kernel); //dx = 0, dy = 1

    Mat absGradX, absGradY;
    cv::convertScaleAbs(gradX, absGradX);
    cv::convertScaleAbs(gradY, absGradY);
    cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, magnitude);

    orientation = Mat::zeros(gradX.rows, gradX.cols, CV_32F); //no CV_16F????
    gradX.convertTo(gradX, CV_32F);
    gradY.convertTo(gradY, CV_32F);
    cv::phase(gradX, gradY, orientation); //values are in radians

    Mat orientationVis;
    cv::normalize(orientation, orientationVis, 0x00, 0xFF, cv::NORM_MINMAX, CV_8U);


    cv::imshow("orientation", orientationVis);
    cv::imshow("magnitude", magnitude);

}



















