#ifndef __LVETOSIGNAL__
#define __LVETOSIGNAL__ 1

#include "LCaloSignal.hh"

const std::size_t VETOPLANES=5; // veto planes

class LVetoSignal : public LCaloSignal{
public:
  LVetoSignal();
  ~LVetoSignal();
  void Dump() const;
};


#endif
