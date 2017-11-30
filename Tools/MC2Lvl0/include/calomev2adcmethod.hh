/*
 * calomev2adcmethod.hh
 *
 *
 *
 */


#ifndef CALOM2AMETHOD_HH
#define CALOM2AMETHOD_HH

#include "csv2fvec.hh"
#include "detector_const.hh"

class calomev2adcmethod
{
   public:
      calomev2adcmethod(std::string datacardname);
      virtual short adcFromMev(float mev, int sensor) = 0;
      void dumpDatacard();
      virtual void setBeamEnergy(int energy) {beamEnergy=energy;}

   protected:
      std::vector<std::vector<float>> datacard;
      int beamEnergy;
      short clipADC(int unclippedADC);

    private:
      virtual void init() = 0;

};

#endif /* CALOMEV2ADCMETHOD_HH */
