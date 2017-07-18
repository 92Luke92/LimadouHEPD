/*
 * trackeradc.cc
 *
 *
 *
 */


#include "trackeradc.hh"
#include <iostream>
#include <cmath> // floor
#include "TVector2.h"




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
   float dummypedvalue=100;
   std::vector<short> sideStrips (SIDE_CHAN, dummypedvalue);
   float Mev2ADCfactor=Mev2ADCFactor(side);
   std::vector<Edep_Pos> SidePMTinfos = allEpos[side];

   for (auto chaninfo: SidePMTinfos) {

      if (chaninfo.totEdep>0) {
         short nStrip=GetLocalStrip(side, chaninfo.position);
         float EMeV=chaninfo.totEdep;
         float EADC=EMeV*Mev2ADCfactor;
         short chanADC=TrimADC(EADC, dummypedvalue);
         sideStrips[nStrip]=chanADC;
      }
   }

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
   // Actually this one is easier  in MCFrame (centered in 0, 0)
   TVector2 flatPos=MCpos.XYvector();
   TVector2 relSidePos=flatPos;
   const float SIDEXDIM=82.5; // from BT positions schema
   const float SIDEYDIM=210;
   const float SENSITIVESIDEXDIM=71.58;
   const float SENSITIVESIDEYDIM=106.63;
   const short NSTRIPS_PSIDE=384;
   const short NSTRIPS_NSIDE=576;

   if (side == p0 || side == n0|| side == p1 || side == n1) relSidePos.SetX(relSidePos.X() + SIDEXDIM);
   if (side == p4 || side == n4|| side == p5 || side == n5) relSidePos.SetX(relSidePos.X() - SIDEXDIM);


   // NSTRIPNSIDE and PSIDE even; so (0, 0) in the middle of two strips
   // We use the floor function, and add the the offset.

   if (side%2 == 0) // P side, strip in x coordinate, short dimension
   {
      float pitch=PITCH*1000; // in mm
      float len2origin=relSidePos.X();
      stripNbr=static_cast<short> ( NSTRIPS_PSIDE/2 + floor(len2origin/pitch) );
   } else {
      float pitch=PITCH*1000*(NSTRIPS_NSIDE/NSTRIPS_PSIDE); // in mm
      float len2origin=relSidePos.Y();
      stripNbr=static_cast<short> ( NSTRIPS_NSIDE/2 + floor(len2origin/pitch) );
   }
   if (stripNbr<0 || stripNbr>=NSTRIPS_PSIDE) {
      std::cerr << "Tracker position -> strip number mismatch " << stripNbr << " for side " << side <<  std::endl;
      flatPos.Print();
      relSidePos.Print();
       if (stripNbr<0) stripNbr=0;
       if (stripNbr>=NSTRIPS_PSIDE) stripNbr=NSTRIPS_PSIDE-1;
   }

   return stripNbr;
}
