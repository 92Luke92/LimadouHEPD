#include "LTrackerCalibrationSlot.hh"
#include "LEvRec0File.hh"

#include <algorithm>
#include <math.h>
#include <iostream>

void LTrackerCalibrationSlot::Reset() {
  StartEvent=-999;
  StopEvent=-999;
  for(int iChan=0; iChan<NCHAN; ++iChan) {
    pedestal[iChan]=0;
    sigma[iChan]=0;
    ngindex[iChan]=0;
    CN_mask[iChan]=0;
  }
  for(int icol=0; icol<N_COLUMN; ++icol) 
     column_mask[icol]=0;

  return;
}

LTrackerCalibrationSlot::LTrackerCalibrationSlot(int StartE, int StopE, double *rawsig, double *ped, double *sig, double *ngi, bool *cnm, bool *col_mask) {
  StartEvent=StartE;
  StopEvent=StopE;
  for(int iChan=0; iChan<NCHAN; ++iChan) {
    sigmaraw[iChan]=rawsig[iChan];
    pedestal[iChan]=ped[iChan];
    sigma[iChan]=sig[iChan];
    ngindex[iChan]=ngi[iChan];
    CN_mask[iChan]=cnm[iChan];
  }
  for(int icol=0; icol<N_COLUMN; ++icol) 
     column_mask[icol]= col_mask[icol];

}

void LTrackerCalibrationSlot::Write(std::ofstream *output) {
  *output << "EVENTS " << StartEvent << " " << StopEvent << std::endl;
  for(int iChan=0; iChan<NCHAN; ++iChan)
    *output << sigmaraw[iChan] << " " << pedestal[iChan] << " " << sigma[iChan] << " " << ngindex[iChan] << " " << CN_mask[iChan] << std::endl;
  return;
}

LTrackerCalibrationSlot LTrackerCalibrationSlot::Read(std::ifstream *input) {
  char word[100];
  int StartEventST, StopEventST;
  double sigmarawST[NCHAN], pedestalST[NCHAN], sigmaST[NCHAN], ngindexST[NCHAN];
 
  bool cnmST[NCHAN];
  bool column_mask[N_COLUMN];
  for(int iCol=0; iCol<N_COLUMN; iCol++)
     column_mask[iCol] = true;

  *input >> word >> StartEventST >>  StopEventST;
  for(int iChan=0; iChan<NCHAN; ++iChan)
    *input >> sigmarawST[iChan] >> pedestalST[iChan] >>  sigmaST[iChan] >> ngindexST[iChan] >> cnmST[iChan];
  
  LTrackerCalibrationSlot result(StartEventST, StopEventST, sigmarawST, pedestalST, sigmaST, ngindexST, cnmST, column_mask);
  
   return result;
}

LTrackerCalibrationSlot LTrackerCalibrationSlot::ReadRoot(const char *input) {

  int StartEventST=0, StopEventST=0;
  double sigmarawST[NCHAN], pedestalST[NCHAN], sigmaST[NCHAN], ngindexST[NCHAN];
  bool cnmST[NCHAN];

  memset(&sigmarawST[0], 0, NCHAN*(sizeof(double)));
  memset(&cnmST[0], 1, NCHAN*(sizeof(bool)));

  LEvRec0 outev;
  LEvRec0File inputFile(input);
  inputFile.SetTheEventPointer(outev);
  inputFile.GetEntry(0);  // ped
  
  for(int iChan=0; iChan<NCHAN; ++iChan)
  {
     pedestalST[iChan] = outev.strip[iChan];
     //std::cout << "pedestalST[" << iChan << "] = " << pedestalST[iChan] << std::endl;
  }
  inputFile.GetEntry(1); // sig
  for(int iChan=0; iChan<NCHAN; ++iChan)
  {
     sigmaST[iChan] = (outev.strip[iChan] & 0x0FFF);
     ngindexST[iChan] = (outev.strip[iChan] & 0x4000);
     
     if (ngindexST[iChan] == 0 )
	ngindexST[iChan] = -1; // good channel
     else  ngindexST[iChan] = 100; // bad channel
     
     //std::cout << "sigmaST[" << iChan << "] = " << sigmaST[iChan] << std::endl;
  }

  LEvRec0Md cevMD;
  inputFile.SetMdPointer(cevMD);
  bool column_mask[N_COLUMN];
  inputFile.GetMDEntry(0); 
  for (int i = 0; i< N_COLUMN; i++)
     column_mask[i] = cevMD.silConfig.ladder_mask[i];

  LTrackerCalibrationSlot result(StartEventST, StopEventST, sigmarawST, pedestalST, sigmaST, ngindexST, cnmST, column_mask);
    
   return result;
}


