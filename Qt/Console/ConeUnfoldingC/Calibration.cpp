#include "Calibration.h"


//detect corner on image using findChessboardConers and use conerSubPix to improve accuracy
std::vector<cv::Point2f> Calibration::getCorners(const cv::Mat& greyImg, cv::Mat& imgWithCorners)
{
    cv::Size patternSize(Config::patternPointsPerRow, Config::patternPointsPerCol);
    std::vector<cv::Point2f> centers;

    bool patternFound = cv::findChessboardCorners(greyImg, patternSize, centers);

	cv::cornerSubPix(greyImg, centers, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1 ));

    cv::cvtColor(greyImg, imgWithCorners, CV_GRAY2BGR);
    cv::drawChessboardCorners(imgWithCorners, patternSize, cv::Mat(centers), patternFound);



    if(patternFound)
        return centers;
    else
        return std::vector<cv::Point2f>();
}

//sets 3D points for chessboard with z = 0, where top left chessboard rectangle is at (0,0,0)
std::vector<cv::Point3f> Calibration::getObjectPoints()
{
    std::vector<cv::Point3f> objectPoints;
    for( int i = 0; i < Config::patternPointsPerCol; i++ )
        for( int j = 0; j < Config::patternPointsPerRow; j++ )
            objectPoints.push_back(cv::Point3f(static_cast<float>( j * Config::patternSquareSize ), static_cast<float>( i * Config::patternSquareSize ), 0));

    return objectPoints;
}

//performs real camera calibration, discarding extrinsic parameters
void Calibration::getIntrinsicParamters(const std::vector<std::vector<cv::Point2f>>& imagePoints, const std::vector<std::vector<cv::Point3f>>& objectPoints,
                                  cv::Mat& cameraMatrix, cv::Mat& distCoeffs, cv::Size calibImgSize)
{
    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;
    cv::calibrateCamera(objectPoints, imagePoints, calibImgSize, cameraMatrix, distCoeffs, rvecs, tvecs);
}


