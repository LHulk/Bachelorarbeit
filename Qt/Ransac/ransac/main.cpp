#include <iostream>

#include "Ellipse.h"

cv::RNG rng;

int main()
{
	cv::Mat img = cv::Mat::zeros(600, 600, CV_8UC3);

	Ellipse e = Ellipse(300, 300, 200, 100, CV_PI/6);
	Ellipse eShadow = Ellipse(300, 300, 300, 220, CV_PI/6);
	//cv::ellipse(img, e.getEllipseAsRotatedRect(), cv::Scalar(0,255,0), 2);
	//cv::ellipse(img, eShadow.getEllipseAsRotatedRect(), cv::Scalar(0,255,0), 2);


	int sizeEllipse = 150;
	int sizeError = 100;

	std::vector<cv::Point> points;
	for(int i = 0; i < sizeEllipse; i++)
	{
		double angle = rng.uniform(0.0, 2*CV_PI);

		//distort data a bit
		cv::Point2d offset = cv::Point2d(rng.uniform(-2,2), rng.uniform(-2,2));

		cv::Point2d ellipsePoint = e.evalAtPhi(angle) + offset;
		cv::circle(img, ellipsePoint, 2, cv::Scalar(0,255,255), -1);

		points.push_back(ellipsePoint);
	}

	//generate errors
	for(int i = 0; i < sizeError; i++)
	{
		/*cv::Point errorPoint = cv::Point(rng.uniform(0,599), rng.uniform(0,599));
		cv::circle(img, errorPoint, 2, cv::Scalar(0,255,255), -1);*/

		double angle = rng.uniform(0.0, 2*CV_PI);

		//distort data a bit
		cv::Point2d offset = cv::Point2d(rng.uniform(-2,2), rng.uniform(-2,2));

		cv::Point2d errorPoint = eShadow.evalAtPhi(angle) + offset;
		cv::circle(img, errorPoint, 2, cv::Scalar(0,255,255), -1);

		points.push_back(errorPoint);

	}


	cv::imshow("points", img);
	cv::waitKey(0);

	double p = 0.99;
	int k = 6;
	double relativeError = sizeError / static_cast<double>(sizeEllipse + sizeError);

	int minN = static_cast<int>(std::ceil(std::log(1.0 - p) / std::log(1.0 - std::pow(1.0 - relativeError, k))));

	std::cout << "relative error: " << relativeError << " with p = " << p << std::endl;
	std::cout << "min its: " << minN << std::endl;

	cv::imwrite("ransac50_0.png", img);


	Ellipse fittedEllipse = Ellipse::robustEllipseFit(points, img.size(), 7.0f, 2.0f, minN);
	cv::ellipse(img, fittedEllipse.getEllipseAsRotatedRect(), cv::Scalar(0,255,0), 2);

	cv::RotatedRect leastSquareEllipse = cv::fitEllipse(points);
	cv::ellipse(img, leastSquareEllipse, cv::Scalar(255, 255, 0), 2);

	cv::imshow("fitted", img);
	cv::imwrite("ransac50_1.png", img);
	cv::waitKey(0);


	return 0;
}

