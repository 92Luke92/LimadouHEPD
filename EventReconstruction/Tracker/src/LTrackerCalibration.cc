#include "LTrackerCalibration.hh"
#include "LEvRec0File.hh"
#include "detector_const.hh"

#include <iostream>
#include <algorithm>
#include <math.h>
#include <random>
#include <chrono>

void LTrackerCalibration::Add(const LTrackerCalibrationSlot lcal) {
  calarray.push_back(lcal);
  ++nSlots;
  std::cout << __LTRACKERCALIBRATION__ << "Current slot number " << nSlots << std::endl;;
  return;
}

void LTrackerCalibration::DefaultFilling() {
  LTrackerCalibrationSlot DefaultSlot;
  calarray.push_back(DefaultSlot);
  return;
}

LTrackerCalibration::LTrackerCalibration() {
  Reset();
}


void LTrackerCalibration::Reset(void) {
  RunId=-99999;
  nSlots=0;
  calarray.resize(0);
  return;
}

LTrackerCalibration::LTrackerCalibration(const int RunIdINP) {
  RunId=RunIdINP;
  nSlots=0;
}

void LTrackerCalibration::Write(std::ofstream *output) const {
  *output << RunId << std::endl;
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
  
  int RunIdST, nSlotsST;

  *input >> RunIdST;

  LTrackerCalibration *result = new LTrackerCalibration(RunIdST);

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
 

LTrackerCalibration* LTrackerCalibration::ReadRoot(const char *fileIn) {
   
   int RunIdST;
   
   LTrackerCalibration *rsult = new LTrackerCalibration(RunIdST);
   
   LEvRec0 outev;
   LEvRec0File inputFile(fileIn);
   inputFile.SetTheEventPointer(outev);
   inputFile.GetEntry(0); 
   RunIdST = outev.run_id;
   
   LTrackerCalibration *result =  new LTrackerCalibration(RunIdST);
   result->Add(LTrackerCalibrationSlot::ReadRoot(fileIn));
   
   return result;
}


LTrackerCalibration& LTrackerCalibration::operator=(const LTrackerCalibration& other) {
  //calarray.resize(0);
  Reset();
  // slots
  for(int is=0; is<other.GetNSlots(); ++is) {
    calarray.push_back(other.GetTrackerCalibrationSlot(is));
  }
  // In/out run info
  RunId = other.GetRunId();

  return *this;
}

LTrackerCalibration& LTrackerCalibration::operator+=(const LTrackerCalibration& rhs) // compound assignment (does not need to be a member,
{                           // but often is, to modify the private members)
  
  
  // slots
  // Firstly, sum up all slots of this
  LTrackerCalibrationSlot tmpSlot;//=calarray.at(0);
  int cntSlot=0;
  for(int is=0; is<nSlots; ++is) {
     std::cout<<"nSlot "<<nSlots<<" Vector size "<<calarray.size()<<std::endl;
     tmpSlot+=(GetTrackerCalibrationSlot(is));
    ++cntSlot;
  }
  tmpSlot/=(static_cast<double>(cntSlot));
  // Secondly, sum up all the rest
  LTrackerCalibrationSlot tmpSlot2;
  int cntSlot2=0;
  for(int is=0; is<rhs.GetNSlots(); ++is) {
    tmpSlot2+=(rhs.GetTrackerCalibrationSlot(is));
    ++cntSlot2;
  }
  tmpSlot2/=(static_cast<double>(cntSlot2));

  LTrackerCalibrationSlot toAdd =  tmpSlot+tmpSlot2;
  Reset();  
Add(toAdd);
  nSlots=1;

  // In/out run info
  RunId = rhs.GetRunId();
  
  return *this; // return the result by reference
}
 
LTrackerCalibration operator+(LTrackerCalibration lhs,        // passing lhs by value helps optimize chained a+b+c
		   const LTrackerCalibration& rhs) // otherwise, both parameters may be const references
{
  lhs += rhs; // reuse compound assignment
  return lhs; // return the result by value (uses move constructor)
}


LTrackerCalibration& LTrackerCalibration::operator/=(const double& rhs) 
{
  for(auto pslot : calarray) pslot/=rhs;
  return *this; // return the result by reference
}
 
LTrackerCalibration* LTrackerCalibration::CreateFakeCalibration(const LTrackerCalibration* seed, const double offset){
  //std::cout<<seed<<std::endl;
  //Read values from real calibration
  const double *pedestal=seed->GetPedestal(0);
  const double *sigma=seed->GetSigma(0);
  std::cout<<"Real values collected"<<std::endl;
  double sigmaNew[NCHAN];
  double pedNew[NCHAN];
  double buffer[NCHAN];
  bool buffer2[NCHAN];
  std::cout<<"Random number genrator initialization"<<std::endl;
  //Random number generator
  typedef std::chrono::high_resolution_clock myclock;
  myclock::time_point beginning = myclock::now();
  std::default_random_engine generator;
  myclock::duration d = myclock::now() - beginning;
  unsigned seed2 = d.count();
  generator.seed(seed2);

  for(int iChan=0;iChan<NCHAN;++iChan){
    std::normal_distribution<> ped_distr (pedestal[iChan]+offset,0.5);
    std::normal_distribution<double> sigma_distr(sigma[iChan],0.5);
    sigmaNew[iChan]=sigma_distr(generator);
    pedNew[iChan]=ped_distr(generator);
    buffer[iChan]=-9999.;
    buffer2[iChan]=true;
  }
  LTrackerCalibrationSlot slot(0,0,buffer,pedNew,sigmaNew,buffer,buffer2);

  LTrackerCalibration* result=new LTrackerCalibration();
  result->Add(slot);
  result->RunId=seed->GetRunId();
  return result;
  
}
