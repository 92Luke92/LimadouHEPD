#include "LTrackerSignal.hh"
#include <random>
#include <chrono>

void LTrackerSignal::Reset() {
  cls.resize(0);
  length=0;
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
