/*
 * trackeradc.hh
 *
 * Tracker Edep to ADC
 *
 */


#ifndef TRACKERADC_HH
#define TRACKERADC_HH

#include <vector>
#include <array>
#include <iostream>
#include "Edep_Pos.h"
#include "detector_const.hh"

class TrackerADC
{
   public:
      TrackerADC( std::vector<std::vector<Edep_Pos>> vvinfo) : allEpos(vvinfo)   {};
  std::array<short,NCHAN> GetStrips();


   private:
     enum trSides : uint {p0, n0, p1, n1, p2, n2, p3, n3, p4, n4, p5, n5 };
     // Next line : see https://stackoverflow.com/questions/8498300/allow-for-range-based-for-with-enum-classes
     const std::array<const trSides, 12> trSidesIterator{p0, n0, p1, n1, p2, n2, p3, n3, p4, n4, p5, n5};
     short int GetLocalStrip(trSides, TVector3);
     std::vector<std::vector<Edep_Pos>> allEpos;
     float Mev2ADCFactor(trSides side);
     std::array<short,SIDE_CHAN> getStripsForSide(trSides side);
     short TrimADC (float raw, float ped);

};

#endif /* TRACKERADC_HH */
