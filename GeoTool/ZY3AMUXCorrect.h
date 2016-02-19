#ifndef ZY3AMUXCORRECT_H
#define ZY3AMUXCORRECT_H

#include "ZY3ACorrect.h"

#define SizePixelZY3AMUX 0.00001994173                   //0.000020
#define MainDistanceZY3AMUX 1.750
#define ImgSizeZY3AMUX 8823

class ZY3AMUXCorrect:public ZY3ACorrect
{
	void doReadBaseConfig(char *psJobID);
//	void doCorrect();

	~ZY3AMUXCorrect();
};
#endif