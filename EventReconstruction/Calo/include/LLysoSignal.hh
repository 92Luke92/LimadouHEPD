#ifndef __LLYSOSIGNAL__
#define __LLYSOSIGNAL__ 1

#include "LCaloSignal.hh"

const std::size_t LYSOCRYSTALS=9;

class LLysoSignal : public LCaloSignal{
public:
  LLysoSignal();
  ~ LLysoSignal();
  void Dump() const;
};


#endif
