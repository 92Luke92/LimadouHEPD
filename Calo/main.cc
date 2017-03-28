#include "LEvRec0File.hh"
#include "LCaloCalibrationManager.hh"          	
#include <iostream>



using namespace std;

int main(int argc,char *argv[]){


// getting pedestals with LCaloManager class

 LCaloCalibrationManager::GetInstance().LoadRun(argv[1]);
 
 double mean0[NPMT]; double rms0[NPMT]; double mean1[NPMT]; double rms1[NPMT];
 
 LCaloCalibrationManager::GetInstance().PMTsMeanRms( mean0, rms0, mean1, rms1);
 


  return 0;
}
