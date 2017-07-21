/*
 * hitstoedeppos.hpp
 *
 * Converts hits collection to EdepPos object
 *
 *
 *
 */


#ifndef HITSTOEDEPPOS_HPP
#define HITSTOEDEPPOS_HPP



// C++ std
#include <iostream>
#include <string>
#include <vector>

// MCEventAnalyze
#include "RootEvent.hh"
#include "RootTrack.hh"
#include "RootVertex.hh"
#include "RootCaloHit.hh"
#include "RootTrackerHit.hh"

// ROOT libs
#include "TVector3.h"


//Local
#include "Edep_Pos.h"
#include "detector_const.hh"

class HitsToEdepPos {
  public:
    HitsToEdepPos();
    std::vector<Edep_Pos> Calo2Edep_PosConverter (std::vector<RootCaloHit> CaloHits);
    std::vector<std::vector<Edep_Pos>> Tracker2Edep_PosConverter (std::vector<RootTrackerHit> TrackerHits);


  private:


    enum PMTenum {T1e, T2e, T3e, T4e, T5e, T6e,
                  P1se, P2sw, P3se, P4sw, P5se, P6sw, P7se, P8sw, P9se, P10sw, P11se, P12sw, P13se, P14sw, P15se, P16sw,
                  VNu, VEu, VSu, VWu, VBne, L9sw, L7nw, L1ne, L8w, L5c,
                  T1w, T2w, T3w, T4w, T5w, T6w,
                  P1nw, P2ne, P3nw, P4ne, P5nw, P6ne, P7nw, P8ne, P9nw, P10ne, P11nw, P12ne, P13nw, P14ne, P15nw, P16ne,
                  VNd, VEd, VSd, VWd, VBsw, L3se, L2e, L6s, L4n, NC
                 };

    enum trSides {p0, n0, p1, n1, p2, n2, p3, n3, p4, n4, p5, n5 };





    std::map<uint, std::vector<PMTenum>> Calo2PMTidx;
    std::map<uint, trSides> Tracker2PMTidx;
    void FillCalo2PMTidx();
    void FillTracker2PMTidx();
    Edep_Pos CaloHit2EdepPos (RootCaloHit hit) ;
    Edep_Pos TrHit2EdepPos (RootTrackerHit hit) ;


};



#endif /* HITSTOEDEPPOS_HPP */
