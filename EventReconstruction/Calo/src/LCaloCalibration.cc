#include "LCaloCalibration.hh"
#include <iostream>

LCaloCalibration::LCaloCalibration() {
  Reset();
}

void LCaloCalibration::Reset(void) {
  RunId=-99999;
  InitialTargetRun=-99999;
  FinalTargetRun=-99999;
  for(int iPmt=0; iPmt<NPMT; ++iPmt) {
    pedestal[iPmt]=0.;
    sigma[iPmt]=0.;
    /*    outliers[iPmt]=0;
    skewness[iPmt]=0.;
    kurtosis[iPmt]=0.; */
  }
  return;
}

LCaloCalibration::LCaloCalibration(const int RunIdINP,
				   const int InitialTargetRunINP, const int FinalTargetRunINP,
				   const double* ped, const double *rms) {  // outliers? momenta 3-4?
  //                               const int* outl,
  //                               const double* skew, const double* kurt);

  RunId=RunIdINP;
  InitialTargetRun=InitialTargetRunINP;
  FinalTargetRun=FinalTargetRunINP;
  for(int iChan=0; iChan<NPMT; ++iChan) {
    pedestal[iChan]=ped[iChan];
    sigma[iChan]=rms[iChan];
    /*    outliers[iChan]=outl[iChan];
    skewness[iChan]=skew[iChan];
    kurtosis[iChan]=kurt[iChan];*/
  }
}


void LCaloCalibration::Write(std::ofstream *output) const {
  *output << RunId << std::endl;
  *output << InitialTargetRun << " " << FinalTargetRun << std::endl;
  for(int iChan=0; iChan<NPMT; ++iChan)
    *output << pedestal[iChan] << " " << sigma[iChan] << " "
      /*	   << outliers[iChan] << " "
	   << skewness[iChan] << " "
	   << kurtosis[iChan] */
	   << std::endl;
  *output << std::endl;
  
  return;
}

void LCaloCalibration::Write(const char *fileOut) const {
  std::ofstream output(fileOut, std::ofstream::out); 
  Write(&output);
  output.close();
  return;
}


LCaloCalibration* LCaloCalibration::Read(std::ifstream *input) {
  
  int RunIdST, InitialTargetRunST, FinalTargetRunST;
  double pedestalST[NPMT];
  double sigmaST[NPMT];
  /*  int outliersST[NPMT];
  double skewnessST[NPMT];
  double kurtosisST[NPMT];
  */

  *input >> RunIdST;
  *input >> InitialTargetRunST >> FinalTargetRunST;

  for(int iChan=0; iChan<NCHAN; ++iChan) {
    *input >> pedestalST[iChan] >>  sigmaST[iChan];
    /*	  >> outliersST[iChan] 
	  >> skewnessST[iChan] 
	  >> kurtosisST[iChan];*/
  }
  LCaloCalibration *result = new LCaloCalibration(RunIdST, InitialTargetRunST, FinalTargetRunST,
						  pedestalST, sigmaST);//, outliersST, skewnessST, kurtosisST);

  return result;
}

LCaloCalibration* LCaloCalibration::Read(const char *fileIn) {
  
  std::ifstream input(fileIn, std::ifstream::in); 
  LCaloCalibration *result = Read(&input);
  input.close();  return result;
}
 
