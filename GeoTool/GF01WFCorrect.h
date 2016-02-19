#ifndef GF01WFCORRECT_H_
#define GF01WFCORRECT_H_

#include "GF01Correct.h"


#define NumFrameGF01WF 12
#define LenFrameGF01WF 96176
#define NumPixelGF01WF 12000
#define SizePixelGF01WF 0.0000065
#define MainDistanceGF01WFCamA   0.27019    //0.269767       0.269999   //
#define ImgSizeGF01WF 12000

class GF01WFCorrect:public GF01Correct
{

public:

	
public:
	void doReadBaseConfig(char *psJobID);
//	void doCorrect();


	~GF01WFCorrect();
};

#endif