#ifndef __LTRACKERCALIBRATIONSLOT__
#define __LTRACKERCALIBRATIONSLOT__ 1

#include "LTrackerMask.hh"
#include "detector_const.hh"
#include <fstream>

class LTrackerCalibrationSlot {
public:
  LTrackerCalibrationSlot(){Reset();};
  LTrackerCalibrationSlot(int StartE, int StopE, double *ped, double *sig, double *ngi, bool *cnm);;
  void Write(std::ofstream *output);
  static LTrackerCalibrationSlot* Read(std::ifstream *input);
  inline int GetStartEvent() const {return StartEvent;};
  inline int GetStopEvent() const {return StopEvent;};
  inline const double* GetPedestal() const {return pedestal;};
  inline const double* GetSigma() const {return sigma;};
  inline const double* GetNGIndex() const {return ngindex;};
  inline const bool* GetCNMask() const {return CN_mask;};
  LTrackerMask GetMaskOnSigma(const double sigmaMin, const double sigmaMax);
  LTrackerMask GetMaskOnNGI(const double NGIMin, const double NGIMax);
  void Reset();
  LTrackerCalibrationSlot& operator=(const LTrackerCalibrationSlot& other);
  LTrackerCalibrationSlot& operator+=(const LTrackerCalibrationSlot& rhs); // compound assignment (does not need to be a member,
  friend LTrackerCalibrationSlot operator+(LTrackerCalibrationSlot lhs,        // passing lhs by value helps optimize chained a+b+c
		     const LTrackerCalibrationSlot& rhs);// otherwise, both parameters may be const references 
  LTrackerCalibrationSlot& operator/=(const double& rhs);
private:
  // Calib infos
  int StartEvent;
  int StopEvent;
  double pedestal[NCHAN];
  double sigma[NCHAN];
  double ngindex[NCHAN];
  bool CN_mask[NCHAN];
};




#endif
