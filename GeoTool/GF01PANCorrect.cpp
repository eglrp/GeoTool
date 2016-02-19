#include "GF01PANCorrect.h"



void GF01PANCorrect::doReadBaseConfig(char *psJobID)
{

//	L0tifName = "I:\\data_0703\\GF01\\GF01_PA0_011774_20150703_MY040_01_014_L1A_01-PAN1.tif";
//	metaName = "I:\\data_0703\\GF01\\GF01_PA1_011774_20150703_MY040_S0.aux.GPS";
//	linetimeName = "I:\\data_0703\\GF01\\GF01_PA1_011774_20150703_MY040_S0_01.dat.linetime.dat";

//	L0tifName = "I:\\data_0703\\GF01\\GF01_PA0_005695_20140517_MY030_01_029_L1A_01-PAN1.tif";
//	metaName = "I:\\data_0703\\GF01\\GF01_PA1_005695_20140517_MY030_S0.aux.GPS";
//	linetimeName = "I:\\data_0703\\GF01\\GF01_PA1_005695_20140517_MY030_S0_01.dat.linetime.dat";

	L0tifName = "D:\\liyuhang\\work\\Z3处理平台\\code\\DATA\\GF01_PA1_005695_20140517_MY030_S0_01\\GF01_PA1_005695_20140517_MY030_S0_01.L0.tif";
	metaName = "D:\\liyuhang\\work\\Z3处理平台\\code\\DATA\\GF01_PA1_005695_20140517_MY030_S0_01\\GF01_PA1_005695_20140517_MY030_S0_01.meta.xml";
	linetimeName = "D:\\liyuhang\\work\\Z3处理平台\\code\\DATA\\GF01_PA1_005695_20140517_MY030_S0_01\\GF01_PA1_005695_20140517_MY030_S0_01.linetime.dat";

	rawDataName=L0tifName.substr(L0tifName.rfind('\\')+1);
	rawDataName = rawDataName.substr(0,strlen(rawDataName.c_str())-7);
	L2tifName = "I:\\data_0703\\GF01\\"+rawDataName+"_L2.tif";

	ImgSize = ImgSizeGF01Pan;
	MainDistance = MainDistanceGF01PanCamA;
	SizePixel = SizePixelGF01PAN;
	BlockSize = 128;
	BigBlockRowSize = BlockSize*20;

	c3 = 0.0;
	c2 = 0.0;
	c1 = 0.0;
	c0 = 0.0;

	pitch = -0.0014;
	roll = 0.0264;
	yaw = 0.0;

	lookangleX = 0.5/180*3.1415926;

}





GF01PANCorrect::~GF01PANCorrect()
{

}