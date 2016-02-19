#include "AbstractCorrect.h"

#include "plotfit.h"

double getSlope(double x1, double y1, double x2, double y2)
{
	return (y1 - y2) / (x1 - x2);
}

void AbstractCorrect::Work(char *psJobID)
{
	doReadBaseConfig(psJobID);
//	doCorrect();
}



AbstractCorrect::~AbstractCorrect()
{

}

double AbstractCorrect::getTimeLine(double line)
{
	double time;

	int LineMin = floor(line);
	int LineMax = ceil(line);

	if (LineMin < 0.0)
	{
		time = LineTime[0]-theLineSamplingPeriod*(0.0-line); 
		return time;
	}
	if (LineMax > ImgSize-1)
	{
		time = LineTime[ImgSize-1]+theLineSamplingPeriod*(line-ImgSize+1); 
		return time;
	}
	time = LineTime[LineMin]+(LineTime[LineMax]-LineTime[LineMin])*(line - LineMin);
	return time;
}

void AbstractCorrect::getBilinearInterpolation(
	const ossim_float64& time,
	const std::vector<ossimDpt3d>& V,
	const std::vector<ossim_float64>& T,
	ossimDpt3d& li) 
{
	ossim_uint32 samp0 = 0;
	while ((samp0 < T.size()) && (T[samp0] < time)) ++samp0;

	if(samp0==0)
	{
		li = V[0];
	}
	else if(samp0 == T.size())
	{
		li = V[1];
	}
	else
	{
		double t = (T[samp0-1]-time)/(T[samp0-1] - T[samp0]);

		li = V[samp0-1] + (V[samp0]-V[samp0-1])*t;
	}
}

void AbstractCorrect::getLagrangeInterpolation(
	const ossim_float64& time,
	const std::vector<ossimDpt3d>& V,
	const std::vector<ossim_float64>& T,
	ossimDpt3d& li)

{
//    std::cout << "V size = " << V.size() << std::endl
//              << "T size = " << T.size() << std::endl;

   ossim_uint32 filter_size = 8;
   //
   // Verify that t is within allowable range:
   //
   ossim_uint32 lagrange_half_filter = 4;

   if(T.size() <= filter_size)
   {
      filter_size = (ossim_uint32)T.size()/2;
      lagrange_half_filter = filter_size/2;
   }
   if ((time <  T[lagrange_half_filter]) ||
       (time >= T[T.size()-lagrange_half_filter] ))
   {
  //    setErrorStatus();
      li.makeNan();

      return;
   }

   //***
   // Search the sampling time array for surrounding samples:
   //***
   ossim_uint32 samp0 = lagrange_half_filter;
   while ((samp0 < T.size()) && (T[samp0] < time)) ++samp0;

   //***
   // Do not use sample if it falls in neighborhood of desired time:
   //***
   ossim_uint32 bump = 0;
 //  if (fabs(T[samp0] - time) < theLineSamplingPeriod/2.0)
 //     bump = 1;

   samp0 -= lagrange_half_filter; // adjust to first sample in window   

   //***
   // Outer summation loop:
   //***
   ossimDpt3d S (0, 0, 0);
   for (ossim_uint32 j=samp0; j<(samp0+filter_size+bump); ++j)
   {
      ossim_float64 numerator   = 1.0;
      ossim_float64 denominator = 1.0;

      //***
      // Skip this sample if too close to desired time:
      //***
      if (bump && (j == (samp0+lagrange_half_filter) ))
         ++j;

      //***
      // Inner loop for product series:
      //***
      for (ossim_uint32 i=samp0; i<(samp0+filter_size+bump); ++i)
      {
         //***
         // Skip this sample if too close to desired time:
         //***
         if (bump && (i == (samp0+lagrange_half_filter) ))
            ++i;

         if (i != j)
         {
            numerator   *= time - T[i];
            denominator *= T[j] - T[i];
         }
      }

      ossimDpt3d p = V[j];
      p = p * numerator;
      p = p / denominator;
      S += p;
   }

   li = S;
}

void AbstractCorrect::getPositionEcf(const ossim_float64& time,
			   ossimEcefPoint& pe) 
{
	ossimDpt3d tempPt;

	if((thePosEcfSamples.size() < 8)||
		(theEphSampTimes.size() < 8))
	{
		getBilinearInterpolation(time, thePosEcfSamples, theEphSampTimes, tempPt);
	}
	else
	{
		getLagrangeInterpolation(time, thePosEcfSamples, theEphSampTimes, tempPt);
	}

	pe = ossimEcefPoint(tempPt.x,
		tempPt.y,
		tempPt.z);
}

void AbstractCorrect::getVelocityEcf(const ossim_float64& time,
		ossimEcefPoint& ve)  
{
		ossimDpt3d tempPt;

		if((theVelEcfSamples.size() < 8) ||
			(theEphSampTimes.size() < 8))
		{
			getBilinearInterpolation (time, theVelEcfSamples, theEphSampTimes, tempPt);
		}
		else
		{
			getLagrangeInterpolation (time, theVelEcfSamples, theEphSampTimes, tempPt);
		}

		ve = ossimEcefPoint(tempPt.x,
			tempPt.y,
			tempPt.z);
}
void AbstractCorrect::extrapolateAttitude(const ossim_float64& time, ossimDpt3d& at)
{
	at.makeNan();
	int last_samp = (int) theAttSampTimes.size() - 1;
	if (last_samp < 1)
		return;

	ossimDpt3d dAtt, dAtt_dt;
	double dt, delta_t;

	// Determine whether extrapolating at the front or the back of the range:
	if (time < theAttSampTimes.front())
	{
		dt = theAttSampTimes[1] - theAttSampTimes[0];
		dAtt = theAttitudeSamples[1] - theAttitudeSamples[0];
		dAtt_dt = dAtt/dt;
		delta_t = time - theAttSampTimes[0];
		at = theAttitudeSamples[0] + (dAtt_dt*delta_t);
	}
	else if (time >= theAttSampTimes.back())
	{
		dt = theAttSampTimes[last_samp] - theAttSampTimes[last_samp-1];
		dAtt = theAttitudeSamples[last_samp] - theAttitudeSamples[last_samp-1];
		dAtt_dt = dAtt/dt;
		delta_t = time - theAttSampTimes[last_samp];
		at = theAttitudeSamples[last_samp] + (dAtt_dt*delta_t);
	}

	return;
}

