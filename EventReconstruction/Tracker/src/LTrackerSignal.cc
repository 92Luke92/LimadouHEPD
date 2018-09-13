#include "LTrackerSignal.hh"
#include "analysis_const.hh"
#include <random>
#include <chrono>

int LTrackerSignal::GetMSCluster(void) {
  int result = -999;
  double oldSN=-999.;
  int size = cls.size();
  for(int i =0; i< size; ++i) {
    double newSN = cls.at(i).GetEtaSN();
    if(newSN > oldSN) {
      oldSN = newSN;
      result = i;
    }
  }
  return result;
}

double LTrackerSignal::GetMSClusterSN(void) {
  int cluster = GetMSCluster();
  if(cluster<0) return -999.;
  else return cls.at(cluster).GetEtaSN();
}

double LTrackerSignal::GetMSClusterCounts(void) {
  int cluster = GetMSCluster();
  if(cluster<0) return -999.;
  return cls.at(cluster).GetEtaCounts();
}

void LTrackerSignal::Reset() {
  cls.resize(0);
  length=0;
  HTCFLAG = false;
  return;
}

LTrackerSignal::LTrackerSignal() {
  Reset();
}


std::size_t LTrackerSignal::push_back(const LTrackerCluster cl) {
  cls.push_back(cl);
  length=cls.size();
  return length;
}

std::size_t LTrackerSignal::GetSize() const {
  return length;
}


void LTrackerSignal::FillRandom(void) {
  unsigned rseed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(rseed);
  const double pedestal = 1365.;
  const double sigmaIN=5.46;
  std::uniform_int_distribution<> distribution(1,26);

  int nclusters = distribution(generator);
  for(int i=0; i<nclusters; ++i) {
    LTrackerCluster cl;
    cl.FillRandom();
    push_back(cl);
  }
  return;
}

void LTrackerSignal::HoldTimeCorrectionSig(){
  if(HTCFLAG == true) return; // no double correction
  for (int icls = 0; icls<cls.size(); ++icls){
    cls.at(icls).HoldTimeCorrectionCls(HOLDCORRCONST);
  }
  HTCFLAG = true;
  return;
}

void LTrackerSignal::Dump() {
  for (int icls = 0; icls<cls.size(); ++icls){
    cls.at(icls).Dump();
  }
  return;
}

/*
void LTrackerSignal::HoldTimeCorrection(const double HOLDCORRCONST){
  if(HTCFLAG == true) return; // no double correction
  for (int icls = 0; icls<cls.size(); ++icls){
      for(int icchan = 1; icchan < CLUSTERCHANNELS-1; ++icchan){ //shift of the central channels of the clusters (NOT BORDERS!)
          cls.at(icls).count[icchan] = (cls.at(icls).count[icchan]-HOLDCORRCONST*cls.at(icls).count[icchan+1])/(1.-HOLDCORRCONST);
      }
  }
  HTCFLAG = true;
  return;
}
*/
