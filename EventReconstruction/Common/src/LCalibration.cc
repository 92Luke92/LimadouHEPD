#include "LCalibration.hh"
#include <iostream>

LCalibration::LCalibration() {
  Reset();
}

void LCalibration::Reset() {
  if(tracker) {
    delete tracker;
    tracker=0;
  }
  if(calo_HG) {
    delete calo_HG;
    calo_HG=0;
  }
  if(calo_LG) {
    delete calo_LG;
    calo_LG=0;
  }
  return;
}

LCalibration::LCalibration(LCaloCalibration *calo_HG_IN,
			   LCaloCalibration *calo_LG_IN,
			   LTrackerCalibration *tracker_IN) {
  calo_HG = calo_HG_IN;
  calo_LG = calo_LG_IN;
  tracker = tracker_IN;
}
  
void LCalibration::Write(const char *fileOut) const {
  std::cout << __LCALIBRATION__ << "Writing calibration file " << fileOut << "..." << std::endl;
  std::ofstream output(fileOut, std::ofstream::out); 
  Write(&output);
  output.close();
  return;
}

void LCalibration::Write(std::ofstream *fileOut) const {
  calo_HG->Write(fileOut);
  calo_LG->Write(fileOut);
  tracker->Write(fileOut);
  return;
}


LCalibration* LCalibration::Read(const char *fileIn) {
  std::cout << __LCALIBRATION__ << "Reading calibration file " << fileIn << "..." << std::endl;
  std::ifstream input(fileIn, std::ifstream::in); 
  LCalibration *result = Read(&input);
  input.close();
  std::cout << __LCALIBRATION__ << "Calibration file read." << std::endl;
  return result;
}

LCalibration* LCalibration::Read(std::ifstream *fileIn) {
  std::cout << __LCALIBRATION__ << "Reading calo_HG... " << std::endl;
  LCaloCalibration *calo_HG_read = LCaloCalibration::Read(fileIn);
  std::cout << __LCALIBRATION__ << "calo_HG read. " << std::endl << std::flush;
  std::cout << __LCALIBRATION__ << "Reading calo_LG... " << std::endl;
  LCaloCalibration *calo_LG_read = LCaloCalibration::Read(fileIn);
  std::cout << __LCALIBRATION__ << "calo_LG read. " << std::endl;
  std::cout << __LCALIBRATION__ << "Reading tracker... " << std::endl;
  LTrackerCalibration* tracker_read = LTrackerCalibration::Read(fileIn);
  std::cout << __LCALIBRATION__ << "tracker read. " << std::endl;
  LCalibration *result = new LCalibration(calo_HG_read, calo_LG_read, tracker_read);
  return result;
}


int LCalibration::GetRunId() const {
  if(CheckStatus()==false) return -999;
  // Take calo_HG as reference
  return calo_HG->GetRunId();
}

int LCalibration::GetInitialTargetRun() const {
  if(CheckStatus()==false) return -999;
  // Take calo_HG as reference
  return calo_HG->GetInitialTargetRun();
}
int LCalibration::GetFinalTargetRun() const {
  if(CheckStatus()==false) return -999;
  // Take calo_HG as reference
  return calo_HG->GetFinalTargetRun();
}

bool LCalibration::CheckStatus(void) const {
  if(&calo_HG==0 || &calo_LG==0 || &tracker ==0 ) return false;
  return true;
}
