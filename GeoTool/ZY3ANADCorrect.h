#ifndef ZY3ANADCORRECT_H
#define ZY3ANADCORRECT_H

#include "ZY3ACorrect.h"


#define SizePixelZY3ANAD 0.00000700342619406
#define MainDistanceZY3ANAD 1.7
#define ImgSizeZY3ANAD 24529

class ZY3ANADCorrect:public ZY3ACorrect
{
	void doReadBaseConfig(char *psJobID);
//	void doCorrect();

	~ZY3ANADCorrect();
};
#endif