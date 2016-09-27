#include "Misc.h"

//determine angle from vector pt1 -> pt2 with x axis
double Misc::angleWithX(const cv::Point2d& pt1, const cv::Point2d& pt2)
{
	cv::Point2d dirPt = pt2 - pt1;
	dirPt = 1 / cv::norm(dirPt) * dirPt;
	double angle = atan2(dirPt.y, dirPt.x);
	angle = Misc::mod(angle, 2 * CV_PI);
	return angle;
}

//sort sample points on every ellipse so that ith position in all vectors correspond to same line
void Misc::sort(std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<Ellipse>& ellipses)
{
	//determine center from innermost circle/ellipse by using average
	cv::Point2d innerMostCenter = cv::Point2d(0, 0);
	for(const auto& pt : pointsPerEllipse[0])
		innerMostCenter += cv::Point2d(pt.x, pt.y);

	innerMostCenter = 1.0 / pointsPerEllipse[0].size() * innerMostCenter;

	//sort first ellipse by angle
	auto sortByAngle = [innerMostCenter](const cv::Point2f& pt1, const cv::Point2f& pt2)
	{
		return angleWithX(innerMostCenter, cv::Point2d(pt1.x, pt1.y)) < angleWithX(innerMostCenter, cv::Point2d(pt2.x, pt2.y));
	};

	std::sort(pointsPerEllipse[0].begin(), pointsPerEllipse[0].end(), sortByAngle);

	//sort rest by shortest distance
	for(size_t i = 1; i < pointsPerEllipse.size(); i++)
	{
		std::vector<cv::Point2f> sorted;
		sorted.resize(pointsPerEllipse[i].size());

		for(const auto& pt : pointsPerEllipse[i])
		{
			auto minElementIt = std::min_element(pointsPerEllipse[0].begin(), pointsPerEllipse[0].end(), [pt](const cv::Point2f& pt1, const cv::Point2f& pt2) { return cv::norm(pt - pt1) < cv::norm(pt - pt2); });
			int dist = std::distance(pointsPerEllipse[0].begin(), minElementIt);
			sorted[dist] = pt;
		}

		pointsPerEllipse[i] = sorted;
	}

}
