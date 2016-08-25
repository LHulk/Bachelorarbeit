#include "Transformation.h"


static bool isDebug = true;

Transformation::Transformation()
{
}

//TODO: maybe ransac?
//TODO user solvePnP or solvePnPRansac after determining intrinsic paramters through regular camera calibration (calibrateCamera)
//minimize reprojection error
cv::Mat Transformation::getProjectiveMatrixBrute(const std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<std::vector<cv::Point3f>>& worldCoords)
{
	int n = Config::numCircleSamples;
	int m = Config::numLineSamples;
	int l = Config::numCircleSamples * Config::numLineSamples; //number of total samples

	/*std::vector<cv::Point2f> allPoints;
	for(const auto& ptList : pointsPerEllipse)
		for(const auto& pt : ptList)
			allPoints.push_back(pt);

	//6 points define projection matrix
	std::vector<cv::Point2f> combination;
	std::vector<std::vector<cv::Point2f>> combinations;

	permute(0, 2, allPoints.size(), combination, allPoints, combinations);


	for(const auto& comb : combinations)
	{
		cv::Mat A = cv::Mat::zeros(12, 11, CV_32F);
		cv::Mat D = cv::Mat::zeros(12, 1, CV_32F);
		int current = 0;
		for(const auto& pt : comb)
		{
			//fill corresponding matrix
			cv::Point2f currentImg = pt;
			
			size_t k, l;
			for(size_t i = 0; i < pointsPerEllipse.size(); i++)
			{
				for(size_t j = 0; j < pointsPerEllipse[i].size(); j++)
				{
					if(pointsPerEllipse[i][j] == currentImg)
					{
						k = i; l = j;
						break;
					}
				}
			}

			cv::Point3f currentWorld = worldCoords[k][l];

			A.at<float>(current, 0) = currentWorld.x; A.at<float>(current, 1) = currentWorld.y; A.at<float>(current, 2) = currentWorld.z;
			A.at<float>(current, 3) = 1.0f;
			A.at<float>(current, 8) = -currentImg.x * currentWorld.x; A.at<float>(current, 9) = -currentImg.x * currentWorld.y; A.at<float>(current, 10) = -currentImg.x * currentWorld.z;

			A.at<float>(current + 1, 4) = currentWorld.x; A.at<float>(current + 1, 5) = currentWorld.y; A.at<float>(current + 1, 6) = currentWorld.z;
			A.at<float>(current + 1, 7) = 1.0f;
			A.at<float>(current, 8) = -currentImg.y * currentWorld.x; A.at<float>(current, 9) = -currentImg.y * currentWorld.y; A.at<float>(current, 10) = -currentImg.y * currentWorld.z;

			D.at<float>(current, 0) = currentImg.x;
			D.at<float>(current + 1, 0) = currentImg.y;

			current += 2;

		}

		cv::Mat proj;
		cv::solve(A, D, proj, cv::DECOMP_SVD);
		cv::Mat row = cv::Mat::ones(1, 1, CV_32F);
		proj.push_back(row);
		proj = proj.reshape(0, 3);
	}*/

	std::vector<std::vector<cv::Vec3f>> objectPoints = std::vector<std::vector<cv::Vec3f>>(1, std::vector<cv::Vec3f>());
	for(const auto& ptList : worldCoords)
	{
		for(const auto& pt : ptList)
		{
			objectPoints[0].push_back(cv::Vec3f(pt.x, pt.y, pt.z));
			break;
		}
	}

	std::vector<std::vector<cv::Vec2f>> imagePoints = std::vector<std::vector<cv::Vec2f>>(1, std::vector<cv::Vec2f>());
	for(const auto& ptList : pointsPerEllipse)
	{
		for(const auto& pt : ptList)
		{
			imagePoints[0].push_back(cv::Vec2f(pt.x, pt.y));
			break;
		}

	}


	cv::Size imageSize(1000, 1000);
	cv::Mat cameraMatrix;//= cv::Mat::zeros(3, 3, CV_32F);
	std::vector<cv::Mat> rvecs, tvecs;
	cv::Mat distCoeffs;// = cv::Mat::zeros(8, 1, CV_32F);

	cv::calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);


	return cv::Mat();
}


