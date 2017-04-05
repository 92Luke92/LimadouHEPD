#ifndef __LVETOSIGNAL__
#define __LVETOSIGNAL__ 1

#include "LCaloSignal.hh"

const std::size_t VETOPLANES=5; // veto planes

class LVetoSignal : public LCaloSignal{
public:
  LVetoSignal();
  ~LVetoSignal();
  void Dump() const;
  double GetX(const int iu, const int ipmt) const;
  double GetY(const int iu, const int ipmt) const;
  double GetZ(const int iu, const int ipmt) const;
};


#endif