void AbstractCorrect::computeInstall(FileName &filename,Params &params){

	setFileName(filename);

	doReadBaseConfig(0);

	LoadTime(linetimeName.c_str());

//	LoadXml(metaName.c_str()); // modify 0121
	LoadXmlQ(metaName.c_str()); 
//	LoadAtt(attName.c_str());

	LoadPoint(pointName.c_str());

	IERS::loadIERSFile("finals.data");

	//准确经纬度

	//ossimDpt point1(3203,15840),point2(24899,15840),point3(14632,14078);
	int size  = thePointsPosition.size();
    ossimDpt pointL = thePointsPosition[0];
	ossimDpt pointR = thePointsPosition[size-1];
	ossimDpt pointM = thePointsPosition[size/2];

	ossimGpt pointGL = thePointsLLH[0];
	ossimGpt pointGR = thePointsLLH[size-1];
	ossimGpt pointGM = thePointsLLH[size/2];

	ossimGpt pointg1,pointg2,pointg3;


	lineSampleHeightToWorld(pointL,pointGL.hgt, pointg1);
	lineSampleHeightToWorld(pointR,pointGR.hgt, pointg2);
	lineSampleHeightToWorld(pointM,pointGM.hgt, pointg3);


	double raw_slop = getSlope(pointGL.lon, pointGL.lat, pointGR.lon, pointGR.lat);

	
	double latprecision = 1;
	double lonprecision = 1;
	double yawprecision = 1;

	double temp;
	
	int  isAtt = 1;

	if(isAtt){

	for(int i = 0;i<3;i++){

	lineSampleHeightToWorld(pointL,pointGL.hgt, pointg1);
	lineSampleHeightToWorld(pointR,pointGR.hgt, pointg2);
	temp = (pointg1.lat - pointg2.lat) / (pointg1.lon - pointg2.lon);
	yawprecision = temp - raw_slop;	

	while (abs(yawprecision) >0.00001)
	{
		if (yawprecision > 0)
		{
			yaw = yaw + 0.00001;// cout << 1;
		}
		else
		{
			yaw = yaw - 0.00001; //cout << 2;
		}
		lineSampleHeightToWorld(pointL,pointGL.hgt, pointg1);
		lineSampleHeightToWorld(pointR,pointGR.hgt, pointg2);
		temp = (pointg1.lat - pointg2.lat) / (pointg1.lon - pointg2.lon);
		yawprecision = temp - raw_slop;	
	}
	lineSampleHeightToWorld(pointM,pointGM.hgt, pointg3);
	lonprecision = pointg3.lon - pointGM.lon;
	latprecision = pointg3.lat - pointGM.lat;

	while (abs(lonprecision) > 0.00001)
	{
		while (abs(latprecision) > 0.00001)
		{

			lineSampleHeightToWorld(pointM,pointGM.hgt, pointg3);
			//cout << wp << endl;
			latprecision = pointg3.lat - pointGM.lat;
			if (latprecision > 0)
			{
				roll = roll - 0.000001;
			}
			else
			{
				roll = roll + 0.000001;      //越小越下
			}
			
			//cout << 1 ;
		}
		lonprecision = pointg3.lon - pointGM.lon;
		latprecision = pointg3.lat - pointGM.lat;
		if (lonprecision > 0)
		{
			pitch = pitch + 0.000001;
		}
		else
		{
			pitch = pitch - 0.000001;   //越小越大
		}
	

		lineSampleHeightToWorld(pointM,pointGM.hgt, pointg3);
		lonprecision = pointg3.lon - pointGM.lon;
		//cout << 2 ;
	}
	}

	}
	else
	{
		for(int i = 0;i<3;i++){

			lineSampleHeightToWorld(pointL,pointGL.hgt, pointg1);
			lineSampleHeightToWorld(pointR,pointGR.hgt, pointg2);
			temp = (pointg1.lat - pointg2.lat) / (pointg1.lon - pointg2.lon);
			yawprecision = temp - raw_slop;	

			while (abs(yawprecision) >0.00001)
			{
				if (yawprecision > 0)
				{
					yaw = yaw - 0.00001;// cout << 1;
				}
				else
				{
					yaw = yaw + 0.00001; //cout << 2;
				}
				lineSampleHeightToWorld(pointL,pointGL.hgt, pointg1);
				lineSampleHeightToWorld(pointR,pointGR.hgt, pointg2);
				temp = (pointg1.lat - pointg2.lat) / (pointg1.lon - pointg2.lon);
				yawprecision = temp - raw_slop;	
			}
			lineSampleHeightToWorld(pointM,pointGM.hgt, pointg3);
			lonprecision = pointg3.lon - pointGM.lon;
			latprecision = pointg3.lat - pointGM.lat;

			while (abs(lonprecision) > 0.0001)
			{
				while (abs(latprecision) > 0.00001)
				{

					lineSampleHeightToWorld(pointM,pointGM.hgt, pointg3);
					//cout << wp << endl;
					latprecision = pointg3.lat - pointGM.lat;
					if (latprecision > 0)
					{
						roll = roll - 0.000001;
					}
					else
					{
						roll = roll + 0.000001;      //越小越下
					}

					//cout << 1 ;
				}
				lonprecision = pointg3.lon - pointGM.lon;
				latprecision = pointg3.lat - pointGM.lat;
				if (lonprecision > 0)
				{
					pitch = pitch - 0.000001;
				}
				else
				{
					pitch = pitch + 0.000001;   //越小越大
				}


				lineSampleHeightToWorld(pointM,pointGM.hgt, pointg3);
				lonprecision = pointg3.lon - pointGM.lon;
				//cout << 2 ;
			}
		}
	}

	saveParam(paramName.c_str());

	params.pitch = pitch;
	params.roll = roll;
	params.yaw = yaw;
	params.c0 = c0;
	params.c1 = c1;
	params.c2 = c2;
	params.c3 = c3;
	params.d0 = d0;
	params.d1 = d1;
	params.d2 = d2;
	params.d3 = d3;
}

void AbstractCorrect::computeDistortion(FileName &filename,Params &params){

	setFileName(filename);

	doReadBaseConfig(0);

	LoadTime(linetimeName.c_str());

//	LoadXml(metaName.c_str()); // modify 0121
	LoadXmlQ(metaName.c_str()); 

//	LoadAtt(attName.c_str());

	LoadPoint(pointName.c_str());

	IERS::loadIERSFile("finals.data");

	c0 = 0;
	c1 = 0;
	c2 = 0;
	c3 = 0;

	d0 = 0;
	d1 = 0;
	d2 = 0;
	d3 = 0;

	ossimDpt theImageSize(ImgSize,ImgSize);
	theImageClipRect = ossimDrect(0.0, 0.0, theImageSize.x-1.0, theImageSize.y-1.0);

	ossimDpt ul,ur,ll,lr;
	ossimGpt ulg, urg, lrg, llg;
	lineSampleToWorld(theImageClipRect.ul(), ulg);
	lineSampleToWorld(theImageClipRect.ur(), urg);
	lineSampleToWorld(theImageClipRect.lr(), lrg);
	lineSampleToWorld(theImageClipRect.ll(), llg);

	theSeedFunction = new ossimBilinearProjection(theImageClipRect.ul(),
		theImageClipRect.ur(),
		theImageClipRect.lr(),
		theImageClipRect.ll(),
		ulg,
		urg,
		lrg,
		llg);


	int size  = thePointsPosition.size();

	std::vector<ossimDpt> inversePoints;

	for (int i = 0;i<size;i++)
	{
		ossimGpt gpt = thePointsLLH[i];
		ossimDpt dpt;
		worldToLineSample(gpt,dpt);
		inversePoints.push_back(dpt);
	}
	params.ImgSize = ImgSize;
	params.size = size;
	params.xsample = new double[size];
	params.ysample = new double[size];
	params.zsample = new double[size];

	for (int i = 0;i<size;i++ )
	{
		params.xsample[i] = thePointsPosition[i].x;
		params.ysample[i] = thePointsPosition[i].x - inversePoints[i].x;
		params.zsample[i] = thePointsPosition[i].y - inversePoints[i].y;
	}

	float curveParam[4];  
	plotfit(params.xsample,params.ysample,size,curveParam);

	c0 = curveParam[0];
	c1 = curveParam[1]; 
	c2 = curveParam[2];
	c3 = curveParam[3]; 


	plotfit(params.xsample,params.zsample,size,curveParam);

	d0 = curveParam[0];
	d1 = curveParam[1]; 
	d2 = curveParam[2];
	d3 = curveParam[3];

	saveParam(paramName.c_str());

	params.pitch = pitch;
	params.roll = roll;
	params.yaw = yaw;
	params.c0 = c0;
	params.c1 = c1;
	params.c2 = c2;
	params.c3 = c3;
	params.d0 = d0;
	params.d1 = d1;
	params.d2 = d2;
	params.d3 = d3;

}



