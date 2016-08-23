//cv::circle(vis, Point(vis.cols/2, vis.rows/2), std::min(vis.cols, vis.rows)/2, cv::Scalar(0,0,255), 2);

	//cv::imshow("test", vis);

	/*std::vector<Ellipse> ellipses;

	Algortihms::HoughEllipses(edge, 10, 5, 5, ellipses);


	cv::cvtColor(edge, vis, CV_GRAY2BGR);

	for(const Ellipse& e : ellipses)
	{
		cv::ellipse(vis, e.getCenter(), e.getSemiAxes(), e.getTheta()*180/CV_PI, 0, 360, cv::Scalar(0, 0, 255), 2);
	}

	cv::imshow("ellipses", vis);*/




	//cv::imshow("grey", grey);




	//harris
	/*Mat harris;
    int harris_block = 5, harris_aperture = 5;
    double harris_k = 0.004;
    int harris_thresh = 30;

	cv::cornerHarris(grey, harris, harris_block, harris_aperture, harris_k);
	//cv::cornerSubPix(grey, harris, Size(3,3), cv::Size(-1, -1), cv::TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001 )); //40 its or eps <= 0.001

    Mat harris_norm, harris_norm_scaled;
	cv::normalize( harris, harris_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, Mat() );
	cv::convertScaleAbs( harris_norm, harris_norm_scaled );


    for( int j = 0; j < harris_norm.rows ; j++ )
    {
        for( int i = 0; i < harris_norm.cols; i++ )
        {
            if( (int) harris_norm.at<float>(j,i) > harris_thresh )
            {
				cv::circle( grey, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
            }
        }
     }
	cv::imshow("harris", harris_norm_scaled);
	cv::imshow("grey harris", grey);*/




    /*cv::SimpleBlobDetector::Params params;

    // Change thresholds
    params.minThreshold = 10;
    //params.maxThreshold = 200;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 10;

    // Filter by Circularity
    params.filterByCircularity = true;
    params.minCircularity = 0.0;

    // Filter by Convexity
    params.filterByConvexity = false;
    params.minConvexity = 1;

    // Filter by Inertia
    params.filterByInertia = false;
    params.minInertiaRatio = 0;


    cv::SimpleBlobDetector detector(params);
    std::vector<cv::KeyPoint> keypoints;
    detector.detect( grey, keypoints);

    Mat im_with_keypoints;
    cv::drawKeypoints( sobelEdgeVis, keypoints, im_with_keypoints, Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    cv::imshow("keypoints", im_with_keypoints);*/



   std::vector<cv::Vec3f> circles;

    /// Apply the Hough Transform to find the circles
    cv::HoughCircles( grey, circles, CV_HOUGH_GRADIENT, 1, 1, 10, 50, 0, 200);

    /// Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      cv::circle( sobelEdgeVis, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      cv::circle( sobelEdgeVis, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }

    cv::imshow("hough circle", sobelEdgeVis);



    //prob hough
//std::vector<cv::Vec4i> lines;
//cv::HoughLinesP( sobelEdge, lines, 1, CV_PI/180, 100, 30, 50);
//cv::HoughLinesP( sobelEdge, lines, 1, CV_PI/180, 100, 30, 1000);


/*std::sort(lines.begin(), lines.end(), [](const cv::Vec4i& line1, const cv::Vec4i& line2) {
    Point p1 = Point(line1[0], line1[1]);
    Point p2 = Point(line1[2], line1[3]);
    Point q1 = Point(line2[0], line2[1]);
    Point q2 = Point(line2[2], line2[3]);
    return (cv::norm(p2-p1) > cv::norm(q2-q1));
});*/

/*for( size_t i = 0; i < lines.size(); i++ )
{
    cv::line( sobelEdgeVis, Point(lines[i][0], lines[i][1]),
        Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
}*/


void ConeUnfolding::getGreenRedHue(const Mat& colorImg, Mat& redHueImage, Mat& greenHueImage)
{
	Mat hsvImage;
	cv::cvtColor(colorImg, hsvImage, cv::COLOR_BGR2HSV);

	Mat lowerRedHueImage;
	Mat upperReadHueImage;
    cv::inRange(hsvImage, Scalar(0, 100, 100), Scalar(10, 255, 255), lowerRedHueImage);
    cv::inRange(hsvImage, Scalar(160, 100, 100), Scalar(179, 255, 255), upperReadHueImage);
    cv::inRange(hsvImage, Scalar(40, 100, 100), Scalar(80, 255, 255), greenHueImage);

	//add lower and upper red hue image to one
	cv::addWeighted(lowerRedHueImage, 1.0, upperReadHueImage, 1.0, 0.0, redHueImage);

	//blur both
    cv::GaussianBlur(redHueImage, redHueImage, Size(5, 5), 2, 2);
    cv::GaussianBlur(greenHueImage, greenHueImage, Size(5, 5), 2, 2);

	//morphing to close small holes
	Mat element = cv::getStructuringElement(2, Size(9,9));
	cv::morphologyEx(redHueImage, redHueImage, 3, element);
	cv::morphologyEx(greenHueImage, greenHueImage, 3, element);
}


cv::RotatedRect ConeUnfolding::getEllipse(const Mat& hueImage, bool max)
{
	std::vector<std::vector<Point> > contours;
	cv::findContours(hueImage.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //findContours modifies image

	int index = -1;
	double area = (max) ? 0 : hueImage.cols * hueImage.rows;

	for( size_t i = 0; i < contours.size(); i++ )
	{
		//if im looking for a max and current area is bigger or if im looking for min an current area is smaller
		if((max && cv::contourArea(contours[i]) > area) || (!max && cv::contourArea(contours[i]) < area))
		{
			index = i;
			area = cv::contourArea(contours[i]);
		}
	}

	return cv::fitEllipse(contours[index]);

}
