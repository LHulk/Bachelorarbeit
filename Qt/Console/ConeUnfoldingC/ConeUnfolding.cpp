#include "ConeUnfolding.h"

static bool isDebug = false;


ConeUnfolding::ConeUnfolding()
{
    //Mat grey = cv::imread("../../../img/v5_pattern/pattern2.png", CV_LOAD_IMAGE_GRAYSCALE);
    //cv::Mat grey = cv::imread("../../../img/v5_pattern/pattern3.png", CV_LOAD_IMAGE_GRAYSCALE);
	//cv::Mat grey = cv::imread("../../../img/v5_pattern/test.png", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat grey = cv::imread("../../../img/v5_pattern/blender_perfectCenter.png", CV_LOAD_IMAGE_GRAYSCALE);

    cv::resize(grey, grey, cv::Size(1000, 1000)); //performance
	Config::usedResHeight = 1000;
	Config::usedResHeight = 1000;

	Cone cone = Cone();

	cv::imshow("grey", grey);


    //canny
    cv::Mat canny, orientation;
    EdgeDetection::canny(grey, canny, orientation, Config::cannyLow, Config::cannyHigh, Config::cannyKernel, Config::cannySigma);


    std::vector<cv::Point2f> keyPoints;
	keyPoints = DotDetection::detectDots(grey);

	//TODO: HERE CHECK IF POINTS HAVE BEEN DETECTED

    std::vector<Ellipse> ellipses = Ellipse::detectEllipses(canny);

	std::vector<std::vector<cv::Point2f>> pointsPerEllipse = Ellipse::getEllipsePointMappings(ellipses, keyPoints);
    Misc::sort(pointsPerEllipse, ellipses);

	ellipses = Ellipse::reestimateEllipses(pointsPerEllipse, ellipses);
	std::vector<Line> lines = Line::fitLines(pointsPerEllipse);


    /*cv::Scalar colors[] = { cv::Scalar(0,0,255),  cv::Scalar(0,140,255), cv::Scalar(0,255,0), cv::Scalar(140,255,0), cv::Scalar(255,255,0)};
    cv::cvtColor(canny, canny, CV_GRAY2BGR);
    int i = 0;
    for(const Ellipse& e : ellipses)
    {
        cv::ellipse(canny, e.getEllipseAsRotatedRect(), colors[i++], 2);
    }

    i = 0;
    for(const std::vector<cv::Point2f>& ptList : pointsPerEllipse)
    {
        for(const cv::Point2f& pt : ptList)
            cv::circle(canny, pt, 10, colors[i], -1);
        i++;
    }
    cv::imshow("ellipses", canny);*/

	//std::vector<std::vector<cv::Point3f>> worldCoords = Transformation::getWorldCoordinatesForSamples();

	std::vector<std::vector<cv::Point3f>> worldCoords = cone.calculateWorldCoordinatesForSamples();

	cone.setEllipses(ellipses);
	cone.setLines(lines);
	cone.setSampleCoordsImage(pointsPerEllipse);
	cone.setSampleCoordsWorld(worldCoords);


	cv::Mat proj = Transformation::getProjectiveMatrix(cone);

	//cv::Mat vis;
	//cv::cvtColor(grey, vis, CV_GRAY2BGR);
	//for(size_t i = 0; i < worldCoords.size(); i++)
	//{
	//	for(size_t j = 0; j < worldCoords[i].size(); j++)
	//	{
	//		cv::Point3f currentWorld = worldCoords[i][j];
	//		cv::Point2f currImg = pointsPerEllipse[i][j];

	//		cv::Mat currentWorldMat = cv::Mat::zeros(4, 1, CV_32F);
	//		currentWorldMat.at<float>(0, 0) = currentWorld.x; currentWorldMat.at<float>(1, 0) = currentWorld.y; currentWorldMat.at<float>(2, 0) = currentWorld.z; currentWorldMat.at<float>(3, 0) = 1;
	//		cv::Mat homImg = proj*currentWorldMat;

	//		float w = homImg.at<float>(2, 0);
	//		cv::Point2f reprojected = cv::Point2f(homImg.at<float>(0, 0) / w, homImg.at<float>(1, 0) / w);

	//		cv::circle(vis, currImg, 4, cv::Scalar(0, 255, 0), -1);
	//		cv::circle(vis, reprojected, 4, cv::Scalar(0, 0, 255), -1);
	//	}
	//}
	//cv::imshow("vis", vis);

	Transformation::reverseWarp(grey, proj, cone);
	return;

	
	Transformation::forwardWarp(grey, cone);


    return;
    
}