void AbstractCorrect::getAttitude(const ossim_float64& time,
                                            ossimDpt3d& at)  
{
   if (theAttSampTimes.empty())
   {
     at.makeNan();
     return;
   }

   if ((time <  theAttSampTimes.front()) ||
       (time >= theAttSampTimes.back() ))
   {
      extrapolateAttitude(time, at);
      return;
   }

   //***
   // Search the attitude sampling time array for surrounding samples:
   //***
   int i=0;
   while ((i < (int)theAttSampTimes.size()) &&
          (theAttSampTimes[i] < time)) ++i;
   --i;

   //***
   // Linearly interpolate attitudes angles:
   //***
   ossim_float64 dt1   = time - theAttSampTimes[i];
   ossim_float64 dt0   = theAttSampTimes[i+1] - time;
   ossim_float64 dt    = theAttSampTimes[i+1] - theAttSampTimes[i];

   at = (theAttitudeSamples[i+1]*dt1 + theAttitudeSamples[i]*dt0)/dt;
}

void AbstractCorrect::getAttitudeQ(const ossim_float64& time,
                                            ossimDpt3d& at)  
{
   if (theAttSampTimes.empty())
   {
     at.makeNan();
     return;
   }

   if ((time <  theAttSampTimes.front()) ||
       (time >= theAttSampTimes.back() ))
   {
      extrapolateAttitude(time, at);
      return;
   }

   //***
   // Search the attitude sampling time array for surrounding samples:
   //***
   int i=0;
   while ((i < (int)theAttSampTimes.size()) &&
          (theAttSampTimes[i] < time)) ++i;
   --i;

   //***
   // Linearly interpolate attitudes angles:
   //***

   
   ossim_float64 dt1   = time - theAttSampTimes[i];
   ossim_float64 dt0   = theAttSampTimes[i+1] - time;
   ossim_float64 dt    = theAttSampTimes[i+1] - theAttSampTimes[i];

   at = (theQuater[i+1]*dt1 + theQuater[i]*dt0)/dt;
   
   /*
   ossim_float64 dt1   = time - theAttSampTimes[i];
   ossim_float64 dt0   = theAttSampTimes[i+1] - time;
   ossim_float64 dt    = theAttSampTimes[i+1] - theAttSampTimes[i];

  ossimDpt3d at1 = (theQuater[i+1]*dt1 + theQuater[i]*dt0)/dt;

   double tempt = dt1 / dt;

   double qq1 = theQuater[i].x*(pow(theQuater[i+1].x/theQuater[i].x, tempt));
   double qq2 = theQuater[i].y*(pow(theQuater[i+1].y/theQuater[i].y, tempt));
   double qq3 = theQuater[i].z*(pow(theQuater[i+1].z/theQuater[i].z, tempt));

   at = ossimDpt3d(qq1,qq2,qq3);
   */
}

void AbstractCorrect::getAttitudeQ1(const ossim_float64& time,
                                            ossimDpt3d& at)  
{

	if (theAttSampTimes.empty())
	{
		at.makeNan();
		return;
	}

	ossimDpt3d tempPt;

	if((theAttSampTimes.size() < 8)||
		(theQuater.size() < 8))
	{
		getBilinearInterpolation(time, theQuater, theAttSampTimes, tempPt);
	}
	else
	{
		getLagrangeInterpolation(time, theQuater, theAttSampTimes, tempPt);
	}
	at = tempPt; 
}

void AbstractCorrect::computeCamToSatRotation(NEWMAT::Matrix& result)
{
	//---
	// Linearly interpolate attitudes angles:
	//---

//	ossimDpt3d att;
//	getAttitude1(t, att);
/*    
	double roll = 1.005/180*PI+0.0052;   //GF02
	double pitch = -0.00124;
	double yaw =  0.0;
	ossimDpt3d install(pitch,roll,yaw);
*/

//	ossimDpt3d install(-0.0014,0.0264,0);   //GF01PANMS
//	ossimDpt3d install(-0.00052325,0.418716889,0.0019555);   //GF01WF
	ossimDpt3d install(pitch,roll,yaw);
//	ossimDpt3d install(0,0,0);
	//pitch 向北为正  roll 向西为正 yaw 逆时针为正
	double cp = cos(install.x);
	double sp = sin(install.x);
	double cr = cos(install.y);
	double sr = sin(install.y);
	double cy = cos(install.z);
	double sy = sin(install.z);

	//---
	// Populate rotation matrix:
	//---
	result = NEWMAT::Matrix(3,3);
	result << (cr*cy) << (-cr*sy) << (-sr)
		<< (cp*sy+sp*sr*cy) << (cp*cy-sp*sr*sy) << (sp*cr)
		<< (-sp*sy+cp*sr*cy) << (-sp*cy-cp*sr*sy) <<  cp*cr;
}

void AbstractCorrect::computeSatToEciRotation(NEWMAT::Matrix& result, ossim_float64 t)
{
	
	//---
	// Linearly interpolate attitudes angles:
	//---

	ossimDpt3d att;
	getAttitudeQ(t, att);

//	ossimDpt3d att(-0.666413,0.633612,0.358573);

	//---
	// Apply the attitude adjustable parameters:
	//---
/*
	double dt = theRefImagingTime - t;
	att.x     += thePitchOffset + dt*thePitchRate;
	att.y     += theRollOffset  + dt*theRollRate;
	att.z     += theYawOffset   + dt*theYawRate;
*/
	//---
	// Compute trig functions to populate rotation matrices: ANGLES IN RADIANS
	//---
/*
	double cp = cos(att.x);
	double sp = sin(att.x);
	double cr = cos(att.y);
	double sr = sin(att.y);
	double cy = cos(att.z);
	double sy = sin(att.z);
*/
	double q1 = att.x;
	double q2 = att.y;
	double q3 = att.z;
	double q0 = sqrt(1-q1*q1-q2*q2-q3*q3);
	//---
	// Populate rotation matrix:
	//---
	result = NEWMAT::Matrix(3,3);

	/*
	double matrix00 = (2*(q0*q0+q1*q1)-1);
	double matrix01 = (2*(q1*q2+q0*q3));
	double matrix02 = (2*(q1*q3-q0*q2));
	double matrix10 = (2*(q1*q2-q0*q3));
	double matrix11 = (2*(q0*q0+q2*q2)-1);
	double matrix12 = (2*(q2*q3+q0*q1));
	double matrix20 = (2*(q1*q3+q0*q2));
	double matrix21 = (2*(q2*q3-q0*q1));
	double matrix22 = (2*(q0*q0+q3*q3)-1);
	
	result << matrix00 << matrix01 << matrix02
		<< matrix10 << matrix11 << matrix12
		<< matrix20 << matrix21 <<  matrix22;
	*/

	double a1, a2, a3, b1, b2, b3, c1, c2, c3;
	a1 = q0*q0 + q1*q1 - q2*q2 - q3*q3;
	a2 = 2 * (q2*q1 + q0*q3);
	a3 = 2 * (q1*q3 - q0*q2);
	 
	b1 = 2 * (q1*q2 - q0*q3);
	b2 = 1 - 2 * (q1*q1 + q3*q3);
	b3 = 2 * (q2*q3 + q0*q1);

	c1 = 2 * (q1*q3 + q0*q2);
	c2 = 2 * (q2*q3 - q0*q1);          
	c3 = 1 - 2 * (q1*q1 + q2*q2);

	result <<a1 <<b1 <<c1
		   <<a2 << b2 <<c2
		   <<a3 << b3 <<c3;

	
}

