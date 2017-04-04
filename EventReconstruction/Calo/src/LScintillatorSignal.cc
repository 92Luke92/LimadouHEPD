#include "LScintillatorSignal.hh"

#include <iostream>

LScintillatorSignal::LScintillatorSignal() {
    nunits=SCINTPLANES;
    npmts=2;
    CreateContainers();
}

LScintillatorSignal::~LScintillatorSignal() {

}

void LScintillatorSignal::Dump() const {
  std::cout << "############# Scintillator signal dumping." << std::endl;
  DumpAll();
  return;
}
