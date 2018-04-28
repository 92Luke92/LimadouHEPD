#ifndef __LTRIGGERSIGNAL__
#define __LTRIGGERSIGNAL__ 1

#include "LCaloSignal.hh"

const std::size_t TRIGBARS= 6; // trigger bars 

class LTriggerSignal : public LCaloSignal{
public:
  LTriggerSignal();
  ~LTriggerSignal();
  void Dump() const;
  double GetX(const int iu, const int ipmt) const;
  double GetY(const int iu, const int ipmt) const;
  double GetZ(const int iu, const int ipmt) const;
  inline int GetNOfSignificantBars(const bool isHG, const double threshold = THRESHOLD) const {return GetNOfSignificantUnits(isHG, threshold);};
  inline int GetTheMostSignificantBar(const bool isHG, const double threshold = THRESHOLD) const {return GetTheMostSignificantUnit(isHG, threshold);};
  inline int GetThe2ndMostSignificantBar(const bool isHG, const double threshold = THRESHOLD) const {return GetThe2ndMostSignificantUnit(isHG, threshold);};
  inline double GetSNOfMST(const bool isHG, const double threshold = THRESHOLD) const {return GetSNOfMSU(isHG,threshold);};
  inline double GetSNOf2ndMST(const bool isHG, const double threshold = THRESHOLD) const {return GetSNOf2ndMSU(isHG,threshold);};
  inline double GetCountsOfMST(const bool isHG, const double threshold = THRESHOLD) const {return GetCountsOfMSU(isHG,threshold);};
  inline double GetCountsOf2ndMST(const bool isHG, const double threshold = THRESHOLD) const {return GetCountsOf2ndMSU(isHG,threshold);};
  bool SignalSharedAmongBars(const bool isHG, const double threshold = THRESHOLD) const;
  double GetSN(const bool isHG, const double threshold) const;
  double GetCounts(const bool isHG, const double threshold) const;
};


#endif
