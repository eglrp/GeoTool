#ifndef  _PLOTFIT_H_
#define _PLOTFIT_H_

#include <cv.h>  
#include <cxcore.h>  
#include <highgui.h>  
#include <iostream>  

//inDataSet为输入样本数据的序列，类型为CvPoint2D32f  
//curveParam分别存储a0, a1, a2, a3  
bool curveFitting(CvSeq* inDataSet, float curveParam[4])  
{  
	if(!inDataSet)  
		return false;  
	int dataSetSize = inDataSet->total;  

	//系数矩阵存储位置  
	CvMat* cMatrix = cvCreateMat(4, 4, CV_32FC1);  
	cvZero(cMatrix);  

	//常量系数矩阵  
	CvMat* cstMatrix = cvCreateMat(4, 1, CV_32FC1);  
	cvZero(cstMatrix);  

	//(0, 0)  
	*((float*)CV_MAT_ELEM_PTR(*cMatrix, 0, 0)) = static_cast<float>(dataSetSize);  

	CvPoint2D32f* sample = 0;  
	float x = 0.f;  
	float x2 = 0.f;  
	float x3 = 0.f;  
	float x4 = 0.f;  
	float x5 = 0.f;  
	float x6 = 0.f;  
	float y = 0.f;  

	for(int i = 0; i < dataSetSize; ++i)  
	{  
		sample = (CvPoint2D32f*)cvGetSeqElem(inDataSet, i);  

		x = sample->x;  
		x2 = x * x;  
		x3 = x2 * x;  
		x4 = x3 * x;  
		x5 = x4 * x;  
		x6 = x5 * x;  


		*((float*)CV_MAT_ELEM_PTR(*cMatrix, 0, 1)) += x;   
		*((float*)CV_MAT_ELEM_PTR(*cMatrix, 0, 2)) += x2;  
		*((float*)CV_MAT_ELEM_PTR(*cMatrix, 0, 3)) += x3;  
		*((float*)CV_MAT_ELEM_PTR(*cMatrix, 1, 3)) += x4;  
		*((float*)CV_MAT_ELEM_PTR(*cMatrix, 2, 3)) += x5;  
		*((float*)CV_MAT_ELEM_PTR(*cMatrix, 3, 3)) += x6;  

		y = sample->y;  
		*((float*)CV_MAT_ELEM_PTR(*cstMatrix, 0, 0)) += y;  
		*((float*)CV_MAT_ELEM_PTR(*cstMatrix, 1, 0)) += y * x;  
		*((float*)CV_MAT_ELEM_PTR(*cstMatrix, 2, 0)) += y * x2;  
		*((float*)CV_MAT_ELEM_PTR(*cstMatrix, 3, 0)) += y * x3;  

	}  

	*((float*)CV_MAT_ELEM_PTR(*cMatrix, 1, 0)) = *((float*)CV_MAT_ELEM_PTR(*cMatrix, 0, 1));  
	*((float*)CV_MAT_ELEM_PTR(*cMatrix, 1, 1)) = *((float*)CV_MAT_ELEM_PTR(*cMatrix, 0, 2));  
	*((float*)CV_MAT_ELEM_PTR(*cMatrix, 1, 2)) = *((float*)CV_MAT_ELEM_PTR(*cMatrix, 0, 3));  

	*((float*)CV_MAT_ELEM_PTR(*cMatrix, 2, 0)) = *((float*)CV_MAT_ELEM_PTR(*cMatrix, 0, 2));  
	*((float*)CV_MAT_ELEM_PTR(*cMatrix, 2, 1)) = *((float*)CV_MAT_ELEM_PTR(*cMatrix, 1, 2));  
	*((float*)CV_MAT_ELEM_PTR(*cMatrix, 2, 2)) = *((float*)CV_MAT_ELEM_PTR(*cMatrix, 1, 3));  

	*((float*)CV_MAT_ELEM_PTR(*cMatrix, 3, 0)) = *((float*)CV_MAT_ELEM_PTR(*cMatrix, 0, 3));  
	*((float*)CV_MAT_ELEM_PTR(*cMatrix, 3, 1)) = *((float*)CV_MAT_ELEM_PTR(*cMatrix, 1, 3));  
	*((float*)CV_MAT_ELEM_PTR(*cMatrix, 3, 2)) = *((float*)CV_MAT_ELEM_PTR(*cMatrix, 2, 3));  


	CvMat* pcMatrix = cvCreateMat(4, 4, CV_32FC1);  
	cvZero(pcMatrix);  
	CvMat result = cvMat(4, 1, CV_32FC1, curveParam);  
	cvZero(&result);  

	cvInvert(cMatrix, pcMatrix, CV_LU);  

	cvMatMul(pcMatrix, cstMatrix, &result);  

	cvReleaseMat(&pcMatrix);  
	cvReleaseMat(&cstMatrix);  
	return true;  
}  

int plotfit(double xsample[], double ysample[], int nums,float curveParam[4])  
{  

	//新建序列存储输入样本集  
	CvMemStorage* storage = cvCreateMemStorage();  
	CvSeq* dataSet = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint2D32f), storage);  

	CvPoint2D32f sample;  
	for(int i = 0; i < nums; ++i)  
	{  
		sample.x = xsample[i];  
		sample.y = ysample[i];  

		cvSeqPush(dataSet, &sample);  
	}  

	//拟合  
	curveFitting(dataSet, curveParam);  

	cvReleaseMemStorage(&storage);  

	return 0;  
}  

#endif