#ifndef __LCALOEQUALIZATIONMANAGER__
#define __LCALOEQUALIZATIONMANAGER__ "LCaloEqualizationManager   # "

#include <string>
#include "LCalibration.hh"
#include "LCaloEqualization.hh"
#include "LEvRec1File.hh"

class LCaloEqualizationManager {
 private:
  LCaloEqualizationManager();
  ~LCaloEqualizationManager();
  bool verboseFLAG;
  LEvRec1File* calRunFile;
  LCalibration* cal;
  LCaloEqualization* Equalize(const bool isHG);

 public:
  static LCaloEqualizationManager& GetInstance();
  int LoadRun(const std::string inputFileROOT,
              const std::string calibrationFile);
  inline LCaloEqualization* EqualizeHG() { return Equalize(true); };
  inline LCaloEqualization* EqualizeLG() { return Equalize(false); };

  LCaloEqualizationManager(LCaloEqualizationManager const&) = delete;
  void operator=(LCaloEqualizationManager const&) = delete;
};

#endif
