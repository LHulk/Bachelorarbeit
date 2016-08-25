#ifndef CONFIG_H
#define CONFIG_H

enum
{
	CENTER_TO_CENTER,
	CENTER_TO_TIP
};

namespace Config
{
	//pattern settings
	extern int numLineSamples;
	extern int numCircleSamples;

	//vis/performance settings
	extern int usedResWidth;
	extern int usedResHeight;

	//algorithm settings
	extern int cannyLow;
	extern int cannyHigh;
	extern int cannyKernel;
	extern double cannySigma;

	//output settings
	extern int resSlantHeight;

	//3D structure settings in mm
	extern float radiusInner;
	extern float radiusOuter;
	extern float height; //from center to center
	extern int whichHeight;
}

#endif //CONFIG_H

