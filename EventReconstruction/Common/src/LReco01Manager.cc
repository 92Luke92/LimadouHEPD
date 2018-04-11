#include "LReco01Manager.hh"
#include "LSteeringManager.hh"
#include "LTrackerTools.hh"
#include "LCaloTools.hh"

#include <iostream>
#include <sys/stat.h>

LReco01Manager::LReco01Manager() {
  Reset();
}


void LReco01Manager::LoadSteering(const char* steerFileIN) {
  Reset();
  std::cout << __LRECO01MANAGER__ << "Loading the steering file \""
	    << steerFileIN << "\"..." << std::endl;

  steerFile = steerFileIN;
  LSteeringManager steer;
  steer.Load(steerFile);
  // Load calibration
  calFileName = steer.GetParameter<std::string>("CALBFILE");
  cal = LCalibration::ReadROOT(calFileName.c_str());
  //cal = LCalibration::Read(calFileName.c_str());
  // Input file 
  L0fname = steer.GetParameter<std::string>("INPFILE");
  // Output folder
  outDirectory = steer.GetParameter<std::string>("OUTFOLD");
  // Skip Events
  skipEvents = steer.GetParameter<int>("SKIPEVTS");
  // Max File Events
  maxFileEvents = steer.GetParameter<int>("MAXFEVTS");
  // Verbosity
  verboseFLAG = steer.GetParameter<bool>("VERBOSE");
  std::cout << __LRECO01MANAGER__ << "Steering file loaded." << std::endl;
  steeringLoadedFLAG=true;

  if(!CheckLoadedSteering()) Reset();

  return;
}

void LReco01Manager::Reset(void) {
  steerFile="";
  calFileName="";
  outDirectory="";
  skipEvents=0;
  maxFileEvents=0;
  verboseFLAG=true;
  steeringLoadedFLAG=false;
  if(cal) cal->Reset();
  cal=0;
  L0fname="";
  inFile=0;
  outFile=0;
  return;
}

bool LReco01Manager::CheckLoadedSteering(void) const {
  std::cout << __LRECO01MANAGER__ << "CheckLoadedSteering():" << std::endl;
  bool result=true;
  if(steeringLoadedFLAG==false) {
    std::cout << __LRECO01MANAGER__ << "steering file never loaded." << std::endl;
    result=false;
  }
  if(cal->CheckStatus()==false) {
    std::cout << __LRECO01MANAGER__ << "calibration status bad." << std::endl;
    result=false;
  }

  struct stat odir;
  int status = stat(outDirectory.c_str(), &odir);
  if(status!=0) result=false;

  if(! CheckInputFile(L0fname)) {
    std::cout <<  __LRECO01MANAGER__ << "Error: the file " << L0fname << " has been checked and found not to work." << std::endl;
    result=false;
  }
  if(skipEvents<0 && skipEvents!=-1) result=false;
  if(maxFileEvents<0 && maxFileEvents!=-1) result=false;
  if(result==false) {
    std::cout << __LRECO01MANAGER__ << "something is wrong or missing in the steering file" << std::endl;
  }
  else {
    std::cout << __LRECO01MANAGER__ << "steering file \"" << steerFile << "\" ok." << std::endl;
    if(verboseFLAG==true) {
      std::cout << __LRECO01MANAGER__ << "Reco parameters:" << std::endl;
      std::cout << __LRECO01MANAGER__ << "calFileName  " << calFileName << std::endl;
      std::cout << __LRECO01MANAGER__ << "inpFile  " << L0fname << std::endl;
      std::cout << __LRECO01MANAGER__ << "outDirectory " << outDirectory << std::endl;
      std::cout << __LRECO01MANAGER__ << "skipEvents    " << skipEvents << std::endl;
      std::cout << __LRECO01MANAGER__ << "maxFileEvents    " << maxFileEvents << std::endl;
    }
  }
  return result;
}


