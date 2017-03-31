#include <iostream>
#include "LEvRec1File.hh"
#include "LEvRec1.hh"


int main(int argc, char *argv[]) {
  if(argc!=2) {
    std::cerr << "Error! Usage:    ./test_LEvRec1 <outFile>" << std::endl;
    std::cerr << "Aborted." << std::endl;
    return -999;
  }
  
  LEvRec1File fout(argv[1],"WRITE");
  fout.FillRandom(1000);
  fout.Write();
  fout.Close();
  
  LEvRec1File fin(argv[1],"READ");
  LEvRec1 ev;
  fin.SetTheEventPointer(ev);
  int nent=fin.GetEntries();
  
  LEvRec1File fout2("test1.root","WRITE");
  for(int i=0; i<nent; ++i) {
    fin.GetEntry(i);
    fout2.Fill(ev);
  }

  fin.Close();
  fout2.Write();
  fout2.Close();
  
  return 0;
}
