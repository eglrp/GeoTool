#ifndef ZY3ABWDCORRECT_H
#define ZY3ABWDCORRECT_H

#include "ZY3ACorrect.h"

class ZY3ABWDCorrect:public ZY3ACorrect
{
	void doReadBaseConfig(char *psJobID);
//	void doSeparate();
	
	~ZY3ABWDCorrect();
};
#endif