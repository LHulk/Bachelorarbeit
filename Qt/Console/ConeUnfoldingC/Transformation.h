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

	static void getForwardWarpMaps(const Cone& cone, cv::Mat &remapX, cv::Mat &remapY, cv::Mat img = cv::Mat());
	static std::vector<std::vector<cv::Point2f>> getForwardReprojects(const Cone& cone, std::vector<std::vector<cv::Point2f>> pointsPerEllipse);
	static void getReverseWarpMaps(const Cone& cone, cv::Mat &remapX, cv::Mat &remapY, const cv::Mat& proj);
	static std::vector<std::vector<cv::Point2f>> getReverseReprojects(const Cone& cone, const cv::Mat& proj);
	static void inverseRemap(const cv::Mat& src, cv::Mat& dst, const cv::Mat &remapX, const cv::Mat& remapY);

	static std::vector<cv::Point2f> getReprojectionError(const Cone& cone, const cv::Mat& projection);
	static size_t countHoles(const cv::Mat& unfolded, const Cone& cone);
private:
	static void fillSegments(cv::Mat& img, const std::vector<Ellipse>& ellipses, const std::vector<Line>& lines, const std::vector <std::vector<cv::Point2f>>& pointsPerEllipse);
	static cv::Mat generateLateralMask(const Cone& cone);
	static void writeToFile(std::vector<cv::Point2f> point2f, const std::string& filename);
	static void writeToFile(std::vector<cv::Point3f> point3f, const std::string& filename);
};


#endif //TRANSFORMATION_H
