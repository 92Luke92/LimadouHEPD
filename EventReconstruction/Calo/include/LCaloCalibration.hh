#ifndef __LCALOCALIBRATION__
#define __LCALOCALIBRATION__ 1

#include "detector_const.hh"
#include <vector>
#include <fstream>


class LCaloCalibration {
public:
  LCaloCalibration();
  LCaloCalibration(const int RunIdINP,
		   const int InitialTargetRunINP, const int FinalTargetRunINP,
		   const double* ped, const double *rms); // outliers? momenta 3-4?
  //               const int* outliers,
  //               const double* skewness, const double* kurtosis);
  void Write(const char *fileOut) const;
  void Write(std::ofstream *fileOut) const;
  static LCaloCalibration* Read(const char *fileIn);
  static LCaloCalibration* Read(std::ifstream *fileIn);
  inline int GetRunId() const {return RunId;};
  inline int GetInitialTargetRun() const {return InitialTargetRun;};
  inline int GetFinalTargetRun() const {return FinalTargetRun;};
  inline const double* GetPedestal() const {return pedestal;};
  inline const double* GetSigma() const {return sigma;};
  //inline const int* GetOutliers() const {return outliers;};
  //  LCaloMask GetMaskOnSigma(const double sigmaMin, const double sigmaMax);
  //LCaloMask GetMaskOnOutliers(const int outliersMin, const int outliersMax);
  
private:
  // Calib infos
  int RunId;
  int InitialTargetRun;
  int FinalTargetRun;

  double pedestal[NPMT];
  double sigma[NPMT];
  /*  int outliers[NPMT];
  double skewness[NPMT];
  double kurtosis[NPMT];
  */

  void Reset();
};


#endif

