#include "LEvRec1.hh"
#include "LReco01Manager.hh"
#include <iostream>

LEvRec1::LEvRec1() {
  Reset();
}

LEvRec1::~LEvRec1() {

}

void LEvRec1::FillRandom() {
  tracker.FillRandom();
  trig.FillRandom();
  scint.FillRandom();
  veto.FillRandom();
  lyso.FillRandom();
  // other values left as default
  return;
}

void LEvRec1::Reset(){
  runType = 0x0;
  boot_nr=0;
  run_id=0;
  event_index=0;
  event_length=0;
  trigger_index=0;
  hepd_time=0;
  PMTBoard_trigger_counter = 0;
  lost_trigger=0;
  for(int i=0; i<NRATEMETER; ++i) rate_meter[i]=0;
  alive_time=0;
  dead_time=0;

  // if(ORBITAL_INFO)
  // {
  //    rig = 0;
  //    abstime = 0;
  //    B = 0;
  //    L = 0;
  //    alt = 0;
  //    lon = 0;
  //    lat = 0;
  //    B_x = 0;
  //    B_y = 0;  
  //    B_z = 0;
  // }

  tracker.Reset();
  trig.Reset();
  scint.Reset();
  veto.Reset();
  lyso.Reset();
  
  return;
}


void LEvRec1::Dump(int entry) const {
  DumpTracker();
  DumpCalo();
  DumpMD(entry);
  return;
}

void LEvRec1::DumpTracker() const {
  std::cout << "############# Tracker signal dumping." << std::endl;
  for(auto it : tracker.cls) it.Dump();
  return;
}

void LEvRec1::DumpCalo() const {
  DumpTrigger();
  DumpScintillator();
  DumpVeto();
  DumpLyso();
  return;
}

void LEvRec1::DumpTrigger() const {
  trig.Dump();
  return;
}

void LEvRec1::DumpScintillator() const {
  scint.Dump();
  return;
}

void LEvRec1::DumpVeto() const {
  veto.Dump();
  return;
}

void LEvRec1::DumpLyso() const {
  lyso.Dump();
  return;
}

void LEvRec1::DumpMD(int entry) const {
   if(entry == 0 || entry == 1 )
      lev0MD.Dump() ;
   
  return;
}


