#ifndef MISC_H
#define MISC_H

#include <algorithm>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "Ellipse.h"

class Misc
{
public:
	static int mod(int a, int b) { return ((a % b) + b) % b; }
	static double mod(double a, double b) { return std::fmod(std::fmod(a, b) + b, b); }
	static double mod(int a, double b) { return mod(static_cast<double>(a), b); }
	static double mod(double a, int b) { return mod(a, static_cast<double>(b)); }

	static int round(double a) { return static_cast<int>(std::lround(a)); }

	static float clamp(float n, float lower, float upper) { return std::max(lower, std::min(n, upper)); }

	static double angleWithX(const cv::Point2d& pt1, const cv::Point2d& pt2);

	static void sort(std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<Ellipse>& ellipses);

};

#endif //MISC_H