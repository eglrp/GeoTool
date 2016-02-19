#ifndef ZY3AFWDCORRECT_H
#define ZY3AFWDCORRECT_H

#include "ZY3ACorrect.h"


class ZY3AFWDCorrect:public ZY3ACorrect
{
	void doReadBaseConfig(char *psJobID);
//	void doSeparate();

	~ZY3AFWDCorrect();
};
#endif