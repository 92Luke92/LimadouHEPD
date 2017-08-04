#include "LEvRec1File.hh"
#include "LEvRec1.hh"
#include "LOfflineTrigger.hh"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TString.h"
#include "TList.h"
#include <iostream>


int main(int argc, char **argv) {
  if(argc!=4) {
    std::cerr << "************ Usage:" << std::endl
	      << "************ ~user> ./LSkimmingEvents.cc <inputDir> <inputPath> <outputFile>" << std::endl
	      << "************ aborting."
	      << std::endl;
    return -999;
  }
  //define the desired offline trigger
  LOfflineTrigger mytrig;
  mytrig.LoadConfiguration("central_muon_lyso4");
  LOfflineTrigger veto;
  veto.LoadConfiguration("VL");
  //mytrig.LoadConfiguration("central_muon_tight");
  //define output file
  LEvRec1File outputFile(argv[3],"WRITE");
  LEvRec1 ev; 
  //change directory to the target one
  TSystemDirectory dir(argv[1], argv[2]);
  TList *files = dir.GetListOfFiles();
  if(files) {
    TSystemFile *file;
    TIter next(files);
    TString fname;
    std::string point = ".";
    std::string pointpoint = "..";
    int counter = 0;
    int counter2 = 0;
    while ((file=(TSystemFile*)next())) {
      counter++;
      //if (counter>100) break; 
      fname = file->GetName();
      if(fname == point.c_str() || fname == pointpoint.c_str()) continue;
      TString ffname = TString(argv[2])+"/"+fname;
      std::cout << "********* Processing file " << ffname << std::endl;
      LEvRec1File inputFile(ffname.Data(),"READ");
      inputFile.SetTheEventPointer(ev);
      int nentries=inputFile.GetEntries();
      for(int ie=0; ie<nentries; ++ie) { 
        inputFile.GetEntry(ie);
        if(mytrig.Trigger(ev)==false||veto.Trigger(ev)==true) continue;
        counter2 ++;
        std::cout<< "----> event " << counter2 << std::endl;
        outputFile.Fill(ev);
        }
      inputFile.Close();
    }
  }
  outputFile.Write();
  outputFile.Close();

  return 0;
}
