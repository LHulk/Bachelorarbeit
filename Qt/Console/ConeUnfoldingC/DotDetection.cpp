#include "DotDetection.h"

const bool isDebug = true;

DotDetection::DotDetection()
{

}

void DotDetection::detectDots(const cv::Mat& greyImage, std::vector<cv::Point2f>& centers, std::vector<cv::Rect>& boundingRects)
{
    cv::Mat blured;
    cv::GaussianBlur(greyImage, blured, cv::Size(3,3), 2.0, 2.0);

    // Setup SimpleBlobDetector parameters.
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

    boundingRects = getBoundingrects(blured, centers);

    if(isDebug)
    {
        cv::Mat imKeypoints;
        cv::drawKeypoints( blured, keyPoints, imKeypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        cv::imshow("keypoints", imKeypoints);
    }

}


std::vector<cv::Rect> DotDetection::getBoundingrects(const cv::Mat& bluredGreyImg, const std::vector<cv::Point2f>& points)
{
    std::vector<cv::Rect> boundingRects;
    boundingRects.reserve(points.size());

    cv::Mat img = bluredGreyImg.clone();

    for(const auto& keyPt : points)
        cv::floodFill(img, keyPt, cv::Scalar(255), nullptr, cv::Scalar(10), cv::Scalar(10), 8);

    cv::threshold(img, img, 254, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::Scalar color = cv::Scalar(255);
        cv::Rect bounding = cv::minAreaRect(cv::Mat(contours[i])).boundingRect();
        cv::rectangle(img, bounding, color, 2);

        boundingRects.push_back(bounding);
    }

    if(isDebug)
        cv::imshow("bounding rects keypoint", img);

    return boundingRects;
}

