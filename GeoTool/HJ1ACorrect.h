#ifndef HJ1ACORRECT_H_
#define HJ1ACORRECT_H_

#include "AbstractCorrect.h"


class HJ1ACorrect: public AbstractCorrect
{
public:


public:

	HJ1ACorrect();
	virtual ~HJ1ACorrect();
//	virtual void doCorrect();

	int imagingRay(const ossimDpt& image_point,
		ossimEcefRay& image_ray);



};

#endif