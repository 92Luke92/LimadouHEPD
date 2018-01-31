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
#include <string>
#include <vector>
#include <map>

// MCEventAnalyze
#include "RootCaloHit.hh"
#include "RootTrackerHit.hh"

// ROOT libs
#include "TVector3.h"

//Local
#include "Edep_Pos.h"
#include "detector_const.hh"
#include "pmtenum.hh"

class HitsToEdepPos {
  public:
    HitsToEdepPos();
    std::vector<Edep_Pos> Calo2Edep_PosConverter (std::vector<RootCaloHit> CaloHits);
    std::vector<std::vector<Edep_Pos>> Tracker2Edep_PosConverter (std::vector<RootTrackerHit> TrackerHits);


  private:

    enum trSides {p0, n0, p1, n1, p2, n2, p3, n3, p4, n4, p5, n5 };


    std::map<uint, std::vector<PMTenum>> Calo2PMTidx;
    std::map<uint, trSides> Tracker2PMTidx;
    void FillCalo2PMTidx();
    void FillTracker2PMTidx();
    template <typename RootHit> TVector3 Hit2Pos(RootHit hit);
    Edep_Pos CaloHit2EdepPos (RootCaloHit hit) ;
    Edep_Pos TrHit2EdepPos (RootTrackerHit hit) ;


};



#endif /* HITSTOEDEPPOS_HPP */
