#ifndef ABSTRACTCORRECT_H
#define ABSTRACTCORRECT_H


#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
//#include<gdal.h>
#include <gdal_priv.h>
#include "linux_win.h"
//#include "XPathForXmlFile.h"
#include <time.h>
#include <math.h>
#include "resample.h"
#include <fstream>
#include<iomanip>
#include<iostream>
#include<sstream> 

using namespace std;

#include <ossim/projection/ossimSpot5Model.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/support_data/ossimSpotDimapSupportData.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimLsrPoint.h>
#include <ossim/base/ossimEcefRay.h>
#include <ossim/base/ossimLsrRay.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimNotifyContext.h>


#include <ossim/elevation/ossimElevManager.h>

#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimBilinearProjection.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimPolygon.h>

//#define PI 3.1415926

#include "ossimBilinearProjectionMy.h";
#include "FileName.h"
#include "Params.h"

#include"IERS.hpp"
#include"UTCTime.hpp"
#include "Matrix.hpp"

using namespace gpstk;



class AbstractCorrect
{
public: 
	char *m_psJobId;

	string	filePath;
	string rawDataName;
	string L0tifName;
	string linetimeName;
	string pointName;
	string metaName;
	string L2tifName;
	string paramName;

	string attName;

	int LenFrame;
	int NumFrame;

	std::vector<ossimDpt3d>     thePosEcfSamples;
	std::vector<ossimDpt3d>     theVelEcfSamples;
	std::vector<ossim_float64>  theEphSampTimes;
	std::vector<ossimDpt3d>     theAttitudeSamples; // x=pitch, y=roll, z=yaw
	std::vector<ossim_float64>  theAttSampTimes;

	std::vector<ossimDpt3d>     theQuater;

	std::vector<ossimDpt> thePointsPosition;
	std::vector<ossimGpt> thePointsLLH;

	//ossimRefPtr<ossimProjection> theSeedFunction;

	//ossimRefPtr<ossimProjection> theSeedFunction;
	ossimBilinearProjection *theSeedFunction;

	ossimPolygon   theBoundGndPolygonALL;
	ossim_float64  theLineSamplingPeriod;   //gf01  
	ossimDpt       theGSD;             // meters
	ossim_float64  theMeanGSD;         // meters

	ossimDrect     theImageClipRect;
	ossimDpt       theRefImgPt;        // should be image center

	bool theExtrapolateImageFlag;



	//char *lineTimefile;
	int ImgSize;//29200;
	double SizePixel;
	double MainDistance;
	int BlockSize;
	int BigBlockRowSize;

	double *LineTime;

	//径向畸变系数
	double c3,c2,c1,c0;

	//切向畸变系数
	double d3,d2,d1,d0;

	double pitch;
	double roll;
	double yaw;

	double lookangleX;

	//add 20150705
	int nSrcXsize;
	int nSrcYsize;


public:
	void Work(char *psJobID);

	virtual void doReadBaseConfig(char *psJobID)=0;

	virtual ~AbstractCorrect();

	double getTimeLine(double line);

	void getBilinearInterpolation(
	const ossim_float64& time,
	const std::vector<ossimDpt3d>& V,
	const std::vector<ossim_float64>& T,
	ossimDpt3d& li) ;

	void getLagrangeInterpolation(
	const ossim_float64& time,
	const std::vector<ossimDpt3d>& V,
	const std::vector<ossim_float64>& T,
	ossimDpt3d& li);

	void getPositionEcf(const ossim_float64& time,
			   ossimEcefPoint& pe); 


	void getVelocityEcf(const ossim_float64& time,
		ossimEcefPoint& ve);  

	void extrapolateAttitude(const ossim_float64& time, ossimDpt3d& at);

	void getAttitude(const ossim_float64& time,
                                            ossimDpt3d& at);  

	void getAttitudeQ(const ossim_float64& time,
                                            ossimDpt3d& at);  

	void getAttitudeQ1(const ossim_float64& time,
		ossimDpt3d& at);  

	void computeInstall(FileName &filename,Params &param);

	void computeDistortion(FileName &filename,Params &param);

	void computeCamToSatRotation(NEWMAT::Matrix& result);

	void computeSatToEciRotation(NEWMAT::Matrix& result, ossim_float64 t);

	void computeEciToEcrRotation(NEWMAT::Matrix& result, ossim_float64 t);

	void computeSatToOrbRotation(NEWMAT::Matrix& result, ossim_float64 t);

	void computeAttToOrbRotation(NEWMAT::Matrix& result, ossim_float64 t);


	void getPixelLookAngleX(const ossim_float64& sample,
												   ossim_float64& pa); 

	void getPixelLookAngleY(const ossim_float64& sample,
												   ossim_float64& pa); 


    virtual int imagingRay(const ossimDpt& image_point,
			   ossimEcefRay& image_ray); 

	int imagingRay1(const ossimDpt& image_point,
		ossimEcefRay& image_ray); 

//	int imagingRay2(const ossimDpt& image_point,
//		ossimEcefRay& image_ray); 

	

	void lineSampleToWorld(const ossimDpt& image_point,
                                         ossimGpt&       gpt); 

	bool insideImage(const ossimDpt& p);


	void lineSampleHeightToWorld(const ossimDpt& image_point,
                                              const ossim_float64& heightEllipsoid,
                                              ossimGpt& worldPoint);

//	void lineSampleHeightToWorldCorner(const ossimDpt& image_point,
 //                                             const ossim_float64& heightEllipsoid,
 //                                             ossimGpt& worldPoint);


	ossimGpt extrapolate (const ossimDpt& imagePoint);


	void worldToLineSample(const ossimGpt& worldPoint,
                                         ossimDpt&       ip); 

//	virtual int LoadTime(const char* lineTimefile);

	int LoadTime(const char* lineTimefile);
	
	xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, const xmlChar *szXpath); 

	double getValue(xmlNodePtr node,char *item);


//	virtual int LoadXml(const char *szDocName);

	int LoadXml(const char *szDocName);

	virtual int LoadAtt(const char *szDocName);

	int LoadPoint(const char *szDocName);

	int LoadXmlQ(const char *szDocName);

	int LoadParam(const char *szDocName);

	int saveParam(const char *szDocName);

	int setFileName(FileName &filename);
};
#endif
