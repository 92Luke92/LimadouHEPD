#include "LReco01Manager.hh"
#include <iostream>

int main(int argc, char **argv) {
  if(argc!=2) {
    std::cerr << "************ Usage:" << std::endl
	      << "************ ~user> ./Reco01 <optionsFile.opt>" << std::endl
	      << "************ aborting."
	      << std::endl;
    return -999;
  }
  
  LReco01Manager::GetInstance().LoadSteering(argv[1]);
  LReco01Manager::GetInstance().Run();

  return 0;
}
