#include "ZY3AMUXCorrect.h"


void ZY3AMUXCorrect::doReadBaseConfig(char *psJobID)
{
	L0tifName = "H:\\ZY3A0112\\0级数据\\MUX\\20131104\\ZY3A_MUX_010111_20131104_MY431_S0_L0.tif";
	metaName = "H:\\ZY3A0112\\0级数据\\MUX\\20131104\\ZY3A_MUX_010111_20131104_MY431_S0_meta.xml";
	linetimeName = "H:\\ZY3A0112\\0级数据\\MUX\\20131104\\ZY3A_MUX_010111_20131104_MY431_S0_linetime.dat";

	//	L0tifName = "I:\\data_0703\\ZY3A\\20150610\\ZY3A_NAD_018981_20150610_MY140_01_012_L1A_00\\ZY3A_NAD_018981_20150610_MY140_01_012_L1A_00.tif";
	//	metaName = "I:\\data_0703\\ZY3A\\20150610\\ZY3A_NAD_018981_20150610_MY140_01.aux.xml";
	//	linetimeName = "I:\\data_0703\\ZY3A\\20150610\\ZY3A_NAD_018981_20150610_MY140_01_012_L1A_00\\ZY3A_NAD_018981_20150610_MY140_01_012_L1A_00.linetime.DAT";

	rawDataName=L0tifName.substr(L0tifName.rfind('\\')+1);
	rawDataName = rawDataName.substr(0,strlen(rawDataName.c_str())-7);
	L2tifName = "H:\\ZY3A0112\\0级数据\\MUX\\20131104\\"+rawDataName+"_L2_1.tif";

	ImgSize = ImgSizeZY3AMUX;
	MainDistance = MainDistanceZY3AMUX;
	SizePixel = SizePixelZY3AMUX;
	BlockSize = 128;
	BigBlockRowSize = BlockSize*20;

	c3 = 0.0;
	c2 = 0.0;
	c1 = 0.0;
	c0 = 0.0;

	pitch =-0.10763665418;
	roll = -0.0015761;
	yaw = -0.0024446666;

	lookangleX = 0.0;
}



ZY3AMUXCorrect::~ZY3AMUXCorrect()
{

}