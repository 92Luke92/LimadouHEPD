#include "LEvRec1.hh"
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
  return;
}

void LEvRec1::Reset(){
  tracker.cls.resize(0);
  trig.Reset();
  scint.Reset();
  veto.Reset();
  lyso.Reset();
  
  return;
}


void LEvRec1::Dump() {
  DumpTracker();
  DumpCalo();
  return;
}

void LEvRec1::DumpTracker() {
  for(auto it : tracker.cls) it.Dump();
  return;
}

void LEvRec1::DumpCalo() {
  DumpTrigger();
  DumpScintillator();
  DumpVeto();
  DumpLyso();
  return;
}

void LEvRec1::DumpTrigger() {
  trig.Dump();
  return;
}

void LEvRec1::DumpScintillator() {
  scint.Dump();
  return;
}

void LEvRec1::DumpVeto() {
  veto.Dump();
  return;
}

void LEvRec1::DumpLyso() {
  lyso.Dump();
  return;
}


void LEvRec1::CopyFromLEvRec1Stream(const LEvRec1Stream evstr) {
  Reset();
  
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
    std::cerr << "LEvRec1Stream::CopyFromLEvRec1 error! Event containing "
	      << nClusters << " clusters passed as input. I copy only the first "
	      << "MAXNCLUSTERS=" << MAXNCLUSTERS << std::endl;
    std::cerr << "nClusters too is set to this number." << std::endl;
    nClusters=MAXNCLUSTERS;
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
  }
  for(int i=0; i<2*NSCINTPLANES; ++i) {
    scint_countHG[i]=0.;
    scint_snHG[i]=0.;
    scint_countLG[i]=0.;
    scint_snLG[i]=0.;
  }
  for(int i=0; i<2*NVETOSCINT; ++i) {
    veto_countHG[i]=0.;
    veto_snHG[i]=0.;
    veto_countLG[i]=0.;
    veto_snLG[i]=0.;
  }
  for(int i=0; i<NLYSOCRYSTALS; ++i) {
    lyso_countHG[i]=0.;
    lyso_snHG[i]=0.;
    lyso_countLG[i]=0.;
    lyso_snLG[i]=0.;
  }
  return;
}

LEvRec1Stream::~LEvRec1Stream() {
  Reset();
}
