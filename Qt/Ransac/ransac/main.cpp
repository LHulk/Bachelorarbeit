#include <iostream>

#include "Ellipse.h"

cv::RNG rng;

int main()
{
	cv::Mat img = cv::Mat::zeros(700, 700, CV_8UC3);

	Ellipse e = Ellipse(350, 350, 200, 150, CV_PI/6);
	Ellipse eShadow = Ellipse(350, 350, 300, 250, CV_PI/6);
	//cv::ellipse(img, e.getEllipseAsRotatedRect(), cv::Scalar(0,255,0), 2);
	//cv::ellipse(img, eShadow.getEllipseAsRotatedRect(), cv::Scalar(0,255,0), 2);


	int sizeEllipse = 500;
	int sizeError = 500;

	std::vector<cv::Point> points;
	for(int i = 0; i < sizeEllipse; i++)
	{
		double angle = rng.uniform(0.0, 2*CV_PI);

		//distort data a bit
		cv::Point2d offset = cv::Point2d(rng.uniform(0,0), rng.uniform(0,0));

		cv::Point2d ellipsePoint = e.evalAtPhi(angle) + offset;
		cv::circle(img, ellipsePoint, 2, cv::Scalar(0,255,255), -1);

		points.push_back(ellipsePoint);
	}

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


	cv::imshow("points", img);
	cv::waitKey(0);

	double p = 0.9999;
	int k = 6;
	double relativeError = sizeError / static_cast<double>(sizeEllipse + sizeError);

	int minN = static_cast<int>(std::ceil(std::log(1.0 - p) / std::log(1.0 - std::pow(1.0 - relativeError, k))));

	std::cout << "relative error: " << relativeError << " with p = " << p << std::endl;
	std::cout << "min its: " << minN << std::endl;

	//cv::imwrite("ransac50_0.png", img);

	Config::usedResHeight = 700;
	Config::usedResWidth = 700;
	float dist = 1.0f;
	Ellipse fittedEllipse = Ellipse::robustEllipseFit(points, img.size(), dist, 10.0f, 20);
	cv::ellipse(img, fittedEllipse.getEllipseAsRotatedRect(), cv::Scalar(0,255,0), 2);

	cv::RotatedRect leastSquareEllipse = cv::fitEllipse(points);
	cv::ellipse(img, leastSquareEllipse, cv::Scalar(255, 255, 0), 2);

	cv::imshow("fitted", img);
	//cv::imwrite("ransac50_1.png", img);
	cv::waitKey(0);


	return 0;
}