void LReco01Manager::Run(void) {
  if(steeringLoadedFLAG==false) {
    std::cout << __LRECO01MANAGER__ << "How to run? Steering file never loaded!" << std::endl;
    return;
  }

  // Actual run
  int fevtcounter=0;
  std::cout << __LRECO01MANAGER__ << "Reading L0 file " << L0fname << std::endl;
  inFile = new LEvRec0File(L0fname.c_str());
  NewOutFile();
  LEvRec0 lev0;
  inFile->SetTheEventPointer(lev0);
  LEvRec0Md lev0MD;
  inFile->SetMdPointer(lev0MD);

  int nentries=inFile->GetEntries();
  std::cout << __LRECO01MANAGER__ << "Looping on file " << L0fname << " (" << nentries << " entries)" << std::endl;
  int startEvent = (skipEvents==-1 ? 0 : skipEvents);
  for(int i0=startEvent; i0<nentries; ++i0){
    if(i0%PRINTOUTEVENTS==0) {
      std::cout << __LRECO01MANAGER__
		<<"event " << i0 << "/" <<nentries << std::endl;
    }
    inFile->GetEntry(i0);
    
    LEvRec1 l1ev = L0ToL1(lev0, cal);
    outFile->Fill(L0ToL1(lev0, cal));
    ++fevtcounter;
    if(maxFileEvents!=-1 && fevtcounter>maxFileEvents-1) break;

    if(i0 == 0 || i0 == 1)
    {
        inFile->GetMDEntry(i0);
	l1ev.lev0MD = lev0MD;

	outFile->SetMDTreeAddress(l1ev);
	outFile->FillMD();
    }
  }

  std::cout << __LRECO01MANAGER__ << std::endl;
  inFile->Close();
  delete inFile;
  outFile->Write();
  outFile->Close();
  delete outFile;

  return;
}


void LReco01Manager::NewOutFile(void) {
  if(!inFile) {
    std::cout << __LRECO01MANAGER__ << "No input file to get the output name from." << std::endl << std::flush;
    return;
  }
  if(outFile!=0 && outFile->IsOpen()) {
    outFile->Write();
    outFile->Close();
    delete outFile;
  }

  // Set the new name
  std::string l1name = L0NameToL1Name();
  outFile = new LEvRec1File(l1name.c_str(),"WRITE");
  std::cout << __LRECO01MANAGER__ << "Output file \"" << l1name << "\" created." << std::endl;
  return;
}


std::string LReco01Manager::L0NameToL1Name(void) { // Naming convention needed!
  std::string l0name=inFile->GetName();
  std::string l1name=l0name;
  
  // Getting the basename
  size_t index=l1name.rfind("/");
  l1name.erase(l1name.begin(),l1name.begin()+index+1);

  // Replacing L0 --> L1                                               
  index=0;
  while(true) {
    /* Locate the substring to replace. */
    index = l1name.find("L0", index);
    if (index == std::string::npos) break;

    /* Make the replacement. */
    l1name.replace(index, 2, "L1");

    /* Advance index forward so the next iteration doesn't pick it up as well. \
     */
    index += 6;
  }
  // No need to change the suffix
  std::string result=outDirectory + "/" + l1name;
  
  std::cout << __LRECO01MANAGER__ << "Output file name \"" << result << "\"" << std::endl;
  return result;
}


LEvRec1 LReco01Manager::L0ToL1(const LEvRec0 lev0IN, const LCalibration *calIN) {
  LEvRec1 result;

  result.runType = lev0IN.runType;
  result.boot_nr = lev0IN.boot_nr; 
  result.run_id = lev0IN.run_id;
  result.event_index = lev0IN.event_index;
  result.event_length = lev0IN.event_length;
  result.trigger_index = lev0IN.trigger_index;
  result.hepd_time = lev0IN.hepd_time;
  result.PMTBoard_trigger_counter  = lev0IN.PMTBoard_trigger_counter;
  result.lost_trigger = lev0IN.lost_trigger;
  for(int i=0; i<NRATEMETER; ++i) result.rate_meter[i] = lev0IN.rate_meter[i];
  result.alive_time = lev0IN.alive_time;
  result.dead_time = lev0IN.dead_time;
  
  if(lev0IN.runType==0x4E) {
    result.tracker=GetTrackerSignal(lev0IN, *calIN);
  }
  else if(lev0IN.runType==0x55) {
    result.tracker=GetTrackerSignalCompressed(lev0IN, *calIN);
  }
  else {
    std::cerr << __LRECO01MANAGER__ << "RunType unmanageable: " << lev0IN.runType << std::endl; 
    LEvRec1 ___result_res;
    return ___result_res;
  }
  result.trig=GetTriggerSignal(lev0IN, *calIN);
  result.scint=GetScintillatorSignal(lev0IN, *calIN);
  result.veto=GetVetoSignal(lev0IN, *calIN);
  result.lyso=GetLysoSignal(lev0IN, *calIN);

  return result;
}

bool LReco01Manager::CheckInputFile(const std::string fname) const {
  bool result =true;
  LEvRec0File fin(fname.c_str());
  if(fin.GetEntries()<MINL0EVENTS) result=false;
  return result;
}

LReco01Manager& LReco01Manager::GetInstance() {
  static LReco01Manager instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
  return instance;
}


LReco01Manager::~LReco01Manager() {
  // do not care about singleton destructor
}


