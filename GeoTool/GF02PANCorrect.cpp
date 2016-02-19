#include "GF02PANCorrect.h"



void GF02PANCorrect::doReadBaseConfig(char *psJobID)
{
	paramName = "..//conf//GF02_PAN.txt";
	LoadParam(paramName.c_str());
		
	ImgSize = ImgSizeGF02Pan;
	MainDistance = MainDistanceGF02PAN;
	SizePixel = SizePixelGF02Pan;

	/*
	c3 = 9.75952104829620e-11;
	c2 = -4.21775645207851e-06;
	c1 = 0.0591200193005039;
	c0 = -271.263958098117;

	d3 = 8.22278190006644e-13;
	d2 = 9.15557708092247e-07;
	d1 = -0.0270293504228771;
	d0 = 194.312775809473;
	
	pitch = 0.025816;   //4698  ¼Ó×ËÌ¬
	roll = -0.008689;
	yaw = -0.005041;
	*/
}


GF02PANCorrect::~GF02PANCorrect()
{
}