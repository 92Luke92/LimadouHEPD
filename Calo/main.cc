#include "LEvRec0File.hh"
#include "LCaloCalManager.hh"          	
#include <iostream>



using namespace std;

int main(int argc,char *argv[]){


// getting pedestals with LCaloManager class

 LCaloCalManager::GetInstance().LoadRun(argv[1]);
 
 double mean0[NPMT]; double rms0[NPMT]; double mean1[NPMT]; double rms1[NPMT];
 
 LCaloCalManager::GetInstance().PMTsMeanRms( mean0, rms0, mean1, rms1);
 


  return 0;
}
