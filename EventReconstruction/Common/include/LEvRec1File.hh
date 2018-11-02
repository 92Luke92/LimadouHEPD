#ifndef __LEVREC1FILE__
#define __LEVREC1FILE__ 1

#include "LEvRec1.hh"
#include "LEvRec0.hh"

#include "TFile.h"
#include "TTree.h"


class LEvRec1File {
public:
  LEvRec1File(const char *inpFile, const char *openMode);
  int SetTheEventPointer(LEvRec1 &event);
  // bool GetEntry(int iEntry, LEvRec1 &event); // for future... NO ROOT!
  int GetEntry(int iEntry);
  int GetEntries();
  int GetMDEntry(int iEntry);
  int GetMDEntries();
  void Write();
  void Close();
  int SetMDPointer(LEvRec1 &ev);  

  inline bool IsOpen() {return fFile->IsOpen();}
  inline bool ContainsMD() {return metaDataFlag; }

  // Write MODE
  int SetRunId(const int rid);
  void Fill(const LEvRec1 event);
  void FillMD();
  void FillRandom(const int nEvents);
  void SetMDTreeAddress(LEvRec1 &l1ev);
   
  ~LEvRec1File();
  
  friend class LEvRec0File; // to set RunId and BootNr...
  
private:
  LEvRec1Stream evstr; // read-write information stream
  LEvRec1 *evptr;      // pointer to LEvRec1 event for reading
  
  TFile *fFile;
  TTree *fTree;
  TTree * fTreeMD;
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
  TBranch *trigger_trigger_flagBR;
  TBranch *trigger_is_saturatedBR;
  TBranch *scint_countHGBR;
  TBranch *scint_snHGBR;
  TBranch *scint_countLGBR;
  TBranch *scint_snLGBR;
  TBranch *scint_trigger_flagBR;
  TBranch *scint_is_saturatedBR;
  TBranch *veto_countHGBR;
  TBranch *veto_snHGBR;
  TBranch *veto_countLGBR;
  TBranch *veto_snLGBR;
  TBranch *veto_trigger_flagBR;
  TBranch *veto_is_saturatedBR;
  TBranch *lyso_countHGBR;
  TBranch *lyso_snHGBR;
  TBranch *lyso_countLGBR;
  TBranch *lyso_snLGBR;
  TBranch *lyso_trigger_flagBR;
  TBranch *lyso_is_saturatedBR;

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
 
   TBranch *rigBR;     
   TBranch *abstimeBR ;
   TBranch *BBR ;    
   TBranch *lonBR ;    
   TBranch *latBR ;  
   TBranch *altBR ;  
   TBranch *LBR ;  
   TBranch *B_xBR ;    
   TBranch *B_yBR ;  
   TBranch *B_zBR ;  
   
  TBranch *run_idMDBR ;
  TBranch *boot_nrMDBR;
  TBranch *run_typeMDBR;
  TBranch *run_durationBR;
  TBranch *orbitZoneBR;

  TBranch *silConfigurationBR;
  TBranch *trigger_maskBR;
  TBranch *easiroc_confBR;
  TBranch *PMT_maskBR;
  TBranch *HV_maskBR;
  TBranch *HV_valueBR;
  TBranch *gen_trig_maskBR;
  TBranch *OBDH_infoBR;
  TBranch *GPS_infoBR;
  TBranch *AOCC_infoBR;
  TBranch *star_sensor_infoBR;
  TBranch *PMT_rate_meterBR;
  TBranch *CPU_tempBR;
  TBranch *PMT_tempBR;
  TBranch *CPU_timeBR;
  TBranch *CPU_timestampBR;
  TBranch *status_registerBR;


   
  bool WritableFLAG;
  bool metaDataFlag;
  void CreateTrees();
  void InitializeBranches();
  void SetAddresses();
};




#endif
