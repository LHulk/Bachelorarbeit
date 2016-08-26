#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <vector>

#include "Config.h"

class Calibration
{
public:
    static std::vector<cv::Point2f> getCorners(const cv::Mat& greyImg, cv::Mat &imgWithCorners);
    static std::vector<cv::Point3f> getObjectPoints();
    static void getIntrinsicParamters(const std::vector<std::vector<cv::Point2f>>& imagePoints, const std::vector<std::vector<cv::Point3f>>& objectPoints,
                                      cv::Mat& cameraMatrix, cv::Mat& distCoeffs, cv::Size calibImgSize);
};

#endif // CALIBRATION_H
