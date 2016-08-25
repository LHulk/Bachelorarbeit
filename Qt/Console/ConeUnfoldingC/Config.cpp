#include "Config.h"

//default values
namespace Config
{
	//pattern settings
	int numLineSamples = 6;
	int numCircleSamples = 5;

	//vis/performance settings
	int usedResWidth = 1000;
	int usedResHeight = 1000;

	//algorithm settings
	int cannyLow = 100;
	int cannyHigh = 254;
	int cannyKernel = 3;
	double cannySigma = 0.5f;

	//output settings
	int resSlantHeight = 500;

	//3D structure settings in mm
	float radiusInner = 13;
	float radiusOuter = 60;
	float height = 110; //from center of big radius to cone tip
	int whichHeight = CENTER_TO_TIP;
}
