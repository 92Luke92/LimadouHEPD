// Local
#include "detectorID.h"
#include "PMTID.h"

// C++ std
#include <iostream>

// MCEventAnalyze
#include "RootEvent.hh"
#include "RootTrack.hh"
#include "RootVertex.hh"
#include "RootCaloHit.hh"
#include "RootTrackerHit.hh"

// ROOT libs
#include "TTree.h"
#include "TVector3.h"

//Various
#include "LEvRec0.hh"

float Vector3Dist(TVector3 v1, TVector3 v2)
{
   TVector3 diff= v1-v2;
   return static_cast<float> (diff.Mag()); // get magnitude (=rho=Sqrt(x*x+y*y+z*z)))

}

//void InitTreeT(TTree* T);

int main() {

   LEvRec0 evtstruc;
   evtstruc.DumpEventIndex();
   RootTrackerHit hit;
   std::cout << " >>> ELOSS 3" << hit.GetELoss() << std::endl;
   return 0;

   //TTree outtree=GetTTreeT();

}


//TTree GetTTreeT() {return TTree::TTree("T", "T");}


/*class LVL0Tree () {


};*/

