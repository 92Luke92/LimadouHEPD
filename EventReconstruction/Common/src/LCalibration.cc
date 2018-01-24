#include "LCalibration.hh"
#include "LEvRec0File.hh"
#include "LTrackerCalibration.hh"
#include "LCaloCalibration.hh"


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
  RunId = DEFAULT;
  return;
}

LCalibration::LCalibration(LCaloCalibration *calo_HG_IN,
			   LCaloCalibration *calo_LG_IN,
			   LTrackerCalibration *tracker_IN) {
  calo_HG = calo_HG_IN;
  calo_LG = calo_LG_IN;
  tracker = tracker_IN;
  RunId = calo_HG_IN->GetRunId();
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

void LCalibration::WriteROOT(const char *fileOut) const {
  
  const double *_pmt_HG = calo_HG->GetPedestal();
  const double *_pmt_LG = calo_LG->GetPedestal();
  const double *_sil = tracker->GetPedestal(0); // nSlot = 0????
  // const double *_silNGindexMask = tracker->GetMaskOnNGI();
  // const double *_silsigMask = tracker->GetMaskOnSigma(); 

  LEvRec0 outev;
  LEvRec0Md outevMD;
  LEvRec0File inputFile(GetInputFile());
  inputFile.SetMdPointer(outevMD);
  
  std::cout << __LCALIBRATION__ << "Writing calibration root file " << fileOut
	    << " ..." << std::endl;

  std::cout << __LCALIBRATION__ << "Writing calibration root file " << fileOut
	    << " ..." << std::endl;
  
  LEvRec0File outRootfile(fileOut, outev, outevMD);
  //outev.run_id = (RunId | 0x8000); // todo: decide if needed 
  
  for (int j = 0 ; j< 2; j++) // ped, sigma
  {
    outev.runType = 0x1B;
    outev.event_index = j;
    for (int i = 0 ; i< NPMT; i++)
    {
      outev.pmt_high[i] = (unsigned short)_pmt_HG[i];
      outev.pmt_low[i] = (unsigned short)_pmt_LG[i];
    }
    for (int i = 0 ; i< NCHAN; i++)
      outev.strip[i] = (short)_sil[i];
    
    inputFile.GetMDEntry(j);
    
    outRootfile.Fill();
    
    _pmt_HG = calo_HG->GetSigma();
    _pmt_LG = calo_LG->GetSigma();
    _sil    = tracker->GetSigma(0);
    
  }  
  outRootfile.Write();  
  
  return;
}

LCalibration * LCalibration::ReadROOT(const char *fileIn) {

   std::cout << __LCALIBRATION__ << "Reading calo_HG from root file... " << std::endl;
   LCaloCalibration *calo_HG_read = LCaloCalibration::ReadRoot(fileIn, HIGH);
   std::cout << __LCALIBRATION__ << "calo_HG read. " << std::endl << std::flush;
   std::cout << __LCALIBRATION__ << "Reading calo_LG from root file... " << std::endl;
   LCaloCalibration *calo_LG_read = LCaloCalibration::ReadRoot(fileIn, LOW);
   std::cout << __LCALIBRATION__ << "calo_LG read. " << std::endl;
   std::cout << __LCALIBRATION__ << "Reading tracker from root file... " << std::endl;
   LTrackerCalibration* tracker_read = LTrackerCalibration::ReadRoot(fileIn);
   std::cout << __LCALIBRATION__ << "tracker read. " << std::endl;
  
   LCalibration *result = new LCalibration(calo_HG_read, calo_LG_read, tracker_read);

  return result;
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
  RunId = other.GetRunId();
  
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
  RunId = std::min(RunId, rhs.GetRunId()); // arbitrary. Why?

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




LCalibration* LCalibration::CreateFakeCalibration(const LCalibration *seed, double tracker_offset,double calo_offset) {
  
  std::cout<<"Calibration read. Starting fake production"<<std::endl;

  LTrackerCalibration* tracker=LTrackerCalibration::CreateFakeCalibration(seed->GetTrackerCalibration(),tracker_offset);
  std::cout<<"Fake Tracker Calibration Created!"<<std::endl;
  LCaloCalibration* calo_HG=LCaloCalibration::CreateFakeCalibration(seed->GetCaloHGCalibration(),calo_offset);
  std::cout<<"Fake HG Calo Calibration Created!"<<std::endl;
  LCaloCalibration* calo_LG=LCaloCalibration::CreateFakeCalibration(seed->GetCaloLGCalibration(),calo_offset);
  std::cout<<"Fake LG Calo Calibration Created!"<<std::endl;
  LCalibration* result=new LCalibration(calo_HG,calo_LG,tracker);
  return result;

}

