#include "LCalibrationManager.hh"
#include "LCaloCalibrationManager.hh"
#include "LTrackerCalibrationManager.hh"

LCalibrationManager::~LCalibrationManager() {
  // do not care about singleton destructor
}


LCalibrationManager& LCalibrationManager::GetInstance() {
  static LCalibrationManager instance; // Guaranteed to be destroyed.
                                              // Instantiated on first use.
  return instance;
}


//---------------------------------------------------------------------------

bool LCalibrationManager::SetVerbosity(const bool boolFLAG) {
  verboseFLAG=boolFLAG;
  LTrackerCalibrationManager::GetInstance().SetVerbosity(boolFLAG);
  LCaloCalibrationManager::GetInstance().SetVerbosity(boolFLAG);
  return boolFLAG;
}


//---------------------------------------------------------------------------


int LCalibrationManager::LoadRun(const char *fileInp) {
  LTrackerCalibrationManager::GetInstance().LoadRun(fileInp);
  LCaloCalibrationManager::GetInstance().LoadRun(fileInp);
  
  return 0;
}


//---------------------------------------------------------------------------

void LCalibrationManager::SetTargetRuns(const int InitialRun, const int FinalRun) {
  LTrackerCalibrationManager::GetInstance().SetTargetRuns(InitialRun, FinalRun);
  LCaloCalibrationManager::GetInstance().SetTargetRuns(InitialRun, FinalRun);
  return;
}


//---------------------------------------------------------------------------

LCalibration* LCalibrationManager::Calibrate(const int nEvents, const int skipEvents) {

  LCaloCalibration *caloHG_cal = LCaloCalibrationManager::GetInstance().CalibrateHG(nEvents, skipEvents);
  LCaloCalibration *caloLG_cal = LCaloCalibrationManager::GetInstance().CalibrateLG(nEvents, skipEvents);
  LTrackerCalibration *track_cal = LTrackerCalibrationManager::GetInstance().Calibrate(nEvents, skipEvents);

  LCalibration *result = new LCalibration(caloHG_cal, caloLG_cal, track_cal);
  return result;
}

