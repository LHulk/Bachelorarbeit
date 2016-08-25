#ifndef DOTDETECTION_H
#define DOTDETECTION_H

//CV
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class DotDetection
{
public:
	static std::vector<cv::Point2f> detectDots(const cv::Mat& greyImage);
};

#endif // DOTDETECTION_H
