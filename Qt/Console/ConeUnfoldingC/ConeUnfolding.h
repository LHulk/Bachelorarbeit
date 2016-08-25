#ifndef CONEUNFOLDING_H
#define CONEUNFOLDING_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <vector>
#include <iostream>

#include "EdgeDetection.h"
#include "Ellipse.h"
#include "Line.h"
#include "DotDetection.h"
#include "Config.h"
#include "Transformation.h"
#include "Cone.h"


class ConeUnfolding
{
public:
	ConeUnfolding();
	static double angleWithX(const cv::Point2d& pt1, const cv::Point2d& pt2);
private:
	void sort(std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<Ellipse>& ellipses);
	

};

#endif // CONEUNFOLDING_H
