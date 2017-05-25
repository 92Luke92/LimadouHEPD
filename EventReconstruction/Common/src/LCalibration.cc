#include "LCalibration.hh"
#include <iostream>
#include <sstream>
#include <algorithm>

const int DEFAULT=-1;

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
  InitialRunId = DEFAULT;
  FinalRunId = DEFAULT;
  return;
}

LCalibration::LCalibration(LCaloCalibration *calo_HG_IN,
			   LCaloCalibration *calo_LG_IN,
			   LTrackerCalibration *tracker_IN) {
  calo_HG = calo_HG_IN;
  calo_LG = calo_LG_IN;
  tracker = tracker_IN;
  InitialRunId = calo_HG_IN->GetRunId();
  FinalRunId = calo_HG_IN->GetRunId();
}
  
void LCalibration::WriteTXT(const char *fileOut) const {
  std::cout << __LCALIBRATION__ << "Writing calibration file " << fileOut << "..." << std::endl;
  std::ofstream output(fileOut, std::ofstream::out); 
  WriteTXT(&output);
  output.close();
  return;
}

void LCalibration::WriteTXT(std::ofstream *fileOut) const {
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


bool LCalibration::CheckStatus(void) const {
  if(calo_HG==0 || calo_LG==0 || tracker ==0 ) return false;
  return true;
}


LCalibration& LCalibration::operator=(const LCalibration& other) {
  InitialRunId = other.GetInitialRunId();
  FinalRunId = other.GetFinalRunId();

  calo_HG = new LCaloCalibration();
  (*calo_HG) = (*other.GetCaloHGCalibration());
  calo_LG = new LCaloCalibration();
  (*calo_LG) = (*other.GetCaloLGCalibration());
  tracker = new LTrackerCalibration();
  (*tracker) = (*other.GetTrackerCalibration());

  return *this;
}

LCalibration& LCalibration::operator+=(const LCalibration& rhs) // compound assignment (does not need to be a member,
{                           // but often is, to modify the private members)
  
  if(rhs.CheckStatus() == false) {
    std::cerr << __LCALIBRATION__ << "Error in += call. Returning this unchanged" << std::endl; 
    return *this;
  }
  
  (*calo_HG) += (*rhs.GetCaloHGCalibration());
  (*calo_LG) += (*rhs.GetCaloLGCalibration());
  (*tracker) += (*rhs.GetTrackerCalibration());

  // In/out run info
  InitialRunId = std::min(InitialRunId, rhs.GetInitialRunId());
  FinalRunId = std::max(FinalRunId, rhs.GetFinalRunId());

  return *this; // return the result by reference
}
 

LCalibration operator+(LCalibration lhs,        // passing lhs by value helps optimize chained a+b+c
		   const LCalibration& rhs) // otherwise, both parameters may be const references
{
  lhs += rhs; // reuse compound assignment
  return lhs; // return the result by value (uses move constructor)
}


LCalibration& LCalibration::operator/=(const double& rhs) {
  
  (*calo_HG) /= rhs;
  (*calo_LG) /= rhs;
  (*tracker) /= rhs;

  return *this; // return the result by reference
}




 
