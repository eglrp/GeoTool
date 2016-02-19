#include "CorrectFactory.h"
#include "AbstractCorrect.h"

#include "FileName.h"
#include "Params.h"

int Install(char *SatelliteName,char *SensorName,FileName &filename,Params &params)
{
//	char *jobID = NULL;

	AbstractCorrect *Correct = CorrectFactory::Creat(SatelliteName,SensorName);
	if (Correct!=NULL)
	{
		Correct->computeInstall(filename,params);
	} 
	else
	{
		cout<<"no solution!"<<endl;
		return -1;
	}
	delete Correct;
	return 0;
}


int Distortion(char *SatelliteName,char *SensorName,FileName &filename ,Params &params)
{

	AbstractCorrect *Correct = CorrectFactory::Creat(SatelliteName,SensorName);
	if (Correct!=NULL)
	{
		Correct->computeDistortion(filename,params);
	} 
	else
	{
		cout<<"no solution!"<<endl;
		return -1;
	}
	delete Correct;
	return 0;

}

