#include "LEvRec1File.hh"
#include "LL1Selector.hh"
#include "detector_const.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TH2D.h"

int main(int argc, char **argv) {
  if(argc!=3) {
    std::cerr << "************ Usage:" << std::endl
	      << "************ ~user> ./L1Analysis <inputFileList> <outputFile>" << std::endl
	      << "************ aborting."
	      << std::endl;
    return -999;
  }

  std::string basename;
  std::ostringstream ostr;

  TFile fout(argv[2],"new");


  ///////////////////////
  //LL1Selector sel;
  //std::string selection("default");
  /************/
  //sel.SetThreshold(3.);
  //std::string selection("NoCorrTAny");
  /************/
  //sel.SetThreshold(125.); // sn<=125 is expected to contain 95% of electrons at 30 MeV
  //std::string selection("NoCorrVAny");
  /************/
  //sel.LoadConfiguration(selection);
  
/***********/
  LL1Selector sel5;
  std::string selection5("NoT6");
  sel5.SetThreshold(3.);
  sel5.LoadConfiguration(selection5);

  //LL1Selector sel4;
  //std::string selection4("InsidePlane1Cone");
  //sel4.LoadConfiguration(selection4);
  

  /*LL1Selector sel2;
  sel2.SetThreshold(3.); 
  std::string selection2("NoCorrTAny");
  sel2.LoadConfiguration(selection2);*/

  basename = std::string("L1_SELNoT6_3_correlation");
  //basename = std::string("L1_SELNoCorrTAnyNoCorrVAny_correlation");

  const int nbins=500;
  const double st_x=0.;
  const double sp_x=5000.;
  ///////////////////////
 

  

  TH2D *****hCorr_tr_tr = new TH2D****[NTRIGSCINT];
  for(int i=0; i< NTRIGSCINT; ++i) {
    hCorr_tr_tr[i] = new TH2D***[2];
    for(int j=0; j<2; ++j) {  
      hCorr_tr_tr[i][j] = new TH2D**[NTRIGSCINT];
      for(int i2=i; i2< NTRIGSCINT; ++i2) {  
        if(i2==i && j==1) continue; // do not correlate with yourself!
        else {
          hCorr_tr_tr[i][j][i2] = new TH2D*[2];
          for(int j2=0; j2<2; ++j2) {
            ostr << basename << "____"
            << "tr" << "_" << i << "_" << j << "__"
            << "tr" << "_" << i2 << "_" << j2;
            hCorr_tr_tr[i][j][i2][j2] = new TH2D(ostr.str().c_str(),ostr.str().c_str(), nbins, st_x, sp_x, nbins, st_x, sp_x);
            ostr.str("");
            ostr << "tr" << "_" << i << "_" << j << " [ADC]";
            hCorr_tr_tr[i][j][i2][j2]->GetXaxis()->SetTitle(ostr.str().c_str());
            ostr.str("");
            ostr << "tr" << "_" << i2 << "_" << j2 << " [ADC]";;
            hCorr_tr_tr[i][j][i2][j2]->GetYaxis()->SetTitle(ostr.str().c_str());
            ostr.str("");
            hCorr_tr_tr[i][j][i2][j2]->SetDirectory(&fout);
          }
        }
      }
    }
  }

  TH2D *****hCorr_tr_sc = new TH2D****[NTRIGSCINT];
  for(int i=0; i< NTRIGSCINT; ++i) {
    hCorr_tr_sc[i] = new TH2D***[2];
    for(int j=0; j<2; ++j) {  
      hCorr_tr_sc[i][j] = new TH2D**[NSCINTPLANES];
      for(int i2=0; i2< NSCINTPLANES; ++i2) {  
        hCorr_tr_sc[i][j][i2] = new TH2D*[2];
        for(int j2=0; j2<2; ++j2) {
          ostr << basename << "____"
          << "tr" << "_" << i << "_" << j << "__"
          << "sc" << "_" << i2 << "_" << j2;
          hCorr_tr_sc[i][j][i2][j2] = new TH2D(ostr.str().c_str(),ostr.str().c_str(), nbins, st_x, sp_x, nbins, st_x, sp_x);
          ostr.str("");
          ostr << "tr" << "_" << i << "_" << j << " [ADC]";;
          hCorr_tr_sc[i][j][i2][j2]->GetXaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          ostr << "sc" << "_" << i2 << "_" << j2 << " [ADC]";;
          hCorr_tr_sc[i][j][i2][j2]->GetYaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          hCorr_tr_sc[i][j][i2][j2]->SetDirectory(&fout);
        }
      }
    }
  }

  TH2D *****hCorr_tr_vt = new TH2D****[NTRIGSCINT];
  for(int i=0; i< NTRIGSCINT; ++i) {
    hCorr_tr_vt[i] = new TH2D***[2];
    for(int j=0; j<2; ++j) {  
      hCorr_tr_vt[i][j] = new TH2D**[NVETOSCINT];
      for(int i2=0; i2< NVETOSCINT; ++i2) {  
        hCorr_tr_vt[i][j][i2] = new TH2D*[2];
        for(int j2=0; j2<2; ++j2) {
          ostr << basename << "____"
          << "tr" << "_" << i << "_" << j << "__"
          << "vt" << "_" << i2 << "_" << j2;
          hCorr_tr_vt[i][j][i2][j2] = new TH2D(ostr.str().c_str(),ostr.str().c_str(), nbins, st_x, sp_x, nbins, st_x, sp_x);
          ostr.str("");
          ostr << "tr" << "_" << i << "_" << j << " [ADC]";;
          hCorr_tr_vt[i][j][i2][j2]->GetXaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          ostr << "vt" << "_" << i2 << "_" << j2 << " [ADC]";;
          hCorr_tr_vt[i][j][i2][j2]->GetYaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          hCorr_tr_vt[i][j][i2][j2]->SetDirectory(&fout);
        }
      }
    }
  }

  TH2D *****hCorr_tr_ly = new TH2D****[NTRIGSCINT];
  for(int i=0; i< NTRIGSCINT; ++i) {
    hCorr_tr_ly[i] = new TH2D***[2];
    for(int j=0; j<2; ++j) {  
      hCorr_tr_ly[i][j] = new TH2D**[NLYSOCRYSTALS];
      for(int i2=0; i2< NLYSOCRYSTALS; ++i2) {  
        hCorr_tr_ly[i][j][i2] = new TH2D*[1];
        for(int j2=0; j2<1; ++j2) {
          ostr << basename << "____"
          << "tr" << "_" << i << "_" << j << "__"
          << "ly" << "_" << i2 << "_" << j2;
          hCorr_tr_ly[i][j][i2][j2] = new TH2D(ostr.str().c_str(),ostr.str().c_str(), nbins, st_x, sp_x, nbins, st_x, sp_x);
          ostr.str("");
          ostr << "tr" << "_" << i << "_" << j << " [ADC]";;
          hCorr_tr_ly[i][j][i2][j2]->GetXaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          ostr << "ly" << "_" << i2 << "_" << j2 << " [ADC]";;
          hCorr_tr_ly[i][j][i2][j2]->GetYaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          hCorr_tr_ly[i][j][i2][j2]->SetDirectory(&fout);
        }
      }
    }
  }

  TH2D *****hCorr_sc_sc = new TH2D****[NSCINTPLANES];
  for(int i=0; i< NSCINTPLANES; ++i) {
    hCorr_sc_sc[i] = new TH2D***[2];
    for(int j=0; j<2; ++j) {  
      hCorr_sc_sc[i][j] = new TH2D**[NSCINTPLANES];
      for(int i2=i; i2< NSCINTPLANES; ++i2) {  
        if(i2==i && j==1) continue; // do not correlate with yourself!
        else {
          hCorr_sc_sc[i][j][i2] = new TH2D*[2];
          for(int j2=0; j2<2; ++j2) {
            ostr << basename << "____"
            << "sc" << "_" << i << "_" << j << "__"
            << "sc" << "_" << i2 << "_" << j2;
            hCorr_sc_sc[i][j][i2][j2] = new TH2D(ostr.str().c_str(),ostr.str().c_str(), nbins, st_x, sp_x, nbins, st_x, sp_x);
            ostr.str("");
            ostr << "sc" << "_" << i << "_" << j << " [ADC]";;
            hCorr_sc_sc[i][j][i2][j2]->GetXaxis()->SetTitle(ostr.str().c_str());
            ostr.str("");
            ostr << "sc" << "_" << i2 << "_" << j2 << " [ADC]";;
            hCorr_sc_sc[i][j][i2][j2]->GetYaxis()->SetTitle(ostr.str().c_str());
            ostr.str("");
            hCorr_sc_sc[i][j][i2][j2]->SetDirectory(&fout);
          }
        }
      }
    }
  }

  TH2D *****hCorr_sc_vt = new TH2D****[NSCINTPLANES];
  for(int i=0; i< NSCINTPLANES; ++i) {
    hCorr_sc_vt[i] = new TH2D***[2];
    for(int j=0; j<2; ++j) {  
      hCorr_sc_vt[i][j] = new TH2D**[NVETOSCINT];
      for(int i2=0; i2< NVETOSCINT; ++i2) {  
        hCorr_sc_vt[i][j][i2] = new TH2D*[2];
        for(int j2=0; j2<2; ++j2) {
          ostr << basename << "____"
          << "sc" << "_" << i << "_" << j << "__"
          << "vt" << "_" << i2 << "_" << j2;
          hCorr_sc_vt[i][j][i2][j2] = new TH2D(ostr.str().c_str(),ostr.str().c_str(), nbins, st_x, sp_x, nbins, st_x, sp_x);
          ostr.str("");
          ostr << "sc" << "_" << i << "_" << j << " [ADC]";;
          hCorr_sc_vt[i][j][i2][j2]->GetXaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          ostr << "vt" << "_" << i2 << "_" << j2 << " [ADC]";;
          hCorr_sc_vt[i][j][i2][j2]->GetYaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          hCorr_sc_vt[i][j][i2][j2]->SetDirectory(&fout);
        }
      }
    }
  }

  TH2D *****hCorr_sc_ly = new TH2D****[NSCINTPLANES];
  for(int i=0; i< NSCINTPLANES; ++i) {
    hCorr_sc_ly[i] = new TH2D***[2];
    for(int j=0; j<2; ++j) {  
      hCorr_sc_ly[i][j] = new TH2D**[NLYSOCRYSTALS];
      for(int i2=0; i2< NLYSOCRYSTALS; ++i2) {  
        hCorr_sc_ly[i][j][i2] = new TH2D*[1];
        for(int j2=0; j2<1; ++j2) {
          ostr << basename << "____"
          << "sc" << "_" << i << "_" << j << "__"
          << "ly" << "_" << i2 << "_" << j2;
          hCorr_sc_ly[i][j][i2][j2] = new TH2D(ostr.str().c_str(),ostr.str().c_str(), nbins, st_x, sp_x, nbins, st_x, sp_x);
          ostr.str("");
          ostr << "sc" << "_" << i << "_" << j << " [ADC]";;
          hCorr_sc_ly[i][j][i2][j2]->GetXaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          ostr << "ly" << "_" << i2 << "_" << j2 << " [ADC]";;
          hCorr_sc_ly[i][j][i2][j2]->GetYaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          hCorr_sc_ly[i][j][i2][j2]->SetDirectory(&fout);
        }
      }
    }
  }

  TH2D *****hCorr_vt_vt = new TH2D****[NVETOSCINT];
  for(int i=0; i< NVETOSCINT; ++i) {
    hCorr_vt_vt[i] = new TH2D***[2];
    for(int j=0; j<2; ++j) {  
      hCorr_vt_vt[i][j] = new TH2D**[NVETOSCINT];
      for(int i2=i; i2< NVETOSCINT; ++i2) {  
        if(i2==i && j==1) continue; // do not correlate with yourself!
        else {
          hCorr_vt_vt[i][j][i2] = new TH2D*[2];
          for(int j2=0; j2<2; ++j2) {
            ostr << basename << "____"
            << "vt" << "_" << i << "_" << j << "__"
            << "vt" << "_" << i2 << "_" << j2;
            hCorr_vt_vt[i][j][i2][j2] = new TH2D(ostr.str().c_str(),ostr.str().c_str(), nbins, st_x, sp_x, nbins, st_x, sp_x);
            ostr.str("");
            ostr << "vt" << "_" << i << "_" << j << " [ADC]";;
            hCorr_vt_vt[i][j][i2][j2]->GetXaxis()->SetTitle(ostr.str().c_str());
            ostr.str("");
            ostr << "vt" << "_" << i2 << "_" << j2 << " [ADC]";;
            hCorr_vt_vt[i][j][i2][j2]->GetYaxis()->SetTitle(ostr.str().c_str());
            ostr.str("");
            hCorr_vt_vt[i][j][i2][j2]->SetDirectory(&fout);
          }
        }
      }
    }
  }

  TH2D *****hCorr_vt_ly = new TH2D****[NVETOSCINT];
  for(int i=0; i< NVETOSCINT; ++i) {
    hCorr_vt_ly[i] = new TH2D***[2];
    for(int j=0; j<2; ++j) {  
      hCorr_vt_ly[i][j] = new TH2D**[NLYSOCRYSTALS];
      for(int i2=0; i2< NLYSOCRYSTALS; ++i2) {  
        hCorr_vt_ly[i][j][i2] = new TH2D*[1];
        for(int j2=0; j2<1; ++j2) {
          ostr << basename << "____"
          << "vt" << "_" << i << "_" << j << "__"
          << "ly" << "_" << i2 << "_" << j2;
          hCorr_vt_ly[i][j][i2][j2] = new TH2D(ostr.str().c_str(),ostr.str().c_str(), nbins, st_x, sp_x, nbins, st_x, sp_x);
          ostr.str("");
          ostr << "vt" << "_" << i << "_" << j << " [ADC]";;
          hCorr_vt_ly[i][j][i2][j2]->GetXaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          ostr << "ly" << "_" << i2 << "_" << j2 << " [ADC]";;
          hCorr_vt_ly[i][j][i2][j2]->GetYaxis()->SetTitle(ostr.str().c_str());
          ostr.str("");
          hCorr_vt_ly[i][j][i2][j2]->SetDirectory(&fout);
        }
      }
    }
  }

  TH2D *****hCorr_ly_ly = new TH2D****[NLYSOCRYSTALS];
  for(int i=0; i< NLYSOCRYSTALS; ++i) {
    hCorr_ly_ly[i] = new TH2D***[1];
    for(int j=0; j<1; ++j) {  
      hCorr_ly_ly[i][j] = new TH2D**[NLYSOCRYSTALS];
      for(int i2=i; i2< NLYSOCRYSTALS; ++i2) {  
        if(i2==i && j==0) continue; // do not correlate with yourself!
        else {
          hCorr_ly_ly[i][j][i2] = new TH2D*[1];
          for(int j2=0; j2<1; ++j2) {
            ostr << basename << "____"
            << "ly" << "_" << i << "_" << j << "__"
            << "ly" << "_" << i2 << "_" << j2;
            hCorr_ly_ly[i][j][i2][j2] = new TH2D(ostr.str().c_str(),ostr.str().c_str(), nbins, st_x, sp_x, nbins, st_x, sp_x);
            ostr.str("");
            ostr << "ly" << "_" << i << "_" << j << " [ADC]";;
            hCorr_ly_ly[i][j][i2][j2]->GetXaxis()->SetTitle(ostr.str().c_str());
            ostr.str("");
            ostr << "ly" << "_" << i2 << "_" << j2 << " [ADC]";;
            hCorr_ly_ly[i][j][i2][j2]->GetYaxis()->SetTitle(ostr.str().c_str());
            ostr.str("");
            hCorr_ly_ly[i][j][i2][j2]->SetDirectory(&fout);
          }
        }
      }
    }
  }




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
      //if(!sel4.Select(cev)) continue;
      if(!sel5.Select(cev)) continue;
      
      ///////////////////////////// Trigger bars
      for(int i=0; i< NTRIGSCINT; ++i) {
        for(int j=0; j<2; ++j) {
          // Trigger bars
          for(int i2=i; i2< NTRIGSCINT; ++i2) {  
            if(i2==i && j==1) continue; // do not correlate with yourself!
            else {
              for(int j2=0; j2<2; ++j2) {
                hCorr_tr_tr[i][j][i2][j2]->Fill(cev.trig.cont_hg[i][j],cev.trig.cont_hg[i2][j2]);
              }
            }
          }
          // Scintillator planes
          for(int i2=0; i2< NSCINTPLANES; ++i2) {  
            for(int j2=0; j2<2; ++j2) {
              hCorr_tr_sc[i][j][i2][j2]->Fill(cev.trig.cont_hg[i][j],cev.scint.cont_hg[i2][j2]);
            }
          }
          // Veto planes
          for(int i2=0; i2< NVETOSCINT; ++i2) {  
            for(int j2=0; j2<2; ++j2) {
              hCorr_tr_vt[i][j][i2][j2]->Fill(cev.trig.cont_hg[i][j],cev.veto.cont_hg[i2][j2]);
            }
          }
          // Lyso crystals
          for(int i2=0; i2< NLYSOCRYSTALS; ++i2) {  
            for(int j2=0; j2<1; ++j2) {
              hCorr_tr_ly[i][j][i2][j2]->Fill(cev.trig.cont_hg[i][j],cev.lyso.cont_hg[i2][j2]);
            }
          }
        }
      }

      ///////////////////////////// Scintillator planes
      for(int i=0; i< NSCINTPLANES; ++i) {
        for(int j=0; j<2; ++j) {  
          // Scintillator planes
          for(int i2=i; i2< NSCINTPLANES; ++i2) {  
            if(i2==i && j==1) continue; // do not correlate with yourself!
            else {
              for(int j2=0; j2<2; ++j2) {
                hCorr_sc_sc[i][j][i2][j2]->Fill(cev.scint.cont_hg[i][j],cev.scint.cont_hg[i2][j2]);
              }
            }
          }
          // Veto planes
          for(int i2=0; i2< NVETOSCINT; ++i2) {  
            for(int j2=0; j2<2; ++j2) {
              hCorr_sc_vt[i][j][i2][j2]->Fill(cev.scint.cont_hg[i][j],cev.veto.cont_hg[i2][j2]);
            }
          }
          // Lyso crystals
          for(int i2=0; i2< NLYSOCRYSTALS; ++i2) {  
            for(int j2=0; j2<1; ++j2) {
              hCorr_sc_ly[i][j][i2][j2]->Fill(cev.scint.cont_hg[i][j],cev.lyso.cont_hg[i2][j2]);
            }
          }
        }
      }

      ///////////////////////////// Veto planes
      for(int i=0; i< NVETOSCINT; ++i) {
        for(int j=0; j<2; ++j) {  
          // Veto planes
          for(int i2=i; i2< NVETOSCINT; ++i2) {  
            if(i2==i && j==1) continue; // do not correlate with yourself!
            else {
              for(int j2=0; j2<2; ++j2) {
                hCorr_vt_vt[i][j][i2][j2]->Fill(cev.veto.cont_hg[i][j],cev.veto.cont_hg[i2][j2]);
              }
            }
          }
          // Lyso crystals
          for(int i2=0; i2< NLYSOCRYSTALS; ++i2) {  
            for(int j2=0; j2<1; ++j2) {
              hCorr_vt_ly[i][j][i2][j2]->Fill(cev.veto.cont_hg[i][j],cev.lyso.cont_hg[i2][j2]);
            }
          }
        }
      }


      ///////////////////////////// Lyso crystals
      for(int i=0; i< NLYSOCRYSTALS; ++i) {
        for(int j=0; j<1; ++j) {  
          for(int i2=i; i2< NLYSOCRYSTALS; ++i2) {  
            if(i2==i && j==0) continue; // do not correlate with yourself!
            else {
              for(int j2=0; j2<1; ++j2) {
                hCorr_ly_ly[i][j][i2][j2]->Fill(cev.lyso.cont_hg[i][j],cev.lyso.cont_hg[i2][j2]);
              }
            }
          }
        }
      }

    }// entries loop
    inputFile.Close();
  }//file loop

  fout.Write();
  fout.Close();

  return 0;
}
