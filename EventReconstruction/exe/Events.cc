#include <iostream>
#include "LEvRec0File.hh"

int main(int argc, char *argv[2]) {
  if(argc!=2) {
    std::cerr << "Error! Usage:    ./Events <rootfile>" << std::endl;
    std::cerr << "Aborted." << std::endl;
    return -999;
  }
  LEvRec0File f(argv[1]);
  std::cout<< f.GetEntries() << std::endl;
  return f.GetEntries();
}
