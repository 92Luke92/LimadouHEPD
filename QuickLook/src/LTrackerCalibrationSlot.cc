#include "LTrackerCalibrationSlot.hh"

LTrackerCalibrationSlot::LTrackerCalibrationSlot(int StartE, int StopE, double* rawsig, double *ped, double *sig, double *ngi, bool *cnm) {
  StartEvent=StartE;
  StopEvent=StopE;
  for(int iChan=0; iChan<NCHAN; ++iChan) {
    sigmaraw[iChan]=rawsig[iChan];
    pedestal[iChan]=ped[iChan];
    sigma[iChan]=sig[iChan];
    ngindex[iChan]=ngi[iChan];
    CN_mask[iChan]=cnm[iChan];
  }
}

void LTrackerCalibrationSlot::Write(std::ofstream *output) {
  *output << "EVENTS " << StartEvent << " " << StopEvent << std::endl;
  for(int iChan=0; iChan<NCHAN; ++iChan)
    *output << pedestal[iChan] << " " << sigma[iChan] << " " << ngindex[iChan] << " " << CN_mask[iChan] << std::endl;
  return;
}

LTrackerCalibrationSlot* LTrackerCalibrationSlot::Read(std::ifstream *input) {
  char word[100];
  int StartEventST, StopEventST;
  double sigmarawST[NCHAN], pedestalST[NCHAN], sigmaST[NCHAN], ngindexST[NCHAN];
  bool cnmST[NCHAN];
  *input >> word >> StartEventST >>  StopEventST;
  for(int iChan=0; iChan<NCHAN; ++iChan)
    *input >> sigmarawST[iChan] >> pedestalST[iChan] >>  sigmaST[iChan] >> ngindexST[iChan] >> cnmST[iChan];
  
  LTrackerCalibrationSlot *result = new LTrackerCalibrationSlot(StartEventST, StopEventST, sigmarawST, pedestalST, sigmaST, ngindexST, cnmST);
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

