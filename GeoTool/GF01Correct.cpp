#include "GF01Correct.h"





GF01Correct::GF01Correct()
{
	
}



GF01Correct::~GF01Correct()
{

}
/*
int GF01Correct::LoadXml(const char *szDocName)
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
	char str[200];
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
		ftime.getline(str,200);
		sscanf(str,"%s %lf %lf %lf %lf %lf %lf %lf",&gps,&time,&VP.x,&VP.y,&VP.z,&VV.x,&VV.y,&VV.z);
		theEphSampTimes.push_back(time);
		thePosEcfSamples.push_back(VP);
		theVelEcfSamples.push_back(VV);
	}

	ftime.close();
	return 0;
}

int GF01Correct::LoadTime(const char* lineTimefile)
{

	double time;
	int numline,unkown;
	int firstline;
	int numsline;
	int startline =965222;// 418888; for 0702
	int endline =1007684;// 461418;
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