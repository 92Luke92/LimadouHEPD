#ifndef __LSCINTILLATORSIGNAL__
#define __LSCINTILLATORSIGNAL__ 1

#include "LCaloSignal.hh"

const std::size_t SCINTPLANES=16; // scintillator planes

class LScintillatorSignal : public LCaloSignal{
public:
  LScintillatorSignal();
  ~LScintillatorSignal();
  void Dump() const;
  double GetX(const int iu, const int ipmt) const;
  double GetY(const int iu, const int ipmt) const;
  double GetZ(const int iu, const int ipmt) const;

  double GetSNOfUnit(const int unit, const bool isHG) const; // PMT 0 broken
  double GetCountsOfUnit(const int unit, const bool isHG) const; // PMT 0 broken
 };


#endif
