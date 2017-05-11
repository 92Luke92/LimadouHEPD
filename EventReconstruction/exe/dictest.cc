#include "LEvRec0File.hh"
#include "LCaloCalibrationManager.hh"
#include <iostream>
#include "TFile.h"
#include "LEvRec0.hh"
#include "pmtnames.hh"
#include <string>


using namespace std;

int main (int argc, char *argv[])
{
    //LEvRec0File file("../../Limadou/Limadou/EvRec/HEPD_QM_Beam_Test_ROOTfiles/RUN18_E30MeV_Pos6.root");
    //LEvRec0File file ("../../Limadou/Limadou/EvRec/HEPD_QM_Beam_Test_ROOTfiles/RUN5_E30MeV_Pos1.root");
    if (argc<2) return 1;
    LEvRec0File file (argv[1]);
    LEvRec0 cev;
    file.SetTheEventPointer (cev);
    long nev = file.GetEntries();
    std::cout << "Num of Events on File " << nev << std::endl;
    // get maximum possible events
    //if (nev>10000) nev=10000;
    for (int iev = 0; iev < nev; iev++) {
        file.GetEntry (iev);
        cout << iev;
        for (int iPMT = 0; iPMT < NPMT; iPMT++) {
            cout << "," << cev.pmt_low[iPMT];
        }
        for (int iPMT = 0; iPMT < NPMT; iPMT++) {
            cout << "," << cev.pmt_high[iPMT] ;
        }
        for (int iPMT = 0; iPMT < NPMT; iPMT++) {
            cout << "," << cev.trigger_flag[iPMT] ;
        }
        cout << "," << cev.alive_time<< "," << cev.dead_time<< endl;

    }
    return 0;
}

