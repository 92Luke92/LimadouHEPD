#include "LEvRec0File.hh"
#include <iostream>
#include <string>



int main (int argc, char *argv[])
{
    //LEvRec0File file("../../Limadou/Limadou/EvRec/HEPD_QM_Beam_Test_ROOTfiles/RUN18_E30MeV_Pos6.root");
    //LEvRec0File file ("../../Limadou/Limadou/EvRec/HEPD_QM_Beam_Test_ROOTfiles/RUN5_E30MeV_Pos1.root");
    std::string filename {"/storage/gpfs_data/limadou/data/L0/test/RUN_3C_174MeV_SERVO_EASIROC2_HOT0-999.root"};
    if (argc>1) filename=argv[1];
    LEvRec0File file (filename.c_str());
    LEvRec0 cev;
    file.SetTheEventPointer (cev);
    long nev = file.GetEntries();
    //std::cout << "Num of Events on File " << nev << std::endl;
     //get maximum possible events
    if (nev>10000) nev=10000;
    for (int iev = 0; iev < nev; iev++) {
        file.GetEntry (iev);
        std::cout << iev;
        for (uint iCHAN = 0; iCHAN < 4096; iCHAN++) {
            std::cout << "," << cev.strip[iCHAN];
        }
    std::cout << std::endl;
    }
    return 0;
}