void LEvRec1::CopyFromLEvRec1Stream(const LEvRec1Stream evstr) {
  Reset();
  
  // info other than signal
  runType = evstr.runType;
  boot_nr = evstr.boot_nr; 
  run_id = evstr.run_id;
  event_index = evstr.event_index;
  event_length = evstr.event_length;
  trigger_index = evstr.trigger_index;
  hepd_time = evstr.hepd_time;
  PMTBoard_trigger_counter  = evstr.PMTBoard_trigger_counter;
  lost_trigger = evstr.lost_trigger;
  for(int i=0; i<NRATEMETER; ++i) rate_meter[i] = evstr.rate_meter[i];
  alive_time = evstr.alive_time;
  dead_time = evstr.dead_time;

  // if(ORBITAL_INFO)
  // {
  //    rig = evstr.rig;
  //    abstime = evstr.abstime;
  //    B = evstr.B;
  //    lat = evstr.lat;
  //    lon = evstr.lon;
  //    alt = evstr.alt;
  //    L = evstr.L;
  //    B_x = evstr.B_x;
  //    B_y = evstr.B_y;
  //    B_z = evstr.B_z;
  // }
  
  // tracker
  for(int i=0; i<evstr.nClusters; ++i) {
    LTrackerCluster cl;
    cl.seed = evstr.seed[i];
    cl.count[0] = evstr.count0[i];
    cl.count[1] = evstr.count1[i];
    cl.count[2] = evstr.count2[i];
    cl.count[3] = evstr.count3[i];
    cl.count[4] = evstr.count4[i];
    cl.sn[0] = evstr.sn0[i];
    cl.sn[1] = evstr.sn1[i];
    cl.sn[2] = evstr.sn2[i];
    cl.sn[3] = evstr.sn3[i];
    cl.sn[4] = evstr.sn4[i];
    cl.sigma[0] = evstr.count0[i]/evstr.sn0[i];
    cl.sigma[1] = evstr.count1[i]/evstr.sn1[i];
    cl.sigma[2] = evstr.count2[i]/evstr.sn2[i];
    cl.sigma[3] = evstr.count3[i]/evstr.sn3[i];
    cl.sigma[4] = evstr.count4[i]/evstr.sn4[i];
    cl.ComputeEta();
    tracker.push_back(cl);
  }

  // calo
  int nunits=trig.GetNUnits();
  int npmts=trig.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ip=0; ip<npmts; ++ip){
      trig.cont_hg[iu][ip]=evstr.trigger_countHG[iu*npmts+ip];
      trig.sn_hg[iu][ip]=evstr.trigger_snHG[iu*npmts+ip];
      trig.cont_lg[iu][ip]=evstr.trigger_countLG[iu*npmts+ip];
      trig.sn_lg[iu][ip]=evstr.trigger_snLG[iu*npmts+ip];
      trig.trigger_flag[iu][ip]=evstr.trigger_trigger_flag[iu*npmts+ip];
      trig.is_saturated[iu][ip]=evstr.trigger_is_saturated[iu*npmts+ip];
    }
  }
  nunits=scint.GetNUnits();
  npmts=scint.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ip=0; ip<npmts; ++ip){
      scint.cont_hg[iu][ip]=evstr.scint_countHG[iu*npmts+ip];
      scint.sn_hg[iu][ip]=evstr.scint_snHG[iu*npmts+ip];
      scint.cont_lg[iu][ip]=evstr.scint_countLG[iu*npmts+ip];
      scint.sn_lg[iu][ip]=evstr.scint_snLG[iu*npmts+ip];
      scint.trigger_flag[iu][ip]=evstr.scint_trigger_flag[iu*npmts+ip];
      scint.is_saturated[iu][ip]=evstr.scint_is_saturated[iu*npmts+ip];
    }
  }
  nunits=veto.GetNUnits();
  npmts=veto.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ip=0; ip<npmts; ++ip){
      veto.cont_hg[iu][ip]=evstr.veto_countHG[iu*npmts+ip];
      veto.sn_hg[iu][ip]=evstr.veto_snHG[iu*npmts+ip];
      veto.cont_lg[iu][ip]=evstr.veto_countLG[iu*npmts+ip];
      veto.sn_lg[iu][ip]=evstr.veto_snLG[iu*npmts+ip];
      veto.trigger_flag[iu][ip]=evstr.veto_trigger_flag[iu*npmts+ip];
      veto.is_saturated[iu][ip]=evstr.veto_is_saturated[iu*npmts+ip];
    }
  }
  nunits=lyso.GetNUnits();
  npmts=lyso.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ip=0; ip<npmts; ++ip){
      lyso.cont_hg[iu][ip]=evstr.lyso_countHG[iu*npmts+ip];
      lyso.sn_hg[iu][ip]=evstr.lyso_snHG[iu*npmts+ip];
      lyso.cont_lg[iu][ip]=evstr.lyso_countLG[iu*npmts+ip];
      lyso.sn_lg[iu][ip]=evstr.lyso_snLG[iu*npmts+ip];
      lyso.trigger_flag[iu][ip]=evstr.lyso_trigger_flag[iu*npmts+ip];
      lyso.is_saturated[iu][ip]=evstr.lyso_is_saturated[iu*npmts+ip];
    }
  }

  return;
}





LEvRec1Stream::LEvRec1Stream() {
  nClusters=0;
  Reset();
}

LEvRec1Stream::LEvRec1Stream(const LEvRec1 event) {
  CopyFromLEvRec1(event);
}

