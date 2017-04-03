#include <iostream>
#include "LCalibrationManager.hh"
#include "LTrackerMask.hh"

int main(int argc, char *argv[]) {
  /*if(argc!=3) {
    std::cerr << "Error! Usage:    ./Calibrate <calRunFile> <calOutFile>" << std::endl;
    std::cerr << "Aborted." << std::endl;
    return -999;
  }
  
  
  LCalibrationManager::GetInstance().LoadRun(argv[1]);
  LCalibration *cal = LCalibrationManager::GetInstance().Calibrate();
  cal->Write(argv[2]);
*/
  
  // Test read-write
  LCalibration *cal2=LCalibration::Read(argv[2]);
  cal2->Write("ciccio.cal");
  

  /*  
  // Test LTrackerMask
  auto ms = cal->GetTrackerCalibration()->GetMaskOnSigma(0, 4.,6.);
  ms.Dump();
  auto mngi = cal->GetTrackerCalibration()->GetMaskOnNGI(0, -100.,5.);
  mngi.Dump();
  auto mtot = ms&&mngi;
  mtot.Dump();
  */
  return 0;
}
