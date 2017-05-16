#include "LCaloCalibration.hh"
#include <iostream>
#include <algorithm>
#include <math.h>

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
  std::cout << __LCALOCALIBRATION__ << "Calibration file writing..." << std::endl;

  *output << RunId << std::endl;
  *output << InitialTargetRun << " " << FinalTargetRun << std::endl;
  for(int iChan=0; iChan<NPMT; ++iChan) {
    *output << pedestal[iChan] << " " << sigma[iChan] << " "
      /*	   << outliers[iChan] << " "
	   << skewness[iChan] << " "
	   << kurtosis[iChan] */
	   << std::endl;
  }
  *output << std::endl;
  std::cout << __LCALOCALIBRATION__ << "Calibration file written." << std::endl;

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

  for(int iChan=0; iChan<NPMT; ++iChan) {
    *input >> pedestalST[iChan] >>  sigmaST[iChan];
    /*	  >> outliersST[iChan]
	  >> skewnessST[iChan]
	  >> kurtosisST[iChan];*/
  }
  std::cout << __LCALOCALIBRATION__ << "Calibration file read." << std::endl << std::flush;;

  LCaloCalibration *result = new LCaloCalibration(RunIdST, InitialTargetRunST, FinalTargetRunST,
						  pedestalST, sigmaST);//, outliersST, skewnessST, kurtosisST);
  std::cout << __LCALOCALIBRATION__ << "Calo calibration created (" << &result << ")" << std::endl << std::flush;

  return result;
}

LCaloCalibration* LCaloCalibration::Read(const char *fileIn) {

  std::ifstream input(fileIn, std::ifstream::in);
  LCaloCalibration *result = Read(&input);
  input.close();
  return result;
}


LCaloCalibration& LCaloCalibration::operator=(const LCaloCalibration& other) {
  for(int ipmt=0; ipmt<NPMT; ++ipmt) {
    pedestal[ipmt] = other.GetPedestal()[ipmt];
    sigma[ipmt] = other.GetSigma()[ipmt];
    /* outliers[ipmt] = other.GetOutliers()[ipmt];
       skewness[ipmt] = other.GetSkewness()[ipmt];
       kurtosis[ipmt] = other.GetKurtosis()[ipmt];*/
  }

  // In/out run info
  RunId = other.GetRunId();
  InitialTargetRun = other.GetInitialTargetRun();
  FinalTargetRun = other.GetFinalTargetRun();

  return *this;
}

LCaloCalibration& LCaloCalibration::operator+=(const LCaloCalibration& rhs) // compound assignment (does not need to be a member,
{                           // but often is, to modify the private members)

  std::vector<double> ped(NPMT,0.);
  std::vector<double> var(NPMT,0.);
  std::vector<int> outl(NPMT,0.);
  std::vector<double> skew(NPMT,0.);
  std::vector<double> kurt(NPMT,0.);

  for(uint ipmt=0; ipmt<NPMT; ++ipmt) {
    double tmp = rhs.GetPedestal()[ipmt];
    ped[ipmt] = pedestal[ipmt]+tmp;
    double tmpsig = rhs.GetSigma()[ipmt];
    var[ipmt] = sigma[ipmt]*sigma[ipmt]+tmpsig*tmpsig;
    /* tmp = rhs.GetOutliers()[ipmt];
       outl[ipmt] = outliers[ipmt] + tmp;
       tmp = other.GetSkewness()[ipmt];
       skew[ipmt] = (skewness[ipmt]*sigma[ipmt]*sigma[ipmt]*sigma[ipmt] + tmp*tmpsig*tmpsig*tmpsig)/
       (sigma[ipmt]*sigma[ipmt]*sigma[ipmt]+tmpsig*tmpsig*tmpsig);  // correlations neglected!!!
       tmp = other.GetKurtosis()[ipmt];
       kurt[ipmt] = (kurtosis[ipmt]*sigma[ipmt]*sigma[ipmt]*sigma[ipmt]*sigma[ipmt] + tmp*tmpsig*tmpsig*tmpsig*tmpsig)/
       (sigma[ipmt]*sigma[ipmt]*sigma[ipmt]*sigma[ipmt]+tmpsig*tmpsig*tmpsig*tmpsig);  // correlations neglected!!!
    */
  }
  for(uint ipmt=0; ipmt<NPMT; ++ipmt) {
    pedestal[ipmt] = ped.at(ipmt);
    sigma[ipmt] = sqrt(var.at(ipmt));
    /*  outliers[ipmt] = outl[ipmt];
    skewness[ipmt] = skew[ipmt];
    kurtosis[ipmt] = kurt[ipmt];
    */
  }

  // In/out run info
  RunId = std::min(RunId, rhs.GetRunId());
  InitialTargetRun = std::min(InitialTargetRun, rhs.GetInitialTargetRun());
  FinalTargetRun = std::max(FinalTargetRun, rhs.GetFinalTargetRun());

  return *this; // return the result by reference
}

LCaloCalibration operator+(LCaloCalibration lhs,        // passing lhs by value helps optimize chained a+b+c
		   const LCaloCalibration& rhs) // otherwise, both parameters may be const references
{
  lhs += rhs; // reuse compound assignment
  return lhs; // return the result by value (uses move constructor)
}


LCaloCalibration& LCaloCalibration::operator/=(const double& rhs) {

  for(int ipmt=0; ipmt<NPMT; ++ipmt) {
    pedestal[ipmt] /= rhs;
    sigma[ipmt] /= sqrt(rhs);
    /*  outliers[ipmt] = static_cast<int>(outliers[ipmt]/rhs);
    skewness[ipmt] /= rhs;
    kurtosis[ipmt] /= rhs;
    */
  }

  return *this; // return the result by reference
}

