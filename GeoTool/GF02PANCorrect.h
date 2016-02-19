#ifndef GF02PANCORRECT_H_
#define GF02PANCORRECT_H_

#include "GF02Correct.h"

#define ImgSizeGF02Pan 29200
#define SizePixelGF02Pan 0.00001
#define MainDistanceGF02PAN 7.785


class GF02PANCorrect:public GF02Correct
{
	void doReadBaseConfig(char *psJobID);
//	void doCorrect();

	~GF02PANCorrect();
};

#endif