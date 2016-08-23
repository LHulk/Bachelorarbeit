//non max supression
	for(int i = 1; i < magnitude.rows - 1; i++)
	{
		for(int j = 1; j < magnitude.cols - 1; j++)
		{
			float angle = orientation.at<float>(i,j);

			float p, q;
			if(angle > CV_PI) //symmetry
				angle -= CV_PI;

			//round for debug
			angle = (std::round(4*angle/CV_PI)/4.0)*CV_PI-0.0001;

			uchar topLeft =		magnitude.at<uchar>(i-1, j-1);
			uchar left =		magnitude.at<uchar>(i, j-1);
			uchar bottomLeft =	magnitude.at<uchar>(i+1, j-1);
			uchar bottom =		magnitude.at<uchar>(i+1, j);
			uchar bottomRight =	magnitude.at<uchar>(i+1, j+1);
			uchar right =		magnitude.at<uchar>(i, j+1);
			uchar topRight =	magnitude.at<uchar>(i-1, j+1);
			uchar top =			magnitude.at<uchar>(i-1, j);
			uchar center =		magnitude.at<uchar>(i, j);

			if(angle <= 0.25*CV_PI) //angle in [0, 45] degrees --> interpolate NE, E
			{
				float d = angle*4/CV_PI; //dist to right
				p = d*topRight + (1-d)*right; //NE, E
				q = d*bottomLeft + (1-d)*left; //SW, W

			}
			else if(angle <= 0.5*CV_PI) //interpolate NE, N
			{
				float d = angle*4/CV_PI - 1; //dist to top right
				p = d*top + (1-d)*topRight; //N, NE
				q = d*bottom + (1-d)*bottomLeft;

			}
			else if(angle <= 0.75*CV_PI)
			{
				float d = angle*4/CV_PI - 2; //dist to top
				p = d*topLeft + (1-d)*top;
				q = d*bottomRight + (1-d)*bottom;

			}
			else// if(angle <= CV_PI)
			{
				float d = angle*4/CV_PI - 3; //dist to top left
				p = d*left + (1-d)*topLeft;
				q = d*right + (1-d)*bottomRight;

			}

			if(p > center || q > center)
				magnitude.at<uchar>(i, j) = 0;

		}
	}

	cv::imshow("NMS", magnitude);

	//hysterese

	Mat mask = Mat::zeros(magnitude.rows, magnitude.cols, CV_8U);
	for(int i = 0; i < magnitude.rows; i++)
	{
		for(int j = 0; j < magnitude.cols; j++)
		{
			if(magnitude.at<uchar>(i, j) >= maxThresh)
				mask.at<uchar>(i,j) = 255;

		}
	}



	int counter;
	do
	{
		counter = 0;

		for(int i = 0; i < magnitude.rows; i++)
		{
			for(int j = 0; j < magnitude.cols; j++)
			{
				if(magnitude.at<uchar>(i, j) >= minThresh && magnitude.at<uchar>(i, j) < maxThresh)
				{
					if(	(mask.at<uchar>(i-1, j-1) == 255 || mask.at<uchar>(i-1, j) == 255 || mask.at<uchar>(i-1, j+1) == 255 || mask.at<uchar>(i, j-1) == 255 ||
						mask.at<uchar>(i, j+1) == 255 || mask.at<uchar>(i+1, j-1) == 255 || mask.at<uchar>(i+1, j) == 255 || 	mask.at<uchar>(i+1, j+1) == 255)
						 && mask.at<uchar>(i, j) != 255)
					{
						mask.at<uchar>(i, j) = 255;
						counter++;
					}

				}


			}
		}

	} while(counter > 0);



	cv::imshow("hysterese", mask);
