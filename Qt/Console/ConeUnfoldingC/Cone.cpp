#include "Cone.h"


Cone::Cone() : _r(Config::radiusInner), _R(Config::radiusOuter)
{
	float height = Config::height;

	switch(Config::whichHeight)
	{
		case CENTER_TO_CENTER:
			_H = height / (1 - _r / _R);
			_dh = height;
			break;
		case CENTER_TO_TIP:
		default:
			_H = height;
			_dh = _H - ((_H / _R) *_r);
			break;
	} 


	_S = std::sqrt(_H*_H + _R*_R);
	_s = std::sqrt((_H*_H*_r*_r / (_R*_R)) + _r*_r);
}


//center is (0,0)
//right hand coordinate system (x,y,z) z is depth directed out of screen plane
std::vector<std::vector<cv::Point3f>> Cone::calculateWorldCoordinatesForSamples() const
{
	int n = Config::numCircleSamples;
	int m = Config::numLineSamples;

	std::vector<std::vector<cv::Point3f>> worldCoordinates = std::vector<std::vector<cv::Point3f>>(n, std::vector<cv::Point3f>(0));

	//iterate through ellipses
	for(int i = 0; i < n; i++)
	{
		float y = static_cast<float>(i) / (n - 1) * _dh;
		float currR = _r + (y / _dh) * (_R - _r);

		//iterate through points on ith ellipse
		for(int j = 0; j < m; j++)
		{
			float angle = j * 2 * static_cast<float>(CV_PI) / (m);
			float x = currR * std::cos(angle);
			float z = currR * std::sin(angle);

			worldCoordinates[i].push_back(cv::Point3f(x, y, z));
		}
	}

	return worldCoordinates;

}


//calculates 2d coordinates for samples on lateral surface (used for reprojection analysis)
std::vector<std::vector<cv::Point2f>> Cone::calculateLateralSamples() const
{
	std::vector<std::vector<cv::Point2f>> laterals;
	double angleOffset = CV_PI / 2;

	int n = Config::numCircleSamples;
	int m = Config::numLineSamples;

	for(int i = 0; i < n; i++)
	{
		std::vector<cv::Point2f> currVec;
		double currS = _s + static_cast<float>(i) / (n - 1) * (_S - _s);
		for(int j = 0; j < m; j++)
		{
			double currAngle = static_cast<float>(j) / m * this->maxAngle() + angleOffset;
			currVec.push_back(currS * cv::Point2f(std::cos(currAngle), std::sin(currAngle)));
		}
		laterals.push_back(currVec);
	}

	return laterals;

}


//3d interpolation for forward warping
cv::Point3f Cone::interPolateRadial(const cv::Point& pt, int val) const
{
	//obtain unique number of ellipse and line segments (see fillSegments)
	val = (val - 2) / 2;
	int i = val / Config::numLineSamples;
	int j = val % Config::numLineSamples;

	Ellipse e1 = _ellipses[i];
	double dist1 = Ellipse::realDistTrans(e1, pt);
	Ellipse e2 = _ellipses[i + 1];
	double dist2 = Ellipse::realDistTrans(e2, pt);

	//define interpolating ellipse by distance to next two ellipses
	Ellipse interEllipse = (dist1 / (dist1 + dist2) * e2) + (dist2 / (dist1 + dist2) * e1);

	cv::Point2d int1 = Ellipse::ellipseLineIntersection(interEllipse, _lines[j]);
	cv::Point2d int2 = Ellipse::ellipseLineIntersection(interEllipse, _lines[(j + 1) % Config::numLineSamples]);

	cv::Point2d tl = _sampleImg[i + 1][j];
	cv::Point2d bl = _sampleImg[i][j];
	cv::Point2d tr = _sampleImg[i + 1][(j + 1) % Config::numLineSamples];
	cv::Point2d br = _sampleImg[i][(j + 1) % Config::numLineSamples];

	//perform linear interpolation on line segments
	double intD1 = (tl.y - bl.y != 0) ? (int1.y - bl.y) / (tl.y - bl.y) : (int1.x - bl.x) / (tl.x - bl.x);
	double intD2 = (tr.y - br.y != 0) ? (int2.y - br.y) / (tr.y - br.y) : (int2.x - br.x) / (tr.x - br.x);
	cv::Point3f temp1, temp2;
	temp1 = intD1 * _sampleWorld[i + 1][j] + (1 - intD1) * _sampleWorld[i][j];
	temp2 = intD2 * _sampleWorld[i + 1][(j + 1) % Config::numLineSamples] + (1 - intD2) * _sampleWorld[i][(j + 1) % Config::numLineSamples];


	//perform angle interpolation on ellipse arc
	cv::Point3f res;
	double angleInt = std::fmod(std::abs(Ellipse::getAngleAt(interEllipse, int1) - Ellipse::getAngleAt(interEllipse, int2)), 2 * CV_PI);
	angleInt = (angleInt > CV_PI) ? 2 * CV_PI - angleInt : angleInt;
	double angle2Int = std::fmod(std::abs(Ellipse::getAngleAt(interEllipse, int1) - Ellipse::getAngleAt(interEllipse, pt)), 2 * CV_PI);
	angle2Int = (angle2Int > CV_PI) ? 2 * CV_PI - angle2Int : angle2Int;
	angleInt = angle2Int / angleInt;
	res = angleInt * temp2 + (1 - angleInt) * temp1;

	return res;
}


//implementation of mapping from 3D cone coordinates to 2D lateral surface
cv::Point2f Cone::coneCoordinatesToLateral(const cv::Point3f& pt) const
{
	cv::Point2f lateralPoint;

	float ri = _s + (pt.y / _dh) * (_S - _s);

	ri = Misc::clamp(ri, _s, _S);

	float cosP = pt.x / (_r + (pt.y / _dh) * (_R - _r));
	float sinP = pt.z / (_r + (pt.y / _dh) * (_R - _r));

	float angle = std::atan2(sinP, cosP);
	angle = static_cast<float>(Misc::mod(angle, 2 * CV_PI));

	angle = angle * (_R / _S);

	lateralPoint.x = -ri * std::sin(angle);
	lateralPoint.y = ri * std::cos(angle);

	return lateralPoint;
}

//implementation of inverse mapping from 2D lateral surface to 3D cone coordinates
cv::Point3f Cone::lateralToConeCoordinates(const cv::Point2f& pt) const
{
	cv::Point3f conePoint;

	float v = std::sqrt(pt.x*pt.x + pt.y*pt.y);
	float sinP = -pt.x / v;
	float cosP = pt.y / v;


	float angle = std::atan2(sinP, cosP);
	angle = static_cast<float>(Misc::mod(angle, 2 * CV_PI));

	angle = angle * (_S / _R);

	conePoint.y = (v - _s) / (_S - _s) * _dh;
	conePoint.y = Misc::clamp(conePoint.y, 0, _dh);

	float ri = _r + (conePoint.y / _dh) * (_R - _r);
	ri = Misc::clamp(ri, _r, _R);

	conePoint.x = ri * std::cos(angle);
	conePoint.z = ri * std::sin(angle);

	return conePoint;
}


Cone::~Cone()
{
}
