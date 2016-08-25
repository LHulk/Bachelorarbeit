#include "Misc.h"


double Misc::angleWithX(const cv::Point2d& pt1, const cv::Point2d& pt2)
{
	cv::Point2d dirPt = pt1 - pt2;
	dirPt = 1 / cv::norm(dirPt) * dirPt;
	double angle = atan2(dirPt.y, dirPt.x);
	angle = Misc::mod(angle, 2 * CV_PI);
	return angle;
}


void Misc::sort(std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<Ellipse>& ellipses)
{
	cv::Point2d innerMostCenter = ellipses[0].getCenter();
	for(size_t i = 0; i < ellipses.size(); i++)
	{
		auto sortByAngle = [innerMostCenter](const cv::Point2f& pt1, const cv::Point2f& pt2)
		{
			return angleWithX(cv::Point2d(pt1.x, pt1.y), innerMostCenter) < angleWithX(cv::Point2d(pt2.x, pt2.y), innerMostCenter);
		};

		std::sort(pointsPerEllipse[i].begin(), pointsPerEllipse[i].end(), sortByAngle);
	}
}