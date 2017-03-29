#ifndef __LEVREC0FILE__
#define __LEVREC0FILE__ 1

#include "LEvRec0.hh"

#include "TFile.h"
#include "TTree.h"


class LEvRec0File {
public:
  LEvRec0File();
  LEvRec0File(const char *inpFile);
  int SetTheEventPointer(LEvRec0 &event);
  // bool GetEntry(int iEntry, LEvRec0 &event); // for future... NO ROOT!
  int GetEntry(int iEntry);
  int GetEntries();
  void Open(const char* inpFile);
  void Reset(void);
  void Close();
  inline bool IsOpen() {return fFile->IsOpen();}
  inline int GetRunId(){return RunId;};
  inline const char* GetName(){return fFile->GetName();};
  ~LEvRec0File();
  
private:
  TFile *fFile;
  TTree *fTree;
  int RunId;
};


#endif
