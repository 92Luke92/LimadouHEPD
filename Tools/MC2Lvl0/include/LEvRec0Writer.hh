#ifndef __LEvRec0Writer__
#define __LEvRec0Writer__ 1

#include "LEvRec0.hh"

#include "TFile.h"
#include "TTree.h"


class LEvRec0Writer {
public:
  LEvRec0Writer();
  LEvRec0Writer(std::string filename);
  int GetEntries() {return fTree->GetEntries();}
  void Fill() {fTree->Fill();}
  void Write() {fTree->Write();}
  void Init(std::string filename);
  void Close();

  LEvRec0* pev() {return &ev;}

  inline bool IsOpen() {return fFile->IsOpen();}
  inline const char* GetName(){return fFile->GetName();}
  ~LEvRec0Writer();

private:
  void SetTheEventPointer();
  LEvRec0 ev;
  TFile *fFile;
  TTree *fTree;

};


#endif