void AbstractCorrect::computeEciToEcrRotation(NEWMAT::Matrix& result, ossim_float64 t)
{
	/*
	CivilTime utc(short(2009),short(1),short(1),short(0),short(0),0.0);
	CommonTime UTC(utc);	
	UTC.addSeconds(t - 28800);
	CivilTime utc1(UTC);
	UTC.setTimeSystem(7);

	EOPDataStore::EOPData ERP = EOPData(UTC);
	Matrix<double> c2tMat = J2kToECEFMatrix(UTC,ERP);
	*/

//	t = 190695602.906724;
	gpstk::CivilTime ct(2009, 1, 1, 0, 0, 0, gpstk::TimeSystem::UTC);
	gpstk::UnixTime ut(ct);
	gpstk::UnixTime ut1(atof(ut.asString().data()) + t, gpstk::TimeSystem::UTC);

	gpstk::CivilTime ct1(ut1);

	gpstk::CommonTime ctime(ut1);
	ctime.setTimeSystem(gpstk::TimeSystem::UTC);

	EOPDataStore::EOPData ERP = gpstk::EOPData(ctime);

	Matrix<double> c2tMat = gpstk::J2kToECEFMatrix(ctime, ERP);

	result = NEWMAT::Matrix(3, 3);
	result << c2tMat(0,0) << c2tMat(0,1) << c2tMat(0,2)
		<< c2tMat(1,0) << c2tMat(1,1) << c2tMat(1,2)
		<< c2tMat(2,0) << c2tMat(2,1) << c2tMat(2,2);
	
}


void AbstractCorrect::computeSatToOrbRotation(NEWMAT::Matrix& result, ossim_float64 t)
{
	
	//---
	// Linearly interpolate attitudes angles:
	//---

	ossimDpt3d att;
//	getAttitude(t, att);

	att = att/180*3.1415926;
	att = 0.0;
//	ossimDpt3d att(0,0,0);

	//---
	// Apply the attitude adjustable parameters:
	//---
/*
	double dt = theRefImagingTime - t;
	att.x     += thePitchOffset + dt*thePitchRate;
	att.y     += theRollOffset  + dt*theRollRate;
	att.z     += theYawOffset   + dt*theYawRate;
*/
	//---
	// Compute trig functions to populate rotation matrices: ANGLES IN RADIANS
	//---
	double cp = cos(att.x);
	double sp = sin(att.x);
	double cr = cos(att.y);
	double sr = sin(att.y);
	double cy = cos(att.z);
	double sy = sin(att.z);

	//---
	// Populate rotation matrix:
	//---
	result = NEWMAT::Matrix(3,3);
	result << (cr*cy) << (-cr*sy) << (-sr)
		<< (cp*sy+sp*sr*cy) << (cp*cy-sp*sr*sy) << (sp*cr)
		<< (-sp*sy+cp*sr*cy) << (-sp*cy-cp*sr*sy) <<  cp*cr;

}

void AbstractCorrect::computeAttToOrbRotation(NEWMAT::Matrix& result, ossim_float64 t)
{
	
	//---
	// Linearly interpolate attitudes angles:
	//---

	ossimDpt3d att;
	getAttitudeQ(t, att);

//	ossimDpt3d att(0,0,0);

	//---
	// Apply the attitude adjustable parameters:
	//---
/*
	double dt = theRefImagingTime - t;
	att.x     += thePitchOffset + dt*thePitchRate;
	att.y     += theRollOffset  + dt*theRollRate;
	att.z     += theYawOffset   + dt*theYawRate;
*/
	//---
	// Compute trig functions to populate rotation matrices: ANGLES IN RADIANS
	//---
/*
	double cp = cos(att.x);
	double sp = sin(att.x);
	double cr = cos(att.y);
	double sr = sin(att.y);
	double cy = cos(att.z);
	double sy = sin(att.z);
*/
	double q1 = att.x;
	double q2 = att.y;
	double q3 = att.z;
	double q0 = sqrt(1-q1*q1-q2*q2-q3*q3);
	//---
	// Populate rotation matrix:
	//---
	result = NEWMAT::Matrix(3,3);
/*
	result << (cr*cy) << (-cr*sy) << (-sr)
		<< (cp*sy+sp*sr*cy) << (cp*cy-sp*sr*sy) << (sp*cr)
		<< (-sp*sy+cp*sr*cy) << (-sp*cy-cp*sr*sy) <<  cp*cr;
*/
	double matrix11 = (2*(q0*q0+q1*q1)-1);
	double matrix12 = (2*(q1*q2+q0*q3));
	double matrix13 = (2*(q1*q3-q0*q2));
	double matrix21 = (2*(q1*q2-q0*q3));
	double matrix22 = (2*(q0*q0+q2*q2)-1);
	double matrix23 = (2*(q2*q3+q0*q1));
	double matrix31 = (2*(q1*q3+q0*q2));
	double matrix32 = (2*(q2*q3-q0*q1));
	double matrix33 = (2*(q0*q0+q3*q3)-1);

	result << (2*(q0*q0+q1*q1)-1) << (2*(q1*q2+q0*q3)) <<(2*(q1*q3-q0*q2))
		<< (2*(q1*q2-q0*q3)) << (2*(q0*q0+q2*q2)-1) << (2*(q2*q3+q0*q1))
		<< (2*(q1*q3+q0*q2)) << (2*(q2*q3-q0*q1)) <<  (2*(q0*q0+q3*q3)-1);


}

void AbstractCorrect::getPixelLookAngleX(const ossim_float64& sample,
												   ossim_float64& pa) 
{
//	pa = 0.5/180*PI;   //GF01PAN
//	pa = 0.695/180*PI;   //GF01B1
//	pa = 0.38/180*PI;   //GF02PAN
//	pa = 0.008070869;   //GF02B1
//	pa = 0.0;   //GF01WF
	double delta = (d3*pow(sample,3)+d2*pow(sample,2)+d1*sample+d0);
	pa = atan((delta*SizePixel)/MainDistance); 
}

void AbstractCorrect::getPixelLookAngleY(const ossim_float64& sample,
												   ossim_float64& pa) 
{
//	pa = atan((MainPointCamA-(NumPixel-sample))*SizePixel/MainDistanceCamA);

	double MainPointCamA1 = ImgSize/2.0;
//	double MainPointCamA1 = 5995.59;
//	double delta = (c3*pow(sample,3)+c2*pow(sample,2)+c1*sample+c0)*0.000001;   GF01 WF
	double delta = (c3*pow(sample,3)+c2*pow(sample,2)+c1*sample+c0);
//	double flag = (MainPointCamA1 > sample)?-1.0:1.0;
//	int MainPointCamA1 = ImgSizeGF02Ms/2;
//	pa = -atan((MainPointCamA1-sample)*SizePixelMs/MainDistanceMS); 
	pa = -atan(((MainPointCamA1-sample + delta)*SizePixel)/MainDistance); 
//	pa = -atan((MainPointCamA1-(sample+6144*2-380))*SizePixel/MainDistance);  //3

//		pa = 0.38/180*PI;   //GF02
}

