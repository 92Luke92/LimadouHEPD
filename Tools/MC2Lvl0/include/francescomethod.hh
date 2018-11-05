/*
 * francescomethod.hh
 *
 *
 *
 */

#ifndef FRANCESCOMETHOD_HH
#define FRANCESCOMETHOD_HH

#include <prescaledmev2adcmethod.hh>
#include "TRandom3.h"

class FrancescoMethod: public prescaledmev2adcmethod
{
   public:
      FrancescoMethod(std::string datacardname,std::string pardatacardname);
      float adcFromMevNoPed(float mev, int sensor);
      void setBeamEnergy(int energy) {beamEnergy=energy;};

   protected:
   struct PMTnumbersFrancesco : public PMTnumbers {
	// linear function par. for 1) sigma smearing 1 , 2) sigma smearing 2 , 3) Mean Fixing
	float a1,a2,a3=0;
        float b1,b2,b3=0;
	float c3=0;
	PMTnumbersFrancesco(){};
  	PMTnumbersFrancesco(const PMTnumbers& other):PMTnumbers(other){};
    };


   private:

   TRandom3 * Rand;
   std::vector<std::vector<float>> pardatacard;

   std::array<PMTnumbersFrancesco, NPMT>  MyPMTs;

   void  initRandom(std::string pardatacardname);
   void  updatePMTs();
   void  convertParameterDatacard();
   short SmearADC(short ADC,int sensor);
};

#endif