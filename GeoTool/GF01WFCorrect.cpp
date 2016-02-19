#include "GF01WFCorrect.h"



void GF01WFCorrect::doReadBaseConfig(char *psJobID)
{
	/*
	L0tifName = "D:\\liyuhang\\work\\Z3处理平台\\code\\DATA\\GF01_WF1_005689_20140516_MY030_S0_01\\GF01_WF1_005689_20140516_MY030_S0_01_L0.tif";
	metaName = "D:\\liyuhang\\work\\Z3处理平台\\code\\DATA\\GF01_WF1_005689_20140516_MY030_S0_01\\GF01_WF1_005689_20140516_MY030_S0_01_meta.xml";
	linetimeName = "D:\\liyuhang\\work\\Z3处理平台\\code\\DATA\\GF01_WF1_005689_20140516_MY030_S0_01\\GF01_WF1_005689_20140516_MY030_S0_01_linetime.dat";
	*/

	/*
	L0tifName = "H:\\GF01WF1224\\20140410\\L1_\\GF01_WF3_005154_20140410_MY031_S0_03.tif";
	metaName = "H:\\GF01WF1224\\20140410\\L1_\\GF01_WF3_005154_20140410_MY031_S0_03_meta.xml";
	linetimeName = "H:\\GF01WF1224\\20140410\\L1_\\GF01_WF3_005154_20140410_MY031_S0_03_linetime.dat";
	*/
	/*
	L0tifName = "H:\\GF01WF1224\\20140516\\L1_\\GF01_WF3_005689_20140516_MY031_S0_03.tif";
	metaName = "H:\\GF01WF1224\\20140516\\L1_\\GF01_WF3_005689_20140516_MY031_S0_03_meta.xml";
	linetimeName = "H:\\GF01WF1224\\20140516\\L1_\\GF01_WF3_005689_20140516_MY031_S0_03_linetime.dat";
	*/

	L0tifName = "H:\\GF01WF1224\\20151230\\L1_\\GF01_WF3_014423_20151229_MY241_S0_03.tif";
	metaName = "H:\\GF01WF1224\\20151230\\L1_\\GF01_WF3_014423_20151229_MY241_S0_03_meta.xml";
	linetimeName = "H:\\GF01WF1224\\20151230\\L1_\\GF01_WF3_014423_20151229_MY241_S0_03_linetime.dat";
	
/*
  L0tifName = "H:\\GF01WF1224\\20150226\\L1_\\GF01_WF3_009907_20150226_MY021_S0_03.tif";
  metaName = "H:\\GF01WF1224\\20150226\\L1_\\GF01_WF3_009907_20150226_MY021_S0_03_meta.xml";
  linetimeName = "H:\\GF01WF1224\\20150226\\L1_\\GF01_WF3_009907_20150226_MY021_S0_03_linetime.dat";
 */ 
	rawDataName=L0tifName.substr(L0tifName.rfind('\\')+1);
	rawDataName = rawDataName.substr(0,strlen(rawDataName.c_str())-7);
	L2tifName = "H:\\GF01WF1224\\20151230\\L2_\\"+rawDataName+"_L2_test.tif";

	ImgSize = ImgSizeGF01WF;
	MainDistance = MainDistanceGF01WFCamA;
	SizePixel = SizePixelGF01WF;
	BlockSize = 1000;
	BigBlockRowSize = BlockSize*10;

	//WF1
	/*
	c3 = 4.561150868244867e-09;
	c2 = -8.211988871393059e-05;
	c1 = 0.478396969184312;
	c0 = -8.992797478106946e+02;
	*/
	//WF3
	c3 = 4.44932576463357e-09;
	c2 = -8.00941387973806e-05;
	c1 = 0.472364761262389;
	c0 = -912.129683315616;

	/*
	pitch = -0.00052325;
	roll = 0.418716889;
	yaw = 0.0019555;
	*/
/*
	pitch =-0.006751167;// -0.006658;   //南北
	roll = -0.139504;        //东西-0.13942636024844720496894409937888  -0.13942636024; 
	yaw =-0.043139999999900015;
*/
	pitch = -7.4534161490683229813664596273292e-4;// -0.006658;   //南北
	roll =-0.13965927950310559006211180124224;        //东西-0.13942636024844720496894409937888  -0.13942636024; 
	yaw = -1.7000000000000799e-005;

	lookangleX = 0.0;

}



/*
void GF01WFCorrect::doCorrect()
{
	Paracorrect();
}
*/

GF01WFCorrect::~GF01WFCorrect()
{
}
