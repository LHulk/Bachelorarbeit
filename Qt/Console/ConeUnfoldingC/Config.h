#ifndef CONFIG_H
#define CONFIG_H

#include <cstddef>

namespace Config
{
	//pattern settings
	extern size_t numLineSamples;
	extern size_t numCircleSamples;

	//vis/performance settings
	extern int usedResWidth;
	extern int usedResHeight;

	//3D structure settings in mm
	extern float radiusInner;
	extern float radiusOuter;
	extern float height; //from center to center
    extern float lambda; //scale factor for R,r,h
}

#endif //CONFIG_H

