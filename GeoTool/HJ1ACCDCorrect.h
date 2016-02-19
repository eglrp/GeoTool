#ifndef HJ1ACCDCORRECT_H_
#define HJ1ACCDCORRECT_H_


#include "HJ1ACorrect.h"


class HJ1ACCDCorrect:public HJ1ACorrect
{
	void doReadBaseConfig(char *psJobID);
//	void doCorrect();


	~HJ1ACCDCorrect();
};

#endif