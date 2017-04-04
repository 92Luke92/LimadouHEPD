#ifndef __LSCINTILLATORSIGNAL__
#define __LSCINTILLATORSIGNAL__ 1

#include "LCaloSignal.hh"

const std::size_t SCINTPLANES=16; // scintillator planes

class LScintillatorSignal : public LCaloSignal{
public:
  LScintillatorSignal();
  ~LScintillatorSignal();
  void Dump() const;
};


#endif
