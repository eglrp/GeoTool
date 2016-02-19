#include "HJ1ACCDCorrect.h"



void HJ1ACCDCorrect::doReadBaseConfig(char *psJobID)
{
/*
	L0tifName = "H:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\MY_HJ1A_20141208_033670_0012_CCD1_B1.dat.L0.tif";
	metaName = "H:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\MY_HJ1A_20141208_033670_0012_CCD1_B1.dat.meta.xml";
	linetimeName = "H:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\MY_HJ1A_20141208_033670_0012_CCD1_B1.dat.linetime.dat";
*/
	
	L0tifName = "H:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\MY_HJ1A_20151125_038863_0012_CCD1_B1.dat.L0.tif";
	metaName = "H:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\MY_HJ1A_20151125_038863_0012_CCD1_B1.dat.meta.xml";
	linetimeName = "H:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\MY_HJ1A_20151125_038863_0012_CCD1_B1.dat.linetime.dat";
	
	/*
	rawDataName=L0tifName.substr(L0tifName.rfind('\\')+1);
	rawDataName = rawDataName.substr(0,strlen(rawDataName.c_str())-7);
	L2tifName = "G:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\"+rawDataName+"_L2.tif";
*/
//	L0tifName = "G:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\HJ1A_C1_032137_20140826_MY000_S1.dat.L0.tif";
//	metaName = "G:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\HJ1A_C1_032137_20140826_MY000_S1.dat.meta.xml";
//	linetimeName = "G:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\HJ1A_C1_032137_20140826_MY000_S1.dat.linetime.dat";

//	L0tifName = "G:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\SY_HJ1A_20150305_034953_0023_CCD1_B1.dat.L0.tif";
//	metaName = "G:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\SY_HJ1A_20150305_034953_0023_CCD1_B1.dat.meta.xml";
//	linetimeName = "G:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\SY_HJ1A_20150305_034953_0023_CCD1_B1.dat.linetime.dat";
/*
	L0tifName = "H:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\MY_HJ1A_20150306_034968_0012_CCD1_B1.dat.L0.tif";
	metaName = "H:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\MY_HJ1A_20150306_034968_0012_CCD1_B1.dat.meta.xml";
	linetimeName = "H:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\MY_HJ1A_20150306_034968_0012_CCD1_B1.dat.linetime.dat";
*/
	rawDataName=L0tifName.substr(L0tifName.rfind('\\')+1);
	rawDataName = rawDataName.substr(0,strlen(rawDataName.c_str())-7);
	L2tifName = "H:\\HJ1A\\CCD1_1208\\代码\\HJ1A_1214\\HJ1A\\"+rawDataName+"_L2.tif";

	ImgSize = 12000;
	MainDistance = 0.1408;
	SizePixel = 0.0000065;
	BlockSize = 128;
	BigBlockRowSize = BlockSize*20;


//	c3 = 8.305445207428129e-10;
//	c2 = -1.483436504785721e-05;
//	c1 = 1.086798421494566;
//	c0 = -1.701735526124577e+02;

//	c3 = 8.745437649682962e-10;
//	c2 = -1.566942837053319e-05;
//	c1 = 0.091175393651625;
//	c0 = -1.725817312244887e+02;
/*
	c3 = 5.684534472293923e-09;    //last version
	c2 = -1.018512844084657e-04;
	c1 = 0.592640058735561;
	c0 = -1.121781252959177e+03;
	*/

	c3 = 5.52422287434010e-09;    //last version
	c2 = -9.93824151004735e-05;
	c1 = 0.579478574541137;
	c0 = -1105.74441326323;



	roll = -0.26694616777;
//	pitch = -0.006218;
	pitch = -0.005418;          //-0.006218+0.0008
	yaw =  -0.002932628001;
/*
	roll = -0.26889816776994779;
	pitch = -0.0053049999999999851;
	yaw = -0.0028536280009999896;
*/
	lookangleX = 0.0;

}






HJ1ACCDCorrect::~HJ1ACCDCorrect()
{
}
