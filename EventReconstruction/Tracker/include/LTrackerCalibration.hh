#ifndef __LTRACKERCALIBRATION__
#define __LTRACKERCALIBRATION__ 1

#include "LTrackerCalibrationSlot.hh"
#include <vector>


class LTrackerCalibration {
public:
  LTrackerCalibration();
  LTrackerCalibration(const int RunIdINP, const int InitialTargetRunINP, const int FinalTargetRun);
  void Add(const LTrackerCalibrationSlot *lcal);
  void Write(const char *fileOut);
  static LTrackerCalibration* Read(const char *fileIn);
  inline int GetNSlots() const {return nSlots;};
  inline int GetRunId() const {return RunId;};
  inline int GetInitialTargetRun() const {return InitialTargetRun;};
  inline int GetFinalTargetRun() const {return FinalTargetRun;};
  inline const double* GetPedestal(const int nSlot) const {return calarray.at(nSlot).GetPedestal();};
  inline const double* GetSigma(const int nSlot) const {return calarray.at(nSlot).GetSigma();};
  inline const double* GetNGIndex(const int nSlot) const {return calarray.at(nSlot).GetNGIndex();};
  inline const bool* GetCNMask(const int nSlot) const {return calarray.at(nSlot).GetCNMask();};
  inline LTrackerMask GetMaskOnSigma(const int nSlot, const double sigmaMin, const double sigmaMax){return calarray.at(nSlot).GetMaskOnSigma(sigmaMin, sigmaMax);};
  inline LTrackerMask GetMaskOnNGI(const int nSlot, const double ngiMin, const double ngiMax){return calarray.at(nSlot).GetMaskOnNGI(ngiMin, ngiMax);};
  
private:
  // Calib infos
  int RunId;
  int nSlots;
  int InitialTargetRun;
  int FinalTargetRun;
  std::vector<LTrackerCalibrationSlot> calarray;

  void Reset();
};


#endif

