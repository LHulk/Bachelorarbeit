#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <numeric>

#include "Config.h"
#include "Ellipse.h"
#include "Line.h"
#include "ConeUnfolding.h"
#include "Misc.h"
#include <fstream>

class Transformation
{
public:
	static std::vector<std::vector<cv::Point3f>> getWorldCoordinatesForSamples();
	static void getWorldCoordinatesInt(const std::vector<Ellipse>& ellipses, const std::vector<Line>& lines, const std::vector <std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<std::vector<cv::Point3f>>& worldCoords, const cv::Mat& img);
	static cv::Mat Transformation::getProjectiveMatrix(const std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<std::vector<cv::Point3f>>& worldCoords);
	static cv::Mat getProjectiveMatrixBrute(const std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<std::vector<cv::Point3f>>& worldCoords);
	static void reverseWarp(const cv::Mat& greyImg, const cv::Mat& proj);
private:
	Transformation();

	static void fillSegments(cv::Mat& img, const std::vector<Ellipse>& ellipses, const std::vector<Line>& lines, const std::vector <std::vector<cv::Point2f>>& pointsPerEllipse);

	static cv::Point2d coneCoordinatesToLateral(const cv::Point3d& pt);
	static cv::Point3d lateralToConeCoordinates(const cv::Point2d& pt);

	static cv::Point3f interPolateRadial(const cv::Point& pt, int val, const std::vector<Ellipse>& ellipses, const std::vector<Line>& lines, const std::vector <std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<std::vector<cv::Point3f>>& worldCoords);
	
	static void writeToFile(std::vector<cv::Point2f> point2f, const std::string& filename);
	static void writeToFile(std::vector<cv::Point3f> point3f, const std::string& filename);
};


#endif //TRANSFORMATION_H
