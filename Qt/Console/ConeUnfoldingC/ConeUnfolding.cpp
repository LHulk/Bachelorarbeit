#include "ConeUnfolding.h"

static bool isDebug = true;


#include <chrono>
#include <ctime>

ConeUnfolding::ConeUnfolding()
{
	//Mat grey = cv::imread("../../../img/v5_pattern/pattern2.png", CV_LOAD_IMAGE_GRAYSCALE);
	//cv::Mat grey = cv::imread("../../../img/v5_pattern/pattern3.png", CV_LOAD_IMAGE_GRAYSCALE);
	//cv::Mat grey = cv::imread("../../../img/v5_pattern/test.png", CV_LOAD_IMAGE_GRAYSCALE);
	//cv::Mat grey = cv::imread("../../../img/v5_pattern/blender_perfectCenterDebug.png", CV_LOAD_IMAGE_GRAYSCALE);
	//cv::Mat grey = cv::imread("../../../img/v5_pattern/blender_perfectCenterDebug.png", CV_LOAD_IMAGE_GRAYSCALE);
	//cv::Mat grey = cv::imread("../../../img/rasp/30-08_16-51-18.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//cv::Mat grey = cv::imread("../../../img/rasp/30-08_16-27-48.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	/*cv::Mat calib = cv::imread("../../../img/calibration/chessboard/25-08_21-47-35.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::resize(calib, calib, cv::Size(1000, 1000 * calib.rows / calib.cols));

	cv::Mat debug;
	Calibration::getCorners(calib, debug);
	cv::imshow("corners", debug);
	return;*/
	cv::Mat grey = cv::imread("../../../img/rasp/30-08_17-26-20.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	std::string prefix = "../../GUI/ConsoleUnfoldingGUI/xml/";
	for(int i = 1050; i <= 1200; i+= 50)
	{
		//std::cout << i << std::endl;

		cv::FileStorage fs(prefix + std::to_string(i) + ".xml.gz", cv::FileStorage::READ);
		cv::Mat camera, dist, reWX, reWY;
		fs["cameraMatrix"] >> camera;
		fs["distCoeffs"] >> dist;

		fs["remapXWarp"] >> reWX;
		fs["remapYWarp"] >> reWY;
		fs.release();

		cv::Mat reCX, reCY;
		cv::initUndistortRectifyMap(camera, dist, cv::Mat(), camera, grey.size(), CV_32FC1, reCX, reCY);

		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		for(int i = 0; i < 500; i++)
		{
			cv::Mat unwarpedCam;
			cv::remap(grey, unwarpedCam, reCX, reCY, cv::INTER_CUBIC);

			cv::Mat unwarpedCone;
			cv::remap(unwarpedCam, unwarpedCone, reWX, reWY, cv::INTER_CUBIC);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		long long millisecs = (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count())/500;
		std::cout << millisecs << std::endl;



	}

	std::cout << "finished" << std::endl;
	return;

	cv::Mat original = grey.clone();

	Config::scaleFactor = 1000.0/grey.cols;
	cv::resize(grey, grey, cv::Size(1000, 1000 * grey.rows / grey.cols));
	Config::usedResWidth = grey.cols;
	Config::usedResHeight = grey.rows;

	Cone cone = Cone();

	cv::imshow("grey", grey);
	//cv::imwrite("grey.png", grey);


	//canny
	cv::Mat canny, orientation;
	EdgeDetection::canny(grey, canny, orientation, Config::cannyLow, Config::cannyHigh, Config::cannyKernel, Config::cannySigma);

	cv::imshow("canny", canny);


	std::vector<cv::Point2f> keyPoints;
	keyPoints = DotDetection::detectDots(grey);

	//TODO: HERE CHECK IF POINTS HAVE BEEN DETECTED

	std::vector<Ellipse> ellipses = Ellipse::detectEllipses(canny);
	std::vector<std::vector<cv::Point2f>> pointsPerEllipse = Ellipse::getEllipsePointMappings(ellipses, keyPoints);
	Misc::sort(pointsPerEllipse, ellipses);




	/*cv::Mat debug = cv::Mat::zeros(grey.size(), CV_8UC3);

	int i = 0;
	for(const Ellipse& e : ellipses)
	{
		float currentStep = (1.0f * i++) / (ellipses.size() - 1);
		cv::ellipse(debug, e.getEllipseAsRotatedRect(), cv::Scalar(currentStep * 150, 255, 255), 2);
	}

	i = 0;
	for(const std::vector<cv::Point2f>& ptList : pointsPerEllipse)
	{
		float currentStep = (1.0f * i++) / (ellipses.size() - 1);
		for(const cv::Point2f& pt : ptList)
			cv::circle(debug, pt, 10, cv::Scalar(currentStep * 150, 255, 255), -1);
	}

	cv::cvtColor(debug, debug, CV_HSV2BGR);
	cv::imshow("debug ellipses", debug);*/


	

	ellipses = Ellipse::reestimateEllipses(pointsPerEllipse, ellipses);
	std::vector<Line> lines = Line::fitLines(pointsPerEllipse);


	
	/*cv::Scalar colors[] = {cv::Scalar(0, 0, 255), cv::Scalar(0, 140, 255), cv::Scalar(0, 255, 0), cv::Scalar(140, 255, 0), cv::Scalar(255, 255, 0)};
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


	std::vector<std::vector<cv::Point3f>> worldCoords = cone.calculateWorldCoordinatesForSamples();

	cone.setEllipses(ellipses);
	cone.setLines(lines);
	cone.setSampleCoordsImage(pointsPerEllipse);
	cone.setSampleCoordsWorld(worldCoords);


	cv::Mat proj = Transformation::getProjectiveMatrix(cone);

	/*cv::Mat vis;
	cv::cvtColor(grey, vis, CV_GRAY2BGR);
	for(size_t i = 0; i < worldCoords.size(); i++)
	{
		for(size_t j = 0; j < worldCoords[i].size(); j++)
		{
			cv::Point3f currentWorld = worldCoords[i][j];
			cv::Point2f currImg = pointsPerEllipse[i][j];

			cv::Mat currentWorldMat = cv::Mat::zeros(4, 1, CV_32F);
			currentWorldMat.at<float>(0, 0) = currentWorld.x; currentWorldMat.at<float>(1, 0) = currentWorld.y; currentWorldMat.at<float>(2, 0) = currentWorld.z; currentWorldMat.at<float>(3, 0) = 1;
			cv::Mat homImg = proj*currentWorldMat;

			float w = homImg.at<float>(2, 0);
			cv::Point2f reprojected = cv::Point2f(homImg.at<float>(0, 0) / w, homImg.at<float>(1, 0) / w);

			cv::circle(vis, currImg, 2, cv::Scalar(0, 255, 0), -1);
			cv::circle(vis, reprojected, 2, cv::Scalar(0, 0, 255), -1);
		}
	}
	cv::imshow("repro projection", vis);



	std::vector<std::vector<cv::Point2f>> rerveseReprojects = Transformation::getReverseReprojects(cone, proj);
	cv::cvtColor(grey, vis, CV_GRAY2BGR);
	for(size_t i = 0; i < worldCoords.size(); i++)
	{
		for(size_t j = 0; j < worldCoords[i].size(); j++)
		{
			cv::Point2f currImg = pointsPerEllipse[i][j];
			cv::Point2f currReproject = rerveseReprojects[i][j];

			cv::circle(vis, currImg, 2, cv::Scalar(0, 255, 0), -1);
			cv::circle(vis, currReproject, 2, cv::Scalar(0, 0, 255), -1);
		}
	}
	cv::imshow("repro reverseWarp", vis);


	std::vector<std::vector<cv::Point2f>> rerveseReprojects2 = Transformation::getForwardReprojects(cone, pointsPerEllipse);
	cv::cvtColor(grey, vis, CV_GRAY2BGR);
	for(size_t i = 0; i < worldCoords.size(); i++)
	{
		for(size_t j = 0; j < worldCoords[i].size(); j++)
		{
			cv::Point2f currImg = pointsPerEllipse[i][j];
			cv::Point2f currReproject = rerveseReprojects2[i][j];

			cv::circle(vis, currImg, 2, cv::Scalar(0, 255, 0), -1);
			cv::circle(vis, currReproject, 2, cv::Scalar(0, 0, 255), -1);
		}
	}
	cv::imshow("repro forwardWarp", vis);*/

	cv::Mat mapx, mapy;
	bool isReverse = true;
	cv::Mat greyWarped;

	if(isReverse)
	{
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
