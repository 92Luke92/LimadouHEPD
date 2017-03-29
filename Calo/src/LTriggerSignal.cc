#include "LTriggerSignal.hh"

#include <iostream>

LTriggerSignal::LTriggerSignal() {
    nunits=TRIGBARS;
    npmts=2;
    CreateContainers();
}

LTriggerSignal::~LTriggerSignal() {

}

void LTriggerSignal::Dump() {
  std::cout << "############# Trigger signal dumping." << std::endl;
  DumpAll();
  return;
}