LTrackerMask LTrackerCalibrationSlot::GetMaskOnSigma(const double sigmaMin, const double sigmaMax) {
  LTrackerMask result;
  for(int iChan=0; iChan<NCHAN; ++iChan) {
    result[iChan] = (sigma[iChan]>=sigmaMin && sigma[iChan]<=sigmaMax);
  }
  return result;
}


LTrackerMask LTrackerCalibrationSlot::GetMaskOnNGI(const double ngiMin, const double ngiMax) {
  LTrackerMask result;
  for(int iChan=0; iChan<NCHAN; ++iChan) {
    result[iChan] = (ngindex[iChan]>=ngiMin && ngindex[iChan]<=ngiMax);
  }
  return result;
}

LTrackerMask LTrackerCalibrationSlot::GetMaskOnColumn() {
  LTrackerMask result;
  for(int iChan=0; iChan<NCHAN; ++iChan) {
     if(column_mask[iChan/COLUMN_CHAN] == false ) result[iChan] = false;
     else result[iChan] = true;
  }
  return result;
}


LTrackerCalibrationSlot::LTrackerCalibrationSlot(const LTrackerCalibrationSlot &other) {
    for(int ichan=0; ichan<NCHAN; ++ichan) {
      sigmaraw[ichan]= other.GetSigmaRaw()[ichan];
      pedestal[ichan] = other.GetPedestal()[ichan];
      sigma[ichan] = other.GetSigma()[ichan];
      ngindex[ichan] = other.GetNGIndex()[ichan];
      CN_mask[ichan] = other.GetCNMask()[ichan];
    }
    for(int iCol=0; iCol<N_COLUMN; ++iCol) 
       column_mask[iCol] = other.GetColumnMask()[iCol];
    
    // event info
    StartEvent = other.GetStartEvent();
    StopEvent = other.GetStopEvent();
}

LTrackerCalibrationSlot& LTrackerCalibrationSlot::operator=(const LTrackerCalibrationSlot& other) {
  if(this != &other) {
    for(int ichan=0; ichan<NCHAN; ++ichan) {
      sigmaraw[ichan]= other.GetSigmaRaw()[ichan];
      pedestal[ichan] = other.GetPedestal()[ichan];
      sigma[ichan] = other.GetSigma()[ichan];
      ngindex[ichan] = other.GetNGIndex()[ichan];
      CN_mask[ichan] = other.GetCNMask()[ichan];
      
    }
    for(int iCol=0; iCol<N_COLUMN; ++iCol) 
       column_mask[iCol] = other.GetColumnMask()[iCol];
    // event info
    StartEvent = other.GetStartEvent();
    StopEvent = other.GetStopEvent();
  }
  
  return *this;
}

LTrackerCalibrationSlot& LTrackerCalibrationSlot::operator+=(const LTrackerCalibrationSlot& rhs) // compound assignment (does not need to be a member,
{                           // but often is, to modify the private members)
  
  for(int ichan=0; ichan<NCHAN; ++ichan) {
    pedestal[ichan] += (rhs.GetPedestal()[ichan]);
    double tmp = rhs.GetSigma()[ichan];
    double var = tmp*tmp + sigma[ichan]*sigma[ichan] ;
    double tmpraw=rhs.GetSigmaRaw()[ichan];
    double varraw = tmpraw*tmpraw+sigmaraw[ichan]*sigmaraw[ichan];
    sigma[ichan] = sqrt(var);
    sigmaraw[ichan]= sqrt(varraw);
    tmp = (rhs.GetNGIndex()[ichan]);
    var = tmp*tmp + ngindex[ichan]*ngindex[ichan];
    ngindex[ichan] += sqrt(var);
    CN_mask[ichan] = (CN_mask[ichan]||rhs.GetCNMask()[ichan]);
  }
  for(int iCol=0; iCol<N_COLUMN; ++iCol) 
     column_mask[iCol] = rhs.GetColumnMask()[iCol];
     
  // event info
  StartEvent = std::min(StartEvent,rhs.GetStartEvent());
  StopEvent = std::max(StopEvent,rhs.GetStopEvent());
  
  return *this; // return the result by reference
}
 
LTrackerCalibrationSlot operator+(LTrackerCalibrationSlot lhs,        // passing lhs by value helps optimize chained a+b+c
		   const LTrackerCalibrationSlot& rhs) // otherwise, both parameters may be const references
{
  lhs += rhs; // reuse compound assignment
  return lhs; // return the result by value (uses move constructor)
}


LTrackerCalibrationSlot& LTrackerCalibrationSlot::operator/=(const double& rhs) {
  for(int ichan=0; ichan<NCHAN; ++ichan) {
    pedestal[ichan] /= rhs;
    sigmaraw[ichan] /= sqrt(rhs);
    sigma[ichan] /= sqrt(rhs);
    ngindex[ichan] /= sqrt(rhs);
  }

  return *this; // return the result by reference
}
