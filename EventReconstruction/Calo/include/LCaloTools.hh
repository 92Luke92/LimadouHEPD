#ifndef __LCALOTOOLS__
#define __LCALOTOOLS__

#include "LEvRec0.hh"
#include "LCalibration.hh"
#include "LTriggerSignal.hh"
#include "LScintillatorSignal.hh"
#include "LLysoSignal.hh"
#include "LVetoSignal.hh"

LTriggerSignal GetTriggerSignal(const LEvRec0 lev0, const LCalibration cal);
LScintillatorSignal GetScintillatorSignal(const LEvRec0 lev0, const LCalibration cal);
LLysoSignal GetLysoSignal(const LEvRec0 lev0, const LCalibration cal);
LVetoSignal GetVetoSignal(const LEvRec0 lev0, const LCalibration cal);


#endif
