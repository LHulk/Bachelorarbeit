#ifndef CONE_H
#define CONE_H

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


//enum
//{
//	CENTER_TO_CENTER,
//	CENTER_TO_TIP
//};

class Cone
{
public:
	Cone();
	Cone(const Cone& Cone);
	Cone& operator=(const Cone& other);

	std::vector<std::vector<cv::Point3f>> getWorldCoordinatesForSamples();
	cv::Point3f interPolateRadial(const cv::Point& pt, int val);

	cv::Point2f coneCoordinatesToLateral(const cv::Point3f& pt);
	cv::Point3f lateralToConeCoordinates(const cv::Point2f& pt);
	

	~Cone();
private:
	float _r; //inner radius
	float _R; //outer radius
	float _H; // center to tip
	float _dh; //center to center
	float _S; //slant height whole cone
	float _s; //slant height of imaginary cone (difference from frustum to whole cone)
	
	std::vector<Line> _lines;
	std::vector<Ellipse> _ellipses;
	std::vector<std::vector<cv::Point2f>> _sampleImg;
	std::vector<std::vector<cv::Point3f>> _sampleWorld;

public:
	float R() { return _R;  }
	float r() { return _r;  }
	float H() { return _H; }
	float dh() { return _dh; }
	float h() { return _H - _dh; }

	void setLines(std::vector<Line> lines) { _lines = lines; }
	void setEllipses(std::vector<Ellipse> ellipses) { _ellipses = ellipses; }
	void setSampleCoordsImage(std::vector<std::vector<cv::Point2f>> sampleCoords) { _sampleImg = sampleCoords; }

	std::vector<Line> lines() { return _lines; }
	std::vector<Ellipse> ellipses() { return _ellipses; }
	std::vector<std::vector<cv::Point2f>> sampleCoordsImage() { return _sampleImg; }
	std::vector<std::vector<cv::Point3f>> sampleCoordsWorld() { return _sampleWorld; }
};

#endif //CONE_H