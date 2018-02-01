/*
 * calomev2adcmethod.hh
 *
 *
 *
 */


#ifndef CALOM2AMETHOD_HH
#define CALOM2AMETHOD_HH

#include <array>
#include <string>

#include "csv2fvec.hh"
#include "detector_const.hh"
#include "LCaloCalibration.hh"

class calomev2adcmethod
{
   public:
      calomev2adcmethod(std::string datacardname);
      short adcFromMev(float mev, int sensor);
      void dumpDatacard();
      virtual void setBeamEnergy(int energy) {beamEnergy=energy;}
      void setPedFromCalib(LCaloCalibration calocalib);
      void setPedFromDatacard(std::string dataname="laurentHGpeakshift.csv");

   protected:
      std::vector<std::vector<float>> datacard;
      virtual float adcFromMevNoPed(float mev, int sensor) = 0;
      int beamEnergy;
      short clipADC(int unclippedADC);
      short clipADC(float unclippedADC)  { return clipADC (static_cast<int> (unclippedADC));}
      LCaloCalibration calocalib;
      std::array<float, NPMT> pedestals;

    private:
      virtual void init() = 0;



};

#endif /* CALOMEV2ADCMETHOD_HH */
