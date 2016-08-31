#include "Misc.h"


double Misc::angleWithX(const cv::Point2d& pt1, const cv::Point2d& pt2)
{
	cv::Point2d dirPt = pt2 - pt1;
	dirPt = 1 / cv::norm(dirPt) * dirPt;
	double angle = atan2(dirPt.y, dirPt.x);
	angle = Misc::mod(angle, 2 * CV_PI);
	return angle;
}


void Misc::sort(std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<Ellipse>& ellipses)
{
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

	/*cv::Mat debug = cv::Mat::zeros(726, 1000, CV_8UC3);
	cv::circle(debug, innerMostCenter, 4, cv::Scalar(255, 255, 0), -1);
	for(const auto& ptList : pointsPerEllipse)
	{
		for(const auto& pt : ptList)
		{
			cv::circle(debug, pt, 4, cv::Scalar(255, 255, 255), -1);
			//std::cout << angleWithX(cv::Point2d(pt.x, pt.y), innerMostCenter) << std::endl;
		}
	}*/
}