//TODO: 30 points 
cv::Mat Transformation::getProjectiveMatrix(const std::vector<std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<std::vector<cv::Point3f>>& worldCoords)
{
	int m = Config::numLineSamples;
	int l = Config::numCircleSamples * Config::numLineSamples;
	cv::Mat A = cv::Mat::zeros(2 * l, 11, CV_32F);
	cv::Mat D = cv::Mat::zeros(2 * l, 1, CV_32F);
	for(int i = 0; i < 2*l; i+=2)
	{
		cv::Point2f currentImg = pointsPerEllipse[(i / 2) / m][(i / 2) % m];
		cv::Point3f currentWorld = worldCoords[(i / 2) / m][(i / 2) % m];

		A.at<float>(i, 0) = currentWorld.x; A.at<float>(i, 1) = currentWorld.y; A.at<float>(i, 2) = currentWorld.z;
		A.at<float>(i, 3) = 1.0f;
		A.at<float>(i, 8) = -currentImg.x * currentWorld.x; A.at<float>(i, 9) = -currentImg.x * currentWorld.y; A.at<float>(i, 10) = -currentImg.x * currentWorld.z;

		A.at<float>(i + 1, 4) = currentWorld.x; A.at<float>(i + 1, 5) = currentWorld.y; A.at<float>(i + 1, 6) = currentWorld.z;
		A.at<float>(i + 1, 7) = 1.0f;
		A.at<float>(i + 1, 8) = -currentImg.y * currentWorld.x; A.at<float>(i + 1, 9) = -currentImg.y * currentWorld.y; A.at<float>(i + 1, 10) = -currentImg.y * currentWorld.z;

		D.at<float>(i, 0) = currentImg.x;
		D.at<float>(i + 1, 0) = currentImg.y;
	}

	cv::Mat proj;
	cv::solve(A, D, proj, cv::DECOMP_SVD);
	cv::Mat row = cv::Mat::ones(1, 1, CV_32F);
	proj.push_back(row);
	proj = proj.reshape(0, 3);

	return proj;
}


