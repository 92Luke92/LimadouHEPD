#ifndef __LCaloCalManager__
#define __LCaloCalManager__ 1


#include "LEvRec0File.hh"

class LCaloCalManager {

public:
  static LCaloCalManager& GetInstance();
  
  int LoadRun(const char *fileInp);
  void SetTargetRuns(const int InitialRun, const int FinalRun=-1);
  int PMTsMeanRms(double *HGmean, double *HGrms,  double *LGmean, double *LGrms);
  int PMTsMomenta34(double *HGm3, double *HGm4,  double *LGm3, double *LGm4);
  int PMTsWindowedRmsHG(int pmt, double old_mean, double old_rms,  double *res, int *cntssxdx);// res contains: new_mean, new_sigma;
                                                                           // cntssxdx cointains: out-of-distribution counts (sx,dx)
  int PMTsWindowedRmsLG(int pmt, double old_mean, double old_rms,  double *res, int *cntssxdx);
  int PMTsMeanRmsData(int pmt, double *res);
  double GetPeak(int pmtnum);
  double GetPeakLG(int pmtnum);

private:
  LEvRec0File *calRunFile;  // pointer to the run used for calibration
  int InitialTargetRun;
  int FinalTargetRun;
  LCaloCalManager();
  
  ~LCaloCalManager();

  bool verboseFLAG;

  /*  
  // C++ 03
  // ========
  // Dont forget to declare these two. You want to make sure they
  // are unacceptable otherwise you may accidentally get copies of
  // your singleton appearing.
  LCaloCalibration(LCaloCalibration const&);              // Don't Implement
  void operator=(LCaloCalibration const&); // Don't implement
  */

  //  /* Following implementation to bepreferred, Not yet fully compatible

  // C++ 11
  // =======
  // We can use the better technique of deleting the methods
  // we don't want.
public:
  LCaloCalManager(LCaloCalManager const&) = delete;
  void operator=(LCaloCalManager const&) = delete;
  // */
};

#endif
