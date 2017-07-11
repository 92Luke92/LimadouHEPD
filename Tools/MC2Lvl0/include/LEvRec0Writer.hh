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
  void Fill() {
    fTree->Fill();
    fTmdTree->Fill();
  }
  void Write() {
    fTree->Write();
    fTmdTree->Write();
  }
  void Init(std::string filename);
  void Close();

  LEvRec0* pev() {return &evT;}
  LEvRec0md* pevmd() {return &evTmd;}

  inline bool IsOpen() {return fFile->IsOpen();}
  inline const char* GetName(){return fFile->GetName();}
  ~LEvRec0Writer();

private:
  void SetTheEventPointer();
  LEvRec0 evT;
  LEvRec0md evTmd;

  TFile *fFile;
  TTree *fTree;
  TTree *fTmdTree;

};


#endif
