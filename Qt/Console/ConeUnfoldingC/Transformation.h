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
#include <fstream>

#include "Config.h"
#include "Ellipse.h"
#include "Line.h"
#include "ConeUnfolding.h"
#include "Misc.h"
#include "Cone.h"


class Transformation
{
public:
	static cv::Mat Transformation::getProjectiveMatrix(const Cone& cone);
	static cv::Mat getProjectiveMatrixBrute(const std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<std::vector<cv::Point3f>>& worldCoords);

	static void forwardWarp(const cv::Mat& img, const Cone& cone);
	static void reverseWarp(const cv::Mat& greyImg, const cv::Mat& proj, const Cone& cone);
private:
	static void fillSegments(cv::Mat& img, const std::vector<Ellipse>& ellipses, const std::vector<Line>& lines, const std::vector <std::vector<cv::Point2f>>& pointsPerEllipse);
	
	static void writeToFile(std::vector<cv::Point2f> point2f, const std::string& filename);
	static void writeToFile(std::vector<cv::Point3f> point3f, const std::string& filename);
};


#endif //TRANSFORMATION_H
