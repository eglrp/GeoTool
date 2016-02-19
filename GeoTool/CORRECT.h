#ifndef  _CORRECT_H_
#define _CORRECT_H_

#include "FileName.h"
#include "Params.h"

int Install(char *SatelliteName,char *SensorName,FileName &filename,Params &params);

int Distortion(char *SatelliteName,char *SensorName,FileName &filename,Params &params);

#endif