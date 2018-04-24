#include "LEvRec1File.hh"
#include "LL1Selector.hh"
#include "detector_const.hh"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "TH1D.h"
#include "TH2D.h"

int main(int argc, char **argv) {
  if(argc!=3) {
    std::cerr << "************ Usage:" << std::endl
	      << "************ ~user> ./L1Analysis <inputFile> <outputFile>" << std::endl
	      << "************ aborting."
	      << std::endl;
    return -999;
  }

  std::string basename;
  std::ostringstream ostr;

  TFile fout(argv[2],"new");

  TH1D ***hCounts = new TH1D**[NTRIGSCINT];
  basename = std::string("hCounts");
  for(int i=0; i< NTRIGSCINT; ++i) {
    hCounts[i] = new TH1D*[2];
    for(int j=0; j<2; ++j) {
      ostr << basename << "_t" << i+1 << "_PMT" << j; 
      hCounts[i][j] = new TH1D(ostr.str().c_str(), "",2000,0.,4000.);
      ostr.str("");
      hCounts[i][j]->SetDirectory(&fout);
    }
  }
  
  TH1D ***hSNs = new TH1D**[NTRIGSCINT];
  basename = std::string("hSNs");
  for(int i=0; i< NTRIGSCINT; ++i) {
    hSNs[i] = new TH1D*[2];
    for(int j=0; j<2; ++j) {
      ostr << basename << "_t" << i+1 << "_PMT"<< j;
      hSNs[i][j] = new TH1D(ostr.str().c_str(), "",2000,0.,1400.);
      ostr.str("");
      hSNs[i][j]->SetDirectory(&fout);
    }
  }
  
  TH1D ***pCounts = new TH1D**[NSCINTPLANES];
  basename = std::string("hCounts");
  for(int i=0; i< NSCINTPLANES; ++i) {
    pCounts[i] = new TH1D*[2];
    for(int j=0; j<2; ++j) {
      ostr << basename << "_p" << i+1 << "_PMT" << j; 
      pCounts[i][j] = new TH1D(ostr.str().c_str(), "",2000,0.,4000.);
      ostr.str("");
      pCounts[i][j]->SetDirectory(&fout);
    }
  }
  
  TH1D ***pSNs = new TH1D**[NSCINTPLANES];
  basename = std::string("hSNs");
  for(int i=0; i< NSCINTPLANES; ++i) {
    pSNs[i] = new TH1D*[2];
    for(int j=0; j<2; ++j) {
      ostr << basename << "_p" << i+1 << "_PMT"<< j;
      pSNs[i][j] = new TH1D(ostr.str().c_str(), "",2000,0.,1400.);
      ostr.str("");
      pSNs[i][j]->SetDirectory(&fout);
    }
  }
  
  TH2D  plotCarfora("plotCarfora","",6000,-10000.,100000.,600,-3000.,30000);
  plotCarfora.SetDirectory(&fout);


///////////////////////

  //LL1Selector sel;
  //std::string selection("default");
  /************/
  //sel.SetThreshold(3.);
  //std::string selection("NoCorrTAny");
  /************/
  LL1Selector sel;
  sel.SetThreshold(3.); // sn<=125 is expected to contain 95% of electrons at 30 MeV
  std::string selection("NoCorrVAny");
  sel.LoadConfiguration(selection);
  /***********/
  LL1Selector sel2;
  sel2.SetThreshold(3.); 
  std::string selection2("NoCorrTAny");
  sel2.LoadConfiguration(selection2);
 /***********/
  LL1Selector sel3;
  sel3.SetThreshold(5.); 
  std::string selection3("NoPlane5PMT0");
  sel3.LoadConfiguration(selection3);
 /***********/
  LL1Selector sel4;
  std::string selection4("InsidePlane1Cone");
  sel4.LoadConfiguration(selection4);
 /***********/
  LL1Selector sel5;
  std::string selection5("NoT6");
  sel5.SetThreshold(3.);
  sel5.LoadConfiguration(selection5);
 ///////////////////////
 

  std::ifstream fin(argv[1],std::ifstream::in);
  std::string fname;
  while(1) {
    fin >> fname;
    if(fin.eof()) break;
    std::cout << "Working on " << fname << std::endl;
    
    LEvRec1File inputFile(fname.c_str(),"READ");
    LEvRec1 cev;
    inputFile.SetTheEventPointer(cev);

    int nentries=inputFile.GetEntries();
    for(int ie=0; ie<nentries; ++ie) {
      inputFile.GetEntry(ie);
      //if(!sel.Select(cev)) continue;
      //if(!sel2.Select(cev)) continue;
      //if(!sel3.Select(cev)) continue;
      if(!sel5.Select(cev)) continue;
      for(int i=0; i< NTRIGSCINT; ++i) {
        for(int j=0; j<2; ++j) {
          hCounts[i][j]->Fill(cev.trig.cont_hg[i][j]);
          hSNs[i][j]->Fill(cev.trig.sn_hg[i][j]);
        }
      }
      for(int i=0; i< NSCINTPLANES; ++i) {
        for(int j=0; j<2; ++j) {  
        pCounts[i][j]->Fill(cev.scint.cont_hg[i][j]);
         pSNs[i][j]->Fill(cev.scint.sn_hg[i][j]);
        }
      }
      bool isHG=true;
      bool isLG=!isHG;
      double calCounts = cev.scint.GetCounts(isHG);
      double lysoCounts = cev.lyso.GetCounts(isLG);
      plotCarfora.Fill(calCounts, lysoCounts);
    }
      
    inputFile.Close();
  }
  fout.Write();
  fout.Close();

  return 0;
}
