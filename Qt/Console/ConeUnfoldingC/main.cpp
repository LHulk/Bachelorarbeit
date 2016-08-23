#include <QCoreApplication>
#include "ConeUnfolding.h"
#include "Config.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	Config::numCircleSamples = 5;
	Config::numLineSamples = 6;

	ConeUnfolding unfold = ConeUnfolding();

    return a.exec();
}
