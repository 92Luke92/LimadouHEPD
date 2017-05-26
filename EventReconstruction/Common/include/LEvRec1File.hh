#ifndef __LEVREC1FILE__
#define __LEVREC1FILE__ 1

#include "LEvRec1.hh"

#include "TFile.h"
#include "TTree.h"


class LEvRec1File {
public:
  LEvRec1File(const char *inpFile, const char *openMode);
  int SetTheEventPointer(LEvRec1 &event);
  // bool GetEntry(int iEntry, LEvRec1 &event); // for future... NO ROOT!
  int GetEntry(int iEntry);
  int GetEntries();
  void Write();
  void Close();
  inline bool IsOpen() {return fFile->IsOpen();}

  // Write MODE
  int SetRunId(const int rid);
  void Fill(const LEvRec1 event);
  void FillRandom(const int nEvents);
  
  ~LEvRec1File();
  
  friend class LEvRec0File; // to set RunId and BootNr...
  
private:
  LEvRec1Stream evstr; // read-write information stream
  LEvRec1 *evptr;      // pointer to LEvRec1 event for reading
  TFile *fFile;
  TTree *fTree;
  TTree * fTreeMd;
  TBranch *tracker_nClBR;
  TBranch *tracker_seedBR;
  TBranch *tracker_count0BR;
  TBranch *tracker_count1BR;
  TBranch *tracker_count2BR;
  TBranch *tracker_count3BR;
  TBranch *tracker_count4BR;
  TBranch *tracker_sn0BR;
  TBranch *tracker_sn1BR;
  TBranch *tracker_sn2BR;
  TBranch *tracker_sn3BR;
  TBranch *tracker_sn4BR;
  TBranch *trigger_countHGBR;
  TBranch *trigger_snHGBR;
  TBranch *trigger_countLGBR;
  TBranch *trigger_snLGBR;
  TBranch *scint_countHGBR;
  TBranch *scint_snHGBR;
  TBranch *scint_countLGBR;
  TBranch *scint_snLGBR;
  TBranch *veto_countHGBR;
  TBranch *veto_snHGBR;
  TBranch *veto_countLGBR;
  TBranch *veto_snLGBR;
  TBranch *lyso_countHGBR;
  TBranch *lyso_snHGBR;
  TBranch *lyso_countLGBR;
  TBranch *lyso_snLGBR;
  
  TBranch *runTypeBR;
  TBranch *boot_nrBR;
  TBranch *run_idBR;
  TBranch *event_indexBR;
  TBranch *event_lengthBR;
  TBranch *trigger_indexBR;
  TBranch *hepd_timeBR;
  TBranch *PMTBoard_trigger_counterBR;
  TBranch *lost_triggerBR;
  TBranch *rate_meterBR;
  TBranch *alive_timeBR;
  TBranch *dead_timeBR;

  bool WritableFLAG;
  void CreateTrees();
  void InitializeBranches();
  void SetAddresses();
};




#endif
