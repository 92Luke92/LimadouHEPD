#include "LTrackerCalibration.hh"
#include <iostream>

void LTrackerCalibration::Add(const LTrackerCalibrationSlot *lcal) {
  calarray.push_back(*lcal);
  ++nSlots;
  std::cout << "LTrackerCalibration: " << "current slot number " << nSlots << std::endl;;
  return;
}

LTrackerCalibration::LTrackerCalibration() {
  Reset();
}


void LTrackerCalibration::Reset(void) {
  RunId=-99999;
  nSlots=0;
  InitialTargetRun=-99999;
  FinalTargetRun=-99999;
  calarray.resize(0);
  return;
}

LTrackerCalibration::LTrackerCalibration(const int RunIdINP,  const int InitialTargetRunINP, const int FinalTargetRunINP) {
  RunId=RunIdINP;
  nSlots=0;
  InitialTargetRun=InitialTargetRunINP;
  FinalTargetRun=FinalTargetRunINP;
}

void LTrackerCalibration::Write(std::ofstream *output) const {
  *output << RunId << std::endl;
  *output << InitialTargetRun << " " << FinalTargetRun << std::endl;
  *output << nSlots << std::endl;

  for(auto cslotit : calarray) cslotit.Write(output);
  
  *output << std::endl;
    
  return;
}


void LTrackerCalibration::Write(const char *fileOut) const {
  std::ofstream output(fileOut, std::ofstream::out); 
  Write(&output);
  output.close();
  return;
}


LTrackerCalibration* LTrackerCalibration::Read(std::ifstream *input) {
  
  int RunIdST, InitialTargetRunST, FinalTargetRunST, nSlotsST;

  *input >> RunIdST;
  *input >> InitialTargetRunST >> FinalTargetRunST;

  LTrackerCalibration *result = new LTrackerCalibration(RunIdST, InitialTargetRunST, FinalTargetRunST);

  *input >> nSlotsST;
  for(int is=0; is<nSlotsST; ++is) result->Add(LTrackerCalibrationSlot::Read(input));

  return result;
}
 
LTrackerCalibration* LTrackerCalibration::Read(const char *fileIn) {
  
  std::ifstream input(fileIn, std::ifstream::in); 
  LTrackerCalibration *result = Read(&input);
  input.close();

  return result;
}
 
