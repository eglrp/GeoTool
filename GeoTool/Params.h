#ifndef PARAMS_H_
#define PARAMS_H_

#include <string>

using namespace std;

class Params
{
public:

	//�������ϵ��
	double c3,c2,c1,c0;

	//�������ϵ��
	double d3,d2,d1,d0;

	double pitch;
	double roll;
	double yaw;

	int ImgSize;
	int size;
	double *xsample;
	double *ysample;
	double *zsample;

};

#endif