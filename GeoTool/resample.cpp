#include<iostream>
using namespace std;
#include "resample.h"
//#include "Matrix.hpp"
//#include "Vector.hpp"
//using namespace gpstk;

#define CubicConvolution(distance1,distance2,distance3,f0,f1,f2,f3) \
	(             f1                                               \
	+ distance1*0.5*(f2 - f0)                                     \
	+ distance2*0.5*(2.0*f0 - 5.0*f1 + 4.0*f2 - f3)               \
	+ distance3*0.5*(3.0*(f1 - f2) + f3 - f0))

//int WeightTable[4][4][16];
//int MultiplyTable[4][4][16][1024];
double WeightTable[4][4][16];
double MultiplyTable[4][4][16][1024];

unsigned short getValue(int NumCol,int NumRow,int Width,int Heigh,int Bands,int nBand,int unsigned short *buf)
{
	int NumCol1 = min(max(NumCol,0),Width-1);
	int NumRow1 = min(max(NumRow,0),Heigh-1);

//	unsigned short result = *(buf+NumRow1*Width+NumCol1);
	unsigned short result = *(buf+nBand*Width*Heigh+NumRow1*Width+NumCol1);
	return result;
}


/*
unsigned short resample(double v,double u,unsigned short *data)
{
	unsigned short result = 0;
	Matrix<double> A(1,4,0.0);
	A(0,0) = cubic1To2(1+u);
	A(0,1) = cubic0To1(u);
	A(0,2) = cubic0To1(1-u);
	A(0,3) = cubic1To2(2-u);
	Matrix<double> B(4,4,0.0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			B(i,j) = *data++;
		}
	}
	Matrix<double> C(4,1,0.0);
	C(0,0) = cubic1To2(1+v);
	C(1,0) = cubic0To1(v);
	C(2,0) = cubic0To1(1-v);
	C(3,0) = cubic1To2(2-v);

	Matrix<double> AB(1,4,0.0);
	AB = A*B;

	Matrix<double> ABC(1,1,0.0);

	ABC = AB*C;

	result = ABC(0,0);

	return result; 
}

double cubic0To1(double x)
{
	return 1-2*pow(x,2)+pow(x,3);
}
double cubic1To2(double x)
{
	return 4-8*x+5*pow(x,2)-pow(x,3);
}




void generateCubicWeightTable()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{

			Matrix<double> A(4,1,0.0);
			A(0,0) = cubic1To2(1+(2*i+1)/8.0);
			A(1,0) = cubic0To1((2*i+1)/8.0);
			A(2,0) = cubic0To1(1-(2*i+1)/8.0);
			A(3,0) = cubic1To2(2-(2*i+1)/8.0);
			Matrix<double> B(1,4,0.0);
			B(0,0) = cubic1To2(1+(2*j+1)/8.0);
			B(0,1) = cubic0To1((2*j+1)/8.0);
			B(0,2) = cubic0To1(1-(2*j+1)/8.0);
			B(0,3) = cubic1To2(2-(2*j+1)/8.0);

			Matrix<double> AB(4,4,0.0);
			AB = A*B;
			for (int k = 0; k < 4; k++)
			{
				for (int l = 0;l < 4;l++)
				{
					//				WeightTable[i][j][k*4+l] = (int)(AB(k,l)*pow(2.0,18));
					WeightTable[i][j][k*4+l] = AB(k,l);
				}
			}
		}
	}
}
void generateMultiplyTable()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{	
			for (int k = 0; k < 16; k++)
			{
				for (int l = 0;l < 1024;l++)
				{
					MultiplyTable[i][j][k][l] = WeightTable[i][j][k]*l;					
				}
			}
		}
	}
}
void printWeighTable()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{	
			for (int k = 0; k < 4; k++)
			{
				for (int l = 0;l < 4;l++)
				{
					cout<<WeightTable[i][j][k*4+l]<<"  ";
				}
				cout<<endl;
			}
			cout<<endl;
			cout<<endl;
		}
	}

}

unsigned short resampleT(double v,double u,unsigned short *data)
{
	int U,V;
	if (u < 1.0/4)
		U = 0;
	else if(u < 1.0/2)
		U = 1;
	else if(u < 3.0/4)
		U = 2;
	else
		U = 3;

	if (v < 1.0/4)
		V = 0;
	else if(v < 1.0/2)
		V = 1;
	else if(v < 3.0/4)
		V = 2;
	else
		V = 3;
	double result = 0.0;
	//	int result = 0;
	for (int i = 0; i < 16; i++)
	{	
		//		result += WeightTable[U][V][4*i+j]*(*(data+4*i+j));
		result += MultiplyTable[U][V][i][*(data+i)];	
	}

	//    result = result/pow(2.0,18);  //2^18
	result = max(min(result,1024.0),0.0);
	return (unsigned short)result;
}

int InitResample()
{
	generateCubicWeightTable();
	generateMultiplyTable();
	return 0;
}

unsigned short resampleTT(double v,double u,int nXCol1,int nYRow1,int InWidth,int InHeigh,unsigned short *bufIn)
{
	unsigned short data[16];

	data[0] = getValue(nXCol1-1,nYRow1-1,InWidth,InHeigh,bufIn);
	data[1] = getValue(nXCol1,nYRow1-1,InWidth,InHeigh,bufIn);
	data[2] = getValue(nXCol1+1,nYRow1-1,InWidth,InHeigh,bufIn);
	data[3] = getValue(nXCol1+2,nYRow1-1,InWidth,InHeigh,bufIn);
	data[4] = getValue(nXCol1-1,nYRow1,InWidth,InHeigh,bufIn);
	data[5] = getValue(nXCol1,nYRow1,InWidth,InHeigh,bufIn);
	data[6] = getValue(nXCol1+1,nYRow1,InWidth,InHeigh,bufIn);
	data[7] = getValue(nXCol1+2,nYRow1,InWidth,InHeigh,bufIn);
	data[8] = getValue(nXCol1-1,nYRow1+1,InWidth,InHeigh,bufIn);
	data[9] = getValue(nXCol1,nYRow1+1,InWidth,InHeigh,bufIn);
	data[10]= getValue(nXCol1+1,nYRow1+1,InWidth,InHeigh,bufIn);
	data[11]= getValue(nXCol1+2,nYRow1+1,InWidth,InHeigh,bufIn);
	data[12]= getValue(nXCol1-1,nYRow1+2,InWidth,InHeigh,bufIn);
	data[13]= getValue(nXCol1,nYRow1+2,InWidth,InHeigh,bufIn);
	data[14]= getValue(nXCol1+1,nYRow1+2,InWidth,InHeigh,bufIn);
	data[15]= getValue(nXCol1+2,nYRow1+2,InWidth,InHeigh,bufIn);

	int U,V;
	if (u < 1.0/4)
		U = 0;
	else if(u < 1.0/2)
		U = 1;
	else if(u < 3.0/4)
		U = 2;
	else
		U = 3;

	if (v < 1.0/4)
		V = 0;
	else if(v < 1.0/2)
		V = 1;
	else if(v < 3.0/4)
		V = 2;
	else
		V = 3;
	double result = 0.0;
	//	int result = 0;
	for (int i = 0; i < 16; i++)
	{	
		//		result += WeightTable[U][V][4*i+j]*(*(data+4*i+j));
		result += MultiplyTable[U][V][i][*(data+i)];	
	}

	//	result = result/pow(2.0,18);  //2^18
	result = max(min(result,1024.0),0.0);
	return (unsigned short)result;
}
*/
unsigned short resampleGDAL(double dfDeltaX,double dfDeltaY,int nXCol1,int nYRow1,int InWidth,int InHeigh,int Bands,int nBand,unsigned short *bufIn)
{
	double  dfDeltaX2 = dfDeltaX * dfDeltaX;
	double  dfDeltaY2 = dfDeltaY * dfDeltaY;
	double  dfDeltaX3 = dfDeltaX2 * dfDeltaX;
	double  dfDeltaY3 = dfDeltaY2 * dfDeltaY;
	double  adfValue[4] = {0, 0, 0, 0};
	int     i;

	for ( i = -1; i < 3; i++ )
	{

		adfValue[i + 1] =CubicConvolution(dfDeltaX, dfDeltaX2, dfDeltaX3,
			(double)getValue(nXCol1-1,nYRow1+i,InWidth,InHeigh,Bands,nBand,bufIn),
			(double)getValue(nXCol1,nYRow1+i,InWidth,InHeigh,Bands,nBand,bufIn),
			(double)getValue(nXCol1+1,nYRow1+i,InWidth,InHeigh,Bands,nBand,bufIn),
			(double)getValue(nXCol1+2,nYRow1+i,InWidth,InHeigh,Bands,nBand,bufIn));
	}

	double dfValue = CubicConvolution(
		dfDeltaY, dfDeltaY2, dfDeltaY3,
		adfValue[0], adfValue[1], adfValue[2], adfValue[3]);

	unsigned short piValue;
	//NOT HJCCD 1023.0
    //HJCCD 255.0
	if ( dfValue < 0.0 )
		piValue = 0;
	else if ( dfValue > 1023.0 )
		piValue = 1023;
	else
		piValue = (unsigned short)floor(0.5 + dfValue);

	return piValue;

}