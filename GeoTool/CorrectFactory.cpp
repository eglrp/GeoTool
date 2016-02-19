#include "CorrectFactory.h"


AbstractCorrect* CorrectFactory::Creat(char *SatelliteName,char *SensorName)
{

	if (!strcmp(SatelliteName,"ZY3A"))
	{
		if (!strcmp(SensorName,"MUX"))
		{
			return new ZY3AMUXCorrect();
		}
		else if(!strcmp(SensorName, "NAD"))
		{
			return new ZY3ANADCorrect();
		}
		else if (!strcmp(SensorName, "FWD"))
		{
			return new ZY3AFWDCorrect();
		}
		else if (!strcmp(SensorName,"BWD"))
		{
			return new ZY3ABWDCorrect();
		}
		else
		{
			cout<<"WRONG SensorName!"<<endl;
			return 0;
		}
	}
	else if (!strcmp(SatelliteName, "GF01"))
	{
		if (!strcmp(SensorName, "PAN"))
		{
			return new GF01PANCorrect();
		}
		else if(!strcmp(SensorName,"MS"))
		{
			return new GF01MSCorrect();
		}
		else if (!strcmp(SensorName, "WF"))
		{
			return new GF01WFCorrect();
		}
		else
		{
			cout<<"WRONG SensorName!"<<endl;
			return 0;
		}
	}	
	else if (!strcmp(SatelliteName ,"GF02"))
	{
		if (!strcmp(SensorName,"PAN"))
		{
			return new GF02PANCorrect();
		}
		else if(!strcmp(SensorName,"MS"))
		{
			return new GF02MSCorrect();
		}
		else
		{
			cout<<"WRONG SensorName!"<<endl;
			return 0;
		}
	}	
	else if (!strcmp(SatelliteName, "HJ1A"))
	{
		if (!strcmp(SensorName,"CCD"))
		{
			return new HJ1ACCDCorrect();
		}
		else
		{
			cout<<"WRONG SensorName!"<<endl;
			return 0;
		}
	}	
	else
	{
		cout<<"WRONG SatelliteName!"<<endl;
		return 0;
	}

}