#ifndef GF02MSCORRECT_H_
#define GF02MSCORRECT_H_

#include "GF02Correct.h"

class GF02MSCorrect:public GF02Correct
{
	void doReadBaseConfig(char *psJobID);
//	void doCorrect();
	
	~GF02MSCorrect();
};


#endif