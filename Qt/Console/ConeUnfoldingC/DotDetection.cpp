#include "DotDetection.h"

const bool isDebug = true;

std::vector<cv::Point2f> DotDetection::detectDots(const cv::Mat& greyImage)
{
	std::vector<cv::Point2f> centers;

    cv::Mat blured;
    cv::GaussianBlur(greyImage, blured, cv::Size(3,3), 2.0, 2.0);


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

    // 1 circle, 0 ellipse
    params.filterByInertia = false;
    params.minInertiaRatio = 0.1f;

    cv::SimpleBlobDetector detector(params);

    //TODO:
    //what to do if not all keypoints found? readjust params or ignore?
    std::vector<cv::KeyPoint> keyPoints;
    detector.detect( greyImage, keyPoints);

    cv::KeyPoint::convert(keyPoints, centers);



	cv::Mat imKeypoints;
	cv::drawKeypoints(blured, keyPoints, imKeypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    if(isDebug)
    {
        cv::imshow("keypoints", imKeypoints);
    }

	return centers;

}
