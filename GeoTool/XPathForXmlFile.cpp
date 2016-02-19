/********************************************************************
	created:	2007/11/15
	created:	15:11:2007   16:01
	filename:	XpathForXmlFile.cpp
	author:		Wang xuebin 
	depend:		libxml2.lib
	build:		nmake TARGET_NAME=XPathForXmlFile
	
	purpose:	ʹ��XPATH����xml�ĵ��еĽڵ�
*********************************************************************/

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <iostream>
using namespace std;

#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimColumnVector3d.h>

extern std::vector<ossimDpt3d>     thePosEcfSamples;
extern std::vector<ossimDpt3d>     theVelEcfSamples;
extern std::vector<ossim_float64>  theEphSampTimes;
extern std::vector<ossimDpt3d>     theAttitudeSamples; // x=pitch, y=roll, z=yaw
extern std::vector<ossim_float64>  theAttSampTimes;

extern std::vector<ossimDpt3d>     theQuater;

xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, const xmlChar *szXpath) 
{
	xmlXPathContextPtr context;	    //XPATH������ָ��
	xmlXPathObjectPtr result;		//XPATH����ָ�룬�����洢��ѯ���
	
	context = xmlXPathNewContext(doc);		//����һ��XPath������ָ��
	if (context == NULL) 
	{	
		printf("context is NULL\n");
		return NULL; 
	}
	
	result = xmlXPathEvalExpression(szXpath, context); //��ѯXPath���ʽ���õ�һ����ѯ���
	xmlXPathFreeContext(context);				//�ͷ�������ָ��
	if (result == NULL) 
	{
		printf("xmlXPathEvalExpression return NULL\n"); 
		return NULL;  
	}
	
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))   //����ѯ����Ƿ�Ϊ��
	{
		xmlXPathFreeObject(result);
		printf("nodeset is empty\n");
		return NULL;
	}
	
	return result;	
}

double getValue(xmlNodePtr node,char *item)
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

int LoadXml(const char *szDocName)
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

	xmlDocPtr doc = NULL;			 //��������ĵ�ָ�� 
	xmlNodePtr curNode = NULL,curSonNode = NULL;		 //������ָ��(����Ҫ��Ϊ���ڸ��������ƶ�) 

//	char *szDocName = NULL;
/*	if (argc <= 1)  
	{
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}*/
//	szDocName = "gf02_pan.xml";



	doc = xmlReadFile(szDocName,"GB2312",XML_PARSE_RECOVER);  //�����ļ� 
	
	if (NULL == doc) 
	{ 	
		fprintf(stderr,"Document not parsed successfully. \n"); 	
		return -1; 
	} 
	
//	xmlChar *szXpath =BAD_CAST ("/root/node2[@attribute='yes']"); 
	xmlChar *szXpath =BAD_CAST ("/AuxInfo/GpsParam/GpsData"); 	
	xmlXPathObjectPtr app_result = get_nodeset(doc,szXpath);  //��ѯ���õ����
	
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
	xmlFreeDoc(doc);
	return 0;
}

int LoadXmlQ(const char *szDocName)
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

	xmlDocPtr doc = NULL;			 //��������ĵ�ָ�� 
	xmlNodePtr curNode = NULL,curSonNode = NULL;		 //������ָ��(����Ҫ��Ϊ���ڸ��������ƶ�) 

//	char *szDocName = NULL;
/*	if (argc <= 1)  
	{
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}*/
//	szDocName = "gf02_pan.xml";



	doc = xmlReadFile(szDocName,"GB2312",XML_PARSE_RECOVER);  //�����ļ� 
	
	if (NULL == doc) 
	{ 	
		fprintf(stderr,"Document not parsed successfully. \n"); 	
		return -1; 
	} 
	
//	xmlChar *szXpath =BAD_CAST ("/root/node2[@attribute='yes']"); 
	xmlChar *szXpath =BAD_CAST ("/AuxInfo/GpsParam/GpsData"); 	
	xmlXPathObjectPtr app_result = get_nodeset(doc,szXpath);  //��ѯ���õ����
	
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
	app_result = get_nodeset(doc,szXpath);  //��ѯ���õ����

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
				time = getValue(curNode,"AttTime");
				V.x = getValue(curNode,"Q1");
				V.y = getValue(curNode,"Q2");
				V.z = getValue(curNode,"Q3");
			}
			theAttSampTimes.push_back(time);
			theQuater.push_back(V);

		}
		xmlXPathFreeObject (app_result);
	}


	xmlFreeDoc(doc);
	return 0;
}