int AbstractCorrect::imagingRay(const ossimDpt& image_point,
			   ossimEcefRay& image_ray) 
{
	bool runtime_dbflag = 0;
	NEWMAT::Matrix satToOrbit,camToSat;

	ossimDpt iPt = image_point;

	//
	// 1. Establish time of line imaging :  
	//
	double t_line = getTimeLine(iPt.line);

	//
	// 2. Interpolate ephemeris position and velocity (in ECF):
	//
	ossimEcefPoint  tempEcefPoint;
	ossimEcefPoint  P_ecf;
	getPositionEcf(t_line, P_ecf);
	getVelocityEcf(t_line, tempEcefPoint);
	ossimEcefVector V_ecf(tempEcefPoint.x(),
		tempEcefPoint.y(),
		tempEcefPoint.z());


	//
	// 3. Establish the look direction in Vehicle LSR space (S_sat).
	//    ANGLES IN RADIANS
	//
	ossim_float64 Psi_x;
	getPixelLookAngleX(iPt.samp, Psi_x);
	ossim_float64 Psi_y;
	getPixelLookAngleY(iPt.samp, Psi_y);



//	ossimColumnVector3d u_sat (-tan(Psi_y), tan(Psi_x), -(1.0 ));

	ossimColumnVector3d u_cam (-tan(Psi_y), -tan(Psi_x), -(1.0 ));


	//安装矩阵
	computeCamToSatRotation(camToSat);
	

	ossimColumnVector3d u_sat = (camToSat*u_cam).unit();
	


	//
	// 4. Transform vehicle LSR space look direction vector to orbital LSR space
	//    (S_orb):
	//
	computeSatToOrbRotation(satToOrbit, t_line);
//	computeSatToOrbRotationQ(satToOrbit, t_line);

	ossimColumnVector3d u_orb = (satToOrbit*u_sat).unit();


	//
	// 5. Transform orbital LSR space look direction vector to ECF.
	//
	//   a. S_orb space Z-axis (Z_orb) is || to the ECF radial vector (P_ecf),
	//   b. X_orb axis is computed as cross-product between velocity and radial,
	//   c. Y_orb completes the orthogonal S_orb coordinate system.
	//
	ossimColumnVector3d Z_orb (P_ecf.x(),
		P_ecf.y(),
		P_ecf.z());
	Z_orb = Z_orb.unit();

	ossimColumnVector3d X_orb = ossimColumnVector3d(V_ecf.x(),
		V_ecf.y(),
		V_ecf.z()).cross(Z_orb).unit();
	ossimColumnVector3d Y_orb = Z_orb.cross(X_orb);

	NEWMAT::Matrix orbToEcfRotation = NEWMAT::Matrix(3, 3);
	orbToEcfRotation << X_orb[0] << Y_orb[0] << Z_orb[0]
	<< X_orb[1] << Y_orb[1] << Z_orb[1]
	<< X_orb[2] << Y_orb[2] << Z_orb[2];


	ossimColumnVector3d u_ecf  = (orbToEcfRotation*u_orb);

	//
	// Establish the imaging ray given direction and origin:
	//
	image_ray = ossimEcefRay(P_ecf, ossimEcefVector(u_ecf[0], u_ecf[1], u_ecf[2]));
	return 0;
}

void AbstractCorrect::lineSampleToWorld(const ossimDpt& image_point,
                                         ossimGpt&       gpt) 
{
   bool debug = false;  // setable via interactive debugger
   
   if(image_point.hasNans())
   {
      gpt.makeNan();
      return;
   }
   //***
   // Extrapolate if image point is outside image:
   //***


   //***
   // Determine imaging ray and invoke elevation source object's services to
   // intersect ray with terrain model:
   //***
   ossimEcefRay ray;
   imagingRay1(image_point, ray);
   ossimElevManager::instance()->intersectRay(ray, gpt);

}
bool AbstractCorrect::insideImage(const ossimDpt& p)
{
	/*          return( (p.u>=(0.0-FLT_EPSILON)) && */
	/*                  (p.u<=(double)(theImageSize.u-(1-FLT_EPSILON))) && */
	/*                  (p.v>=(0.0-FLT_EPSILON)) && */
	/*                  (p.v<=(double)(theImageSize.v-(1-FLT_EPSILON))) ); */
	// if it's close to the edge we will consider it inside the image
	//
	return theImageClipRect.pointWithin(p, 2.0);
}

void AbstractCorrect::lineSampleHeightToWorld(const ossimDpt& image_point,
                                              const ossim_float64& heightEllipsoid,
                                              ossimGpt& worldPoint)
{
/*
   if (!insideImage1(image_point))
   {
//      if(theSeedFunction.valid())
      {
         theSeedFunction->lineSampleToWorld(image_point, worldPoint);
      }

 //     else
      {
//         worldPoint = extrapolate(image_point, heightEllipsoid);
      }
 //     if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSpot5Model::lineSampleHeightToWorld(): returning..." << std::endl;
   
	 return;
   }
    */ 
   //***
   // First establish imaging ray from image point:
   //***
   ossimEcefRay imaging_ray;
   imagingRay1(image_point, imaging_ray);
   ossimEcefPoint Pecf (imaging_ray.intersectAboveEarthEllipsoid(heightEllipsoid));
   worldPoint = ossimGpt(Pecf);
}

