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
};


#endif
