#ifndef __LTRIGGERSIGNAL__
#define __LTRIGGERSIGNAL__ 1

#include "LCaloSignal.hh"

const std::size_t TRIGBARS= 6; // trigger bars 

class LTriggerSignal : public LCaloSignal{
public:
  LTriggerSignal();
  ~LTriggerSignal();
  void Dump();
};


#endif
