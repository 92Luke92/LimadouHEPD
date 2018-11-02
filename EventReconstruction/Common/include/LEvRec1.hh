#ifndef __LEVREC1__
#define __LEVREC1__ 1

#include "detector_const.hh"
#include "analysis_const.hh"

#include "LTrackerSignal.hh"
#include "LTriggerSignal.hh"
#include "LScintillatorSignal.hh"
#include "LVetoSignal.hh"
#include "LLysoSignal.hh"
#include "LEvRec0.hh"


#include <cstddef>

struct LEvRec1Stream;
class LEvRec0MD;

class LEvRec1 {

public:
  LEvRec1();
  ~LEvRec1();
  inline int GetNOfTrackerClusters(void) const {return static_cast<int>(tracker.GetSize());};
  void Dump(int entry) const;
  void DumpTracker(void) const;
  void DumpCalo(void) const;
  void DumpTrigger(void) const;
  void DumpScintillator(void) const;
  void DumpVeto(void) const;
  void DumpLyso(void) const;
  void DumpMD(int entry) const;
  void FillRandom(void);
  void CopyFromLEvRec1Stream(const LEvRec1Stream evstr);

  // Analysis
  bool DiscontinousSignal(const double threshold_sn) const;
  int GetLastPlaneHit(const double threshold_sn) const;
  double GetMSPlaneToMSBarRatio(const double threshold_sn) const;
  double GetScintCounts(const double threshold_sn) const;
  double GetTriggerCounts(const double threshold_sn) const;
   bool PreSelection(const double threshold_sn, const int mostSigPaddle,
		     const double numOfPlanesInTrig) const;
  bool AutoVeto(const double threshold_veto, const int mostSigPaddle) const ;
  bool isLatVetoHit(const double threshold_sn) const;
  bool LysoVeto(const double threshold_sn) const ;
  bool isBotVetoHit(const double threshold_sn) const ;

  int triggerMultiplicity(const double threshold_sn) const;
  int planeMultiplicity(const double threshold_sn) const ;
  int lysoMultiplicity(const double threshold_sn) const;
  int lastPlaneHit(const double threshold_sn) const;
  bool areallPlaneHit(const double threshold_sn) const;

  unsigned short   runType;
  unsigned short   boot_nr;
  unsigned short   run_id;
  unsigned int     event_index;
  unsigned short   event_length;
  unsigned int     trigger_index;
  unsigned int     hepd_time;
  unsigned int     PMTBoard_trigger_counter;
  unsigned short   lost_trigger;
  unsigned short   rate_meter[NRATEMETER];
  unsigned int     alive_time;
  unsigned int     dead_time;
  double           rig ;
  unsigned int     abstime;
  double           B ;
  double           L ;
  double           alt;
  double           lon;
  double           lat;
  double           B_x;
  double           B_y;  
  double           B_z;


  
  LTrackerSignal tracker;
  LTriggerSignal trig;
  LScintillatorSignal scint;
  LVetoSignal veto;
  LLysoSignal lyso;

  LEvRec0Md   lev0MD;

private:
  void Reset();

};



struct LEvRec1Stream {
  
  // Careful! It is a streaming! The order is important! Do not touch this class
  // if you do not know exactyl what you are doing (do not do that neither in
  // that case)
  
  double trigger_countHG[2*NTRIGSCINT]; // hardcoded because the tree branches are hardcoded
  double trigger_snHG[2*NTRIGSCINT];
  double trigger_countLG[2*NTRIGSCINT];
  double trigger_snLG[2*NTRIGSCINT];
  bool trigger_trigger_flag[2*NTRIGSCINT];
  bool trigger_is_saturated[2*NTRIGSCINT];
  double scint_countHG[2*NSCINTPLANES];
  double scint_snHG[2*NSCINTPLANES];
  double scint_countLG[2*NSCINTPLANES];
  double scint_snLG[2*NSCINTPLANES];
  bool scint_trigger_flag[2*NSCINTPLANES];
  bool scint_is_saturated[2*NSCINTPLANES];
  double veto_countHG[2*NVETOSCINT];
  double veto_snHG[2*NVETOSCINT];
  double veto_countLG[2*NVETOSCINT];
  double veto_snLG[2*NVETOSCINT];
  bool veto_trigger_flag[2*NVETOSCINT];
  bool veto_is_saturated[2*NVETOSCINT];
  double lyso_countHG[NLYSOCRYSTALS];
  double lyso_snHG[NLYSOCRYSTALS];
  double lyso_countLG[NLYSOCRYSTALS];
  double lyso_snLG[NLYSOCRYSTALS];
  bool lyso_trigger_flag[NLYSOCRYSTALS];
  bool lyso_is_saturated[NLYSOCRYSTALS];
  
  int nClusters;
  int seed[MAXNCLUSTERS];
  double count0[MAXNCLUSTERS];
  double count1[MAXNCLUSTERS];
  double count2[MAXNCLUSTERS];
  double count3[MAXNCLUSTERS];
  double count4[MAXNCLUSTERS];
  double sn0[MAXNCLUSTERS];
  double sn1[MAXNCLUSTERS];
  double sn2[MAXNCLUSTERS];
  double sn3[MAXNCLUSTERS];
  double sn4[MAXNCLUSTERS];

  unsigned short   runType;
  unsigned short   boot_nr;
  unsigned short   run_id;
  unsigned int     event_index;
  unsigned short   event_length;
  unsigned int     trigger_index;
  unsigned int     hepd_time;
  unsigned int     PMTBoard_trigger_counter;
  unsigned short   lost_trigger;
  unsigned short   rate_meter[NRATEMETER];
  unsigned int     alive_time;
  unsigned int     dead_time;
  double           rig;
  unsigned int     abstime;
  double           B;
  double           lat;
  double           lon;
  double           alt;
  double           L;
  double           B_x;
  double           B_y;
  double           B_z;

  LEvRec1Stream();
  LEvRec1Stream(const LEvRec1 event);
  void Reset();
  void CopyFromLEvRec1(const LEvRec1 event);
  void DumpTracker (void) const;
  void DumpTrigger (void) const;
  ~LEvRec1Stream();
};


#endif