void LEvRec1Stream::CopyFromLEvRec1(const LEvRec1 event) {
  Reset();
  nClusters = event.GetNOfTrackerClusters();
  if(nClusters>MAXNCLUSTERS) {
    if(LReco01Manager::GetInstance().verboseFLAG>=2) {
      std::cerr << "LEvRec1Stream::CopyFromLEvRec1 error! Event containing "
	      << nClusters << " clusters passed as input. I copy only the first "
	      << "MAXNCLUSTERS=" << MAXNCLUSTERS << std::endl;
      std::cerr << "nClusters too is set to this number." << std::endl;
    }
    nClusters=MAXNCLUSTERS;
    // Warning! This approach relies on the cls vector having decending order in SN.
    // But it does not distinguish the single ladders! You may have the situation when
    // all MAXNCLUSTERS belong to the same plane!
  }
  for(int icl=0; icl<nClusters; ++icl) {
    seed[icl] = event.tracker.cls.at(icl).seed;
    count0[icl] = event.tracker.cls.at(icl).count[0];
    count1[icl] = event.tracker.cls.at(icl).count[1];
    count2[icl] = event.tracker.cls.at(icl).count[2];
    count3[icl] = event.tracker.cls.at(icl).count[3];
    count4[icl] = event.tracker.cls.at(icl).count[4];
    sn0[icl] = event.tracker.cls.at(icl).sn[0];
    sn1[icl] = event.tracker.cls.at(icl).sn[1];
    sn2[icl] = event.tracker.cls.at(icl).sn[2];
    sn3[icl] = event.tracker.cls.at(icl).sn[3];
    sn4[icl] = event.tracker.cls.at(icl).sn[4];
  }
  
  int nunits=event.trig.GetNUnits();
  int npmts=event.trig.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ip=0; ip<npmts; ++ip){
      trigger_countHG[iu*npmts+ip]=event.trig.cont_hg[iu][ip];
      trigger_snHG[iu*npmts+ip]=event.trig.sn_hg[iu][ip];
      trigger_countLG[iu*npmts+ip]=event.trig.cont_lg[iu][ip];
      trigger_snLG[iu*npmts+ip]=event.trig.sn_lg[iu][ip];
      trigger_trigger_flag[iu*npmts+ip]=event.trig.trigger_flag[iu][ip];
      trigger_is_saturated[iu*npmts+ip]=event.trig.is_saturated[iu][ip];
    }
  }

  nunits=event.scint.GetNUnits();
  npmts=event.scint.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ip=0; ip<npmts; ++ip){
      scint_countHG[iu*npmts+ip]=event.scint.cont_hg[iu][ip];
      scint_snHG[iu*npmts+ip]=event.scint.sn_hg[iu][ip];
      scint_countLG[iu*npmts+ip]=event.scint.cont_lg[iu][ip];
      scint_snLG[iu*npmts+ip]=event.scint.sn_lg[iu][ip];
      scint_trigger_flag[iu*npmts+ip]=event.scint.trigger_flag[iu][ip];
      scint_is_saturated[iu*npmts+ip]=event.scint.is_saturated[iu][ip];
    }
  }

  nunits=event.veto.GetNUnits();
  npmts=event.veto.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ip=0; ip<npmts; ++ip){
      veto_countHG[iu*npmts+ip]=event.veto.cont_hg[iu][ip];
      veto_snHG[iu*npmts+ip]=event.veto.sn_hg[iu][ip];
      veto_countLG[iu*npmts+ip]=event.veto.cont_lg[iu][ip];
      veto_snLG[iu*npmts+ip]=event.veto.sn_lg[iu][ip];
      veto_trigger_flag[iu*npmts+ip]=event.veto.trigger_flag[iu][ip];
      veto_is_saturated[iu*npmts+ip]=event.veto.is_saturated[iu][ip];
    }
  }

  nunits=event.lyso.GetNUnits();
  npmts=event.lyso.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ip=0; ip<npmts; ++ip){
      lyso_countHG[iu*npmts+ip]=event.lyso.cont_hg[iu][ip];
      lyso_snHG[iu*npmts+ip]=event.lyso.sn_hg[iu][ip];
      lyso_countLG[iu*npmts+ip]=event.lyso.cont_lg[iu][ip];
      lyso_snLG[iu*npmts+ip]=event.lyso.sn_lg[iu][ip];
      lyso_trigger_flag[iu*npmts+ip]=event.lyso.trigger_flag[iu][ip];
      lyso_is_saturated[iu*npmts+ip]=event.lyso.is_saturated[iu][ip];
    }
  }

  // info other than signal
  runType = event.runType;
  boot_nr = event.boot_nr; 
  run_id = event.run_id;
  event_index = event.event_index;
  event_length = event.event_length;
  trigger_index = event.trigger_index;
  hepd_time = event.hepd_time;
  PMTBoard_trigger_counter  = event.PMTBoard_trigger_counter;
  lost_trigger = event.lost_trigger;
  for(int i=0; i<NRATEMETER; ++i) rate_meter[i] = event.rate_meter[i];
  alive_time = event.alive_time;
  dead_time = event.dead_time;

  return;
}

