
#include "LEvRec2.hh"
#include <iostream>

LEvRec2::LEvRec2() {
  Reset();
}

LEvRec2::~LEvRec2() {
}

void LEvRec2::Reset(){
  Energy = 0;
  EnergyLoss = 0;
  Angle = 0;
  ParticleID = 0;
  return;
}

void LEvRec2::CopyFromLEvRec2Stream(const LEvRec2Stream evstr) {
  Reset();

  Energy = evstr.Energy;
  EnergyLoss = evstr.EnergyLoss;
  Angle = evstr.Angle;
  ParticleID = evstr.ParticleID;

  return;
}

LEvRec2Stream::LEvRec2Stream() {
  Reset();
}

LEvRec2Stream::LEvRec2Stream(const LEvRec2 event) {
  CopyFromLEvRec2(event);
}

void LEvRec2Stream::CopyFromLEvRec2(const LEvRec2 event) {
  Reset();

  Energy = event.Energy;
  EnergyLoss = event.EnergyLoss;
  Angle = event.Angle;
  ParticleID = event.ParticleID;

  return;
}

void LEvRec2Stream::Reset() {
  return;
}

LEvRec2Stream::~LEvRec2Stream() {
  Reset();
}