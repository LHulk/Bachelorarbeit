#ifndef CONEUNFOLDING_H
#define CONEUNFOLDING_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <vector>
#include <iostream>

#include "EdgeDetection.h"
#include "Ellipse.h"
#include "Line.h"
#include "DotDetection.h"
#include "Config.h"
#include "Transformation.h"
#include "Cone.h"
#include "Calibration.h"


class ConeUnfolding
{
public:
	ConeUnfolding();	

};

#endif // CONEUNFOLDING_H
