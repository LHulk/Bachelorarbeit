#ifndef MISC_H
#define MISC_H

#include<algorithm>

class Misc
{
public:
	static int mod(int a, int b) { return ((a % b) + b) % b; }
	static double mod(double a, double b) { return std::fmod(std::fmod(a, b) + b, b); }
	static double mod(int a, double b) { return mod(static_cast<double>(a), b); }
	static double mod(double a, int b) { return mod(a, static_cast<double>(b)); }

	static int round(double a) { return static_cast<int>(std::lround(a)); }

	static float clamp(float n, float lower, float upper) { return std::max(lower, std::min(n, upper)); }

};

#endif //MISC_H