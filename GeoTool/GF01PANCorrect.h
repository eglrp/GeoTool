#ifndef GF01PANCORRECT_H_
#define GF01PANCORRECT_H_

#include "GF01Correct.h"

#define SizePixelGF01PAN 0.00001
#define MainDistanceGF01PanCamA 3.288319
#define ImgSizeGF01Pan 18196


class GF01PANCorrect:public GF01Correct
{

public:
	void doReadBaseConfig(char *psJobID);
//	void doCorrect();


	~GF01PANCorrect();
};


#endif