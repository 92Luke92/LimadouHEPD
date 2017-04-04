#ifndef __LRECO01MANAGER__
#define __LRECO01MANAGER__ "LReco01Manager   ########## "

#include <vector>
#include <string>
#include "LEvRec0File.hh"
#include "LEvRec1File.hh"
#include "LCalibration.hh"

const int MINL0EVENTS=100;
const int PRINTOUTEVENTS=1000;

class LReco01Manager {

public:
  static LReco01Manager& GetInstance();
  void LoadSteering(const char *steerFileIN);
  void Run(void);

private:
  LReco01Manager();
  ~LReco01Manager();

  void Reset(void);
  bool CheckLoadedSteering(void) const;
  int LoadInpFileList(void);
  bool CheckInputFile(const std::string fname) const;
  void NewOutFile(void);
  LEvRec1 L0ToL1(const LEvRec0 lev0, const LCalibration *cal);
  std::string L0NameToL1Name(void);
  
  std::string steerFile;

  std::string calFileName;
  LCalibration *cal;
  std::string inpFileList;
  std::string outDirectory;
  int maxEvents; // max events to be processed in total
  int maxFileEvents; // max events to be processed for each file
  bool verboseFLAG;
  bool steeringLoadedFLAG;

  std::vector< std::string > L0fname;
  LEvRec0File *inFile;
  LEvRec1File *outFile;

  /*  
  // C++ 03
  // ========
  // Dont forget to declare these two. You want to make sure they
  // are unacceptable otherwise you may accidentally get copies of
  // your singleton appearing.
  LReco01Manager(LReco01Manager const&);              // Don't Implement
  void operator=(LReco01Manager const&); // Don't implement
  */

  //  /* Following implementation to bepreferred, Not yet fully compatible

  // C++ 11
  // =======
  // We can use the better technique of deleting the methods
  // we don't want.
public:
  LReco01Manager(LReco01Manager const&) = delete;
  void operator=(LReco01Manager const&) = delete;
  // */
};

#endif
