#ifndef __LCALOMANAGER__
#define __LCALOMANAGER__ 1

#include "LCalibration.hh"

class LCalibrationManager {
  
public:
  static LCalibrationManager& GetInstance();
  
  int LoadRun(const char *fileInp);
  void SetTargetRuns(const int InitialRun, const int FinalRun=-1);
  LCalibration* Calibrate(const int nEvents=-1, const int skipEvents=-1);
  
private:
  LCalibrationManager(){;};
  ~LCalibrationManager();

  bool verboseFLAG;



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
