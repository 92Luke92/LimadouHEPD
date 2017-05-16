#include "LCaloEqualizationManager.hh"
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "LSteeringManager.hh"
#include "TF1.h"
#include "TH1I.h"
#include "TSpectrum.h"

LCaloEqualizationManager::LCaloEqualizationManager() {
  calRunFile = 0;
  steer.verboseFLAG = true;
  steer.L1fname.clear();
}

LCaloEqualizationManager &LCaloEqualizationManager::GetInstance() {
  static LCaloEqualizationManager instance;  // Guaranteed to be destroyed.
  return instance;
}

//---------------------------------------------------------------------------
void LCaloEqualizationManager::LoadSteering(const std::string steerFileIN) {
  std::cout << __LCALOEQUALIZATIONMANAGER__ << "Loading the steerig file \""
            << steerFileIN << "\"..." << std::endl;

  LSteeringManager steerMan;
  steerMan.Load(steerFileIN);
  // Load calibration
  steer.calFileName = steerMan.GetParameter<std::string>("CALBFILE");
  cal = LCalibration::Read(steer.calFileName.c_str());
  // Input file list
  steer.inpFileList = steerMan.GetParameter<std::string>("INPLIST");
  // Output folder
  steer.outDirectory = steerMan.GetParameter<std::string>("OUTFOLD");
  // Verbose Flag
  steer.verboseFLAG = steerMan.GetParameter<bool>("VERBOSE");
  // Sigma for peak searching
  steer.sigma = steerMan.GetParameter<double>("SIGMA");

  // Threshold for peak searching
  steer.threshold = steerMan.GetParameter<double>("THRESHOLD");
  std::cout << "steer.sigma: " << steer.sigma
            << "steer.threshold: " << steer.threshold << std::endl;
  std::cout << __LCALOEQUALIZATIONMANAGER__ << "Steering file loaded."
            << std::endl;
}
//---------------------------------------------------------------------------
int LCaloEqualizationManager::Run() {
  int nFilesToBeProcessed = LoadInputFileList();
  if (nFilesToBeProcessed != 1) {
    std::cerr
        << "Equalization procedure runs over 1 single file: the list contains "
        << nFilesToBeProcessed << " files!" << std::endl;
    return -1;
  }

  LoadRun(steer.L1fname.at(0),
          steer.calFileName);  // later on we can add running on multiple files
  std::cout << __LCALOEQUALIZATIONMANAGER__ << "Processing file "
            << steer.L1fname.at(0) << "..." << std::endl;
  LCaloEqualization *eqHG = EqualizeHG();
  LCaloEqualization *eqLG = EqualizeLG();
  eqHG->Write("eqHG.txt");
  eqLG->Write("eqLG.txt");
  return 0;
}

//---------------------------------------------------------------------------
int LCaloEqualizationManager::LoadInputFileList() {
  steer.L1fname.resize(0);
  std::ifstream in(steer.inpFileList.c_str());
  std::string fname;
  while (in >> fname) {
    steer.L1fname.push_back(fname);
  }

  int result = steer.L1fname.size();
  std::cout << __LCALOEQUALIZATIONMANAGER__ << result
            << " files about to be processed." << std::endl;
  return result;
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

  if (steer.verboseFLAG)
    std::cout << __LCALOEQUALIZATIONMANAGER__
              << (isHG ? "High gain " : "Low gain ") << "equalization started"
              << std::endl;

  LEvRec1 cev;
  calRunFile->SetTheEventPointer(cev);
  int nEvents = calRunFile->GetEntries();

  // Events

  TH1I h_pmt[NPMT];

  for (int k = 0; k < NPMT; k++) {
    TString hname = (isHG ? "PMT_High_Gain_Ch_" : "PMT_Low_Gain_Ch_");
    hname += k;
    h_pmt[k].SetName(hname);
    h_pmt[k].SetTitle(hname);
    h_pmt[k].SetBins(250, -100, 900);
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
        (isHG ? h_pmt[idPMT].Fill(cev.trig.sn_hg[itu][itpmt])
              : h_pmt[idPMT].Fill(cev.trig.sn_lg[itu][itpmt]));
        idPMT++;
      }
    }

    int nScintUnits = cev.scint.GetNUnits();
    int nScintPMTs = cev.scint.GetNPMTs();
    for (int isu = 0; isu < nScintUnits; isu++) {
      for (int ispmt = 0; ispmt < nScintPMTs; ispmt++) {
        (isHG ? h_pmt[idPMT].Fill(cev.scint.sn_hg[isu][ispmt])
              : h_pmt[idPMT].Fill(cev.scint.sn_lg[isu][ispmt]));
        idPMT++;
      }
    }

    int nVetoUnits = cev.veto.GetNUnits();
    int nVetoPMTs = cev.veto.GetNPMTs();
    for (int ivu = 0; ivu < nVetoUnits; ivu++) {
      for (int ivpmt = 0; ivpmt < nVetoPMTs; ivpmt++) {
        (isHG ? h_pmt[idPMT].Fill(cev.veto.sn_hg[ivu][ivpmt])
              : h_pmt[idPMT].Fill(cev.veto.sn_lg[ivu][ivpmt]));
        idPMT++;
      }
    }

    int nLysoUnits = cev.lyso.GetNUnits();
    int nLysoPMTs = cev.lyso.GetNPMTs();

    for (int ilu = 0; ilu < nLysoUnits; ilu++) {
      for (int ilpmt = 0; ilpmt < nLysoPMTs; ilpmt++) {
        (isHG ? h_pmt[idPMT].Fill(cev.lyso.sn_hg[ilu][ilpmt])
              : h_pmt[idPMT].Fill(cev.lyso.sn_lg[ilu][ilpmt]));
        idPMT++;
      }
    }
  }
  std::cout << "\nFinding peaks..." << std::endl;

  LCaloEqualization *result = new LCaloEqualization();
  std::string outNameROOT_DEBUG;
  if (isHG)
    outNameROOT_DEBUG = "equalizationHG.root";
  else
    outNameROOT_DEBUG = "equalizationLG.root";
  TFile out(outNameROOT_DEBUG.c_str(), "RECREATE");
  for (int ch = 0; ch < NPMT; ch++) {
    TSpectrum s;
    h_pmt[ch].Smooth(1);

    Int_t nfound = s.Search(&(h_pmt[ch]), steer.sigma, "", steer.threshold);
    Int_t nPeaks = s.GetNPeaks();               // Get number of peaks
    Float_t *peakPositions = s.GetPositionX();  // Get peak positions
    Float_t *peakHeights = s.GetPositionY();    // Get height of the peaksù
    bool isPeakFound = false;

    for (int ip = 0; ip < nPeaks; ip++) {
      if (peakPositions[ip] > 15) {
        // double maxFit = 2 * peakPositions[ip];
        double binWidth = h_pmt[ch].GetBinWidth(1);
        h_pmt[ch].Fit("landau", "", "q", peakPositions[ip] - 2 * binWidth,
                      peakPositions[ip] + 5 * binWidth);
        isPeakFound = true;
        break;
      }
    }
    if (isPeakFound) {
      TF1 *f = h_pmt[ch].GetFunction("landau");
      result->addFactors(f->GetParameter(1), f->GetParameter(2));
    } else
      result->addFactors(-1, -1);
    h_pmt[ch].Write();
  }
  out.Close();
  std::cout << "End Equalization..." << std::endl;

  return result;
}

//---------------------------------------------------------------------------

LCaloEqualizationManager::~LCaloEqualizationManager() {
  // do not care about singleton destructor
}
