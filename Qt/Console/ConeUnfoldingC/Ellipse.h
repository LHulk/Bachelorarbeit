#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include <cmath>
#include <vector>
#include <random>
#include <ctime>

#include "Line.h"
#include "Config.h"


class Ellipse
{
public:
    //constructors
    Ellipse();
    Ellipse(double x0, double y0, double a, double b, double theta);
    Ellipse(cv::Point2d center, cv::Size2d semiAxes, double theta);
    Ellipse(cv::Mat params);
    Ellipse(cv::RotatedRect rect);
    //Ellipse(const Ellipse& other);
    //Ellipse& operator=(const Ellipse& other);


    cv::Point2d evalAtPhi(double phi) const;
    cv::RotatedRect getEllipseAsRotatedRect() const;

	Ellipse operator*(double scalar);
	Ellipse operator+(Ellipse& other);
	friend Ellipse operator*(double scalar, Ellipse& ellipse);

    static std::vector<Ellipse> detectEllipses(const cv::Mat& edgeImage);
	static double realDistTrans(const Ellipse& ellipse, const cv::Point& pt);
	static double realDistTrans(const Ellipse& ellipse, const cv::Point& pt, cv::Point& intersect);
    static std::vector<std::vector<cv::Point2f>> getEllipsePointMappings(const std::vector<Ellipse>& ellipses, const std::vector<cv::Point2f>& points);
	static std::vector<Ellipse> Ellipse::reestimateEllipses(const std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<Ellipse>& ellipses);
	static cv::Point2d ellipseLineIntersection(const Ellipse& ellipse, const Line& line);
	static double getAngleAt(const Ellipse& ellipse, const cv::Point2d &pt);


private:
    double _x0; //center x
    double _y0; //center y
	double _a; //semi major axis
	double _b; //semi minor axis
	double _theta; //orientation

public:
    //trivial getters
    cv::Point2d getCenter() const;
    double getX() const;
    double getY() const;
    double getSemiMajor() const;
    double getSemiMinor() const;
    double getMajor() const;
    double getMinor() const;
    cv::Size2d getAxes() const;
    cv::Size2d getSemiAxes() const;
    double getTheta() const;

private:
    static std::vector<cv::Point> rayCast(const cv::Mat& edgeImage, cv::Point caster, int numSamples);
    static Ellipse robustEllipseFit(const std::vector<cv::Point> &points, cv::Size szImg, double dist, float maxAtoBRatio, size_t numSteps);
    static Ellipse solveEllipseEquation(const std::vector<cv::Point> &points);
    static bool checkBoundaries(const Ellipse& e, const cv::Size& sz, float maxAToBRatio);

    static double realDist(const Ellipse& ellipse, const cv::Point2d& pt, cv::Point2d& intersect);
    static double findRoot(double r0, double z0, double z1, double g);

	static cv::Point2d transformToOrigin(const Ellipse& ellipse, const cv::Point2d& point);
	static cv::Point2d invTransformToOrigin(const Ellipse& ellipse, const cv::Point2d& point);
    static cv::Point2d rotatePoint(const cv::Point2d pt, double angle);
};


#endif // ELLIPSE_H
