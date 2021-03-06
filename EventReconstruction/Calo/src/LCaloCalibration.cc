#include "LCaloCalibration.hh"
#include "LEvRec0File.hh"
#include "detector_const.hh"

#include <iostream>
#include <algorithm>
#include <math.h>
#include <random>
#include <chrono>

const double __CHECKCALO__=0.1;

const bool LCaloCalibration::CheckStatus(const double *refPedestal, const double *refSigma) const {
  bool result = true;
  for(int iPmt=0; iPmt<NPMT; ++iPmt) {
    if(fabs((pedestal[iPmt]-refPedestal[iPmt])/pedestal[iPmt])>__CHECKCALO__) result = false;
    if(fabs((sigma[iPmt]-refSigma[iPmt])/sigma[iPmt])>__CHECKCALO__) result = false;
  }
  if(result==false) std::cerr << __LCALOCALIBRATION__ << " warning! CheckStatus pictured a bad situation!" << std:: endl;
  return false;
}

LCaloCalibration::LCaloCalibration() {
  Reset();
}

void LCaloCalibration::Reset(void) {
  RunId=-99999;
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
				   const double* ped, const double *rms) {  // outliers? momenta 3-4?
  //                               const int* outl,
  //                               const double* skew, const double* kurt);

  RunId=RunIdINP;
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

  int RunIdST;
  double pedestalST[NPMT];
  double sigmaST[NPMT];
  /*  int outliersST[NPMT];
  double skewnessST[NPMT];
  double kurtosisST[NPMT];
  */

  *input >> RunIdST;

  for(int iChan=0; iChan<NPMT; ++iChan) {
    *input >> pedestalST[iChan] >>  sigmaST[iChan];
    /*	  >> outliersST[iChan]
	  >> skewnessST[iChan]
	  >> kurtosisST[iChan];*/
  }
  std::cout << __LCALOCALIBRATION__ << "Calibration file read." << std::endl << std::flush;;

  LCaloCalibration *result = new LCaloCalibration(RunIdST,
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

LCaloCalibration* LCaloCalibration::ReadRoot(const char *fileIn, enum pmt_calib_type flag) {

   int RunIdST;
   double pedestalST[NPMT];
   double sigmaST[NPMT];
   
   LEvRec0 outev;
   LEvRec0File inputFile(fileIn);
   inputFile.SetTheEventPointer(outev);

   inputFile.GetEntry(0); // ped

   RunIdST = outev.run_id;
   for(int iChan=0; iChan<NPMT; ++iChan){
      if(flag == HIGH)
      {
	 pedestalST[iChan] = outev.pmt_high[iChan];
	 //std::cout << " pedestalST[" << iChan << "] = " << pedestalST[iChan] << std::endl;
      }
      else if (flag == LOW)
      {
	 pedestalST[iChan] = outev.pmt_low[iChan];
	 //std::cout << " pedestalST[" << iChan << "] = " << pedestalST[iChan] << std::endl;
      }
   }

   inputFile.GetEntry(1); // sig
   
   for(int iChan=0; iChan<NPMT; ++iChan){
      if(flag == HIGH)
      {
	 sigmaST[iChan] = outev.pmt_high[iChan];
	 //std::cout << " sigmaST[" << iChan << "] = "  << sigmaST[iChan] << std::endl;
		 
      }
      else if (flag == LOW)
      {
	 sigmaST[iChan] = outev.pmt_low[iChan];
	 //std::cout << " sigmaST[" << iChan << "] = " << sigmaST[iChan] << std::endl;
		 
      }
   }

   std::cout << __LCALOCALIBRATION__ << "Root Calibration file read."
	     << std::endl << std::flush;;

  LCaloCalibration *result = new LCaloCalibration(RunIdST,
						  pedestalST, sigmaST);//, outliersST, skewnessST, kurtosisST);
  std::cout << __LCALOCALIBRATION__ << "Calo calibration created ("
	    << &result << ")" << std::endl << std::flush;

  return result;

}


const int LCaloCalibration::trigger_cal(const int i, const int j, int flag) const {
  if(i<0 || i>=NTRIGSCINT) return -999;
  if(j<0 || j>=2) return -999;
  if(flag==PED) return pedestal[i+j*NPMT/2];
  else if(flag==SIG) return sigma[i+j*NPMT/2];
  else return -999;
}

const int LCaloCalibration::plane_cal(const int i, const int j, int flag) const {
  if(i<0 || i>=NSCINTPLANES) return -999;
  if(j<0 || j>=2) return -999;
  if(flag==PED) return pedestal[NTRIGSCINT+i+j*NPMT/2];
  else if(flag==SIG) return sigma[NTRIGSCINT+i+j*NPMT/2];
  else return -999;
}

const int LCaloCalibration::lyso_cal(const int i, int flag) const {
   if(i<0 || i>=NLYSOCRYSTALS) return -999;
   if (flag ==PED)
   {
      if(i==0) return pedestal[28]; // North West
      if(i==1) return pedestal[62]; // North
      if(i==2) return pedestal[29]; // North East
      if(i==3) return pedestal[30]; // West
      if(i==4) return pedestal[31]; // Center
      if(i==5) return pedestal[60]; // East
      if(i==6) return pedestal[27]; // South West
      if(i==7) return pedestal[61]; // South
      if(i==8) return pedestal[59]; // South East
   }
   if (flag == SIG)
   {
      if(i==0) return sigma[28]; // North West
      if(i==1) return sigma[62]; // North
      if(i==2) return sigma[29]; // North East
      if(i==3) return sigma[30]; // West
      if(i==4) return sigma[31]; // Center
      if(i==5) return sigma[60]; // East
      if(i==6) return sigma[27]; // South West
      if(i==7) return sigma[61]; // South
      if(i==8) return sigma[59]; // South East
   }

}

const int LCaloCalibration::veto_cal(const int i, const int j, int flag) const {
  if(i<0 || i>=NVETOSCINT) return -999;
  if(j<0 || j>=2) return -999;
  if (flag==PED)
     return pedestal[NTRIGSCINT+NSCINTPLANES +i+j*NPMT/2];
  if (flag==SIG)
     return sigma[NTRIGSCINT+NSCINTPLANES +i+j*NPMT/2];

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

  return *this;
}

LCaloCalibration& LCaloCalibration::operator+=(const LCaloCalibration& rhs) // compound assignment (does not need to be a member,
{                           // but often is, to modify the private members)

  std::vector<double> ped(NPMT,0.);
  std::vector<double> var(NPMT,0.);
  std::vector<int> outl(NPMT,0.);
  std::vector<double> skew(NPMT,0.);
  std::vector<double> kurt(NPMT,0.);

  for(uint16_t ipmt=0; ipmt<NPMT; ++ipmt) {
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
  for(uint16_t ipmt=0; ipmt<NPMT; ++ipmt) {
    pedestal[ipmt] = ped.at(ipmt);
    sigma[ipmt] = sqrt(var.at(ipmt));
    /*  outliers[ipmt] = outl[ipmt];
    skewness[ipmt] = skew[ipmt];
    kurtosis[ipmt] = kurt[ipmt];
    */
  }

  // In/out run info
  RunId = std::min(RunId, rhs.GetRunId());

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

LCaloCalibration* LCaloCalibration::CreateFakeCalibration(const LCaloCalibration* seed,const double offset){
  const double* pedestal=seed->GetPedestal();
  const double* sigma=seed->GetSigma();

  double sigmaNew[NPMT];
  double pedNew[NPMT];

  typedef std::chrono::high_resolution_clock myclock;
  myclock::time_point beginning = myclock::now();
  std::default_random_engine generator;
  myclock::duration d = myclock::now() - beginning;
  unsigned seed2 = d.count();
  generator.seed(seed2);

  for(int iChan=0;iChan<NPMT;++iChan){
    std::normal_distribution<double> ped_distr(pedestal[iChan]+offset,0.5);
    std::normal_distribution<double> sigma_distr(sigma[iChan],0.5);
    sigmaNew[iChan]=sigma_distr(generator);
    pedNew[iChan]=ped_distr(generator);
  }
  
  LCaloCalibration* result=new LCaloCalibration(seed->GetRunId(),pedNew,sigmaNew);
  return result;

}
