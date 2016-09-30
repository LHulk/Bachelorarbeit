#include "ConeUnfolding.h"

static bool isDebug = false;

/*console application for cone calibration
 * wihtout intrinsic camera calibration */
//see settings file for cone and calibration pattern settings
ConeUnfolding::ConeUnfolding()
{
	//get image, scale it and clone for later scaling
	cv::Mat grey = cv::imread("../../../img/v5_pattern/pattern2.png", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat original = grey.clone();
	Config::scaleFactor = 1000.0/grey.cols;
	cv::resize(grey, grey, cv::Size(1000, 1000 * grey.rows / grey.cols));
	Config::usedResWidth = grey.cols;
	Config::usedResHeight = grey.rows;

	Cone cone = Cone();
	cv::imshow("grey", grey);


	//get edges
	cv::Mat canny, orientation;
	EdgeDetection::canny(grey, canny, orientation, Config::cannyLow, Config::cannyHigh, Config::cannyKernel, Config::cannySigma);

	cv::imshow("canny", canny);


	//detect samples
	std::vector<cv::Point2f> keyPoints;
	keyPoints = DotDetection::detectDots(grey);

	//detect ellipses and samples to lines and ellipses
	std::vector<Ellipse> ellipses = Ellipse::detectEllipses(canny);
	std::vector<std::vector<cv::Point2f>> pointsPerEllipse = Ellipse::getEllipsePointMappings(ellipses, keyPoints);
	Misc::sort(pointsPerEllipse, ellipses);
	
	//reestimate
	ellipses = Ellipse::reestimateEllipses(pointsPerEllipse, ellipses);

	//get line segments
	std::vector<Line> lines = Line::fitLines(pointsPerEllipse);


	//show mappings
	if(isDebug)
	{
		cv::Scalar colors[] = {cv::Scalar(0, 0, 255), cv::Scalar(0, 140, 255), cv::Scalar(0, 255, 0), cv::Scalar(140, 255, 0), cv::Scalar(255, 255, 0)};
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
	}


	//obtain world coords for all samples
	std::vector<std::vector<cv::Point3f>> worldCoords = cone.calculateWorldCoordinatesForSamples();

	//set cone properties
	cone.setEllipses(ellipses);
	cone.setLines(lines);
	cone.setSampleCoordsImage(pointsPerEllipse);
	cone.setSampleCoordsWorld(worldCoords);

	

	//switch mode
	cv::Mat mapx, mapy;
	bool isReverse = true;
	cv::Mat greyWarped;

	if(isReverse)
	{
		cv::Mat proj = Transformation::getProjectiveMatrix(cone);
		Transformation::getReverseWarpMaps(cone, mapx, mapy, proj);
		cv::remap(original, greyWarped, mapx, mapy, cv::INTER_CUBIC);
	}
	else
	{
		Transformation::getForwardWarpMaps(cone, mapx, mapy);
		Transformation::inverseRemap(original, greyWarped, mapx, mapy);

	}



	cv::imshow("grey warped", greyWarped);




}
