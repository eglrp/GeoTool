//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimBilinearProjection.h 17497 2010-06-02 01:43:43Z gpotts $
#ifndef ossimBilinearProjectionMy_HEADER
#define ossimBilinearProjectionMy_HEADER

#include <vector>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/projection/ossimOptimizableProjection.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimLeastSquaresBilin.h>

class  ossimBilinearProjectionMy 
   
{
public:

   ossimBilinearProjectionMy(const ossimDpt& ul,
                           const ossimDpt& ur,
                           const ossimDpt& lr,
                           const ossimDpt& ll,
                           const ossimGpt& ulg,
                           const ossimGpt& urg,
                           const ossimGpt& lrg,
                           const ossimGpt& llg);

   //返回多项式参数
   void gettheXFitLSParms(double& pa, double& pb_x, double& pc_y, double& pd_xy);
   void gettheYFitLSParms(double& pa, double& pb_x, double& pc_y, double& pd_xy);
 
protected:
   virtual ~ossimBilinearProjectionMy();
   void initializeBilinear();

   /**
    * Checks theLineSamplePt for nans.
    * @return true if any point has a nan.
    * @note If theLineSamplePt size is 0 this returns false.
    */
   bool dPtsHaveNan() const;
   
   /**
    * Checks theGeographicPt for nans.
    * @return true if any point has a nan.
    * @note If theLineSamplePt size is 0 this returns false.
    */
   bool gPtsHaveNan() const;


   /**
    * @return The mid point of theLineSamplePts.
    */
   
   std::vector<ossimDpt>  theLineSamplePt;
   std::vector<ossimGpt>  theGeographicPt;
   ossimLeastSquaresBilin theLatFit;
   ossimLeastSquaresBilin theLonFit;
   ossimLeastSquaresBilin theXFit;
   ossimLeastSquaresBilin theYFit;

   bool theInterpolationPointsHaveNanFlag;
   bool theInverseSupportedFlag;



};

#endif /* #ifndef ossimBilinearProjection_HEADER */
