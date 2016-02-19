#ifndef RESAMPLE_H_
#define RESAMPLE_H_

unsigned short resample(double v,double u,unsigned short *data);
unsigned short resampleT(double u,double v,unsigned short *data);
double cubic0To1(double x);
double cubic1To2(double x);
//unsigned short getValue(int NumCol,int NumRow,int Width,int Heigh,unsigned short *buf);
unsigned short getValue(int NumCol,int NumRow,int Width,int Heigh,int Bands,int nBand,int unsigned short *buf);
void printWeighTable();
void generateCubicWeightTable();
void generateMultiplyTable();
int InitResample();
unsigned short resampleTT(double v,double u,int nXCol1,int nYRow1,int InWidth,int InHeigh,unsigned short *bufIn);
unsigned short resampleGDAL(double dfDeltaX,double dfDeltaY,int nXCol1,int nYRow1,int InWidth,int InHeigh,int,int,unsigned short *bufIn);
#endif