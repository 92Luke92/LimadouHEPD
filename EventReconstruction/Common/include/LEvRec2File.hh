#ifndef __LEVREC2FILE__
#define __LEVREC2FILE__ 1

#include "LEvRec2.hh"

#include "TFile.h"
#include "TTree.h"

class LEvRec2File {
public:
  LEvRec2File(const char *inpFile, const char *openMode);
  int SetTheEventPointer(LEvRec2 &event);

  int GetEntry(int iEntry);
  int GetEntries();
  void Write();
  void Close();
  inline bool IsOpen() {return fFile->IsOpen();}

  // Write MODE
  int SetRunId(const int rid);
  void Fill(const LEvRec2 event);
  void FillRandom(const int nEvents);
  
  ~LEvRec2File();

  private:
    LEvRec2Stream evstr; // read-write information stream
    LEvRec2 *evptr;      // pointer to LEvRec2 event for reading
    TFile *fFile;
    TTree *fTree;
    TTree * fTreeMd;
    TBranch *Francesco;
    TBranch *Ester;
    TBranch *Roberto;

    bool WritableFLAG;
    void CreateTrees();
    void InitializeBranches();
    void SetAddresses();
};

#endif