//ossimGpt extrapolate (const ossimDpt& imagePoint,
//                                        const double&   height)
ossimGpt AbstractCorrect::extrapolate (const ossimDpt& imagePoint)
{
   theExtrapolateImageFlag = true;
 //  if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) <<  "DEBUG ossimSensorModel::extrapolate: entering... " << std::endl;

   //---
   // If image point supplied has NaN components, return now with a NaN point.
   // This prevents an infinite recursion between model worldToLineSample
   // and this method:
   //---
   if (imagePoint.hasNans())
   {
      theExtrapolateImageFlag = false;
 //     if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::extrapolate: returning..." << std::endl;
      return ossimGpt(ossim::nan(), ossim::nan(), ossim::nan());
   }

//   if(theSeedFunction.valid())
   {
      ossimGpt wpt;

      theSeedFunction->lineSampleToWorld(imagePoint, wpt);
      theExtrapolateImageFlag = false;
      return wpt;
   }
   //***
   // Determine which edge is intersected by the radial, and establish
   // intersection:
   //***
   ossimGpt gpt;
   ossimDpt edgePt (imagePoint);
   ossimDpt image_center (theRefImgPt);
   theImageClipRect.clip(image_center, edgePt);

   //***
   // Need edgePt relative to image center. Compute an epsilon perturbation in
   // the direction of edgePt for later computing directional derivative,
   // and back out the offset to origin:
   //***
   ossimDpt deltaPt (edgePt - image_center);
   ossimDpt epsilon (deltaPt/deltaPt.length());
   edgePt -= epsilon;  // insure that we are inside the image
   ossimDpt edgePt_prime (edgePt - epsilon); // epsilon=1pixel
       
   //***
   // Establish ground point corresponding to edge point and edgePt+epsilon:
   //***
   ossimGpt edgeGP;
   ossimGpt edgeGP_prime;

//   if (ossim::isnan(height))
   {
      lineSampleToWorld(edgePt, edgeGP);
      lineSampleToWorld(edgePt_prime, edgeGP_prime);
   }
//   else
   {
//      lineSampleHeightToWorld(edgePt, height, edgeGP);
//      lineSampleHeightToWorld(edgePt_prime, height, edgeGP_prime);
   }
   
   //***
   // Compute approximate directional derivatives of lat and lon along radial
   // at the edge:
   //***
   double dpixel    = (edgePt-edgePt_prime).length();
   double dlat_drad = (edgeGP.lat - edgeGP_prime.lat)/dpixel;
   double dlon_drad = (edgeGP.lon - edgeGP_prime.lon)/dpixel;

   //***
   // Now extrapolate to image point of interest:
   //***
   double delta_pixel = (imagePoint - edgePt).length();

   gpt.lat = edgeGP.lat + dlat_drad*delta_pixel;
   gpt.lon = edgeGP.lon + dlon_drad*delta_pixel;
//   if ( ossim::isnan(height) )
   {
      gpt.hgt = ossimElevManager::instance()->getHeightAboveEllipsoid(gpt);
   }
 //  else
   {
 //     gpt.hgt = height;
   }
   theExtrapolateImageFlag = false;
 //  if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::extrapolate: returning..." << std::endl;
   return gpt;
}


void AbstractCorrect::worldToLineSample(const ossimGpt& worldPoint,
                                         ossimDpt&       ip) 
{
   //   static bool recursionFlag = false;

   static const double PIXEL_THRESHOLD    = .1; // acceptable pixel error
   static const int    MAX_NUM_ITERATIONS = 20;


   if(worldPoint.isLatNan()||
      worldPoint.isLonNan())
     {
       ip.makeNan();
       return;
     }
      
   //***
   // First check if the world point is inside bounding rectangle:
   //***
   int iters = 0;
   ossimDpt wdp (worldPoint);
   //   if ((!recursionFlag)&&!(theBoundGndPolygon.pointWithin(wdp)))

/*
   if((theBoundGndPolygon.getNumberOfVertices() > 0)&&
      (!theBoundGndPolygon.hasNans()))
   {
      if (!(theBoundGndPolygon.pointWithin(wdp)))
      {
//       if(theSeedFunction.valid())
         {
            theSeedFunction->worldToLineSample(worldPoint, ip);
         }

         return;
      }         
   }
*/

   //***
   // Substitute zero for null elevation if present:
   //***
   double height = worldPoint.hgt;
   if ( ossim::isnan(height) )
   {
      height = 0.0;
   }

   if ( height < -9998.0 )
   {
	   height = 0.0;
   }
   //
   // Utilize iterative scheme for arriving at image point. Begin with guess
   // at image center:
   //
//   if(theSeedFunction.valid())
   {
      theSeedFunction->worldToLineSample(worldPoint, ip);
   }
/*
   else
   {
      ip.u = theRefImgPt.u;
      ip.v = theRefImgPt.v;
   }
 */

   ossimDpt ip_du;
   ossimDpt ip_dv;

   ossimGpt gp, gp_du, gp_dv;
   double dlat_du, dlat_dv, dlon_du, dlon_dv;
   double delta_lat, delta_lon, delta_u, delta_v;
   double inverse_norm;
   bool done = false;
   //***
   // Begin iterations:
   //***
   do
   {
      //***
      // establish perturbed image points about the guessed point:
      //***
      ip_du.u = ip.u + 1.0;
      ip_du.v = ip.v;
      ip_dv.u = ip.u;
      ip_dv.v = ip.v + 1.0;
      
      //***
      // Compute numerical partials at current guessed point:
      //***
      lineSampleHeightToWorld(ip,    height, gp);
      lineSampleHeightToWorld(ip_du, height, gp_du);
      lineSampleHeightToWorld(ip_dv, height, gp_dv);

      if(gp.isLatNan() || gp.isLonNan())
      {
         gp = extrapolate(ip);
      }
      if(gp_du.isLatNan() || gp_du.isLonNan())
      {
         gp_du = extrapolate(ip_du);
      }
      if(gp_dv.isLatNan() | gp_dv.isLonNan())
      {
         gp_dv = extrapolate(ip_dv);       
      }
	  
      dlat_du = gp_du.lat - gp.lat; //e
      dlon_du = gp_du.lon - gp.lon; //g
      dlat_dv = gp_dv.lat - gp.lat; //f
      dlon_dv = gp_dv.lon - gp.lon; //h
      
      //
      // Test for convergence:
      //
      delta_lat = worldPoint.lat - gp.lat;
      delta_lon = worldPoint.lon - gp.lon;


      //
      // Compute linearized estimate of image point given gp delta:
      //
      inverse_norm = dlat_dv*dlon_du - dlat_du*dlon_dv; // fg-eh
      
      if (!ossim::almostEqual(inverse_norm, 0.0, DBL_EPSILON))
      {
         delta_u = (-dlon_dv*delta_lat + dlat_dv*delta_lon)/inverse_norm;
         delta_v = ( dlon_du*delta_lat - dlat_du*delta_lon)/inverse_norm;
         ip.u += delta_u;
         ip.v += delta_v;
      }
      else
      {
         delta_u = 0;
         delta_v = 0;
      }
      done = ((fabs(delta_u) < PIXEL_THRESHOLD)&&
              (fabs(delta_v) < PIXEL_THRESHOLD));
      iters++;
   } while ((!done) &&
             (iters < MAX_NUM_ITERATIONS));
//    } while (((fabs(delta_u) > PIXEL_THRESHOLD) ||
//              (fabs(delta_v) > PIXEL_THRESHOLD)) &&
//             (iters < MAX_NUM_ITERATIONS));

   //***
   // Note that this error mesage appears only if max count was reached while
   // iterating. A linear (no iteration) solution would finish with iters =
   // MAX_NUM_ITERATIONS + 1:
   //***
   if (iters >= MAX_NUM_ITERATIONS)
   {
//       std::cout << "MAX ITERATION!!!" << std::endl;
//       std::cout << "delta_u = "   << delta_u
//                 << "\ndelta_v = " << delta_v << "\n";
   }
   else
   {
//       std::cout << "ITERS === " << iters << std::endl;
   }
//    std::cout << "iters = " << iters << "\n";
   //***
   // The image point computed this way corresponds to full image space.
   // Apply image offset in the case this is a sub-image rectangle:
   //***

 //   ip -= theSubImageOffset;

   return;
}

int AbstractCorrect::LoadTime(const char* lineTimefile)
{
	LineTime = new double[ImgSize];
	double time;
	int numline1,numline2;

	char str[40];
	ifstream ftime(lineTimefile,ios::in);
	if (!ftime)
	{
		cout<<"linetime file is not generated"<<endl;
	}
	for (int i = 0;i < ImgSize; i++)
	{
		ftime.getline(str,40);
		sscanf(str,"%d %d %lf",&numline1,&numline2,&time);
		*(LineTime+i) = time;
	
	}
	theLineSamplingPeriod = (*(LineTime+ImgSize-1) - *LineTime)/ImgSize;
	ftime.close();
	return 0;
}

