#include <iostream>
#include "LEvRec0File.hh"
#include "LEvRec0.hh"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

int main(int argc, char *argv[])

{
    

//TFile * fileo = new TFile("out.root","RECREATE");
TTree *tree  = new TTree("tree","tree");
typedef struct {int rates[65]; int times[3];} EVENT;
static EVENT event;
tree->Branch("Event", &event, "rates[65]/I:times[3]/I");



   LEvRec0File rootfile(argv[1]);
   LEvRec0 ev;
   LEvRec0Md metaData;
   rootfile.SetTheEventPointer(ev);
   rootfile.SetMdPointer(metaData);
  
   int Tmd_entries =  rootfile.GetMDEntries();
   if (Tmd_entries % 2)
      Tmd_entries -= 1;
   //std::cout << "Number of Tmd entries: " << Tmd_entries << std::endl;

// loop on entries
// first entry is 0
    for (int entr=1; entr<2; entr++){


   
  rootfile.GetMDEntry(entr);
   
for (int i = 0; i <65; i++) {  event.rates[i] = metaData.PMT_rate_meter[i]; 
event.times[1]=atoi(argv[2]);
event.times[2]=atoi(argv[3]);
// std::cout << event.rates[i]<< std::endl;
   } 

tree->Fill();}

rootfile.Close();
TFile * fileo = new TFile("out.root","RECREATE");
tree->Write();
fileo->Write();
fileo->Close();

return 0;

}