void Transformation::reverseWarp(const cv::Mat& greyImg, const cv::Mat& proj)
{
	double H = Config::height;
	double R = Config::radiusOuter;
	double r = Config::radiusInner;
	double h = H - H / R*r;

	double S = std::sqrt(H*H + R*R);
	double s = std::sqrt((H*H*r*r / (R*R)) + r*r);
	double maxAngle = 360 * R / S;

	double scale = (1 / S) * Config::resSlantHeight;
	S *= scale;
	s *= scale;

	int width = static_cast<int>(std::ceil(S));
	int height = static_cast<int>(std::ceil(S + S*std::cos((180 - maxAngle)*CV_PI/180)));

	cv::Point origin = cv::Point(width, Misc::round(std::ceil(height - S)));

	cv::Mat mask = cv::Mat::zeros(height, width, CV_8U);
	cv::Mat resImg = cv::Mat::zeros(height, width, CV_8U);
	cv::Mat mapx = cv::Mat::zeros(height, width, CV_32F);
	cv::Mat mapy = cv::Mat::zeros(height, width, CV_32F);

	cv::ellipse(mask, origin, cv::Size2d(S, S), 0, 90, Misc::round(90 + maxAngle), cv::Scalar(255), 3);
	cv::ellipse(mask, origin, cv::Size2d(s, s), 0, 90, 90 + maxAngle, cv::Scalar(255), 3);
	cv::line(mask, cv::Point(origin.x, Misc::round(origin.y + s)), cv::Point(origin.x, Misc::round(origin.y + S)), cv::Scalar(255), 3);
	double angleStd = (90 + maxAngle) - 0.5; //0.5 for rounding errors
	cv::line(mask, cv::Point2f(origin.x, origin.y) + s*cv::Point2f(std::cos(angleStd * CV_PI / 180), std::sin(angleStd * CV_PI / 180)), cv::Point2f(origin) + S*cv::Point2f(std::cos(angleStd * CV_PI / 180), std::sin(angleStd * CV_PI / 180)), cv::Scalar(255), 3);
	cv::floodFill(mask, cv::Point2f(origin.x, origin.y) + S / 2 * cv::Point2f(-1, -1), cv::Scalar(255), nullptr, cv::Scalar(10), cv::Scalar(10));


	std::vector<cv::Point3f> test;
	std::vector<cv::Point2f> test2;
	for(int r = 0; r < height; r++)
	{
		for(int c = 0; c < width; c++)
		{
			if(mask.at<uchar>(r, c) != 0)
			{
				//test2.push_back(cv::Point2d(c, r) - cv::Point2d(origin));
				cv::Point3f coneCoords = lateralToConeCoordinates(1/scale*cv::Point2d((cv::Point2d(c, r) - cv::Point2d(origin))));
				//test.push_back(coneCoords);
				cv::Mat coneCoordsMat = cv::Mat(coneCoords);
				cv::Mat homogenous = cv::Mat::ones(1, 1, CV_32F);
				coneCoordsMat.push_back(homogenous);

				cv::Mat homImage = proj * coneCoordsMat;

				float w = homImage.at<float>(2, 0);
				cv::Point2f image = cv::Point2f(homImage.at<float>(0, 0) / w, homImage.at<float>(1, 0) / w);
				mapx.at<float>(r, c) = image.x;
				mapy.at<float>(r, c) = image.y;
			}
		}
	}

	//writeToFile(test, "reverseCone.txt");
	//writeToFile(test2, "reverseLateral.txt");
	//exit(0);

	cv::remap(greyImg, resImg, mapx, mapy, cv::INTER_CUBIC);
	cv::imshow("res", resImg);

}

void Transformation::writeToFile(std::vector<cv::Point2f> point2f, const std::string& filename)
{
	std::string x = ""; std::string y = "";
	for(const cv::Point2f& pt : point2f)
	{
		x += std::to_string(pt.x) + " ";
		y += std::to_string(pt.y) + " ";
	}

	std::ofstream file;
	file.open(filename);
	file << x << "\n";
	file << y << "\n";
	file.close();
}
void Transformation::writeToFile(std::vector<cv::Point3f> point3f, const std::string& filename)
{
	std::string x = ""; std::string y = ""; std::string z = "";
	for(const cv::Point3f& pt : point3f)
	{
		x += std::to_string(pt.x) + " ";
		y += std::to_string(pt.y) + " ";
		z += std::to_string(pt.z) + " ";
	}

	std::ofstream file;
	file.open(filename);
	file << x << "\n";
	file << y << "\n";
	file << z << "\n";
	file.close();
}

//center is (0,0)
//right hand coordinate system (x,y,z) z is depth directed out of screen plane
std::vector<std::vector<cv::Point3f>> Transformation::getWorldCoordinatesForSamples()
{
	int n = Config::numCircleSamples;
	int m = Config::numLineSamples;
	float H = Config::height;
	float r = Config::radiusInner;
	float R = Config::radiusOuter;
	float h = H - H / R*r;

	std::vector<std::vector<cv::Point3f>> worldCoordinates = std::vector<std::vector<cv::Point3f>>(n, std::vector<cv::Point3f>(0));

	//iterate through ellipses
	for(int i = 0; i < n; i++)
	{
		float y = static_cast<float>(i) / (n - 1) * h;
		float currR = r + (y / h) * (R - r);

		//iterate through points on ith ellipse
		for(int j = 0; j < m; j++)
		{
			float angle = j * 2*static_cast<float>(CV_PI) / (m);
			float x = currR * std::cos(angle);
			float z = currR * std::sin(angle);
			
			worldCoordinates[i].push_back(cv::Point3f(x, y, z));
		}
	}


	return worldCoordinates;

}