// loadTime for iecas ZY3A 20150705
/*
int AbstractCorrect::LoadTime(const char* lineTimefile)
{
	
	double time,interval;
	int bits,bytes;
	int numline;
	int numsline;
	int startTime;

	char str[100];
	ifstream ftime(lineTimefile,ios::in);
	if (!ftime)
	{
		cout<<"linetime file is not generated"<<endl;
	}
	
	ftime.getline(str,40);
	sscanf(str,"%d",&numsline);
	numsline++;
	LineTime = new double[numsline];

	ftime.getline(str,40);
	sscanf(str,"%d",&startTime);

	for (int i = 0;i < numsline; i++)
	{
		ftime.getline(str,100);
		sscanf(str,"%d %lf %lf %d %d",&numline,&time,&interval,&bits,&bytes);
		*(LineTime+i) = startTime + time;
	}
	theLineSamplingPeriod = (*(LineTime+numsline-1) - *LineTime)/numsline;
	ftime.close();
	return 0;
}
*/
// loadTime for iecas GF 20150706


int AbstractCorrect::imagingRay1(const ossimDpt& image_point,
			   ossimEcefRay& image_ray) 
{
	bool runtime_dbflag = 0;
	NEWMAT::Matrix satToOrbit,camToSat,satToEci,eciToEcr;

	ossimDpt iPt = image_point;

	//
	// 1. Establish time of line imaging :  
	//
	double t_line = getTimeLine(iPt.line);

	//
	// 2. Interpolate ephemeris position and velocity (in ECF):
	//
	
//	ossimEcefPoint  tempEcefPoint;
	ossimEcefPoint  P_ecf;
	getPositionEcf(t_line, P_ecf);
//	getVelocityEcf(t_line, tempEcefPoint);
//	ossimEcefVector V_ecf(tempEcefPoint.x(),
//		tempEcefPoint.y(),
//		tempEcefPoint.z());

	//
	// 3. Establish the look direction in Vehicle LSR space (S_sat).
	//    ANGLES IN RADIANS
	//
	ossim_float64 Psi_x;
	getPixelLookAngleX(iPt.samp, Psi_x);
	ossim_float64 Psi_y;
	getPixelLookAngleY(iPt.samp, Psi_y);



//	ossimColumnVector3d u_sat (-tan(Psi_y), tan(Psi_x), -(1.0 ));

	ossimColumnVector3d u_cam (-tan(Psi_x), -tan(Psi_y), 1.0 );


//	u_cam = u_cam.unit();
	//安装矩阵
	computeCamToSatRotation(camToSat);

//	computePianzhiRotation(pianzhi);

	ossimColumnVector3d u_sat = ((camToSat*u_cam)).unit();
	

	computeSatToEciRotation(satToEci, t_line);

	ossimColumnVector3d u_eci = (satToEci*u_sat).unit();


	computeEciToEcrRotation(eciToEcr,t_line);

	ossimColumnVector3d u_ecr  = (eciToEcr*u_eci).unit();	

	image_ray = ossimEcefRay(P_ecf, ossimEcefVector(u_ecr[0], u_ecr[1], u_ecr[2]));


	return 0;
}


xmlXPathObjectPtr AbstractCorrect::get_nodeset(xmlDocPtr doc, const xmlChar *szXpath) 
{
	xmlXPathContextPtr context;	    //XPATH上下文指针
	xmlXPathObjectPtr result;		//XPATH对象指针，用来存储查询结果
	
	context = xmlXPathNewContext(doc);		//创建一个XPath上下文指针
	if (context == NULL) 
	{	
		printf("context is NULL\n");
		return NULL; 
	}
	
	result = xmlXPathEvalExpression(szXpath, context); //查询XPath表达式，得到一个查询结果
	xmlXPathFreeContext(context);				//释放上下文指针
	if (result == NULL) 
	{
		printf("xmlXPathEvalExpression return NULL\n"); 
		return NULL;  
	}
	
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))   //检查查询结果是否为空
	{
		xmlXPathFreeObject(result);
		printf("nodeset is empty\n");
		return NULL;
	}
	
	return result;	
}

double AbstractCorrect::getValue(xmlNodePtr node,char *item)
{
	double value = 0;
	xmlNodePtr curSonNode = node->children;
	while (curSonNode!=NULL)
	{
		if ((!xmlStrcmp(curSonNode->name, (const xmlChar *)item)))
		{
			sscanf((char*)xmlNodeGetContent(curSonNode),"%lf",&value);
			return value;
		}
		else
		curSonNode = curSonNode->next;
	}
	cout<<"Can not find"<<item<<endl;
	return 0;
}

int AbstractCorrect::LoadXml(const char *szDocName)
{

	thePosEcfSamples.clear();
	theVelEcfSamples.clear();
	theEphSampTimes.clear();

	theAttitudeSamples.clear();
	theAttSampTimes.clear();

	ossimDpt3d VP;
	ossimDpt3d VV;
	ossimDpt3d V;
	double time;

	xmlDocPtr doc = NULL;			 //定义解析文档指针 
	xmlNodePtr curNode = NULL,curSonNode = NULL;		 //定义结点指针(你需要它为了在各个结点间移动) 

//	char *szDocName = NULL;
/*	if (argc <= 1)  
	{
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}*/
//	szDocName = "gf02_pan.xml";



	doc = xmlReadFile(szDocName,"GB2312",XML_PARSE_RECOVER);  //解析文件 
	
	if (NULL == doc) 
	{ 	
		fprintf(stderr,"Document not parsed successfully. \n"); 	
		return -1; 
	} 
	
//	xmlChar *szXpath =BAD_CAST ("/root/node2[@attribute='yes']"); 
	xmlChar *szXpath =BAD_CAST ("/AuxInfo/GpsParam/GpsData"); 	
	xmlXPathObjectPtr app_result = get_nodeset(doc,szXpath);  //查询并得到结果
	
	if (NULL == app_result) 
	{
		printf("app_result is NULL\n"); 
		return -1;
	}
	xmlChar *szValue = NULL;

	if(app_result) 
	{
		xmlNodeSetPtr nodeset = app_result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) 
		{
			curNode = nodeset->nodeTab[i];  

			if(curNode != NULL) 
			{				
				time = getValue(curNode,"GpsTime");
				VP.x = getValue(curNode,"PosX");
				VP.y = getValue(curNode,"PosY");
				VP.z = getValue(curNode,"PosZ");
				VV.x = getValue(curNode,"VelX");
				VV.y = getValue(curNode,"VelY");
				VV.z = getValue(curNode,"VelZ");
			}
			theEphSampTimes.push_back(time);
			thePosEcfSamples.push_back(VP);
			theVelEcfSamples.push_back(VV);
		}
		xmlXPathFreeObject (app_result);
	}

	/*
	szXpath =BAD_CAST ("/AuxInfo/AttParam/AttData"); 	
	app_result = get_nodeset(doc,szXpath);  //查询并得到结果

	if (NULL == app_result) 
	{
		printf("app_result is NULL\n"); 
		return -1;
	}


	if(app_result) 
	{
		xmlNodeSetPtr nodeset = app_result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) 
		{
			curNode = nodeset->nodeTab[i];  

			if(curNode != NULL) 
			{				
				time = getValue(curNode,"AttTime");

				V.x = getValue(curNode,"Pitch");
				V.y = getValue(curNode,"Roll");
				V.z = getValue(curNode,"Yaw");

			}
			theAttSampTimes.push_back(time);
			theAttitudeSamples.push_back(V);

		}
		xmlXPathFreeObject (app_result);
	}
	*/
	xmlFreeDoc(doc);
	return 0;
}

