#ifndef __LTRACKERSIGNAL__
#define __LTRACKERSIGNAL__ 1

#include "LTrackerCluster.hh"
#include <vector>
#include <cstddef>

const int MAXNCLUSTERS=25;


class LTrackerSignal {
public:
  LTrackerSignal();
  void Reset();
  std::size_t GetSize() const;
  std::size_t push_back(const LTrackerCluster cl);
  void FillRandom(void);
  void HoldTimeCorrectionSig();
  //void HoldTimeCorrection(const double HOLDCORRCONST);
  std::vector<LTrackerCluster> cls;
private:
  std::size_t length;
  bool HTCFLAG;
};


#endif
