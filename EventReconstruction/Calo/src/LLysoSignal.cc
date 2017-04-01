#include "LLysoSignal.hh"

#include <iostream>

LLysoSignal::LLysoSignal() {
    nunits=LYSOCRYSTALS;
    npmts=1;
    CreateContainers();
}

LLysoSignal::~LLysoSignal() {

}

void LLysoSignal::Dump() {
  std::cout << "############# Lyso signal dumping." << std::endl;
  DumpAll();
  return;
}