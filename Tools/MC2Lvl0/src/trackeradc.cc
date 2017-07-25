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




std::array<short,NCHAN> TrackerADC::GetStrips()
{
  std::array<short,NCHAN> allStrips;
    for (trSides side : trSidesIterator) {
        uint offset = static_cast<uint> (side) * SIDE_CHAN;
        std::array<short, SIDE_CHAN> sideStrips = getStripsForSide (side);
        for (uint is = 0; is < SIDE_CHAN; is++)
            allStrips[offset + is] = sideStrips[is];
    }
    return allStrips;
}


float TrackerADC::Mev2ADCFactor (trSides side)
{
    float factor = 400;
    switch (side) {
    case p0: factor = 343; break; // avg p2, p3
    case n0: factor = 425; break; // n2
    case p1: factor = 343; break;
    case n1: factor = 425; break;
    case p2: factor = 341.667; break;
    case n2: factor = 425.333; break;
    case p3: factor = 345.33; break;
    case n3: factor = 0; break; // shit ain't working
    case p4: factor = 343; break;
    case n4: factor = 425; break;
    case p5: factor = 343; break;
    case n5: factor = 425; break;
    }
    return factor;
}




std::array<short, SIDE_CHAN> TrackerADC::getStripsForSide (trSides side)
{

    uint offset = static_cast<uint> (side) * SIDE_CHAN;
    std::array<short, SIDE_CHAN> sideStrips;
    for (int ic=0; ic<SIDE_CHAN; ic++) sideStrips[ic] = trPed[offset+ic];
    float Mev2ADCfactor = Mev2ADCFactor (side);
    std::vector<Edep_Pos> SidePMTinfos = allEpos[side];
    int i=0;
    for (auto chaninfo : SidePMTinfos) {
        if (chaninfo.totEdep > 0) {
            short nStrip = GetLocalStrip (side, chaninfo.position);
            float EMeV = chaninfo.totEdep;
            float EADC = EMeV * Mev2ADCfactor;
            short chanADC = TrimADC (EADC, trPed[offset+i]);
            sideStrips[nStrip] = chanADC;
        }
        i++;
    }
    return sideStrips;
}

short TrackerADC::TrimADC (float raw, float ped)
{
    int untrimmed = static_cast<int> (raw + ped);
    if (untrimmed > NADC) untrimmed = NADC - 1;
    short trimmed = static_cast<short> (untrimmed);
    return trimmed;
}


short TrackerADC::GetLocalStrip (trSides side, TVector3 MCpos)
{
    short stripNbr = 0;
    // Actually this one is easier  in MCFrame (centered in 0, 0)
    // But beware of x and y planes!
    const float SIDEXDIM = 82.5; // from BT positions schema
    const float TRYDIM = 209.66; // From Ester's doc
    const float SIDEYDIM = TRYDIM / 3;
    TVector2 flatPos = MCpos.XYvector();
    if (side % 2 == 0) { // P side, chan increases if x decreases
        const float PITCH_PSIDE_MM = PITCH * 1000;
        float len2origin = flatPos.X();
        if (side == p0 || side == p1) len2origin += SIDEXDIM; // top
        if (side == p4 || side == p5) len2origin -= SIDEXDIM; //
        stripNbr = static_cast<short> ( SIDE_CHAN / 2 - floor (len2origin / PITCH_PSIDE_MM) );
    } else { // S side, chan increases if y increases
        const float PITCH_NSIDE_MM = SIDEYDIM / SIDE_CHAN;
        float len2origin = flatPos.Y();
        float len2border= len2origin+TRYDIM / 2;
        short strips2border = static_cast<short> (floor(len2border / PITCH_NSIDE_MM));
        stripNbr = strips2border % SIDE_CHAN;
    }
    if (stripNbr < 0 || stripNbr >= SIDE_CHAN) {
        std::cerr << "Tracker position -> strip number mismatch " << stripNbr << " for side " << side <<  std::endl;
        flatPos.Print();
        if (stripNbr < 0) stripNbr = 0;
        if (stripNbr >= SIDE_CHAN) stripNbr = SIDE_CHAN - 1;
    }
    return stripNbr;
}
