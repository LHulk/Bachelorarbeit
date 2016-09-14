#include <iostream>
#include <cmath>

#include "Ellipse.h"

cv::RNG rng;

double sq(double val)
{
	return val*val;
}

double distTo(const Ellipse& e1, const Ellipse& e2)
{
	double norm = sq(e1.getSemiMinor() - e2.getSemiMinor()) + sq(e1.getSemiMajor() - e2.getSemiMajor()) +
			sq(e1.getCenter().x - e2.getCenter().x) + sq(e1.getCenter().y - e2.getCenter().y);

	norm = std::sqrt(norm);
	return norm;
}

int main()
{
	cv::Mat img = cv::Mat::zeros(700, 700, CV_8UC3);

	//for(int i = 0; i < )
	Ellipse e = Ellipse(350, 350, 300, 300, 0);
	//Ellipse eShadow = Ellipse(350, 350, 300, 250, 0);
	//cv::ellipse(img, e.getEllipseAsRotatedRect(), cv::Scalar(0,255,0), 2);
	//cv::ellipse(img, eShadow.getEllipseAsRotatedRect(), cv::Scalar(0,255,0), 2);

	int sizeEllipse = 500;
	std::vector<cv::Point> points;
	std::string str1 = "[";
	std::string str2 = "[";
	for(int i = 0; i < sizeEllipse; i++)
	{
		double angle = rng.uniform(0.0, 2*CV_PI);

		//distort data a bit
		//cv::Point2d offset = cv::Point2d(rng.uniform(0,0), rng.uniform(0,0));
		cv::Point2d offset = cv::Point2d(0,0);

		cv::Point2d ellipsePoint = e.evalAtPhi(angle) + offset;
		cv::circle(img, ellipsePoint, 2, cv::Scalar(0,255,255), -1);

		points.push_back(ellipsePoint);
	}

	for(int i = 0; i <= 1000; i++)
	{
		int sizeError = i;

		//generate errors
		for(int i = 0; i < sizeError; i++)
		{
			cv::Point errorPoint = cv::Point(rng.uniform(0,699), rng.uniform(0,699));
			cv::circle(img, errorPoint, 2, cv::Scalar(0,255,255), -1);

			/*double angle = rng.uniform(0.0, 2*CV_PI);

			//distort data a bit
			cv::Point2d offset = cv::Point2d(rng.uniform(0,0), rng.uniform(0,0));

			cv::Point2d errorPoint = eShadow.evalAtPhi(angle) + offset;
			cv::circle(img, errorPoint, 2, cv::Scalar(0,255,255), -1);*/

			points.push_back(errorPoint);

		}


		//cv::imshow("points", img);
		//cv::waitKey(0);

		double p = 0.9999;
		int k = 6;
		double relativeError = sizeError / static_cast<double>(sizeEllipse + sizeError);

		int minN = static_cast<int>(std::ceil(std::log(1.0 - p) / std::log(1.0 - std::pow(1.0 - relativeError, k))));

		//std::cout << "relative error: " << relativeError << " with p = " << p << std::endl;
		//std::cout << "min its: " << minN << std::endl;

		//cv::imwrite("ransac50_0.png", img);

		std::cout << i << ": with min it: " << minN << std::endl;

		Config::usedResHeight = 700;
		Config::usedResWidth = 700;
		float dist = 1.0f;
		Ellipse fittedEllipse = Ellipse::robustEllipseFit(points, img.size(), dist, 10.0f, (minN > 0) ? minN : 1);
		//cv::ellipse(img, fittedEllipse.getEllipseAsRotatedRect(), cv::Scalar(0,255,0), 2);

		//std::cout << distTo(e, fittedEllipse) << std::endl;
		str1 += std::to_string(distTo(e, fittedEllipse)) + ",";


		Ellipse leastSquareEllipse = cv::fitEllipse(points);
		//cv::ellipse(img, leastSquareEllipse.getEllipseAsRotatedRect(), cv::Scalar(255, 255, 0), 2);

		//std::cout << distTo(e, leastSquareEllipse) << std::endl;
		str2 += std::to_string(distTo(e, leastSquareEllipse)) + ",";

		//cv::imshow("fitted", img);
		//cv::imwrite("ransac50_1.png", img);
		//cv::waitKey(0);
	}

	str1 += "];"; str2 += "];";
	std::cout << str1 << std::endl;
	std::cout << str2 << std::endl;


	return 0;
}

