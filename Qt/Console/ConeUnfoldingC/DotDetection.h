#ifndef DOTDETECTION_H
#define DOTDETECTION_H

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class DotDetection
{
public:
    DotDetection();

    static void detectDots(const cv::Mat& greyImage, std::vector<cv::Point2f>& centers, std::vector<cv::Rect>& boundingRects);

private:
    static std::vector<cv::Rect> getBoundingrects(const cv::Mat& bluredGreyImg, const std::vector<cv::Point2f>& points);
};

#endif // DOTDETECTION_H
