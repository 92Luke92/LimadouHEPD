#ifndef __LTRIGGERPOSITIONESTIMATOR__
#define __LTRIGGERPOSITIONESTIMATOR__  "LTriggerPositionEstimator # "

#include "LTriggerSignal.hh"

class LTriggerPositionEstimator {

public:
  LTriggerPositionEstimator();
  LPosition GetPosition(LTriggerSignal ts);
  unsigned short GetTheMostSignificantBar(LTriggerSignal)

};

#endif
