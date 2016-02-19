#ifndef GF01MSCORRECT_H_
#define GF01MSCORRECT_H_


#include "GF01Correct.h"


class GF01MSCorrect:public GF01Correct
{
	void doReadBaseConfig(char *psJobID);
//	void doCorrect();


	~GF01MSCorrect();
};

#endif