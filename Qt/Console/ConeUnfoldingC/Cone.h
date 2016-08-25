#ifndef CONE_H
#define CONE_H

#include <opencv2/core/core.hpp>

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include "Config.h"
#include "Ellipse.h"
#include "Line.h"
#include "Misc.h"


class Cone
{
public:
	Cone();

	std::vector<std::vector<cv::Point3f>> getWorldCoordinatesForSamples();
	cv::Point3f interPolateRadial(const cv::Point& pt, int val) const;

	cv::Point2f coneCoordinatesToLateral(const cv::Point3f& pt) const;
	cv::Point3f lateralToConeCoordinates(const cv::Point2f& pt) const;

	~Cone();
private:
	float _r; //inner radius
	float _R; //outer radius
	float _H; //center to tip
	float _dh; //center to center
	float _S; //slant height whole cone
	float _s; //slant height of imaginary cone (difference from frustum to whole cone)
	
	std::vector<Line> _lines;
	std::vector<Ellipse> _ellipses;
	std::vector<std::vector<cv::Point2f>> _sampleImg;
	std::vector<std::vector<cv::Point3f>> _sampleWorld;

public:
	float R() const { return _R;  }
	float r() const { return _r;  }
	float H() const { return _H; }
	float dh() const  { return _dh; }
	float h() const  { return _H - _dh; }
	float S() const { return _S; }
	float s() const { return _s; }
	double maxAngle() const { return 2 * CV_PI * _R / _S;  }

	void setLines(std::vector<Line> lines) { _lines = lines; }
	void setEllipses(std::vector<Ellipse> ellipses) { _ellipses = ellipses; }
	void setSampleCoordsImage(std::vector<std::vector<cv::Point2f>> sampleCoords) { _sampleImg = sampleCoords; }

	std::vector<Line> lines() const { return _lines; }
	std::vector<Ellipse> ellipses() const { return _ellipses; }
	std::vector<std::vector<cv::Point2f>> sampleCoordsImage() const { return _sampleImg; }
	std::vector<std::vector<cv::Point3f>> sampleCoordsWorld() const { return _sampleWorld; }
};

#endif //CONE_H