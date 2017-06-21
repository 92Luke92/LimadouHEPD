#ifndef __LTRACKERCALIBRATION__
#define __LTRACKERCALIBRATION__ "LTrackerCalibration  ###### "

#include "LTrackerCalibrationSlot.hh"
#include <vector>
#include <fstream>

class LTrackerCalibration {
public:
  LTrackerCalibration();
  LTrackerCalibration(const int RunIdINP);
  void Add(const LTrackerCalibrationSlot lcal);
  void DefaultFilling();
  void Write(const char *fileOut) const;
  void Write(std::ofstream *fileOut) const;
  static LTrackerCalibration* Read(const char *fileIn);
  static LTrackerCalibration* Read(std::ifstream *fileIn);
  inline LTrackerCalibrationSlot GetTrackerCalibrationSlot(const int nSlot) const {return calarray.at(nSlot);};
  inline int GetNSlots() const {return nSlots;};
  inline int GetRunId() const {return RunId;};
  inline const double* GetSigmaRaw(const int nSlot) const {return calarray.at(nSlot).GetSigmaRaw();};
  inline const double* GetPedestal(const int nSlot) const {return calarray.at(nSlot).GetPedestal();};
  inline const double* GetSigma(const int nSlot) const {return calarray.at(nSlot).GetSigma();};
  inline const double* GetNGIndex(const int nSlot) const {return calarray.at(nSlot).GetNGIndex();};
  inline const bool* GetCNMask(const int nSlot) const {return calarray.at(nSlot).GetCNMask();};
  inline LTrackerMask GetMaskOnSigma(const int nSlot, const double sigmaMin, const double sigmaMax){return calarray.at(nSlot).GetMaskOnSigma(sigmaMin, sigmaMax);};
  inline LTrackerMask GetMaskOnNGI(const int nSlot, const double ngiMin, const double ngiMax){return calarray.at(nSlot).GetMaskOnNGI(ngiMin, ngiMax);};
  LTrackerCalibration& operator=(const LTrackerCalibration& other);
  LTrackerCalibration& operator+=(const LTrackerCalibration& rhs); // compound assignment (does not need to be a member,
  friend LTrackerCalibration operator+(LTrackerCalibration lhs,        // passing lhs by value helps optimize chained a+b+c
		     const LTrackerCalibration& rhs);// otherwise, both parameters may be const references 
  LTrackerCalibration& operator/=(const double& rhs);

private:
  // Calib infos
  int RunId;
  int nSlots;
  std::vector<LTrackerCalibrationSlot> calarray;

  void Reset();
};


#endif