void LEvRec1Stream::DumpTrigger(void) const {
  for(int iu=0; iu<NTRIGSCINT; ++iu) {
    for(int ip=0; ip<2; ++ip){
      std::cout << "[iu,ip]=" << iu <<","<<ip <<" " 
		<< trigger_countHG[iu*2+ip] << " "
		<< trigger_snHG[iu*2+ip] << " "
		<< trigger_countLG[iu*2+ip] << " "
		<< trigger_snLG[iu*2+ip] << " "
		<< trigger_trigger_flag[iu*2+ip] << " "
		<< trigger_is_saturated[iu*2+ip] << std::endl;
    }
  }

  return;
}

void LEvRec1Stream::DumpTracker(void) const {
  for(int icl=0; icl<nClusters; ++icl) {
    std::cout << "Seed: "
	      << seed[icl] << std::endl;
    std::cout << "Counts: "
	      << count0[icl] << "  " 
	      << count1[icl] << "  "
	      << count2[icl] << "  "
	      << count3[icl] << "  "
	      << count4[icl] << std::endl;
    std::cout << "sn: "
	      << sn0[icl] << "  " 
	      << sn1[icl] << "  "
	      << sn2[icl] << "  "
	      << sn3[icl] << "  "
	      << sn4[icl] << std::endl;
  }
  return;
}

void LEvRec1Stream::Reset() {
  nClusters=0;
  for(int i=0; i<MAXNCLUSTERS; ++i) {
    seed[i]=0;
    count0[i]=0.;
    count1[i]=0.;
    count2[i]=0.;
    count3[i]=0.;
    count4[i]=0.;
    sn0[i]=0.;
    sn1[i]=0.;
    sn2[i]=0.;
    sn3[i]=0.;
    sn4[i]=0.;
  }
  for(int i=0; i<2*NTRIGSCINT; ++i) {
    trigger_countHG[i]=0.;
    trigger_snHG[i]=0.;
    trigger_countLG[i]=0.;
    trigger_snLG[i]=0.;
    trigger_trigger_flag[i]=false;
    trigger_is_saturated[i]=false;
  }
  for(int i=0; i<2*NSCINTPLANES; ++i) {
    scint_countHG[i]=0.;
    scint_snHG[i]=0.;
    scint_countLG[i]=0.;
    scint_snLG[i]=0.;
    scint_trigger_flag[i]=false;
    scint_is_saturated[i]=false;
  }
  for(int i=0; i<2*NVETOSCINT; ++i) {
    veto_countHG[i]=0.;
    veto_snHG[i]=0.;
    veto_countLG[i]=0.;
    veto_snLG[i]=0.;
    veto_trigger_flag[i]=false;
    veto_is_saturated[i]=false;
  }
  for(int i=0; i<NLYSOCRYSTALS; ++i) {
    lyso_countHG[i]=0.;
    lyso_snHG[i]=0.;
    lyso_countLG[i]=0.;
    lyso_snLG[i]=0.;
    lyso_trigger_flag[i]=false;
    lyso_is_saturated[i]=false;
  }
  
  runType = 0x0;
  boot_nr=0;
  run_id=0;
  event_index=0;
  event_length=0;
  trigger_index=0;
  hepd_time=0;
  PMTBoard_trigger_counter = 0;
  lost_trigger=0;
  for(int i=0; i<NRATEMETER; ++i) rate_meter[i]=0;
  alive_time=0;
  dead_time=0;

  return;
}

LEvRec1Stream::~LEvRec1Stream() {
  Reset();
}

bool LEvRec1::DiscontinousSignal(const double threshold_sn) const {
  int lastPlane = GetLastPlaneHit(threshold_sn);
  if(lastPlane==-999) return true;
  bool isHG=true;
  for(int i = 0; i<lastPlane; ++i ) if(scint.GetSNOfUnit(i, isHG)<threshold_sn) return true;
  return false;
}

