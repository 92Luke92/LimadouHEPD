#ifndef __LCALOCALIBRATION__
#define __LCALOCALIBRATION__ "LCaloCalibration  ######### "

#include "detector_const.hh"
#include <fstream>


class LCaloCalibration {
public:
  LCaloCalibration();
  LCaloCalibration(const int RunIdINP,
		   const double* ped, const double *rms); // outliers? momenta 3-4?
  //               const int* outliers,
  //               const double* skewness, const double* kurtosis);
  void Write(const char *fileOut) const;
  void Write(std::ofstream *fileOut) const;
  static LCaloCalibration* Read(const char *fileIn);
  static LCaloCalibration* Read(std::ifstream *fileIn);
  static LCaloCalibration* ReadRoot(const char *fileIn, enum pmt_calib_type flag);
  static LCaloCalibration* CreateFakeCalibration(const LCaloCalibration* seed,const double offset=0);

  inline int GetRunId() const {return RunId;};
  inline const double* GetPedestal() const {return pedestal;};
  inline const double* GetSigma() const {return sigma;};
  //inline const int* GetOutliers() const {return outliers;};
  //inline const double* GetSkewness() const {return skewness;};
  //inline const double* GetKurtosis() const {return kurtosis;};
  //  LCaloMask GetMaskOnSigma(const double sigmaMin, const double sigmaMax);
  //LCaloMask GetMaskOnOutliers(const int outliersMin, const int outliersMax);
  LCaloCalibration& operator=(const LCaloCalibration& other);
  LCaloCalibration& operator+=(const LCaloCalibration& rhs); // compound assignment (does not need to be a member,
  friend LCaloCalibration operator+(LCaloCalibration lhs,        // passing lhs by value helps optimize chained a+b+c
		     const LCaloCalibration& rhs);// otherwise, both parameters may be const references
  LCaloCalibration& operator/=(const double& rhs);

private:
  // Calib infos
  int RunId;

   double pedestal[NPMT]; // if the order of the channels is according to the level1 format, the ReadRoot method is wrong!!! (Alessandro)
  double sigma[NPMT];  // if the order of the channels is according to the level1 format, the ReadRoot method is wrong!!! (Alessandro)
  /*  int outliers[NPMT];
  double skewness[NPMT];
  double kurtosis[NPMT];
  */

  void Reset();
};


#endif

