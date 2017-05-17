#include "LCalibrationManager.hh"
#include "LCaloCalibrationManager.hh"
#include "LTrackerCalibrationManager.hh"
#include "LSteeringManager.hh"
#include <iostream>
#include <sys/stat.h>

LCalibrationManager::LCalibrationManager() {
  Reset();
}

LCalibrationManager::~LCalibrationManager() {
  // do not care about singleton destructor
}


void LCalibrationManager::LoadSteering(const char* steerFileIN) {
  Reset();
  std::cout << __LCALIBRATIONMANAGER__ << "Loading the steering file \""
	    << steerFileIN << "\"..." << std::endl;

  steerFile = std::string(steerFileIN);
  LSteeringManager steer;
  steer.Load(steerFile);

  // Input file list
  inpFileList = steer.GetParameter<std::string>("INPLIST");
  // Input run mode
  inpRunMode = steer.GetParameter<std::string>("RUNMODE");
  // Output folder
  outDirectory = steer.GetParameter<std::string>("OUTFOLD");
  // Events to skip at the beginning
  skipFileEvents = steer.GetParameter<int>("SKIPEVTS");
  // Events to skip at the beginning of each file
  skipFileEvents = steer.GetParameter<int>("SKIPFEVTS");
  // Max Events to calibrate on
  maxEvents = steer.GetParameter<int>("MAXEVTS");
  // Max File Events
  maxFileEvents = steer.GetParameter<int>("MAXFEVTS");
  // Tracker slot events
  trackerSlotEvents = steer.GetParameter<int>("SLOTEVTS");
  // Initial and Final Target Runs
  InitialTargetRun = steer.GetParameter<int>("INITRUN");
  FinalTargetRun = steer.GetParameter<int>("FINRUN");
  // Verbosity
  verboseFLAG = steer.GetParameter<bool>("VERBOSE");

  std::cout << __LCALIBRATIONMANAGER__ << "Steering file loaded." << std::endl;
  steeringLoadedFLAG=true;

  if(!CheckLoadedSteering()) Reset();

  return;
}

void LCalibrationManager::Reset(void) {
  steerFile="";
  inpFileList="";
  inpRunMode="";
  outDirectory="";
  skipEvents=-1;
  skipFileEvents=-1;
  maxEvents=-1;
  maxFileEvents=-1;
  trackerSlotEvents=-1;
  verboseFLAG=true;
  steeringLoadedFLAG=false;
  InitialTargetRun=-1;
  FinalTargetRun=-1;
  return;
}

void LCalibrationManager::Run() {
  if(steeringLoadedFLAG==false) {
    std::cout << __LCALIBRATIONMANAGER__ << "How to run? Steering file never loaded!" << std::endl;
    return;
  }

  std::vector<std::string> fnames;
  std::ifstream istr(inpFileList, std::ifstream::in);
  std::string line;
  while(getline(istr,line)) {
    if(line=="") continue;
    fnames.push_back(line);
  }

  std::vector<LCalibration*> cal_vector;
  for(auto fname : fnames) {
    LoadRun(fname.c_str());
    std::cout << __LCALIBRATIONMANAGER__ << "Processing file " << fname << "..." << std::endl;
    SetTargetRuns(InitialTargetRun,FinalTargetRun);
    cal_vector.push_back(Calibrate());
  }

  LCalibration outcal=(*cal_vector.front());
  int counter=1;
  for(auto cit = std::next(cal_vector.begin()); cit!= cal_vector.end(); ++cit) {
    outcal+=(**cit); // be careful: no weighting for the actual number of events we calibrated on
    ++counter;
  }
  outcal/=(static_cast<double>(counter));

  std::string outName = outDirectory + "/"
    + outcal.GetWriteOutNameBase()
    + "___inpRunMode_" + inpRunMode
    + ".cal";
  outcal.Write(outName.c_str());

  return;
}


bool LCalibrationManager::CheckLoadedSteering(void) const {
  std::cout << __LCALIBRATIONMANAGER__ << "CheckLoadedSteering():" << std::endl;
  bool result=true;
  if(steeringLoadedFLAG==false) {
    std::cout << __LCALIBRATIONMANAGER__ << "steering file never loaded." << std::endl;
    result=false;
  }
  std::ifstream istr(inpFileList, std::ifstream::in);
  if(istr.good()==false)  {
    std::cout << __LCALIBRATIONMANAGER__ << "Input file list " << inpFileList << " goodness=" << istr.good() << std::endl;
    result=false;
  }
  istr.close();
  if(inpRunMode.compare("virgin")!=0 &&
     inpRunMode.compare("compressed")!=0 &&
     inpRunMode.compare("mixed")!=0 ) {
    std::cout << __LCALIBRATIONMANAGER__ << "Run mode " << inpRunMode << " unknown." << std::endl;
    result=false;
  }

  struct stat odir;
  int status = stat(outDirectory.c_str(), &odir);
  if(status!=0) {
    std::cout << __LCALIBRATIONMANAGER__ << "Output directory " << outDirectory << " status=" << status << std::endl;
    result=false;
  }

  if(skipEvents<0 && skipEvents!=-1) result=false;
  if(skipFileEvents<0 && skipFileEvents!=-1) result=false;
  if(maxEvents<0 && maxEvents!=-1) result=false;
  if(maxFileEvents<0 && maxFileEvents!=-1) result=false;
  if(trackerSlotEvents<0 && trackerSlotEvents!=-1) result=false;
  if(InitialTargetRun<0 && InitialTargetRun!=-1) result=false;
  if(FinalTargetRun<0 && FinalTargetRun!=-1) result=false;

  std::cout << __LCALIBRATIONMANAGER__ << "Temptative calibration parameters:" << std::endl;
  std::cout << __LCALIBRATIONMANAGER__ << "  inpFileList  " << inpFileList << std::endl;
  std::cout << __LCALIBRATIONMANAGER__ << "  inpRunMode   " << inpRunMode << std::endl;
  std::cout << __LCALIBRATIONMANAGER__ << "  outDirectory " << outDirectory << std::endl;
  std::cout << __LCALIBRATIONMANAGER__ << "  maxEvents    " << maxEvents << std::endl;
  std::cout << __LCALIBRATIONMANAGER__ << "  maxFEvents   " << maxFileEvents << std::endl;
  std::cout << __LCALIBRATIONMANAGER__ << "  skipEvents   " << skipEvents << std::endl;
  std::cout << __LCALIBRATIONMANAGER__ << "  skipFEvents  " << skipFileEvents << std::endl;
  std::cout << __LCALIBRATIONMANAGER__ << "  slotEvents   " << trackerSlotEvents << std::endl;
  std::cout << __LCALIBRATIONMANAGER__ << "  IniTargetRun " << InitialTargetRun << std::endl;
  std::cout << __LCALIBRATIONMANAGER__ << "  FinTargetRun " << FinalTargetRun << std::endl;

  if(result==false) {
    std::cout << __LCALIBRATIONMANAGER__ << "something is wrong or missing in the steering file" << std::endl;
  }
  else {
    std::cout << __LCALIBRATIONMANAGER__ << "steering file \"" << steerFile << "\" ok." << std::endl;
  }
  return result;
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


void LCalibrationManager::LoadRun(const char *fileInp) {
  LTrackerCalibrationManager::GetInstance().LoadRun(fileInp);
  LCaloCalibrationManager::GetInstance().LoadRun(fileInp);

  return;
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
