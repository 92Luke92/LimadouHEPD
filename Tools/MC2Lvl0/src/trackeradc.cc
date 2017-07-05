/*
 * trackeradc.cc
 *
 *
 *
 */


#include "trackeradc.hh"




std::vector<short> TrackerADC::GetStrips() {
   std::vector<short> allStrips(NCHAN);
   for (trSides side : trSidesIterator) {
      uint offset = side*SIDE_CHAN;
      std::vector<short> sideStrips=getStripsForSide(side);
      for (uint is=0; is<SIDE_CHAN; is++)
         allStrips[offset+is]=sideStrips[is];
   }
   return allStrips;
}


float TrackerADC::Mev2ADCFactor (trSides side)
{
    float factor = 400;
    switch (side) {
       case p0: factor=343; break; // avg p2, p3
       case n0: factor=425; break;// n2
       case p1: factor=343; break;
       case n1: factor=425; break;
       case p2: factor=341.667; break;
       case n2: factor=425.333; break;
       case p3: factor=345.33; break;
       case n3: factor=0; break; // shit ain't working
       case p4: factor=343; break;
       case n4: factor=425; break;
       case p5: factor=343; break;
       case n5: factor=425; break;
    }
    return factor;
}



std::vector<short> TrackerADC::getStripsForSide (trSides side) {
   std::vector<short> sideStrips (SIDE_CHAN);
   float Mev2ADCfactor=Mev2ADCFactor(side);
   float totEdep=0;
   std::vector<Edep_Pos> SidePMTinfos = allEpos[side];
   for (auto chaninfo: SidePMTinfos) totEdep+=chaninfo.totEdep;
   float averagedEdep=totEdep / float(SIDE_CHAN);
   float averageADC= averagedEdep*Mev2ADCfactor;
   float dummypedvalue=100;
   short chanADC=TrimADC(averageADC, dummypedvalue);
   for (auto strip : sideStrips) strip=chanADC;
   return sideStrips;

}

short TrackerADC::TrimADC (float raw, float ped)
{
    int untrimmed = static_cast<int> (raw + ped);
    if (untrimmed > NADC) untrimmed = NADC - 1;
    short trimmed=static_cast<short> (untrimmed);
    return trimmed;
}
