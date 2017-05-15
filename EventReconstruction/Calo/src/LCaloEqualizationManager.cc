#include "LCaloEqualizationManager.hh"
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "TF1.h"
#include "TH1I.h"
#include "TSpectrum.h"

LCaloEqualizationManager::LCaloEqualizationManager() {
  calRunFile = 0;
  verboseFLAG = true;
}

LCaloEqualizationManager &LCaloEqualizationManager::GetInstance() {
  static LCaloEqualizationManager instance;  // Guaranteed to be destroyed.
  return instance;
}

//---------------------------------------------------------------------------

int LCaloEqualizationManager::LoadRun(const std::string inputFileROOT,
                                      const std::string calibrationFile) {
  // Reset previous loaded runs
  if (calRunFile) {
    if (calRunFile->IsOpen()) calRunFile->Close();
    calRunFile = 0;
  }

  calRunFile = new LEvRec1File(inputFileROOT.c_str(), "READ");
  if (!calRunFile || !(calRunFile->IsOpen())) {
    std::cerr << __LCALOEQUALIZATIONMANAGER__
              << "Error! Attempt to load an equalizaton run, but the file "
                 "cannot be opened."
              << std::endl;
    calRunFile = 0;
    return EXIT_FAILURE;
  }
  cal = LCalibration::Read(calibrationFile.c_str());
  return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

LCaloEqualization *LCaloEqualizationManager::Equalize(const bool isHG) {
  if (calRunFile == 0 || !(calRunFile->IsOpen())) {
    std::cerr << __LCALOEQUALIZATIONMANAGER__
              << "Error! Attempt to call the \"Calibrate\" method, but no "
                 "calibration run loaded."
              << std::endl;
    return 0;
  }

  if (verboseFLAG)
    std::cout << __LCALOEQUALIZATIONMANAGER__
              << (isHG ? "High gain " : "Low gain ") << "equalization started"
              << std::endl;

  LEvRec1 cev;
  calRunFile->SetTheEventPointer(cev);
  int nEvents = calRunFile->GetEntries();

  // Events

  TH1I *h_pmt[NPMT];

  for (int k = 0; k < NPMT; k++) {
    TString hname = (isHG ? "PMT_High_Gain_Ch_" : "PMT_Low_Gain_Ch_");
    hname += k;
    h_pmt[k] = new TH1I(hname, hname, 250, -100, 900);
  }

  std::cout << "Filling histograms: " << std::setw(2) << 0 << "%";

  for (int iEv = 0; iEv < nEvents; iEv++)  // Event loop
  {
    std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2)
              << int(double(iEv) / double(nEvents - 1) * 100) << "%"
              << std::flush;
    calRunFile->GetEntry(iEv);
    int idPMT = 0;
    // trigger
    int nTriggerUnits = cev.trig.GetNUnits();
    int nTriggerPMTs = cev.trig.GetNPMTs();
    for (int itu = 0; itu < nTriggerUnits; itu++) {
      for (int itpmt = 0; itpmt < nTriggerPMTs; itpmt++) {
        (isHG ? h_pmt[idPMT]->Fill(cev.trig.sn_hg[itu][itpmt])
              : h_pmt[idPMT]->Fill(cev.trig.sn_lg[itu][itpmt]));
        idPMT++;
      }
    }

    int nScintUnits = cev.scint.GetNUnits();
    int nScintPMTs = cev.scint.GetNPMTs();
    for (int isu = 0; isu < nScintUnits; isu++) {
      for (int ispmt = 0; ispmt < nScintPMTs; ispmt++) {
        (isHG ? h_pmt[idPMT]->Fill(cev.scint.sn_hg[isu][ispmt])
              : h_pmt[idPMT]->Fill(cev.scint.sn_lg[isu][ispmt]));
        idPMT++;
      }
    }

    int nVetoUnits = cev.veto.GetNUnits();
    int nVetoPMTs = cev.veto.GetNPMTs();
    for (int ivu = 0; ivu < nVetoUnits; ivu++) {
      for (int ivpmt = 0; ivpmt < nVetoPMTs; ivpmt++) {
        (isHG ? h_pmt[idPMT]->Fill(cev.veto.sn_hg[ivu][ivpmt])
              : h_pmt[idPMT]->Fill(cev.veto.sn_lg[ivu][ivpmt]));
        idPMT++;
      }
    }

    int nLysoUnits = cev.lyso.GetNUnits();
    int nLysoPMTs = cev.lyso.GetNPMTs();

    for (int ilu = 0; ilu < nLysoUnits; ilu++) {
      for (int ilpmt = 0; ilpmt < nLysoPMTs; ilpmt++) {
        (isHG ? h_pmt[idPMT]->Fill(cev.lyso.sn_hg[ilu][ilpmt])
              : h_pmt[idPMT]->Fill(cev.lyso.sn_lg[ilu][ilpmt]));
        idPMT++;
      }
    }
  }
  std::cout << "\nFinding peaks..." << std::endl;

  LCaloEqualization *result = new LCaloEqualization();
  TFile out("out.root", "RECREATE");
  for (int ch = 0; ch < NPMT; ch++) {
    TSpectrum s;
    h_pmt[ch]->Smooth(2);

    Int_t nfound = s.Search(h_pmt[ch], 2, "", 0.1);
    Int_t nPeaks = s.GetNPeaks();               // Get number of peaks
    Float_t *peakPositions = s.GetPositionX();  // Get peak positions
    Float_t *peakHeights = s.GetPositionY();    // Get height of the peaksù
    bool isPeakFound = false;

    for (int ip = 0; ip < nPeaks; ip++) {
      if (peakPositions[ip] > 25) {
       // double maxFit = 2 * peakPositions[ip];
		  double binWidth = h_pmt[ch]->GetBinWidth(1);
        h_pmt[ch]->Fit("landau", "", "q", peakPositions[ip] - 4* binWidth, peakPositions[ip] + 4 * binWidth);
        isPeakFound = true;
        break;
      }
    }
    if (isPeakFound) {
      TF1 *f = h_pmt[ch]->GetFunction("landau");
      result->addFactors(f->GetParameter(1), f->GetParameter(2));
    } else
      result->addFactors(-1, -1);
	h_pmt[ch]->Write();
  }
  out.Close();
  std::cout << "End Equalization..." << std::endl;

  for (int k = 0; k < NPMT; k++) {
    delete h_pmt[k];
  }

  return result;
}

//---------------------------------------------------------------------------

LCaloEqualizationManager::~LCaloEqualizationManager() {
  // do not care about singleton destructor
}
