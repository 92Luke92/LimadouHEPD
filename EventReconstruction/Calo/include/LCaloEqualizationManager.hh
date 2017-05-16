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

  struct {
    std::string calFileName;
    std::string inpFileList;
    std::string outDirectory;
    std::vector<std::string> L1fname;
    bool verboseFLAG;
    double sigma;
    double threshold;
  } steer;

  LEvRec1File* calRunFile;
  LCalibration* cal;  // not used now...will be used to check if pedestal
                      // position is drifted...

  int LoadInputFileList();
  int LoadRun(const std::string inputFileROOT,
              const std::string calibrationFile);
  LCaloEqualization* Equalize(const bool isHG);
  inline LCaloEqualization* EqualizeHG() { return Equalize(true); };
  inline LCaloEqualization* EqualizeLG() { return Equalize(false); };

 public:
  static LCaloEqualizationManager& GetInstance();
  void LoadSteering(const std::string steerFileIN);
  int Run();

  LCaloEqualizationManager(LCaloEqualizationManager const&) = delete;
  void operator=(LCaloEqualizationManager const&) = delete;
};

#endif
