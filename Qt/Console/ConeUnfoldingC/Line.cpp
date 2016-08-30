#include "Line.h"

using cv::Point;

static bool isDebug = true;

Line::Line(float rho, float theta)
{
    _isUnbounded = true;

    double a = cos(theta), b = sin(theta);
    double x0 = a*rho, y0 = b*rho;

	size_t t = 2 * std::max(Config::usedResHeight, Config::usedResWidth);

    _start = Point(cvRound(x0 + t*(-b)), cvRound(y0 + t*(a)));
    _end = Point(cvRound(x0 - t*(-b)), cvRound(y0 - t*(a)));

}

Line::Line(cv::Point start, cv::Point end) : _isUnbounded(false), _start(start), _end(end)
{

}

bool Line::intersect(const Line& other, Point& point) const
{
	//2d 'cross' product
	auto crossProduct = [](const Point& p1, const Point& p2) { return (p1.x*p2.y) - (p1.y*p2.x); };

    int c1 = crossProduct(this->getDir(), other.getDir());
    int c2 = crossProduct((other.getStart() - this->getStart()), other.getDir());

    if(c1 == 0)
        return false;

    double a = static_cast<double>(c2)/c1;
    if(a <= 1 && a >= 0)
    {
        point = Point(this->getStart() + a*this->getDir());
        point.x = cvRound(point.x);
        point.y = cvRound(point.y);
        return true;
    }


    return false;
}

bool Line::intersect(const Line& l1, const Line& l2, cv::Point& point)
{
	return l1.intersect(l2, point);
}


std::vector<Line> Line::getLines(const cv::Mat& edgeImage)
{
    cv::Mat debug = edgeImage.clone();
    cv::cvtColor(debug, debug, CV_GRAY2BGR);

    cv::vector<cv::Vec2f> lines;
    cv::HoughLines( edgeImage, lines, 1, CV_PI/180, 100, 2,2);

    cv::vector<Line> linesVec;

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0];
        float theta = lines[i][1];
        Line l = Line(rho,theta);
        linesVec.push_back(l);

		if(isDebug)
			cv::line(debug, l.getStart(), l.getEnd(), cv::Scalar(0, 0, 255), 1);
    }

    if (isDebug)
        cv::imshow("debug getLines", debug);

    return linesVec;
}

cv::Point Line::getWeightedCenter(const std::vector<Line>& lines)
{
    cv::vector<cv::Point> intersectPoints;

    for(const Line& l1 : lines)
    {
        for(const Line& l2 : lines)
        {
            if(l1 != l2)
            {
                cv::Point intersect;
                if(l1.intersect(l2, intersect))
                    intersectPoints.push_back(intersect);
            }
        }
    }

	std::nth_element(intersectPoints.begin(), intersectPoints.begin() + intersectPoints.size() / 2, intersectPoints.end(), [](const cv::Point& p1, const cv::Point& p2)
	{
		return p1.x < p2.x;
	});
	int x = intersectPoints[intersectPoints.size() / 2].x;

	std::nth_element(intersectPoints.begin(), intersectPoints.begin() + intersectPoints.size() / 2, intersectPoints.end(), [](const cv::Point& p1, const cv::Point& p2)
	{
		return p1.y < p2.y;
	});
	int y = intersectPoints[intersectPoints.size() / 2].y;

    /*cv::Point sumP = cv::Point(0,0);
    for(const cv::Point& p : intersectPoints)
    {
        sumP += p;
    }

    sumP = 1.0/intersectPoints.size() * sumP;*/

    //return sumP;
	return cv::Point(x, y);
}


std::vector<Line> Line::fitLines(const std::vector<std::vector<cv::Point2f>>& pointsPerEllipse)
{
	cv::Mat debug = cv::Mat::zeros(Config::usedResHeight, Config::usedResWidth, CV_8UC3);
	std::vector<Line> lines;
	for(size_t i = 0; i < static_cast<size_t>(Config::numLineSamples); i++)
	{
		cv::Point start = pointsPerEllipse[0].at(i); //ith point of innermost Ellipse
		cv::Point end = pointsPerEllipse[Config::numCircleSamples - 1].at(i); //ith point of outermostEllipse
		lines.push_back(Line(start, end));
	}

	for(const auto& pointList : pointsPerEllipse)
	{
		for(size_t i = 0; i < pointList.size(); i++)
		{
			float currentStep = (1.0f * i) / (pointList.size() - 1);

			if(isDebug)
				cv::circle(debug, pointList[i], 8, cv::Scalar(currentStep * 150, 255, 255), -1);
		}
	}


	for(size_t i = 0; i < lines.size(); i++)
	{
		float currentStep = (1.0f * i) / (lines.size() - 1);

		if(isDebug)
			cv::line(debug, lines[i].getStart(), lines[i].getEnd(), cv::Scalar(currentStep * 150, 255, 255), 2);
	}


	if(isDebug)
	{
		cv::cvtColor(debug, debug, CV_HSV2BGR);
		cv::imshow("debug fitLines", debug);
	}

	return lines;
}

Line Line::expand() const
{
	int t = 10;
	Line infLine(_start - t*getDir(), _end + t*getDir());

	return infLine;
}


bool Line::operator==(const Line& other) const
{
    return ((other.getStart() == this->getStart()) && (other.getEnd() == this->getEnd()));

}
bool Line::operator!=(const Line& other) const
{
    return !((other.getStart() == this->getStart()) && (other.getEnd() == this->getEnd()));
}


Point Line::getStart() const { return _start; }

Point Line::getEnd() const { return _end; }

Point Line::getDir() const { return _end - _start; }

cv::Point2d Line::getNormalizedDir() const
{
	cv::Point dir = this->getDir();
	return 1 / cv::norm(dir)*cv::Point2d(dir.x, dir.y);
}




