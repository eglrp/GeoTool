#include "ZY3ANADCorrect.h"


void ZY3ANADCorrect::doReadBaseConfig(char *psJobID)
{
	L0tifName = "I:\\data_0703\\ZY3A\\20150628\\ZY3A_NAD_019248_20150628_MY120_01_040_L1A_00\\ZY3A_NAD_019248_20150628_MY120_01_040_L1A_00.tif";
	metaName = "I:\\data_0703\\ZY3A\\20150628\\ZY3A_NAD_019248_20150628_MY120_01.aux.xml";
	linetimeName = "I:\\data_0703\\ZY3A\\20150628\\ZY3A_NAD_019248_20150628_MY120_01_040_L1A_00\\ZY3A_NAD_019248_20150628_MY120_01_040_L1A_00.linetime.DAT";

//	L0tifName = "I:\\data_0703\\ZY3A\\20150610\\ZY3A_NAD_018981_20150610_MY140_01_012_L1A_00\\ZY3A_NAD_018981_20150610_MY140_01_012_L1A_00.tif";
//	metaName = "I:\\data_0703\\ZY3A\\20150610\\ZY3A_NAD_018981_20150610_MY140_01.aux.xml";
//	linetimeName = "I:\\data_0703\\ZY3A\\20150610\\ZY3A_NAD_018981_20150610_MY140_01_012_L1A_00\\ZY3A_NAD_018981_20150610_MY140_01_012_L1A_00.linetime.DAT";

	rawDataName=L0tifName.substr(L0tifName.rfind('\\')+1);
	rawDataName = rawDataName.substr(0,strlen(rawDataName.c_str())-7);
	L2tifName = "I:\\data_0703\\ZY3A\\"+rawDataName+"_L2.tif";

	ImgSize = ImgSizeZY3ANAD;
	MainDistance = MainDistanceZY3ANAD;
	SizePixel = SizePixelZY3ANAD;
	BlockSize = 128;
	BigBlockRowSize = BlockSize*20;

	c3 = 0.0;
	c2 = 0.0;
	c1 = 0.0;
	c0 = 0.0;
	
	pitch = 0.001296891;
	roll = -0.0015523;
	yaw = -0.003125;

	lookangleX = 0.0;
}



ZY3ANADCorrect::~ZY3ANADCorrect()
{

}