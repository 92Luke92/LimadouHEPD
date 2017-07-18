/*
 * trackeradc.cc
 *
 *
 *
 */


#include "trackeradc.hh"
#include "MCcoorPhysicalFrame.hh"
#include <iostream>
#include <cmath> // floor




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
   //std::cout << " >> " << side<< " "<< totEdep << std::endl;
   float averagedEdep=totEdep ; // / float(SIDE_CHAN);
   float averageADC= averagedEdep*Mev2ADCfactor;
   float dummypedvalue=100;
   short chanADC=TrimADC(averageADC, dummypedvalue);
   sideStrips.assign (SIDE_CHAN, chanADC);

   return sideStrips;

}

short TrackerADC::TrimADC (float raw, float ped)
{
    int untrimmed = static_cast<int> (raw + ped);
    if (untrimmed > NADC) untrimmed = NADC - 1;
    short trimmed=static_cast<short> (untrimmed);
    return trimmed;
}


short TrackerADC::GetLocalStrip(trSides side, TVector3 MCpos) {
   short stripNbr=0;
   MCtoPhysicalDetectorFrame mcVector(MCpos);
   TVector3 physPos=mcVector.GetPhysicalVector();
   TVector3 relSidePos=physPos;
   const float SIDEXDIM=77.58;
   const float SIDEYDIM=109.63;
   const float SENSITIVESIDEXDIM=71.58;
   const float SENSITIVESIDEYDIM=106.63;
   const short NSTRIPSPSIDE=384;
   const short NSTRIPSNSIDE=576;

   if (side == p0 || side == n0|| side == p1 || side == n1) relSidePos.SetX(relSidePos.X() - SIDEXDIM);
   if (side == p4 || side == n4|| side == p5 || side == n5) relSidePos.SetX(relSidePos.X() + SIDEXDIM);

   // NSTRIPNSIDE and PSIDE even; so (0, 0) in the middle of two strips
   // We use the floor function, and add the the offset.

   if (side%2 == 0) // P side, strip in x coordinate, short dimension
   {
      float pitch=PITCH*1000; // in mm
      float len2origin=relSidePos.X();
      stripNbr=static_cast<short> ( NSTRIPSPSIDE/2 + floor(len2origin/pitch) );
   } else {
      float pitch=PITCH*1000*(NSTRIPSNSIDE/NSTRIPSPSIDE); // in mm
      float len2origin=relSidePos.Y();
      stripNbr=static_cast<short> ( NSTRIPSNSIDE/2 + floor(len2origin/pitch) );
   }
   if (stripNbr<0 || stripNbr>NSTRIPSPSIDE)
      std::cerr << "Tracker position -> strip number mismatch " << stripNbr << std::endl;

   return stripNbr;
}
