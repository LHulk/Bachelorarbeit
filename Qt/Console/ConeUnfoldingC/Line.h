#ifndef LINE_H
#define LINE_H


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cmath>

#include "Config.h"

class Line
{
public:
    Line(float rho, float theta);
	Line(cv::Point start, cv::Point end);
    bool intersect(const Line& other, cv::Point& point) const;
	static bool intersect(const Line& l1, const Line& l2, cv::Point& point);
    cv::Point getStart() const;
    cv::Point getEnd() const;
    cv::Point getDir() const;
	cv::Point2d getNormalizedDir() const;
	Line expand() const;

    static std::vector<Line> getLines(const cv::Mat& edgeImage);
    static cv::Point getWeightedCenter(const std::vector<Line>& lines);

	static std::vector<Line> fitLines(const std::vector<std::vector<cv::Point2f>>& pointsPerEllipse);

    bool operator==(const Line& other) const;
    bool operator!=(const Line& other) const;
private:
    bool _isUnbounded;
    cv::Point _start;
    cv::Point _end;
};

#endif // LINE_H
