#ifndef __LCALIBRATIONMANAGER__
#define __LCALIBRATIONMANAGER__ "LCalibrationManager   ##### "

#include "LCalibration.hh"

class LCalibrationManager {
  
public:
  static LCalibrationManager& GetInstance();
  void LoadSteering(const char *steerFileIN);
  LCalibration* Calibrate(const int nEvents=-1, const int skipEvents=-1);
  void Run();
  bool SetVerbosity(const bool boolFLAG);

private:
  std::string steerFile;
  bool verboseFLAG;
  bool steeringLoadedFLAG;

  // Loaded from steering file
  std::string inpFileList;
  std::string inpRunMode;
  std::string outDirectory;
  int skipEvents, skipFileEvents, maxEvents, maxFileEvents, trackerSlotEvents;
  int InitialTargetRun, FinalTargetRun;
    
  void LoadRun(const char *fileInp);
  void SetTargetRuns(const int InitialRun, const int FinalRun=-1);
  LCalibrationManager();
  ~LCalibrationManager();
  void Reset();
  bool CheckLoadedSteering(void) const;


  /*  
  // C++ 03
  // ========
  // Dont forget to declare these two. You want to make sure they
  // are unacceptable otherwise you may accidentally get copies of
  // your singleton appearing.
  LCalibration(LCalibration const&);              // Don't Implement
  void operator=(LCalibration const&); // Don't implement
  */

  //  /* Following implementation to bepreferred, Not yet fully compatible

  // C++ 11
  // =======
  // We can use the better technique of deleting the methods
  // we don't want.
public:
  LCalibrationManager(LCalibrationManager const&) = delete;
  void operator=(LCalibrationManager const&) = delete;
  // */
};

#endif
