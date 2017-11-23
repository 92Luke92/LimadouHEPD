#include "LEvRec2.hh"
#include "LEvRec2File.hh"
#include <iostream>

int main(int argc, char **argv) {
  if(argc!=2) {
    std::cerr << "************ Usage:" << std::endl
	      << "************ ~user> ./Reco01 <filename>" << std::endl
	      << "************ aborting."
	      << std::endl;
    return -999;
  }
  LEvRec2File evrec2(argv[1],"WRITE");
  evrec2.Write();
  return 0;
}
