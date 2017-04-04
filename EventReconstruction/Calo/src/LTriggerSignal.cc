#include "LTriggerSignal.hh"

#include <iostream>

LTriggerSignal::LTriggerSignal() {
    nunits=TRIGBARS;
    npmts=2;
    CreateContainers();
}

LTriggerSignal::~LTriggerSignal() {

}

void LTriggerSignal::Dump() const {
  std::cout << "############# Trigger signal dumping." << std::endl;
  DumpAll();
  return;
}