int LEvRec1::GetLastPlaneHit(const double threshold_sn) const {
  int lastPlane=-999;
  bool isHG=true;
  for(int i=0; i<NSCINTPLANES; ++i) if(scint.GetSNOfUnit(i, isHG)>threshold_sn) lastPlane = i;
  return lastPlane;
}

double LEvRec1::GetMSPlaneToMSBarRatio(const double threshold_sn) const {
  bool isHG=true;
  double result = scint.GetCountsOfMSU(isHG,threshold_sn)/trig.GetCountsOfMSU(isHG,threshold_sn);
  return result;
}

double LEvRec1::GetScintCounts(const double threshold_sn) const {
  bool isHG = true;
  double result = scint.GetCounts(isHG, threshold_sn);
  return result;
}

double LEvRec1::GetTriggerCounts(const double threshold_sn) const {
  bool isHG = true;
  double result =0.;
  if(trig.GetSNOfMSU(isHG,threshold_sn)>threshold_sn) {
    int MSU = trig.GetTheMostSignificantUnit(isHG,threshold_sn);
    result += trig.GetCountsOfMSU(isHG,threshold_sn);
    if(trig.GetSNOf2ndMSU(isHG,threshold_sn)>threshold_sn) {
      int MSU2nd = trig.GetThe2ndMostSignificantUnit(isHG,threshold_sn);
      if(MSU-MSU2nd==1) result += trig.GetCountsOf2ndMSU(isHG,threshold_sn); // only for adjacent bars
    }
  }
  return result;
}


// return false if the s/n of the PMTs in the online trigger is not > threshold
bool LEvRec1::PreSelection(const double threshold_sn, const int mostSigPaddle, 
			   const double numOfPlanesInTrig, const bool isHG) const{ //6 sigma
   bool planeHit = true;
   bool trigHit = true;
   
   if(mostSigPaddle < 0 || mostSigPaddle > 5)
      return false;
      
   if(isHG) {
      if(trig.sn_hg[mostSigPaddle][0] < threshold_sn  ||
	 trig.sn_hg[mostSigPaddle][1] < threshold_sn    )
     	 trigHit = false;
      
      for (int i=0; i<numOfPlanesInTrig;i++)      {
	 if(scint.sn_hg[i][0] < threshold_sn  || 
	    scint.sn_hg[i][1] < threshold_sn   )   {
	    planeHit = false;
	    break;
	 }
      }
   }
   else
   {
      if(trig.sn_lg[mostSigPaddle][0] < threshold_sn  ||
	 trig.sn_lg[mostSigPaddle][1] < threshold_sn    )
	 trigHit = false;
   
      for (int i=0; i<numOfPlanesInTrig;i++)   {
	 if(scint.sn_lg[i][0] < threshold_sn  || 
	    scint.sn_lg[i][1] < threshold_sn   )  {
	    planeHit = false;
	    break;
	 }
      }
	    
   }
   return (trigHit & planeHit);
}

int LEvRec1::GetTheMostSignificantPlane() const {
  int result=-999;
  double sn_max=-999.;
  double sn;
  for(int i=0; i<NSCINTPLANES; i++)
  {
     if(i != 4)
	sn = scint.sn_hg[i][0] + scint.sn_hg[i][1] ;
     else
	sn = scint.sn_hg[i][1]*2;
     
    if(sn > sn_max) {
      sn_max = sn;
      result = i;
    }
  }
  return result;  
}

// return true if more than 1 trigger paddle is hit (s/n > 3)
bool LEvRec1::AutoVeto(const double threshold_veto, const int mostSigPaddle,
		       const bool isHG) const { // 3 sigma
   bool ret = false;
   
   for (int i=0; i<NTRIGSCINT;i++)
   {
      if(i==mostSigPaddle)
	 continue;
      if (isHG){
	 if(trig.sn_hg[i][0] > threshold_veto  && trig.sn_hg[i][1] > threshold_veto)
	    return true;
      }
      else{
	 if(trig.sn_lg[i][0] > threshold_veto  && trig.sn_lg[i][1] > threshold_veto)
	    return true;
      }
   }
   return ret;
}