int AbstractCorrect::LoadAtt(const char *szDocName){
	return 0;
}

int AbstractCorrect::LoadPoint(const char *szDocName){

	thePointsPosition.clear();
	thePointsLLH.clear();

	ossimDpt pointPos;
	ossimGpt pointLLH;
	
	int id;
	char str[50];
	ifstream fpoint(szDocName,ios::in);
	if (!fpoint)
	{
		cout<<"point file is not generated"<<endl;
	}
	fpoint.getline(str,50);
	int nums = 0;
	sscanf(str,"%ld",&nums);
	for(int i = 0;i < nums; i++)
	{
		fpoint.getline(str,50);
		sscanf(str,"%ld %lf %lf %lf %lf %lf",&id,&pointPos.x,&pointPos.y,&pointLLH.lat,&pointLLH.lon,&pointLLH.hgt);
		thePointsPosition.push_back(pointPos);
		thePointsLLH.push_back(pointLLH);
	}

	fpoint.close();
	return 0;

}

int AbstractCorrect::LoadXmlQ(const char *szDocName)
{

	thePosEcfSamples.clear();
	theVelEcfSamples.clear();
	theEphSampTimes.clear();

//	theAttitudeSamples.clear();
	theQuater.clear();
	theAttSampTimes.clear();

	ossimDpt3d VP;
	ossimDpt3d VV;
	ossimDpt3d V;
	double time;

	xmlDocPtr doc = NULL;			 //定义解析文档指针 
	xmlNodePtr curNode = NULL,curSonNode = NULL;		 //定义结点指针(你需要它为了在各个结点间移动) 

//	char *szDocName = NULL;
/*	if (argc <= 1)  
	{
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}*/
//	szDocName = "gf02_pan.xml";



	doc = xmlReadFile(szDocName,"GB2312",XML_PARSE_RECOVER);  //解析文件 
	
	if (NULL == doc) 
	{ 	
		fprintf(stderr,"Document not parsed successfully. \n"); 	
		return -1; 
	} 
	
//	xmlChar *szXpath =BAD_CAST ("/root/node2[@attribute='yes']"); 
	xmlChar *szXpath =BAD_CAST ("/AuxInfo/GpsParam/GpsData"); 	
	xmlXPathObjectPtr app_result = get_nodeset(doc,szXpath);  //查询并得到结果
	
	if (NULL == app_result) 
	{
		printf("app_result is NULL\n"); 
		return -1;
	}
//	xmlChar *szValue = NULL;

	if(app_result) 
	{
		xmlNodeSetPtr nodeset = app_result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) 
		{
			curNode = nodeset->nodeTab[i];  

			if(curNode != NULL) 
			{				
				time = getValue(curNode,"GpsTime");
				VP.x = getValue(curNode,"PosX");
				VP.y = getValue(curNode,"PosY");
				VP.z = getValue(curNode,"PosZ");
				VV.x = getValue(curNode,"VelX");
				VV.y = getValue(curNode,"VelY");
				VV.z = getValue(curNode,"VelZ");
			}
			theEphSampTimes.push_back(time);
			thePosEcfSamples.push_back(VP);
			theVelEcfSamples.push_back(VV);
		}
		xmlXPathFreeObject (app_result);
	}

	szXpath =BAD_CAST ("/AuxInfo/AttParam/AttData"); 	
	app_result = get_nodeset(doc,szXpath);  //查询并得到结果

	if (NULL == app_result) 
	{
		printf("app_result is NULL\n"); 
		return -1;
	}
//	xmlChar *szValue = NULL;
	double lasttime=0.0;
	if(app_result) 
	{
		xmlNodeSetPtr nodeset = app_result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) 
		{
			curNode = nodeset->nodeTab[i];  

			if(curNode != NULL) 
			{				
				time = getValue(curNode,"AttTime");
				V.x = getValue(curNode,"Q1");
				V.y = getValue(curNode,"Q2");
				V.z = getValue(curNode,"Q3");
			}
			if(time!=lasttime){
			theAttSampTimes.push_back(time);
			theQuater.push_back(V);
			}
			lasttime = time;

		}
		xmlXPathFreeObject (app_result);
	}


	xmlFreeDoc(doc);
	return 0;
}

int AbstractCorrect::LoadParam(const char *szDocName){

	pitch = 0;
	roll = 0;
	yaw = 0;

	c0 = 0;
	c1 = 0;
	c2 = 0;
	c3 = 0;

	d0 = 0;
	d1 = 0;
	d2 = 0;
	d3 = 0;

	char str[50];
	char name[10];
	ifstream fparam(szDocName,ios::in);
	if (!fparam)
	{
		cout<<"param file is not generated"<<endl;
	}
	double temp = 0;
	
	 while (!fparam.eof() )  
	{
		fparam.getline(str,50);
		sscanf(str,"%s %lf",&name,&temp);
//		thePointsPosition.push_back(pointPos);
//		thePointsLLH.push_back(pointLLH);
		if (!strcmp(name,"pitch"))
		{
			pitch = temp;
		}
		else if (!strcmp(name,"roll"))
		{
			roll = temp;
		}
		else if (!strcmp(name,"yaw"))
		{
			yaw = temp;
		}
		else if (!strcmp(name,"c0"))
		{
			c0 = temp;
		}
		else if (!strcmp(name,"c1"))
		{
			c1 = temp;
		}
		else if (!strcmp(name,"c2"))
		{
			c2 = temp;
		}
		else if (!strcmp(name,"c3"))
		{
			c3 = temp;
		}
		else if (!strcmp(name,"d0"))
		{
			d0 = temp;
		}
		else if (!strcmp(name,"d1"))
		{
			d1 = temp;
		}
		else if (!strcmp(name,"d2"))
		{
			d2 = temp;
		}
		else if (!strcmp(name,"d3"))
		{
			d3 = temp;
		}
	}

	fparam.close();
	return 0;

}

int AbstractCorrect::saveParam(const char *szDocName){
	ofstream outfile(szDocName); 
	if(!outfile){  
		cout << "Unable to open outfile";  
		exit(1); // terminate with error  
	}  

	outfile << "pitch  " <<pitch << endl;
	outfile << "roll  " <<roll << endl;
	outfile << "yaw  " <<yaw<< endl;
	outfile <<endl;
	outfile <<"c0 "<<c0<<endl;
	outfile <<"c1 "<<c1<<endl;
	outfile <<"c2 "<<c2<<endl;
	outfile <<"c3 "<<c3<<endl;
	outfile <<endl;
	outfile <<"d0 "<<d0<<endl;
	outfile <<"d1 "<<d1<<endl;
	outfile <<"d2 "<<d2<<endl;
	outfile <<"d3 "<<d3<<endl;

	outfile.close();  
}

int AbstractCorrect::setFileName(FileName &filename){
	linetimeName = filename.linetimeName;
	metaName = filename.metaName;
	pointName = filename.pointName;
	return 0;
}

//this is a test