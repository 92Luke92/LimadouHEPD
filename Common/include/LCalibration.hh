#ifndef __LCALIBRATION__
#define __LCALIBRATION__ 1

#include "LTrackerCalibration.hh"
#include "LCaloCalibration.hh"


class LCalibration {
public:
  LCalibration(){;};
  void Read(const char *inFile){;};
  void Reset(){;};
  bool CheckStatus(void) const {return true;};
  LTrackerCalibration tracker;
  LCaloCalibration calo;
};


#endif

