#include "Transformation.h"


static bool isDebug = false;


cv::Mat Transformation::getProjectiveMatrix(const Cone& cone)
{
	int m = Config::numLineSamples;
	int l = Config::numCircleSamples * Config::numLineSamples;

	auto sampleImg = cone.sampleCoordsImage();
	auto sampelWorld = cone.sampleCoordsWorld();

	cv::Mat A = cv::Mat::zeros(2 * l, 11, CV_32F);
	cv::Mat D = cv::Mat::zeros(2 * l, 1, CV_32F);
	for(int i = 0; i < 2*l; i+=2)
	{
		cv::Point2f currentImg = sampleImg[(i / 2) / m][(i / 2) % m];
		cv::Point3f currentWorld = sampelWorld[(i / 2) / m][(i / 2) % m];

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
void Transformation::getForwardWarpMaps(const Cone& cone, cv::Mat& remapX, cv::Mat& remapY, cv::Mat img)
{
	std::vector<Ellipse> ellipses = cone.ellipses();
	cv::Mat segments;
	fillSegments(segments, ellipses, cone.lines(), cone.sampleCoordsImage());

	float bk = Config::scaleFactor;
	Config::scaleFactor = 1.0f;
	cv::Mat segments2;
	fillSegments(segments2, ellipses, cone.lines(), cone.sampleCoordsImage());
	Config::scaleFactor = bk;

	double S = cone.S();
	double s = cone.s();
	double maxAngle = Misc::radToDeg(cone.maxAngle());

	double scaleSlant = (1 / S) * Config::resSlantHeight;
	S *= scaleSlant;
	s *= scaleSlant;

	int width = static_cast<int>(std::ceil(S));
	int height = static_cast<int>(std::ceil(S + S*std::cos(Misc::degToRad(180 - maxAngle))));

	//cv::Mat debug = cv::Mat::zeros(height, width, CV_8U);

	cv::Point origin = cv::Point(width, Misc::round(std::ceil(height - S)));

	//cv::Subdiv2D subdiv(cv::Rect(0, 0, 1000, 1000));
	float scaleImg = 1/Config::scaleFactor;

	cv::Mat mapx = cv::Mat::zeros(static_cast<int>(scaleImg * Config::usedResHeight), static_cast<int>(scaleImg * Config::usedResWidth), CV_32F);
	cv::Mat mapy = cv::Mat::zeros(static_cast<int>(scaleImg * Config::usedResHeight), static_cast<int>(scaleImg * Config::usedResWidth), CV_32F);
	mapx.at<float>(0, 0) = static_cast<float>(width);
	mapy.at<float>(0, 0) = static_cast<float>(height);

	cv::Mat calculated = cv::Mat::zeros(1 / scaleImg * segments.rows, 1 / scaleImg * segments.cols, CV_64FC2);
	cv::Mat calculatedMask = cv::Mat::zeros(1 / scaleImg * segments.rows, 1 / scaleImg * segments.cols, CV_8U);

	//std::vector<cv::Point2f> lateralDebug;
	//std::vector<cv::Point3f> coneDebug;
	for(int r = 0; r < segments.rows; r += 1)
	{
		for(int c = 0; c < segments.cols; c += 1)
		{
			int val = segments.at<uchar>(r, c);
			if(val != 0)
			{
				cv::Point pt = 1 / scaleImg * cv::Point(c, r);
				cv::Point2d lateral;
				if(calculatedMask.at<uchar>(pt) != 0)
				{
					cv::Vec2d val = calculated.at<cv::Vec2d>(pt);
					lateral.x = val(0);
					lateral.y = val(1);
				}
				else
				{
					cv::Point3f res = cone.interPolateRadial(pt, val);
					//coneDebug.push_back(res);

					lateral = scaleSlant * cone.coneCoordinatesToLateral(res);
					lateral = lateral + cv::Point2d(origin.x, origin.y);
					//lateralDebug.push_back(lateral);

					calculatedMask.at<uchar>(pt) = 255;
					calculated.at<cv::Vec2d>(pt) = cv::Vec2d(lateral.x, lateral.y);
				}		

				if(lateral.x > 0 && lateral.y > 0 && lateral.x < width && lateral.y < height)
				{
					mapx.at<float>(r, c) = static_cast<float>(lateral.x);
					mapy.at<float>(r, c) = static_cast<float>(lateral.y);
					//uchar val = img.at<uchar>(r,c);
					//debug.at<uchar>(rounded) = val;
					//locate_point(resImg, subdiv, lateral, cv::Scalar(0,0,255));
					//subdiv.insert(cv::Point2f(lateral));
					//resImg = cv::Scalar::all(0);
					//draw_subdiv(resImg, subdiv, cv::Scalar(255, 255, 255));

				}


			}
		}
	}

	//for(const auto& pt : lateralDebug)
	//{
	//	subdiv.insert(cv::Point2f(pt.x, pt.y));
	//}

	//cv::imshow("debug", debug);

	mapx.copyTo(remapX);
	mapy.copyTo(remapY);
}

void Transformation::inverseRemap(const cv::Mat& src, cv::Mat& dst, const cv::Mat &remapX, const cv::Mat& remapY)
{
	int width = static_cast<int>(remapX.at<float>(0,0));
	int height = static_cast<int>(remapY.at<float>(0,0));

	dst = cv::Mat::zeros(height, width, CV_8U);
	for(int r = 1; r < src.rows - 1; r++)
	{
		for(int c = 1; c < src.cols - 1; c++)
		{
			cv::Point roundedPt = cv::Point(Misc::round(remapX.at<float>(r,c)), Misc::round(remapY.at<float>(r,c)));
			if(roundedPt.x > 0 && roundedPt.x < width && roundedPt.y > 0 && roundedPt.y < height)
			{
				dst.at<uchar>(roundedPt) = (src.at<uchar>(r, c) == 0) ? 1 : src.at<uchar>(r, c);
			}
		}
	}
}



//ONLY WORKS FOR n*m < 127
//TODO BETTER SEED
void Transformation::fillSegments(cv::Mat& img, const std::vector<Ellipse>& ellipses, const std::vector<Line>& lines, const std::vector<std::vector<cv::Point2f>>& pointsPerEllipse)
{
	float sf = 1/Config::scaleFactor;
	img = cv::Mat::zeros(static_cast<int>(sf * Config::usedResHeight), static_cast<int>(sf * Config::usedResWidth), CV_8U);

	auto scaleRotated = [sf](const cv::RotatedRect& rect) { return cv::RotatedRect(sf * rect.center, cv::Size2f(sf * rect.size.width, sf * rect.size.height), rect.angle); };

	//draw lines and ellipses with width of 1
	for(const Ellipse& e : ellipses)
		cv::ellipse(img, scaleRotated(e.getEllipseAsRotatedRect()), cv::Scalar(255), 1);
	for(const Line& l : lines)
		cv::line(img, sf * l.getStart(), sf * l.getEnd(), cv::Scalar(255), 1);


	int m = Config::numLineSamples;
	int currentVal = 2;
	for(int i = 0; i < Config::numCircleSamples - 1; i++)
	{
		for(int j = 0; j < Config::numLineSamples; j++)
		{
			cv::Point2f p1 = sf * pointsPerEllipse[i][j]; cv::Point2f p2 = sf * pointsPerEllipse[i + 1][j]; cv::Point2f p3 = sf * pointsPerEllipse[i + 1][(j + 1) % m];
			cv::Point2f middleLine = (p1 + 0.5*(p2 - p1));
			cv::Point seed = middleLine + 0.5*(p3 - middleLine);
			cv::floodFill(img, seed, cv::Scalar(currentVal), nullptr, cv::Scalar(10), cv::Scalar(10));
			currentVal += 2;
		}
	}

	//'delete' lines and ellipses with width of 1
	for(const Ellipse& e : ellipses)
		cv::ellipse(img, scaleRotated(e.getEllipseAsRotatedRect()), cv::Scalar(0), 1);
	for(const Line& l : lines)
		cv::line(img, sf * l.getStart(), sf * l.getEnd(), cv::Scalar(0), 1);


	if(isDebug)
		cv::imshow("floodfill segments", img);

}





void Transformation::getReverseWarpMaps(const Cone& cone, cv::Mat &remapX, cv::Mat &remapY, const cv::Mat& proj)
{
	double S = cone.S();
	double s = cone.s();
	double maxAngle = Misc::radToDeg(cone.maxAngle());

	double scale = (1 / S) * Config::resSlantHeight;
	S *= scale;
	s *= scale;

	int width = static_cast<int>(std::ceil(S));
	int height = static_cast<int>(std::ceil(S + S*std::cos(Misc::degToRad(180 - maxAngle))));

	cv::Point origin = cv::Point(width, Misc::round(std::ceil(height - S)));

	cv::Mat mask = generateLateralMask(cone);
	cv::Mat mapx = cv::Mat::zeros(height, width, CV_32F);
	cv::Mat mapy = cv::Mat::zeros(height, width, CV_32F);

	//std::vector<cv::Point3f> test;
	//std::vector<cv::Point2f> test2;
	for(int r = 0; r < height; r++)
	{
		for(int c = 0; c < width; c++)
		{
			if(mask.at<uchar>(r, c) != 0)
			{
				//test2.push_back(cv::Point2d(c, r) - cv::Point2d(origin));
                cv::Point3f coneCoords = cone.lateralToConeCoordinates(1/scale*cv::Point2d((cv::Point2d(c, r) - cv::Point2d(origin.x, origin.y))));
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

	mapx.copyTo(remapX);
	mapy.copyTo(remapY);

	remapX = 1/Config::scaleFactor * remapX;
	remapY = 1/Config::scaleFactor * remapY;

	//writeToFile(test, "reverseCone.txt");
	//writeToFile(test2, "reverseLateral.txt");
	//exit(0);

	//cv::remap(greyImg, resImg, mapx, mapy, cv::INTER_CUBIC);
	//cv::imshow("res", resImg);

}



std::vector<std::vector<cv::Point2f>> Transformation::getReverseReprojects(const Cone& cone, const cv::Mat& proj)
{
	std::vector<std::vector<cv::Point2f>> res;
	std::vector<std::vector<cv::Point2f>> laterals = cone.calculateLateralSamples(); //res[0][j] = innermost ellipse

	for(auto ptList : laterals)
	{
		std::vector<cv::Point2f> currVec;
		for(auto pt : ptList)
		{
			cv::Point3f ptCone = cone.lateralToConeCoordinates(pt);

			cv::Mat currentWorldMat = cv::Mat::zeros(4, 1, CV_32F);
			currentWorldMat.at<float>(0, 0) = ptCone.x; currentWorldMat.at<float>(1, 0) = ptCone.y; currentWorldMat.at<float>(2, 0) = ptCone.z; currentWorldMat.at<float>(3, 0) = 1;
			cv::Mat homImg = proj*currentWorldMat;

			float w = homImg.at<float>(2, 0);
			cv::Point2f reprojected = cv::Point2f(homImg.at<float>(0, 0) / w, homImg.at<float>(1, 0) / w);

			currVec.push_back(reprojected);
		}
		res.push_back(currVec);
	}
	return res;
}


cv::Mat Transformation::generateLateralMask(const Cone& cone)
{
	double S = cone.S();
	double s = cone.s();
	double maxAngle = Misc::radToDeg(cone.maxAngle());

	double scale = (1 / S) * Config::resSlantHeight;
	S *= scale;
	s *= scale;

	int width = static_cast<int>(std::ceil(S));
	int height = static_cast<int>(std::ceil(S + S*std::cos(Misc::degToRad(180 - maxAngle))));

	cv::Point origin = cv::Point(width, Misc::round(std::ceil(height - S)));
	cv::Point2f origin2f = cv::Point2f(static_cast<float>(origin.x), static_cast<float>(origin.y));

	cv::Mat mask = cv::Mat::zeros(height, width, CV_8U);

	cv::ellipse(mask, origin, cv::Size2d(S, S), 0, 90, Misc::round(90 + maxAngle), cv::Scalar(255), 3);
	cv::ellipse(mask, origin, cv::Size2d(s, s), 0, 90, 90 + maxAngle, cv::Scalar(255), 3);
	cv::line(mask, cv::Point(origin.x, Misc::round(origin.y + s)), cv::Point(origin.x, Misc::round(origin.y + S)), cv::Scalar(255), 3);

	float angleStd = static_cast<float>(Misc::degToRad((90 + maxAngle) - 0.5)); //0.5 for rounding errors
	cv::line(mask, origin2f + s *cv::Point2f(std::cos(angleStd), std::sin(angleStd)), origin2f + S * cv::Point2f(std::cos(angleStd), std::sin(angleStd)), cv::Scalar(255), 3);

	cv::floodFill(mask, origin2f + S / 2 * cv::Point2f(-1, -1), cv::Scalar(255), nullptr, cv::Scalar(10), cv::Scalar(10));

	return mask;

}



std::vector<cv::Point2f> Transformation::getReprojectionError(const Cone& cone, const cv::Mat& projection)
{
	std::vector<std::vector<cv::Point3f>> worldCoords = cone.sampleCoordsWorld();
	std::vector<std::vector<cv::Point2f>> pointsPerEllipse = cone.sampleCoordsImage();

	std::vector<cv::Point2f> deviations;
	for(size_t i = 0; i < worldCoords.size(); i++)
	{
		for(size_t j = 0; j < worldCoords[i].size(); j++)
		{
			cv::Point3f currentWorld = worldCoords[i][j];
			cv::Point2f currImg = pointsPerEllipse[i][j];

			cv::Mat currentWorldMat = cv::Mat::zeros(4, 1, CV_32F);
			currentWorldMat.at<float>(0, 0) = currentWorld.x; currentWorldMat.at<float>(1, 0) = currentWorld.y; currentWorldMat.at<float>(2, 0) = currentWorld.z; currentWorldMat.at<float>(3, 0) = 1;
			cv::Mat homImg1 = projection * currentWorldMat;

			float w1 = homImg1.at<float>(2, 0);
			cv::Point2f reprojected1 = cv::Point2f(homImg1.at<float>(0, 0) / w1, homImg1.at<float>(1, 0) / w1);

			cv::Point2f diff = currImg - reprojected1;
			deviations.push_back(diff);
		}
	}

	return deviations;
}


size_t Transformation::countHoles(const cv::Mat& unfolded, const Cone& cone)
{
	cv::Mat mask = generateLateralMask(cone);

	size_t counter = 0;
	for(int i = 0; i < mask.rows; i++)
		for(int j = 0; j< mask.cols; j++)
			if(mask.at<uchar>(i,j) != 0 && unfolded.at<uchar>(i,j) == 0)
				counter++;


	return counter;
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
	file.open(filename, std::ios_base::app);
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