// return true  if at least one PMT of the lateral veto has s/n > 3
bool LEvRec1::isLatVetoHit(const double threshold_sn, const bool isHG) const { // 3 sigma
   bool ret = false;

   for(int i = 0; i<(NVETOSCINT-1) ; i++) // no bottom veto
   {
      if (isHG){
	 if(veto.sn_hg[i][0] > threshold_sn && veto.sn_hg[i][1] > threshold_sn)
	    return true;
      }
      else {
	 if(veto.sn_lg[i][0] > threshold_sn && veto.sn_lg[i][1] > threshold_sn)
	    return true;
      }
   }
   return ret;
}


// return true  if at least one PMT of the lyso has s/n > 3
bool LEvRec1::LysoVeto(const double threshold_sn) const { // 3 sigma
   bool ret = false;

   for(int i = 0; i<NLYSOCRYSTALS ; i++) 
   {
      if(lyso.sn_lg[i][0] > threshold_sn)
	 return true;
   }
   return ret;
}

// return true  if at least one PMT of the bottom veto has s/n > 3
bool LEvRec1::isBotVetoHit(const double threshold_sn, const bool isHG) const { // 3 sigma
   bool ret = false;

   if(isHG){
      if(veto.sn_hg[4][0] > threshold_sn && veto.sn_hg[4][1] > threshold_sn)
	 return true;
   }
   else{
      if(veto.sn_lg[4][0] > threshold_sn && veto.sn_lg[4][1] > threshold_sn)
	 return true;
   }
   return ret;
}



int LEvRec1::triggerMultiplicity(const double threshold_sn) const // 10 sigma
{
   int mult=0;
   
   for (int i=0; i<NTRIGSCINT;i++)
   {
      if(trig.sn_hg[i][0] >= threshold_sn && 
   	 trig.sn_hg[i][1] >= threshold_sn   )
   	 mult++;
   }

   return mult;
}

int LEvRec1::planeMultiplicity(const double threshold_sn) const // 10 sigma
{
   int mult=0;
   
   for (int i=0; i<NSCINTPLANES;i++)
   {
      if(i != 4                           &&  // P5 correction
	 scint.sn_hg[i][0] >= threshold_sn && 
	 scint.sn_hg[i][1] >= threshold_sn   )
	 mult++;
      else if(i == 4 && 
	      scint.sn_hg[i][1] >= threshold_sn)
	 mult++;
   }
   
   return mult;
}


int LEvRec1::lysoMultiplicity(const double threshold_sn) const // 10 sigma
{
   int mult=0;
   
   for (int i=0; i<NLYSOCRYSTALS;i++)
      if( lyso.sn_lg[i][0] >=  threshold_sn ) 
	 mult++;

   return mult;
}


// if P1 (P16) lastPlane = 1 (16), if Lyso lastPlaneHit = 17, if bottom lastPlaneHit = 18
int LEvRec1::lastPlaneHit(const double threshold_sn) const
{
   int lastPlane = -1;

   for(int i = 0; i<NSCINTPLANES; i++)
   {
      if (i == 4)
	 if (scint.sn_hg[i][1] >= threshold_sn)    // P5se correction
	    lastPlane=i;
	 else;
      else if(i!=4)
    	 if(scint.sn_hg[i][0] >= threshold_sn && 
	    scint.sn_hg[i][1] >= threshold_sn   )
	    lastPlane=i;
   }
   
   for (int i=0; i<NLYSOCRYSTALS;i++)
   { 
      if( lyso.sn_lg[i][0] >= threshold_sn )
      {
	 lastPlane = 16;
	 break;
      }
   }

   if(veto.sn_hg[4][0] >= threshold_sn  && 
      veto.sn_hg[4][1] >= threshold_sn    )
      lastPlane = 17;

   return lastPlane;
}


bool LEvRec1::areallPlaneHit(const double threshold_sn) const // 10 sigma
{
   bool ret = true;
   for (int i=0; i<NSCINTPLANES;i++)
   {
      if(i!=4)
      {
	 if( scint.sn_hg[i][0] < threshold_sn &&
	     scint.sn_hg[i][1] < threshold_sn   )
	    return false;
      }
      else
	 if( scint.sn_hg[i][1] < threshold_sn)
	    return false;
   }

   return ret;
}


