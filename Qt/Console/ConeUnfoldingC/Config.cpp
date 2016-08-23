#include "Config.h"


//default values
namespace Config
{
	//pattern settings
	size_t numLineSamples = 6;
	size_t numCircleSamples = 5;

	//vis/performance settings
	int usedResWidth = 1000;
	int usedResHeight = 1000;

	//3D structure settings in mm
	float radiusInner = 13;
	float radiusOuter = 60;
	float height = 110; //from center to center
    float lambda = 1.0f;
}
