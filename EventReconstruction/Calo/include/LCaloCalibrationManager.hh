#ifndef __LCALOCALIBRATIONMANAGER__
#define __LCALOCALIBRATIONMANAGER__ "LCaloCalibrationManager   # "

#include "LCaloCalibration.hh"
#include "LEvRec0File.hh"
#include "LStatTools.hh"
#include <functional>

const int HGPEAKFINDERWINDOW = 500;     // units of ADC
const int LGPEAKFINDERWINDOW = 450;     // units of ADC
const int DATACALWINDOWMAX = 450;       // units of ADC
const int DATACALWINDOWMIN = 230;       // units of ADC
const double STARTINGSIGMA = 5.;        // units of ADC

const double RMSFINDINGHALFWINDOW = 3.; // units of "old" RMS
const double RMSCORRECTIONFACTOR = 1./9.865784e-01;  // be careful! Change it if you change the RMSFINDINGHALFWINDOW!

const double SKEWKURTFINDINGHALFWINDOW = 3.;// units of established sigma
const double KURTOSISCORRECTIONFACTOR = (3./2.67281);// be careful! Change it if you change the SKEWKURTFINDINGHALFWINDOW!

class LCaloCalibrationManager {

public:
  static LCaloCalibrationManager& GetInstance();

  int LoadRun(const char *fileInp);
  inline LCaloCalibration* CalibrateHG(const int nEvents=-1, const int skipEvents=-1){return Calibrate(true, nEvents, skipEvents);};
  inline LCaloCalibration* CalibrateLG(const int nEvents=-1, const int skipEvents=-1){return Calibrate(false, nEvents, skipEvents);};
  inline bool SetVerbosity(const bool boolFLAG) { verboseFLAG = boolFLAG; return verboseFLAG; };

private:
  LEvRec0File *calRunFile;  // pointer to the run used for calibration

  int __skipEv;
  int __nEv;

  LCaloCalibrationManager();

  LCaloCalibration* Calibrate(const bool isHG, const int nEvents=-1, const int skipEvents=-1);
  void PMTsRawMeanRms(const bool isHG, double *mean, double *rms) const;
  void PMTsRawMeanRmsHG(double *mean, double *rms) const;
  void PMTsRawMeanRmsLG(double *mean, double *rms) const;
  void PMTsMomenta34(const double *pedestal, const double *sigma, const bool isHG,
		     double *m3, double *m4) const;
  void PMTsMomenta34HG(const double *pedestal, const double *sigma,
		     double *m3, double *m4) const;
  void PMTsMomenta34LG(const double *pedestal, const double *sigma,
		     double *m3, double *m4) const;
                                                                             // cntssxdx cointains: out-of-distribution counts (sx,dx)
  void PMTsWindowedRmsHG(const double *old_mean, const double *old_rms,  double *new_mean, double *new_rms, int *cntssxdx) const;
  void PMTsWindowedRmsLG(const double *old_mean, const double *old_rms,  double *new_mean, double *new_rms, int *cntssxdx) const;
  void PMTsWindowedRms(const double *old_mean, const double *old_rms,  const bool isHG, double *new_mean, double *new_rms, int *cntssxdx) const ;// res contains: new_mean, new_sigma;
                                                                           // cntssxdx cointains: out-of-distribution counts (sx,dx)
  int FindPeak(const int pmtnum, const int PeakFinderWindowWidth, const bool isHG) const;
  int GetPeakHG(const int pmtnum) const;
  int GetPeakLG(const int pmtnum) const;
  int* GetPeaksHG() const;
  int* GetPeaksLG() const;

  std::vector <std::map  <int, float>> MapCalibFromPredicate(std::function <bool(int content, bool trigger_flag, int iCh)> predicate, const bool isHG) const;

  ~LCaloCalibrationManager();


  bool verboseFLAG;


  // Debugging stuff do we need to keep it?
  void PMTsMeanRmsData(const int pmt, double *res) const;



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
  LCaloCalibrationManager(LCaloCalibrationManager const&) = delete;
  void operator=(LCaloCalibrationManager const&) = delete;
  // */
};

#endif