static void draw_subdiv_point(cv::Mat& img, cv::Point2f fp, cv::Scalar color)
{
	circle(img, fp, 3, color, CV_FILLED, 8, 0);
}

static void draw_subdiv(cv::Mat& img, cv::Subdiv2D& subdiv, cv::Scalar delaunay_color)
{
	std::vector<cv::Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	std::vector<cv::Point> pt(3);

	for(size_t i = 0; i < triangleList.size(); i++)
	{
		cv::Vec6f t = triangleList[i];
		pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
		pt[1] = cv::Point(cvRound(t[2]), cvRound(t[3]));
		pt[2] = cv::Point(cvRound(t[4]), cvRound(t[5]));
		cv::line(img, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
		cv::line(img, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
		cv::line(img, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);
	}
}

static void locate_point(cv::Mat& img, cv::Subdiv2D& subdiv, cv::Point2f fp, cv::Scalar active_color)
{
	int e0 = 0, vertex = 0;

	subdiv.locate(fp, e0, vertex);

	if(e0 > 0)
	{
		int e = e0;
		do
		{
			cv::Point2f org, dst;
			if(subdiv.edgeOrg(e, &org) > 0 && subdiv.edgeDst(e, &dst) > 0)
				line(img, org, dst, active_color, 3, CV_AA, 0);

			e = subdiv.getEdge(e, cv::Subdiv2D::NEXT_AROUND_LEFT);
		} while(e != e0);
	}

	draw_subdiv_point(img, fp, active_color);
}



//TODO: border interpolation
void Transformation::getWorldCoordinatesInt(const std::vector<Ellipse>& ellipses, const std::vector<Line>& lines, const std::vector <std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<std::vector<cv::Point3f>>& worldCoords, const cv::Mat& img)
{
	cv::Mat imgBk = img.clone();
	cv::Mat segments;
	fillSegments(segments, ellipses, lines, pointsPerEllipse);

	cv::Mat mask = cv::Mat::zeros(Config::usedResHeight, Config::usedResWidth, CV_8U);
	cv::ellipse(mask, ellipses[Config::numCircleSamples - 1].getEllipseAsRotatedRect(), cv::Scalar(255), -1);

	cv::bitwise_and(imgBk, mask, imgBk);
	cv::imshow("img mask", imgBk);

	double H = Config::height;
	double R = Config::radiusOuter;
	double r = Config::radiusInner;
	double h = H - H / R*r;

	double S = std::sqrt(H*H + R*R);
	double s = std::sqrt((H*H*r*r / (R*R)) + r*r);
	double maxAngle = 360 * R / S;

	double scale = (1 / S) * Config::resSlantHeight;
	S *= scale;
	s *= scale;

	int width = static_cast<int>(std::ceil(S));
	int height = static_cast<int>(std::ceil(S + S*std::cos((180 - maxAngle)*CV_PI / 180)));

	cv::Point origin = cv::Point(width, Misc::round(std::ceil(height - S)));


	cv::Mat resImg = cv::Mat::zeros(height, width, CV_8U);

	//cv::Subdiv2D subdiv(cv::Rect(0, 0, 1000, 1000));

	std::vector<cv::Point2f> lateralDebug;
	std::vector<cv::Point3f> coneDebug;
	for(int r = 0; r < segments.rows; r+=1)
	{
		for(int c = 0; c < segments.cols; c+=1)
		{
			int val = segments.at<uchar>(r, c);
			if(val != 0)
			{
				cv::Point pt(c, r);

				cv::Point3f res = interPolateRadial(pt, val, ellipses, lines, pointsPerEllipse, worldCoords);
				//coneDebug.push_back(res);

				cv::Point2d lateral = scale * coneCoordinatesToLateral(res);
				//lateralDebug.push_back(lateral);
				cv::Point rounded = cv::Point(static_cast<int>(std::lround(lateral.x)), static_cast<int>(std::lround(lateral.y))) + origin;
				if(rounded.x > 0 && rounded.y > 0 && rounded.x < resImg.cols && rounded.y < resImg.rows)
				{
					
					resImg.at<uchar>(rounded) = imgBk.at<uchar>(pt);
					//locate_point(resImg, subdiv, lateral, cv::Scalar(0,0,255));
					//subdiv.insert(cv::Point2f(lateral));
					//resImg = cv::Scalar::all(0);
					//draw_subdiv(resImg, subdiv, cv::Scalar(255, 255, 255));
					

				}
				else //do nothing --> identiy
				{

				}


			}
		}
	}

	/*for(const auto& pt : lateralDebug)
	{
		subdiv.insert(cv::Point2f(pt.x, pt.y));
	}*/

	cv::imshow("resImg", resImg);



}

cv::Point3f Transformation::interPolateRadial(const cv::Point& pt, int val, const std::vector<Ellipse>& ellipses, const std::vector<Line>& lines, const std::vector <std::vector<cv::Point2f>>& pointsPerEllipse, const std::vector<std::vector<cv::Point3f>>& worldCoords)
{
	val = (val - 2) / 2;
	int i = val / Config::numLineSamples;
	int j = val % Config::numLineSamples;

	Ellipse e1 = ellipses[i];
	double dist1 = Ellipse::realDistTrans(e1, pt);
	Ellipse e2 = ellipses[i + 1];
	double dist2 = Ellipse::realDistTrans(e2, pt);
	Ellipse interEllipse = (dist1 / (dist1 + dist2) * e2) + (dist2 / (dist1 + dist2) * e1);

	cv::Point2d int1 = Ellipse::ellipseLineIntersection(interEllipse, lines[j]);
	cv::Point2d int2 = Ellipse::ellipseLineIntersection(interEllipse, lines[(j + 1) % Config::numLineSamples]);


	cv::Point2d tl = pointsPerEllipse[i + 1][j];
	cv::Point2d bl = pointsPerEllipse[i][j];
	cv::Point2d tr = pointsPerEllipse[i + 1][(j + 1) % Config::numLineSamples];
	cv::Point2d br = pointsPerEllipse[i][(j + 1) % Config::numLineSamples];

	double intD1 = (tl.y - bl.y != 0) ? (int1.y - bl.y) / (tl.y - bl.y) : (int1.x - bl.x) / (tl.x - bl.x);
	double intD2 = (tr.y - br.y != 0) ? (int2.y - br.y) / (tr.y - br.y) : (int2.x - br.x) / (tr.x - br.x);

	cv::Point3f temp1, temp2;
	temp1 = intD1*worldCoords[i + 1][j] + (1 - intD1)*worldCoords[i][j];
	temp2 = intD2*worldCoords[i + 1][(j + 1) % Config::numLineSamples] + (1 - intD2)*worldCoords[i][(j + 1) % Config::numLineSamples];

	cv::Point3f res;

	double angleInt = std::fmod(std::abs(Ellipse::getAngleAt(interEllipse, int1) - Ellipse::getAngleAt(interEllipse, int2)), 2 * CV_PI);
	angleInt = (angleInt > CV_PI) ? 2 * CV_PI - angleInt : angleInt;
	double angle2Int = std::fmod(std::abs(Ellipse::getAngleAt(interEllipse, int1) - Ellipse::getAngleAt(interEllipse, pt)), 2 * CV_PI);
	angle2Int = (angle2Int > CV_PI) ? 2 * CV_PI - angle2Int : angle2Int;
	angleInt = angle2Int / angleInt;

	res = angleInt * temp2 + (1 - angleInt) * temp1;

	return res;
}

cv::Point2d Transformation::coneCoordinatesToLateral(const cv::Point3d& pt)
{
	cv::Point2d lateralPoint;

	double H = Config::height;
	double R = Config::radiusOuter;
	double r = Config::radiusInner;
	double h = H - H / R*r;

	double S = std::sqrt(H*H + R*R);
	double s = std::sqrt((H*H*r*r / (R*R)) + r*r);

	double ri = s + (pt.y/h) * (S - s);

	//clamp
	ri = (ri > S) ? S : (ri < s) ? s : ri;

	double cosP = pt.x / (r + (pt.y / h) * (R - r));
	double sinP = pt.z / (r + (pt.y / h) * (R - r));

	double angle = std::atan2(sinP, cosP);
	angle = std::fmod(std::fmod(angle, 2 * CV_PI) + 2*CV_PI, 2*CV_PI);

	angle = angle * (R / S);

	lateralPoint.x = -ri * std::sin(angle);
	lateralPoint.y = ri * std::cos(angle);

	return lateralPoint;
}

cv::Point3d Transformation::lateralToConeCoordinates(const cv::Point2d& pt)
{
	cv::Point3d conePoint;

	double H = Config::height;
	double R = Config::radiusOuter;
	double r = Config::radiusInner;
	double h = H - H / R*r;

	double S = std::sqrt(H*H + R*R);
	double s = std::sqrt((H*H*r*r / (R*R)) + r*r);

	double v = std::sqrt(pt.x*pt.x + pt.y*pt.y);
	double sinP = -pt.x / v;
	double cosP = pt.y / v;
	

	double angle = std::atan2(sinP, cosP);
	angle = std::fmod(std::fmod(angle, 2 * CV_PI) + 2 * CV_PI, 2 * CV_PI);

	angle = angle * (S / R);

	conePoint.y = (v - s) / (S - s) * h;

	//clamp
	conePoint.y = (conePoint.y > h) ? h : (conePoint.y < 0) ? 0 : conePoint.y;

	double ri = r + (conePoint.y / h) * (R - r);

	//clamp
	ri = (ri > R) ? R : (ri < r) ? r : ri;


	conePoint.x = ri * std::cos(angle);
	conePoint.z = ri * std::sin(angle);

	return conePoint;
}


//ONLY WORKS FOR n*m < 127
//TODO BETTER SEED
void Transformation::fillSegments(cv::Mat& img, const std::vector<Ellipse>& ellipses, const std::vector<Line>& lines, const std::vector<std::vector<cv::Point2f>>& pointsPerEllipse)
{
	img = cv::Mat::zeros(Config::usedResHeight, Config::usedResWidth, CV_8U);

	//draw lines and ellipses with width of 1
	for(const Ellipse& e : ellipses)
		cv::ellipse(img, e.getEllipseAsRotatedRect(), cv::Scalar(255), 1);
	for(const Line& l : lines)
		cv::line(img, l.getStart(), l.getEnd(), cv::Scalar(255), 1);


	int m = Config::numLineSamples;
	int currentVal = 2;
	for(size_t i = 0; i < Config::numCircleSamples - 1; i++)
	{
		for(size_t j = 0; j < Config::numLineSamples; j++)
		{
			cv::Point2f p1 = pointsPerEllipse[i][j]; cv::Point2f p2 = pointsPerEllipse[i + 1][j]; cv::Point2f p3 = pointsPerEllipse[i + 1][(j + 1) % m];
			cv::Point2f middleLine = (p1 + 0.5*(p2 - p1));
			cv::Point seed = middleLine + 0.5*(p3 - middleLine);
			cv::floodFill(img, seed, cv::Scalar(currentVal), nullptr, cv::Scalar(10), cv::Scalar(10));
			currentVal += 2;
		}
	}

	//'delete' lines and ellipses with width of 1
	for(const Ellipse& e : ellipses)
		cv::ellipse(img, e.getEllipseAsRotatedRect(), cv::Scalar(0), 1);
	for(const Line& l : lines)
		cv::line(img, l.getStart(), l.getEnd(), cv::Scalar(0), 1);


	if(isDebug)
		cv::imshow("floodfill segments", img);

}
