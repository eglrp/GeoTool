//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimBilinearProjection.cpp 17497 2010-06-02 01:43:43Z gpotts $

#include <sstream>
using namespace std;

#include "ossimBilinearProjectionMy.h"
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>

ossimBilinearProjectionMy::ossimBilinearProjectionMy(const ossimDpt& ul,
                                         const ossimDpt& ur,
                                         const ossimDpt& lr,
                                         const ossimDpt& ll,
                                         const ossimGpt& ulg,
                                         const ossimGpt& urg,
                                         const ossimGpt& lrg,
                                         const ossimGpt& llg)
   :
      theLineSamplePt(4),
      theGeographicPt(4),
      theLatFit(),
      theLonFit()
{
   theLineSamplePt[0] = ul;
   theLineSamplePt[1] = ur;
   theLineSamplePt[2] = lr;
   theLineSamplePt[3] = ll;

   theGeographicPt[0] = ulg;
   theGeographicPt[1] = urg;
   theGeographicPt[2] = lrg;
   theGeographicPt[3] = llg;
   
   initializeBilinear();
}
                       
ossimBilinearProjectionMy::~ossimBilinearProjectionMy()
{
}



void ossimBilinearProjectionMy::initializeBilinear()
{
   theInterpolationPointsHaveNanFlag = dPtsHaveNan()||gPtsHaveNan();
   theInverseSupportedFlag = true;
   if(!theInterpolationPointsHaveNanFlag)
   {
      theLatFit.clear();
      theLonFit.clear();
      theXFit.clear();
      theYFit.clear();
      
      const ossim_uint32 SIZE = (ossim_uint32)theLineSamplePt.size();
      if (SIZE != theGeographicPt.size())
      {
         return;
      }
      
      for (ossim_uint32 i = 0; i < SIZE; ++i)
      {
         theLatFit.addSample(theLineSamplePt[i].x,
                             theLineSamplePt[i].y,
                             theGeographicPt[i].latd());
         
         theLonFit.addSample(theLineSamplePt[i].x,
                             theLineSamplePt[i].y,
                             theGeographicPt[i].lond());
         
         theXFit.addSample(theGeographicPt[i].lond(),
                           theGeographicPt[i].latd(),
                           theLineSamplePt[i].x);
         theYFit.addSample(theGeographicPt[i].lond(),
                           theGeographicPt[i].latd(),
                           theLineSamplePt[i].y);
         
      }
      
      theLatFit.solveLS();
      theLonFit.solveLS();
      theXFit.solveLS();
      theYFit.solveLS();
 
   }
}

bool ossimBilinearProjectionMy::dPtsHaveNan() const
{
   if (theLineSamplePt.size() == 0)
   {
      return false;
   }

   vector<ossimDpt>::const_iterator i = theLineSamplePt.begin();
   while (i != theLineSamplePt.end())
   {
      if ( (*i).hasNans() )
      {
         return true;
      }
      ++i;
   }
   return false;
}

bool ossimBilinearProjectionMy::gPtsHaveNan() const
{
   //---
   // NOTE:  This method ignores nans in the height field.
   //---
   
   if (theGeographicPt.size() == 0)
   {
      return false;
   }

   vector<ossimGpt>::const_iterator i = theGeographicPt.begin();
   while (i != theGeographicPt.end())
   {
      if ( (*i).isLatNan() || (*i).isLonNan() )
      {
         return true;
      }
      ++i;
   }
   return false;
}

void ossimBilinearProjectionMy::gettheXFitLSParms(double& pa, double& pb_x, double& pc_y, double& pd_xy)
{
	theXFit.getLSParms(pa,pb_x,pc_y,pd_xy);
	
}

void ossimBilinearProjectionMy::gettheYFitLSParms(double& pa, double& pb_x, double& pc_y, double& pd_xy)
{
	theYFit.getLSParms(pa,pb_x,pc_y,pd_xy);
}