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
      std::vector<short> GetStrips();


   private:
     enum trSides : uint {p0, n0, p1, n1, p2, n2, p3, n3, p4, n4, p5, n5 };
     // Next line : see https://stackoverflow.com/questions/8498300/allow-for-range-based-for-with-enum-classes
     const std::array<trSides, 12> trSidesIterator{p0, n0, p1, n1, p2, n2, p3, n3, p4, n4, p5, n5};
     short int GetLocalStrip(trSides, TVector3);
     std::vector<std::vector<Edep_Pos>> allEpos;
     float Mev2ADCFactor(trSides side);
     std::array<short, SIDE_CHAN> getStripsForSide(trSides side);
     short TrimADC (float raw, float ped);

     std::array<short, NCHAN> trPed={
 1350, 1320, 1340, 1330, 1350, 1350, 1330, 1330, 1340, 1370, 1340, 1340, 1350,
 1350, 1340, 1360, 1330, 1320, 1340, 1340, 1340, 1350, 1380, 1350, 1360, 1350,
 1340, 1360, 1350, 1350, 1350, 1340, 1350, 1340, 1330, 1340, 1340, 1360, 1340,
 1380, 1360, 1360, 1340, 1340, 1340, 1360, 1360, 1370, 1340, 1360, 1370, 1380,
 1360, 1370, 1380, 1390, 1370, 1380, 1360, 1390, 1380, 1380, 1400, 1370, 1390,
 1340, 1330, 1330, 1320, 1330, 1330, 1340, 1360, 1340, 1320, 1340, 1340, 1340,
 1340, 1320, 1330, 1320, 1310, 1330, 1350, 1330, 1340, 1320, 1330, 1310, 1330,
 1310, 1320, 1310, 1330, 1320, 1300, 1290, 1330, 1300, 1320, 1310, 1300, 1310,
 1310, 1300, 1330, 1310, 1300, 1330, 1310, 1300, 1340, 1310, 1330, 1330, 1310,
 1310, 1330, 1330, 1320, 1320, 1320, 1330, 1320, 1320, 1340, 1350, 1320, 1330,
 1340, 1350, 1310, 1340, 1330, 1350, 1320, 1340, 1340, 1330, 1310, 1340, 1310,
 1340, 1340, 1320, 1330, 1340, 1350, 1330, 1320, 1330, 1330, 1340, 1330, 1330,
 1350, 1340, 1360, 1350, 1370, 1360, 1350, 1360, 1360, 1340, 1360, 1340, 1380,
 1360, 1370, 1360, 1350, 1360, 1370, 1380, 1360, 1370, 1380, 1380, 1380, 1360,
 1370, 1380, 1390, 1380, 1380, 1400, 1400, 1390, 1390, 1420, 1340, 1330, 1380,
 1340, 1340, 1360, 1350, 1360, 1350, 1360, 1370, 1350, 1360, 1350, 1370, 1360,
 1360, 1330, 1350, 1360, 1340, 1370, 1350, 1360, 1350, 1350, 1370, 1350, 1330,
 1360, 1360, 1350, 1360, 1350, 1340, 1360, 1350, 1350, 1370, 1340, 1340, 1320,
 1360, 1340, 1340, 1360, 1370, 1360, 1340, 1330, 1340, 1360, 1350, 1360, 1340,
 1340, 1350, 1370, 1360, 1380, 1370, 1360, 1380, 1360, 1370, 1320, 1330, 1320,
 1320, 1320, 1330, 1310, 1290, 1320, 1320, 1330, 1310, 1330, 1320, 1340, 1320,
 1340, 1310, 1330, 1340, 1340, 1340, 1330, 1320, 1310, 1330, 1320, 1330, 1340,
 1320, 1330, 1330, 1330, 1330, 1300, 1340, 1310, 1350, 1340, 1330, 1330, 1320,
 1330, 1340, 1330, 1330, 1340, 1340, 1340, 1350, 1330, 1340, 1330, 1340, 1340,
 1340, 1340, 1340, 1350, 1340, 1330, 1340, 1330, 1320, 1300, 1310, 1300, 1280,
 1290, 1280, 1270, 1270, 1290, 1290, 1310, 1300, 1280, 1300, 1290, 1290, 1300,
 1290, 1290, 1290, 1300, 1290, 1290, 1290, 1280, 1300, 1310, 1300, 1310, 1290,
 1320, 1310, 1310, 1290, 1340, 1320, 1330, 1300, 1320, 1320, 1320, 1340, 1330,
 1330, 1330, 1340, 1330, 1350, 1340, 1350, 1370, 1350, 1340, 1340, 1370, 1350,
 1360, 1330, 1370, 1350, 1350, 1330, 1340, 1340, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360, 1360,
 1360, 1330, 1280, 1300, 1320, 1320, 1300, 1310, 1330, 1330, 1310, 1310, 1310,
 1310, 1290, 1330, 1320, 1310, 1320, 1300, 1300, 1320, 1320, 1350, 1330, 1320,
 1340, 1340, 1330, 1340, 1310, 1330, 1320, 1320, 1330, 1310, 1330, 1330, 1330,
 1320, 1330, 1330, 1290, 1310, 1320, 1320, 1340, 1340, 1330, 1330, 1340, 1340,
 1340, 1310, 1330, 1330, 1310, 1330, 1320, 1320, 1300, 1320, 1320, 1330, 1310,
 1300, 1340, 1350, 1340, 1340, 1330, 1360, 1340, 1330, 1330, 1350, 1370, 1360,
 1360, 1340, 1340, 1350, 1330, 1320, 1340, 1340, 1350, 1340, 1340, 1310, 1340,
 1330, 1350, 1340, 1360, 1350, 1340, 1320, 1340, 1330, 1320, 1340, 1340, 1350,
 1310, 1360, 1340, 1340, 1340, 1320, 1340, 1340, 1320, 1320, 1320, 1330, 1340,
 1350, 1340, 1340, 1350, 1320, 1350, 1350, 1350, 1340, 1360, 1340, 1350, 1360,
 1290, 1310, 1310, 1300, 1310, 1330, 1310, 1290, 1330, 1320, 1330, 1300, 1320,
 1330, 1320, 1320, 1330, 1310, 1310, 1340, 1350, 1300, 1340, 1310, 1340, 1330,
 1310, 1330, 1330, 1320, 1320, 1320, 1320, 1340, 1340, 1350, 1320, 1330, 1310,
 1320, 1310, 1320, 1320, 1330, 1330, 1340, 1340, 1330, 1320, 1340, 1320, 1320,
 1330, 1310, 1330, 1310, 1290, 1340, 1330, 1320, 1330, 1350, 1330, 1340, 1370,
 1340, 1360, 1340, 1340, 1360, 1360, 1360, 1350, 1350, 1390, 1340, 1370, 1360,
 1360, 1350, 1380, 1370, 1350, 1350, 1370, 1360, 1360, 1340, 1350, 1360, 1350,
 1360, 1360, 1360, 1360, 1340, 1320, 1350, 1350, 1330, 1340, 1340, 1330, 1340,
 1340, 1360, 1350, 1350, 1330, 1340, 1320, 1330, 1310, 1330, 1330, 1340, 1350,
 1330, 1320, 1320, 1340, 1330, 1320, 1320, 1340, 1340, 1340, 1320, 1350, 1360,
 1360, 1350, 1320, 1360, 1370, 1310, 1330, 1340, 1350, 1340, 1320, 1340, 1330,
 1320, 1340, 1330, 1320, 1340, 1350, 1340, 1330, 1320, 1340, 1340, 1320, 1330,
 1330, 1340, 1330, 1340, 1310, 1340, 1320, 1340, 1360, 1350, 1340, 1340, 1360,
 1380, 1350, 1350, 1350, 1360, 1350, 1350, 1370, 1350, 1360, 1350, 1360, 1370,
 1340, 1330, 1380, 1340, 1350, 1350, 1350, 1360, 1330, 1360, 1320, 1340, 1320,
 1340, 1340, 1310, 1340, 1320, 1330, 1310, 1320, 1320, 1310, 1310, 1300, 1330,
 1310, 1320, 1340, 1350, 1310, 1310, 1320, 1320, 1310, 1330, 1330, 1330, 1350,
 1340, 1340, 1310, 1330, 1340, 1330, 1340, 1350, 1300, 1340, 1320, 1310, 1330,
 1330, 1340, 1330, 1320, 1340, 1320, 1310, 1330, 1330, 1340, 1330, 1340, 1340,
 1330, 1330, 1320, 1340, 1340, 1340, 1350, 1340, 1350, 1370, 1350, 1360, 1350,
 1360, 1360, 1340, 1360, 1360, 1320, 1340, 1340, 1360, 1360, 1340, 1360, 1340,
 1350, 1340, 1350, 1310, 1340, 1350, 1370, 1360, 1360, 1350, 1330, 1330, 1370,
 1360, 1360, 1380, 1350, 1370, 1340, 1390, 1360, 1380, 1380, 1380, 1380, 1360,
 1370, 1380, 1390, 1390, 1390, 1380, 1370, 1370, 1370, 1380, 1370, 1380, 1400,
 1370, 1410, 1390, 1370, 1380, 1340, 1400, 1390, 1370, 1370, 1340, 1350, 1370,
 1360, 1330, 1330, 1350, 1360, 1330, 1330, 1330, 1330, 1310, 1320, 1330, 1320,
 1310, 1320, 1310, 1310, 1340, 1300, 1310, 1320, 1320, 1340, 1340, 1300, 1330,
 1320, 1340, 1310, 1350, 1310, 1360, 1320, 1340, 1310, 1320, 1330, 1340, 1320,
 1330, 1330, 1340, 1360, 1350, 1330, 1340, 1330, 1310, 1320, 1340, 1360, 1340,
 1310, 1340, 1340, 1320, 1330, 1360, 1340, 1360, 1360, 1360, 1360, 1360, 1350,
 1350, 1370, 1370, 1350, 1350, 1380, 1360, 1340, 1360, 1360, 1370, 1350, 1330,
 1360, 1370, 1340, 1350, 1350, 1340, 1360, 1360, 1340, 1350, 1340, 1340, 1350,
 1340, 1360, 1340, 1350, 1340, 1340, 1330, 1360, 1340, 1330, 1330, 1370, 1330,
 1330, 1350, 1340, 1330, 1330, 1300, 1340, 1360, 1350, 1310, 1340, 1340, 1340,
 1350, 1340, 1330, 1340, 1320, 1350, 1380, 1370, 1360, 1380, 1340, 1370, 1370,
 1380, 1370, 1360, 1380, 1390, 1380, 1370, 1370, 1400, 1370, 1370, 1370, 1370,
 1360, 1380, 1360, 1350, 1360, 1370, 1370, 1370, 1370, 1340, 1370, 1350, 1380,
 1340, 1350, 1380, 1340, 1350, 1350, 1340, 1380, 1340, 1350, 1340, 1360, 1370,
 1370, 1340, 1370, 1360, 1370, 1350, 1340, 1340, 1330, 1360, 1360, 1380, 1360,
 1360, 1360, 1360, 1380, 1380, 1380, 1360, 1370, 1350, 1370, 1380, 1360, 1360,
 1380, 1350, 1370, 1350, 1350, 1370, 1380, 1370, 1370, 1380, 1360, 1360, 1350,
 1360, 1350, 1360, 1380, 1380, 1360, 1370, 1350, 1360, 1360, 1400, 1370, 1340,
 1370, 1370, 1370, 1360, 1350, 1360, 1360, 1350, 1360, 1350, 1380, 1380, 1360,
 1370, 1370, 1340, 1350, 1360, 1370, 1340, 1350, 1370, 1370, 1370, 1360, 1360,
 1350, 1380, 1370, 1360, 1350, 1350, 1350, 1340, 1370, 1330, 1360, 1350, 1350,
 1340, 1340, 1340, 1330, 1360, 1340, 1330, 1350, 1350, 1350, 1340, 1360, 1360,
 1350, 1360, 1350, 1370, 1340, 1330, 1350, 1390, 1360, 1360, 1390, 1370, 1350,
 1360, 1360, 1360, 1350, 1330, 1330, 1340, 1360, 1340, 1350, 1360, 1370, 1370,
 1320, 1340, 1360, 1350, 1350, 1340, 1350, 1370, 1360, 1360, 1370, 1350, 1380,
 1360, 1360, 1380, 1320, 1320, 1310, 1330, 1320, 1320, 1330, 1330, 1280, 1320,
 1300, 1310, 1330, 1340, 1310, 1320, 1310, 1320, 1330, 1330, 1290, 1320, 1320,
 1320, 1320, 1340, 1330, 1340, 1320, 1330, 1320, 1320, 1350, 1320, 1320, 1320,
 1340, 1330, 1330, 1330, 1320, 1330, 1330, 1370, 1340, 1330, 1340, 1320, 1320,
 1330, 1340, 1340, 1340, 1330, 1340, 1310, 1320, 1360, 1350, 1350, 1350, 1330,
 1330, 1340, 1300, 1320, 1310, 1320, 1320, 1310, 1300, 1320, 1330, 1330, 1310,
 1320, 1330, 1340, 1330, 1340, 1310, 1320, 1320, 1320, 1330, 1310, 1330, 1310,
 1290, 1340, 1330, 1320, 1320, 1340, 1330, 1330, 1300, 1330, 1330, 1330, 1350,
 1340, 1360, 1340, 1340, 1330, 1380, 1340, 1370, 1370, 1350, 1340, 1360, 1360,
 1370, 1360, 1350, 1350, 1370, 1340, 1340, 1360, 1340, 1380, 1370, 1370, 1340,
 1380, 1310, 1310, 1310, 1330, 1320, 1320, 1320, 1310, 1300, 1320, 1300, 1320,
 1310, 1310, 1300, 1310, 1310, 1320, 1300, 1320, 1300, 1310, 1320, 1310, 1290,
 1290, 1310, 1310, 1300, 1310, 1310, 1290, 1300, 1320, 1290, 1320, 1310, 1310,
 1320, 1310, 1300, 1330, 1310, 1320, 1330, 1330, 1310, 1320, 1310, 1310, 1310,
 1310, 1330, 1330, 1310, 1310, 1320, 1330, 1290, 1310, 1310, 1320, 1340, 1350,
 1330, 1340, 1330, 1320, 1310, 1310, 1310, 1320, 1310, 1340, 1320, 1340, 1320,
 1310, 1320, 1320, 1310, 1330, 1320, 1320, 1320, 1300, 1300, 1290, 1360, 1330,
 1350, 1330, 1310, 1300, 1330, 1330, 1330, 1320, 1320, 1340, 1320, 1330, 1330,
 1330, 1320, 1340, 1330, 1340, 1310, 1340, 1310, 1320, 1330, 1330, 1360, 1330,
 1330, 1340, 1340, 1350, 1330, 1340, 1350, 1360, 1320, 1340, 1340, 1350, 1320,
 1310, 1350, 1310, 1330, 1310, 1330, 1340, 1320, 1320, 1340, 1340, 1310, 1330,
 1330, 1340, 1340, 1340, 1340, 1320, 1340, 1320, 1310, 1310, 1320, 1320, 1330,
 1330, 1320, 1340, 1340, 1310, 1340, 1330, 1320, 1340, 1320, 1340, 1330, 1340,
 1330, 1340, 1340, 1310, 1320, 1320, 1320, 1340, 1320, 1330, 1350, 1320, 1310,
 1310, 1330, 1330, 1330, 1340, 1320, 1300, 1310, 1330, 1310, 1310, 1360, 1340,
 1330, 1360, 1360, 1340, 1340, 1320, 1330, 1340, 1340, 1370, 1340, 1340, 1360,
 1360, 1350, 1350, 1350, 1360, 1350, 1360, 1340, 1330, 1330, 1350, 1330, 1350,
 1350, 1330, 1330, 1330, 1340, 1330, 1340, 1340, 1320, 1340, 1340, 1330, 1330,
 1330, 1340, 1330, 1300, 1310, 1330, 1310, 1300, 1320, 1330, 1300, 1320, 1330,
 1340, 1310, 1310, 1310, 1330, 1320, 1330, 1300, 1320, 1350, 1350, 1340, 1360,
 1370, 1350, 1370, 1350, 1370, 1360, 1370, 1350, 1360, 1350, 1370, 1380, 1340,
 1360, 1360, 1340, 1360, 1360, 1360, 1370, 1390, 1360, 1340, 1370, 1370, 1370,
 1390, 1340, 1370, 1370, 1360, 1340, 1370, 1360, 1370, 1360, 1370, 1360, 1350,
 1360, 1360, 1340, 1370, 1340, 1370, 1360, 1340, 1340, 1340, 1350, 1340, 1350,
 1330, 1360, 1330, 1340, 1340, 1360, 1330, 1330, 1330, 1360, 1340, 1340, 1360,
 1350, 1360, 1360, 1390, 1370, 1350, 1360, 1360, 1330, 1350, 1380, 1350, 1370,
 1360, 1360, 1350, 1380, 1360, 1350, 1380, 1370, 1350, 1340, 1340, 1360, 1370,
 1370, 1360, 1350, 1360, 1360, 1360, 1370, 1340, 1360, 1360, 1380, 1330, 1350,
 1340, 1350, 1340, 1340, 1360, 1320, 1350, 1360, 1340, 1350, 1330, 1340, 1350,
 1320, 1320, 1350, 1320, 1340, 1320, 1360, 1340, 1350, 1360, 1370, 1350, 1330,
 1350, 1350, 1370, 1380, 1360, 1340, 1360, 1350, 1340, 1360, 1360, 1350, 1350,
 1340, 1340, 1350, 1350, 1350, 1340, 1370, 1350, 1310, 1330, 1350, 1320, 1330,
 1340, 1370, 1330, 1340, 1330, 1340, 1350, 1340, 1350, 1330, 1340, 1340, 1350,
 1360, 1340, 1340, 1340, 1380, 1340, 1350, 1340, 1340, 1310, 1310, 1320, 1330,
 1350, 1360, 1330, 1360, 1350, 1340, 1360, 1320, 1340, 1330, 1330, 1310, 1340,
 1320, 1330, 1320, 1330, 1310, 1310, 1330, 1310, 1330, 1330, 1310, 1320, 1330,
 1330, 1340, 1340, 1330, 1330, 1340, 1340, 1350, 1330, 1340, 1310, 1310, 1340,
 1340, 1340, 1320, 1340, 1350, 1340, 1350, 1340, 1330, 1340, 1350, 1350, 1320,
 1320, 1340, 1360, 1330, 1340, 1330, 1370, 1340, 1350, 1340, 1360, 1350, 1340,
 1350, 1370, 1320, 1330, 1350, 1360, 1370, 1360, 1380, 1360, 1360, 1370, 1350,
 1370, 1360, 1360, 1360, 1370, 1360, 1390, 1360, 1360, 1380, 1370, 1390, 1370,
 1380, 1370, 1370, 1390, 1370, 1360, 1380, 1370, 1370, 1390, 1360, 1390, 1400,
 1390, 1360, 1380, 1350, 1390, 1390, 1380, 1400, 1370, 1370, 1370, 1340, 1380,
 1360, 1370, 1370, 1340, 1350, 1360, 1370, 1360, 1360, 1380, 1370, 1370, 1350,
 1370, 1350, 1380, 1360, 1370, 1410, 1400, 1410, 1390, 1380, 1380, 1380, 1370,
 1400, 1390, 1360, 1400, 1350, 1360, 1340, 1390, 1360, 1370, 1360, 1360, 1360,
 1360, 1340, 1370, 1360, 1350, 1350, 1350, 1360, 1360, 1380, 1370, 1390, 1370,
 1370, 1350, 1340, 1360, 1360, 1350, 1370, 1360, 1360, 1360, 1380, 1360, 1340,
 1370, 1360, 1360, 1350, 1360, 1380, 1360, 1360, 1360, 1370, 1350, 1360, 1390,
 1380, 1370, 1380, 1340, 1310, 1320, 1320, 1350, 1330, 1300, 1300, 1320, 1320,
 1340, 1330, 1310, 1330, 1330, 1320, 1310, 1320, 1330, 1340, 1310, 1340, 1310,
 1320, 1360, 1320, 1320, 1320, 1330, 1310, 1320, 1340, 1320, 1310, 1340, 1340,
 1340, 1330, 1330, 1340, 1330, 1360, 1340, 1340, 1370, 1340, 1360, 1340, 1330,
 1350, 1350, 1330, 1340, 1340, 1350, 1360, 1350, 1360, 1340, 1350, 1330, 1320,
 1350, 1360, 1340, 1310, 1320, 1310, 1310, 1330, 1310, 1330, 1310, 1310, 1360,
 1330, 1310, 1340, 1330, 1320, 1330, 1310, 1330, 1310, 1320, 1320, 1320, 1320,
 1310, 1320, 1310, 1320, 1320, 1310, 1300, 1320, 1300, 1310, 1300, 1320, 1290,
 1300, 1300, 1310, 1310, 1310, 1300, 1290, 1310, 1290, 1290, 1300, 1310, 1300,
 1300, 1310, 1310, 1320, 1310, 1310, 1320, 1310, 1300, 1320, 1310, 1300, 1310,
 1360, 1310, 1310, 1300, 1310, 1310, 1300, 1330, 1320, 1330, 1350, 1340, 1330,
 1330, 1350, 1350, 1320, 1330, 1330, 1340, 1330, 1340, 1350, 1340, 1340, 1330,
 1320, 1330, 1340, 1340, 1330, 1350, 1350, 1340, 1350, 1340, 1330, 1340, 1340,
 1350, 1340, 1330, 1330, 1350, 1350, 1350, 1340, 1360, 1340, 1340, 1340, 1350,
 1360, 1340, 1340, 1340, 1340, 1360, 1330, 1330, 1360, 1340, 1340, 1320, 1330,
 1330, 1340, 1350, 1340, 1330, 1320, 1320, 1340, 1340, 1350, 1330, 1340, 1330,
 1350, 1360, 1340, 1320, 1320, 1340, 1330, 1320, 1320, 1320, 1340, 1330, 1320,
 1320, 1320, 1320, 1320, 1320, 1310, 1330, 1300, 1320, 1330, 1310, 1310, 1310,
 1300, 1300, 1310, 1320, 1290, 1300, 1310, 1300, 1280, 1300, 1300, 1270, 1310,
 1320, 1300, 1310, 1300, 1290, 1320, 1310, 1280, 1320, 1290, 1310, 1290, 1320,
 1320, 1360, 1370, 1370, 1380, 1360, 1360, 1350, 1340, 1340, 1350, 1350, 1320,
 1340, 1350, 1320, 1320, 1330, 1320, 1330, 1350, 1340, 1330, 1340, 1350, 1340,
 1340, 1360, 1350, 1320, 1310, 1330, 1350, 1340, 1320, 1320, 1340, 1330, 1320,
 1320, 1310, 1330, 1320, 1330, 1340, 1320, 1320, 1330, 1340, 1340, 1320, 1340,
 1320, 1330, 1340, 1320, 1320, 1310, 1340, 1340, 1310, 1340, 1350, 1320, 1340,
 1360, 1320, 1330, 1340, 1350, 1360, 1350, 1350, 1340, 1320, 1340, 1350, 1340,
 1370, 1340, 1340, 1330, 1350, 1360, 1330, 1350, 1350, 1340, 1340, 1340, 1330,
 1340, 1340, 1350, 1370, 1330, 1330, 1350, 1350, 1350, 1350, 1340, 1320, 1340,
 1350, 1340, 1340, 1330, 1360, 1350, 1320, 1340, 1340, 1320, 1360, 1350, 1370,
 1350, 1330, 1320, 1350, 1330, 1330, 1370, 1360, 1350, 1360, 1330, 1320, 1300,
 1320, 1320, 1300, 1310, 1290, 1330, 1310, 1290, 1320, 1320, 1320, 1300, 1300,
 1320, 1320, 1330, 1320, 1310, 1310, 1320, 1310, 1330, 1310, 1350, 1340, 1330,
 1340, 1340, 1350, 1350, 1350, 1350, 1360, 1340, 1350, 1350, 1360, 1350, 1380,
 1340, 1340, 1350, 1340, 1340, 1340, 1340, 1350, 1350, 1350, 1340, 1330, 1360,
 1330, 1360, 1340, 1330, 1350, 1340, 1350, 1370, 1360, 1360, 1350, 1370, 1360,
 1360, 1330, 1360, 1350, 1380, 1360, 1360, 1350, 1350, 1340, 1330, 1330, 1330,
 1360, 1330, 1350, 1350, 1330, 1340, 1310, 1330, 1340, 1310, 1350, 1330, 1330,
 1340, 1340, 1350, 1330, 1340, 1340, 1320, 1340, 1330, 1350, 1330, 1340, 1340,
 1310, 1330, 1310, 1310, 1290, 1330, 1330, 1320, 1310, 1330, 1310, 1320, 1300,
 1330, 1300, 1330, 1330, 1300, 1340, 1320, 1330, 1320, 1380, 1360, 1360, 1350,
 1370, 1350, 1350, 1340, 1370, 1360, 1370, 1340, 1340, 1360, 1360, 1340, 1350,
 1370, 1360, 1340, 1350, 1370, 1360, 1370, 1350, 1360, 1370, 1350, 1360, 1360,
 1350, 1380, 1370, 1340, 1360, 1360, 1360, 1340, 1360, 1340, 1400, 1340, 1340,
 1350, 1340, 1360, 1370, 1360, 1350, 1340, 1350, 1370, 1340, 1370, 1340, 1340,
 1340, 1360, 1350, 1340, 1350, 1340, 1350, 1380, 1400, 1400, 1390, 1380, 1380,
 1360, 1400, 1370, 1370, 1400, 1370, 1370, 1390, 1380, 1370, 1390, 1390, 1370,
 1370, 1360, 1380, 1380, 1350, 1350, 1360, 1360, 1370, 1370, 1360, 1340, 1380,
 1370, 1370, 1360, 1370, 1360, 1370, 1350, 1340, 1360, 1350, 1380, 1340, 1360,
 1360, 1370, 1360, 1370, 1370, 1360, 1370, 1370, 1370, 1360, 1350, 1370, 1380,
 1370, 1350, 1390, 1370, 1400, 1350, 1390, 1340, 1340, 1350, 1330, 1330, 1340,
 1320, 1340, 1350, 1340, 1310, 1310, 1340, 1340, 1340, 1330, 1340, 1350, 1340,
 1330, 1310, 1340, 1340, 1330, 1340, 1340, 1310, 1300, 1330, 1310, 1340, 1330,
 1350, 1310, 1330, 1350, 1320, 1340, 1320, 1370, 1330, 1340, 1330, 1320, 1320,
 1320, 1320, 1330, 1340, 1320, 1310, 1310, 1310, 1330, 1330, 1330, 1320, 1300,
 1310, 1320, 1310, 1320, 1300, 1290, 1370, 1330, 1320, 1340, 1360, 1350, 1360,
 1370, 1340, 1350, 1360, 1370, 1350, 1380, 1360, 1350, 1350, 1340, 1340, 1370,
 1340, 1380, 1370, 1390, 1360, 1380, 1360, 1370, 1350, 1350, 1340, 1360, 1380,
 1370, 1370, 1370, 1370, 1360, 1370, 1370, 1370, 1360, 1380, 1380, 1390, 1380,
 1390, 1360, 1390, 1370, 1380, 1390, 1380, 1370, 1380, 1380, 1380, 1370, 1350,
 1380, 1370, 1380, 1380, 1310, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1310, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1360, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1360, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350, 1350,
 1350, 1350, 1350, 1350, 1350, 1340, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330, 1330,
 1330, 1330, 1330, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340, 1340,
 1340};


};

#endif /* TRACKERADC_HH */
