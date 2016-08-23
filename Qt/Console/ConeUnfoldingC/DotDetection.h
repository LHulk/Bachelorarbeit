#ifndef DOTDETECTION_H
#define DOTDETECTION_H

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class DotDetection
{
public:
    static void detectDots(const cv::Mat& greyImage, std::vector<cv::Point2f>& centers, std::vector<cv::Rect>& boundingRects);
};

#endif // DOTDETECTION_H
