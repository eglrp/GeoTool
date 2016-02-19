#ifndef CORRECTFACTORY_H
#define CORRECTFACTORY_H

#include "AbstractCorrect.h"
#include "ZY3ABWDCorrect.h"
#include "ZY3AFWDCorrect.h"
#include "ZY3AMUXCorrect.h"
#include "ZY3ANADCorrect.h"
#include "GF01PANCorrect.h"
#include "GF01MSCorrect.h"
#include "GF01WFCorrect.h"
#include "GF02PANCorrect.h"
#include "GF02MSCorrect.h"
#include "HJ1ACCDCorrect.h"


class CorrectFactory
{
public:
	static AbstractCorrect* Creat(char *SatelliteName,char *SensorName);
};
#endif
