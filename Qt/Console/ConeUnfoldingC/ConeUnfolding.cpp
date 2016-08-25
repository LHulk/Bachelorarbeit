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

	/*grey = cv::Mat::zeros(grey.size(), CV_8UC3);

	Ellipse e = Ellipse(400, 600, 400, 200, CV_PI / 5);

	cv::ellipse(grey, e.getEllipseAsRotatedRect(), cv::Scalar(255, 0, 0));
	cv::circle(grey, e.getCenter(), 8, cv::Scalar(255, 0, 0), -1);

	cv::Point pt(200, 200);
	cv::Point intersect;

	Ellipse::realDistTrans(e, pt, intersect);

	cv::circle(grey, pt, 8, cv::Scalar(255, 255, 0), -1);
	cv::circle(grey, intersect, 8, cv::Scalar(0, 255, 255), -1);

    
	return;*/

	cv::imshow("grey", grey);

    //std::cout << std::fmod((std::fmod(-7.0, CV_PI) + CV_PI), CV_PI) << std::endl;
    //return;

    //canny
    cv::Mat canny, orientation;
    EdgeDetection::canny(grey, canny, orientation, 100, 254, 3, 0.5f);

    cv::imshow("canny", canny);


    std::vector<cv::Point2f> keyPoints;
	keyPoints = DotDetection::detectDots(grey);
    std::vector<Ellipse> ellipses = Ellipse::detectEllipses(canny);

	std::vector<std::vector<cv::Point2f>> pointsPerEllipse = Ellipse::getEllipsePointMappings(ellipses, keyPoints);
    sort(pointsPerEllipse, ellipses);
	Ellipse::reestimateEllipses(pointsPerEllipse, ellipses);

	std::vector<Line> lines = Line::fitLines(pointsPerEllipse);


    cv::Scalar colors[] = { cv::Scalar(0,0,255),  cv::Scalar(0,140,255), cv::Scalar(0,255,0), cv::Scalar(140,255,0), cv::Scalar(255,255,0)};
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

    cv::imshow("ellipses", canny);

	std::vector<std::vector<cv::Point3f>> worldCoords = Transformation::getWorldCoordinatesForSamples();

	//cv::Mat proj = Transformation::getProjectiveMatrix(pointsPerEllipse, worldCoords);

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

	//Transformation::reverseWarp(grey, proj);
	//return;

	
	Transformation::getWorldCoordinatesInt(ellipses, lines, pointsPerEllipse, worldCoords, grey);


    return;
    
}


void ConeUnfolding::sort(std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<Ellipse>& ellipses)
{
    cv::Mat debug = cv::Mat::zeros(Config::usedResHeight, Config::usedResWidth, CV_8UC3);

	cv::Point2d innerMostCenter = ellipses[0].getCenter();
	for(size_t i = 0; i < ellipses.size(); i++)
	{
		auto sortByAngle = [innerMostCenter, this](const cv::Point2f& pt1, const cv::Point2f& pt2)
		{
			return angleWithX(cv::Point2d(pt1.x, pt1.y), innerMostCenter) < angleWithX(cv::Point2d(pt2.x, pt2.y), innerMostCenter);
		};
		
		std::sort(pointsPerEllipse[i].begin(), pointsPerEllipse[i].end(), sortByAngle);
	}

	for(const std::vector<cv::Point2f>& pointList : pointsPerEllipse)
	{
		for(size_t i = 0; i < pointList.size() && i < 2; i++)
		{
			float currentStep = (1.0f * i) / (pointList.size() - 1);
			cv::circle(debug, pointList[i], 8, cv::Scalar(currentStep * 150, 255, 255), -1);
		}
	}

	cv::cvtColor(debug, debug, CV_HSV2BGR);
	if(isDebug)
		cv::imshow("debug sort", debug);

}

double ConeUnfolding::angleWithX(const cv::Point2d& pt1, const cv::Point2d& pt2)
{
	cv::Point2d dirPt = pt1 - pt2;
	dirPt = 1 / cv::norm(dirPt) * dirPt;
	double angle =  atan2(dirPt.y, dirPt.x);
	angle = Misc::mod(angle, 2 * CV_PI);
	return angle;
}



