#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace EdgeDetection
{
    void canny(const cv::Mat& greyIn, cv::Mat &cannyEdge, cv::Mat& orientation, int minThresh, int maxThresh, int kernel, double sigma = 2.0);
    void sobel(const cv::Mat& greyIn, cv::Mat& magnitude, cv::Mat& orientation, int kernel, double sigma = 0.0);
}

#endif // EDGEDETECTION_H
