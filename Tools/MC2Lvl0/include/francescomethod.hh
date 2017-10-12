/*
 * examplemethod.hh
 *
 *
 *
 */


#ifndef FRANCESCOMETHOD_HH
#define FRANCESCOMETHOD_HH

#include <calomev2adcmethod.hh>

class FrancescoMethod: public calomev2adcmethod
{
   public:
      FrancescoMethod(std::string datacardname);
      short adcFromMev(float mev, int sensor);


   private:

   struct PMTnumbers {
        PMTenum index;
        float pedMean;
        float pedSigma;
        float maxPeak;
        float mevPeak;
        bool  isScint;
        float mev2adc;
        float a=0;
        float b=0;
    };

   std::array<PMTnumbers, NPMT>  PMTs;

   void init();
   void convertDatacard();
   void addMevPeak();
   void computeMev2ADCratio();
	

};

#endif 
