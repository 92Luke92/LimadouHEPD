#include "LTrackerSignal.hh"
#include "analysis_const.hh"
#include <random>
#include <chrono>


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

void LTrackerSignal::HoldTimeCorrection(const double ALPHA){
  if(HTCFLAG == true){
  return; // no double correction
  }
  for (auto cit : cls) {
    for(int icchan = 1; icchan < CLUSTERCHANNELS-1; ++icchan){ //shift of the central channels of the clusters (NOT BORDERS!)
      cit.count[icchan] = (cit.count[icchan]-ALPHA*cit.count[icchan+1])/(1-ALPHA); //this shift takes in account the channel after the considered one
    }
  }
  HTCFLAG = true;
  return;
}
