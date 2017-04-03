#include "LEvRec0File.hh"
#include "LCaloCalibrationManager.hh"          	
#include <iostream>



using namespace std;

int main(int argc,char *argv[]){
  if(argc!=3) {
    std::cerr << "Error! Usage:    ./CalibrateCalo <calRunFile> <calOutFile>" << std::endl;
    std::cerr << "Aborted." << std::endl;
    return -999;
  }
  
  
  LCaloCalibrationManager::GetInstance().LoadRun(argv[1]);
  LCaloCalibration *cal =   LCaloCalibrationManager::GetInstance().CalibrateHG();
  cal->Write(argv[2]);

  /*
  // Test read-write
  LCaloCalibration *cal=LCaloCalibration::Read(argv[2]);
  cal->Write("ciccio.cal");
  */
  return 0;
}
