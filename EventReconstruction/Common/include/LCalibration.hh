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
  inline void SetInitialRunId(const int in) {InitialRunId=in; return;};
  inline void SetFinalRunId(const int in) {FinalRunId=in; return;};
  inline void SetInitialTargetRun(const int in) {InitialTargetRun=in; return;};
  inline void SetFinalTargetRun(const int in) {FinalTargetRun=in; return;};
  inline int GetInitialRunId() const {return InitialRunId;};
  inline int GetFinalRunId() const {return FinalRunId;};
  inline int GetInitialTargetRun() const {return InitialTargetRun;};
  inline int GetFinalTargetRun() const {return FinalTargetRun;};
  bool CheckStatus(void) const;  
  inline LTrackerCalibration* GetTrackerCalibration() const {return tracker;};
  inline LCaloCalibration* GetCaloHGCalibration() const {return calo_HG;} ;
  inline LCaloCalibration* GetCaloLGCalibration() const {return calo_LG;};
  void Reset();
  LCalibration& operator=(const LCalibration& other);
  LCalibration& operator+=(const LCalibration& rhs); // compound assignment (does not need to be a member,
  friend LCalibration operator+(LCalibration lhs,        // passing lhs by value helps optimize chained a+b+c       |bool LCalibrationManager::CheckLoadedSteering(void) const {
		     const LCalibration& rhs);// otherwise, both parameters may be const references 
  LCalibration& operator/=(const double& rhs);
  std::string GetWriteOutNameBase(void);

private:
  int InitialRunId, FinalRunId;
  int InitialTargetRun, FinalTargetRun;
  LTrackerCalibration *tracker;
  LCaloCalibration *calo_HG;
  LCaloCalibration *calo_LG;
};


#endif

