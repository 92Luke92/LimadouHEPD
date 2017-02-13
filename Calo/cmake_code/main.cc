#include "LEvRec0File.hh"
#include "LCaloCalManager.hh"          	
#include <iostream>
#include "TCanvas.h"
//#include "TPad.h"
#include <TROOT.h>
#include <TH1F.h>
#include <TApplication.h>
#include <iomanip> 


using namespace std;

int main(int argc,char *argv[]){

 LCaloCalManager::GetInstance().LoadRun(argv[1]);


 double mean0[NPMT]; double rms0[NPMT]; double mean1[NPMT]; double rms1[NPMT];
 LCaloCalManager::GetInstance().PMTsMeanRms(argv[1], mean0, rms0, mean1, rms1);


 for(int loop2=0; loop2<NPMT; loop2++){
    std::cout <<  std::setprecision(4) << mean0[loop2]  << " " << rms0[loop2]<< std::endl;
 }



  return 0;
}
