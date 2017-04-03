#ifndef __LCALIBRATION__
#define __LCALIBRATION__ "LCalibration   ############ "

#include "LTrackerCalibration.hh"
#include "LCaloCalibration.hh"


class LCalibration {
public:
  LCalibration();
  LCalibration(LCaloCalibration *calo_HG_IN,
	       LCaloCalibration *calo_LG_IN,
	       LTrackerCalibration *tracker_IN);
  void Write(const char *fileOut) const;
  void Write(std::ofstream *fileOut) const;
  static LCalibration* Read(const char *fileIn);
  static LCalibration* Read(std::ifstream *fileIn);
  int GetRunId() const;
  int GetInitialTargetRun() const;
  int GetFinalTargetRun() const;
  bool CheckStatus(void) const;  
  inline LTrackerCalibration* GetTrackerCalibration() const {return tracker;};
  inline LCaloCalibration* GetCaloHGCalibration() const {return calo_HG;} ;
  inline LCaloCalibration* GetCaloLGCalibration() const {return calo_LG;};
  void Reset();
private:
  LTrackerCalibration *tracker;
  LCaloCalibration *calo_HG;
  LCaloCalibration *calo_LG;
};


#endif

