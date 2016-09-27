#include "DotDetection.h"

const bool isDebug = false;

//use opencv's simple blob detector to detect dots in calibration pattern
std::vector<cv::Point2f> DotDetection::detectDots(const cv::Mat& greyImage)
{
	std::vector<cv::Point2f> centers;

    cv::Mat blured;
    cv::GaussianBlur(greyImage, blured, cv::Size(3,3), 2.0, 2.0);


	//filter rules
    cv::SimpleBlobDetector::Params params;
	params.minThreshold = 50;
    params.maxThreshold = 800;

    params.filterByArea = true;
	params.minArea = 50;

    params.filterByCircularity = true;
    params.minCircularity = 0.4f;

    //elipse and cirles are convex, almost 1
    params.filterByConvexity = true;
	params.minConvexity = 0.75f;

    cv::SimpleBlobDetector detector(params);
    std::vector<cv::KeyPoint> keyPoints;
    detector.detect( greyImage, keyPoints);
    cv::KeyPoint::convert(keyPoints, centers);

    if(isDebug)
    {
		cv::Mat imKeypoints;
		cv::drawKeypoints(blured, keyPoints, imKeypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        cv::imshow("keypoints", imKeypoints);
    }

	return centers;
}
