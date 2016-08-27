#include "Config.h"

//default values
namespace Config
{
    //cone pattern settings
	int numLineSamples = 6;
	int numCircleSamples = 5;

    //calibration pattern settings
    int patternPointsPerRow = 6;
    int patternPointsPerCol = 9;
    int patternSquareSize = 25; //square center to square center distance in mm

	//vis/performance settings
	int usedResWidth = 1000;
	int usedResHeight = 1000;

	//algorithm settings
	int cannyLow = 100;
	int cannyHigh = 254;
	int cannyKernel = 3;
	double cannySigma = 0.5f;

	//output settings
	int resSlantHeight = 300;

	//3D structure settings in mm
	float radiusInner = 13;
	float radiusOuter = 60;
	float height = 110; //from center of big radius to cone tip
	int whichHeight = CENTER_TO_TIP;

	float scaleFactor = 1;
}
