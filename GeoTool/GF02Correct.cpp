#include "GF02Correct.h"


GF02Correct::GF02Correct()
{
	
}


GF02Correct::~GF02Correct()
{

}
/*
int GF02Correct::LoadXml(const char *szDocName)
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
	char str[100];
	ifstream ftime(szDocName,ios::in);
	if (!ftime)
	{
		cout<<"meta file is not generated"<<endl;
	}

	ftime.getline(str,100);
	ftime.getline(str,100);
	ftime.getline(str,100);
	int nums;
	sscanf(str,"%d",&nums);
	char gps[4];
	for (int i = 0;i<nums;i++)
	{
		ftime.getline(str,100);
		sscanf(str,"%s %lf %lf %lf %lf %lf %lf %lf",&gps,&time,&VP.x,&VP.y,&VP.z,&VV.x,&VV.y,&VV.z);
		theEphSampTimes.push_back(time);
		thePosEcfSamples.push_back(VP);
		theVelEcfSamples.push_back(VV);
	}

	ftime.close();
	return 0;
}

int GF02Correct::LoadAtt(const char *szDocName)
{

	theQuater.clear();
	theAttSampTimes.clear();


	ossimDpt3d V;
	double time,temp;
	char str[110];
	ifstream ftime(szDocName,ios::in);
	if (!ftime)
	{
		cout<<"meta file is not generated"<<endl;
	}

	ftime.getline(str,110);
	ftime.getline(str,110);
	ftime.getline(str,110);
	int nums;
	sscanf(str,"%d",&nums);
	char Quater[7];
	for (int i = 0;i<nums;i++)
	{
		ftime.getline(str,110);
		sscanf(str,"%s %lf %lf %lf %lf %lf",&Quater,&time,&temp,&V.x,&V.y,&V.z);
		theAttSampTimes.push_back(time);
		theQuater.push_back(V);
	}

	ftime.close();
	return 0;
}

int GF02Correct::LoadTime(const char* lineTimefile)
{

	double time;
	int numline,unkown;
	int firstline;
	int numsline;
	int startline = 989616;  //<- 4698    //541142;
	int endline = 1018816;                // 610255;
//	int startline =   115521;  //<-7637  133585;  //7666   //541142;              
//	int endline =     144721;        //  162785;           // 610255;    
	int startTime;

	char str[100];
	ifstream ftime(lineTimefile,ios::in);
	if (!ftime)
	{
		cout<<"linetime file is not generated"<<endl;
	}

	ftime.getline(str,40);
	ftime.getline(str,40);
	sscanf(str,"%d %d %lf",&firstline,&unkown,&time);
//	sscanf(str,"%d %d %lf",&,&unkown,&time);
	numsline = startline - firstline;
	for (int i = 0;i<numsline-1;i++)
	{
		ftime.getline(str,40);
	}
	sscanf(str,"%d %d %lf",&numline,&unkown,&time);

	if (numline != startline - 1)
	{
		cout<<"wrong in load time"<<endl;
		return 0; 
	}
	numsline = endline -startline;
	LineTime = new double[numsline];

	for (int i = 0;i < numsline; i++)
	{
		ftime.getline(str,40);
		sscanf(str,"%d %d %lf",&numline,&unkown,&time);
		*(LineTime+i) = time;
	}
	theLineSamplingPeriod = (*(LineTime+numsline-1) - *LineTime)/numsline;
	ftime.close();
	return 0;
}
*/