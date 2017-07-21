/*
 * hitstoedeppos.cpp
 *
 *
 *
 */


#include "hits2edeppos.hh"


HitsToEdepPos::HitsToEdepPos()
{
    FillCalo2PMTidx();
    FillTracker2PMTidx();
}


std::vector<Edep_Pos> HitsToEdepPos::Calo2Edep_PosConverter (std::vector<RootCaloHit> CaloHits)
{
    std::vector<Edep_Pos> calEPos (NPMT);
    std::map<int, bool> firstHitHasInfo;
    for (RootCaloHit hit : CaloHits) {
        int subdetIdx = hit.GetVolume();
        if (firstHitHasInfo.count (subdetIdx) > 0) continue;
        firstHitHasInfo[subdetIdx] = true;
        Edep_Pos hitEpos = CaloHit2EdepPos (hit);
        for (PMTenum pmt : Calo2PMTidx[subdetIdx])
            calEPos[pmt] = hitEpos;
    }
    return calEPos;
}


Edep_Pos HitsToEdepPos::CaloHit2EdepPos (RootCaloHit hit)
{
    Edep_Pos hitEpos;
    for (auto eltp : hit.GetParticleList() )   hitEpos.totEdep += hit.GetEdep (eltp);
    float vx = (hit.GetExitPoint().z() + hit.GetEntryPoint().z() ) / 2;
    float vy = (hit.GetExitPoint().x() + hit.GetEntryPoint().x() ) / 2;
    float vz = (hit.GetExitPoint().y() + hit.GetEntryPoint().y() ) / 2;
    hitEpos.position = TVector3  (vx, vy, vz);
    return hitEpos;
}





std::vector<std::vector<Edep_Pos>> HitsToEdepPos::Tracker2Edep_PosConverter (std::vector<RootTrackerHit> TrackerHits) {
   std::vector<std::vector<Edep_Pos>> trEpos (12);
   for (RootTrackerHit hit : TrackerHits) {
        int subdetIdx = hit.GetDetectorId();
        Edep_Pos hitEpos = TrHit2EdepPos (hit);
        trEpos[Tracker2PMTidx[subdetIdx]].push_back(hitEpos);
    }
   return trEpos;
}



Edep_Pos HitsToEdepPos::TrHit2EdepPos (RootTrackerHit hit)
{
    Edep_Pos hitEpos;
    hitEpos.totEdep = hit.GetELoss();
    float vx = (hit.GetExitPoint().z() + hit.GetEntryPoint().z() ) / 2;
    float vy = (hit.GetExitPoint().x() + hit.GetEntryPoint().x() ) / 2;
    float vz = (hit.GetExitPoint().y() + hit.GetEntryPoint().y() ) / 2;
    hitEpos.position = TVector3  (vx, vy, vz);
    return hitEpos;
}



void HitsToEdepPos::FillCalo2PMTidx ()
{
    Calo2PMTidx = {
// Lyso
        {1111, {L1ne}},
        {1112, {L4n}},
        {1113, {L7nw}},
        {1121, {L2e}},
        {1122, {L5c}},
        {1123, {L8w}},
        {1131, {L3se}},
        {1132, {L6s}},
        {1133, {L9sw}},
//Planes
        {1201, {P16ne, P16sw}},
        {1202, {P15nw, P15se}},
        {1203, {P14ne, P14sw}},
        {1204, {P13nw, P13se}},
        {1205, {P12ne, P12sw}},
        {1206, {P11nw, P11se}},
        {1207, {P10ne, P10sw}},
        {1208, {P9nw,  P9se}},
        {1209, {P8ne,  P8sw}},
        {1210, {P7nw,  P7se}},
        {1211, {P6ne,  P6sw}},
        {1212, {P5nw,  P5se}},
        {1213, {P4ne,  P4sw}},
        {1214, {P3nw,  P3se}},
        {1215, {P2ne,  P2sw}},
        {1216, {P1nw,  P1se}},
//Trigger
        {1311, {T1e, T1w}},
        {1312, {T2e, T2w}},
        {1313, {T3e, T3w}},
        {1321, {T4e, T4w}},
        {1322, {T5e, T5w}},
        {1323, {T6e, T6w}},
//Veto
        {1411, {VEu, VEd }},
        {1412, {VWu, VWd }},
        {1421, {VNu, VNd }},
        {1422, {VSu, VSd }},
        {1430, {VBne, VBsw}}
    };
}



void HitsToEdepPos::FillTracker2PMTidx()
{
    Tracker2PMTidx = {
        {2111, n1},
        {2112, p1},
        {2121, n3},
        {2122, p3},
        {2131, n5},
        {2132, p5},
        {2211, n0},
        {2212, p0},
        {2221, n2},
        {2222, p2},
        {2231, n4},
        {2232, p4},
    };
}

