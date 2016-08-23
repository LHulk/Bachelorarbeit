#ifndef STORAGE_H
#define STORAGE_H


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

#include "Line.h"
#include "Ellipse.h"

struct Storage
{
	std::vector<Line> lines;
	std::vector<Ellipse> ellipses;
	std::vector<std::vector<cv::Point2f>> sampleImageCoordinates;
	std::vector<std::vector<cv::Point3f>> sampleWorldCoordinates;
};


#endif //STORAGE_H
