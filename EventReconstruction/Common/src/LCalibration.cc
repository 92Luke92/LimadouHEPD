#include "LCalibration.hh"

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
  std::ifstream input(fileIn, std::ifstream::in); 
  LCalibration *result = Read(&input);
  input.close();
  return result;
}

LCalibration* LCalibration::Read(std::ifstream *fileIn) {
  LCaloCalibration *calo_HG_read = LCaloCalibration::Read(fileIn);
  LCaloCalibration *calo_LG_read = LCaloCalibration::Read(fileIn);
  LTrackerCalibration* tracker_read = LTrackerCalibration::Read(fileIn);
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
  if(calo_HG==0 || calo_LG==0 || tracker ==0 ) return false;
  if(calo_HG->GetRunId() != calo_LG->GetRunId() ||
     calo_LG->GetRunId() != tracker->GetRunId()) return false;
  if(calo_HG->GetInitialTargetRun() != calo_LG->GetInitialTargetRun() ||
     calo_LG->GetInitialTargetRun() != tracker->GetInitialTargetRun()) return false;
  if(calo_HG->GetFinalTargetRun() != calo_LG->GetFinalTargetRun() ||
     calo_LG->GetFinalTargetRun() != tracker->GetFinalTargetRun()) return false;
  
  return true;
}
