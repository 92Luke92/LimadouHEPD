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
  void WriteTXT(const char *fileOut) const;
  void WriteTXT(std::ofstream *fileOut) const;
  void WriteROOT(const char *fileOut) const;
  static LCalibration* Read(const char *fileIn);
  static LCalibration* Read(std::ifstream *fileIn);
  static LCalibration* ReadROOT(const char *fileIn);

  static LCalibration* CreateFakeCalibration(const LCalibration* seed,double tracker_offset=0,double calo_offset=0); //To create a fake we start from a real calibration file!

  inline void SetInputFile(const char * name) {filename = name; return;};
  inline const char * GetInputFile() const {return filename;};
  inline void SetRunId(const int in) {RunId=in; return;};
  inline int GetRunId() const {return RunId;};
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

private:
  int RunId;
  const char * filename;
  LTrackerCalibration *tracker;
  LCaloCalibration *calo_HG;
  LCaloCalibration *calo_LG;
};


#endif

