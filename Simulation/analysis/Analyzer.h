//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Nov 25 10:41:51 2013 by ROOT version 5.34/09
// from TTree EventTree/The Tree with the variable used to performe the calculation of energy deposition on the HEPD detector
// found on file: Electron5MeV_4M.root
//////////////////////////////////////////////////////////

#ifndef Analyzer_h
#define Analyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TString.h>
#include <TMath.h>
#include <iostream>
#include <fstream>

// Header file for the classes stored in the TTree if any.
#include "../source/Dataformats/include/RootEvent.hh"
#include "../source/Dataformats/include/RootTrack.hh"
#include "../source/Dataformats/include/RootVertex.hh"
#include "../source/Dataformats/include/RootCaloHit.hh"
#include "../source/Dataformats/include/RootTrackerHit.hh"

#include <TObject.h>
#include <TVector3.h>

#define ELECTRON "e-"
#define POSITRON "e+" //
#define MUON "mu-"  //
#define ANTI_MUON "mu+" //
#define ANTI_NU_E "anti_nu_e" //
#define NU_E "nu_e" //
#define ANTI_NU_MU "anti_nu_mu" //
#define NU_MU "nu_mu" //
#define GAMMA "gamma"
#define NEUTRON "neutron"
#define PROTON "proton"

// ADC - keV energy conversion factors (coming from real data!) 
double const p2ch_ADCconv = 341.667;    // [ADC/MeV]
double const p3ch_ADCconv = 345.33;    // [ADC/MeV]
double const n2ch_ADCconv = 425.333;   // [ADC/MeV]
double const n3ch_ADCconv = n2ch_ADCconv;   // [ADC/MeV]   ->   channel not working!

// Average electronic noise contribution obtained from "trackercal.txt" by using "sigmaADC_mean.py"
double const sigmaNoise = 3.6027857719; // [ADC]

// other constants
double const layerWidth = 0.30;  // [mm]
double const mProton = 938.2720813;  // in MeV


class Analyzer {
public :
  TTree          *fTree;        //!pointer to the analyzed TTree
  TChain         *fTreeChain;   //!pointer to the analyzed TChain
  Int_t           fCurrent;     //!current Tree number in a TChain
  // Declaration of leaf types
  RootEvent   *Event;
  double VETOEdep;
  double TOTALEdep;

  // List of branches
  TBranch        *b_Event;   //!
   
  float theERange;
  Bool_t checkPos;
  Bool_t checkTheta;
  Double_t Xlimit,Ylimit,Thetalimit;

  Int_t numbCaloLayer,numbLayerScint,numbLayerCrystal;

  TFile* fichhist;

  float Xgen; // cm
  float Ygen; // cm
  float sigma; // solid angle

  // particle observables
  Double_t pcharge;
  Double_t A;
  Double_t Mass;

  // hist acc
  TH2D* rtheta_gen;
  TH2D* rtheta_acc;
  TH2D* rtheta_acc_sv; //sans veto
  TH2D* rtheta_acc_svl; //sans veto lateral
  
  // muon
  TH2D* xy_gen;
  TH1D* ke_gen;
  TH1D* costhe_gen;

  // histograms regarding acceptance
  TProfile*          layerDepthElectron;
  TProfile*          layerDepthProton;
  TProfile*          energyAVGRangeElectron;
  TProfile*          energySUMRangeElectron;
  TProfile*          energyAVGRangeProton;
  TProfile*          energySUMRangeProton;
  TH1F*              Ecinetique_gen;
  TH1F*              Ecinetique_acc_trk;
  TH1F*              Ecinetique_acc_sci;
  TH1F*              Ecinetique_acc_veto;
  TH2D*              Vertex_xy_gen;
  TH2D*              Vertex_xy_acc_trk;
  TH2D*              Vertex_xy_acc_sci;
  TH2D*              Vertex_xy_acc_veto;
  TH2D*              Vertex_xy_gen_ce;
  TH2D*              Vertex_xy_acc_trk_ce;
  TH2D*              Vertex_xy_acc_sci_ce;
  TH2D*              Vertex_xy_acc_veto_ce;
  TH1F*              Pcinetique_gen;
  TH1F*              Pcinetique_acc_trk;
  TH1F*              Pcinetique_acc_sci;
  TH1F*              Pcinetique_acc_veto;

  // histograms for track multiplicity and secondary nuclei
  TH1D*              hntrace;
  TH1D*              hcsec;
  TH1D*              hcesec[28];

  // histograms for the energy loss of incoming Protons (can be applied also to other incoming particle type)
  TH1D*              hELossPrimaryEvent;
  TH1D*              hELossSecondaryEvent;
  TH1D*              hELossPrimaryTracker;
  TH1D*              hELossSecondaryTracker;
  TH1D*              hELossPrimaryCalo;
  TH1D*              hELossSecondaryCalo;
  TH1D*              hELossEvent;
  TH1D*              hELossTracker;
  TH1D*              hELossCalo;

  // histograms for the energy loss in tracker layers (useful to reconstrcut dE/dX)
  TH1D*              hELossTrackerLayer1;
  TH1D*              hELossTrackerLayer2;
  TH1D*              hBetaGammaLayer1;
  TH1D*              hBetaGammaLayer2;
  TH1D*              hADCLayer1_pch;
  TH1D*              hADCLayer2_pch;
  TH1D*              hADCLayer1_nch;
  TH1D*              hADCLayer2_nch;
  TH1D*              hADCLayer1_pch_noise;
  TH1D*              hADCLayer2_pch_noise;
  TH1D*              hADCLayer1_nch_noise;
  TH1D*              hADCLayer2_nch_noise;

  Analyzer();
  virtual ~Analyzer();
  // all the available functions in the Analyzer class (most of them are defined in "Analyzer.C") 
  virtual void     SetFile(TString fileName);
  virtual void     SetFileChain(TString fileName);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     InitChain();
  virtual void     Loop_acc();
  virtual void     Loop_muon();
  virtual void     Loop();
  virtual void     Loop_P();
  virtual void     Loop_ELoss(); //
  virtual void     Loop_P_ELoss_Trk(); //
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  virtual void     Particle(const char* cpart, FILE* frout_charge);
  virtual void     InitHisto_Muon_acc();
  virtual void     InitHisto_e_acc(float energie);
  virtual void     InitHisto_p_acc(float energie);
  virtual void     CloseHisto_acc();
  virtual void     CloseHisto_Muon_acc();
  virtual void     InitHisto();
  virtual void     CloseHisto();
  virtual void     InitHisto_ELoss();  //
  virtual void     CloseHisto_ELoss();  //
  virtual void     InitHisto_P_ELoss_Trk();  //
  virtual void     CloseHisto_P_ELoss_Trk();  //
  virtual void     InitHisto_P();
  virtual void     CloseHisto_P();
  virtual void     SetThetaAcceptance(Double_t theta); //in degree
  virtual void     SetAcceptanceWindows(Double_t X, Double_t Y); //in millimiters
  virtual Double_t ComputeAngle(std::vector<RootTrackerHit>& myTkHit);
  virtual Double_t ComputeAngleWithSmearing(std::vector<RootTrackerHit>& myTkHit,Double_t delta);
};

#endif
 
#ifdef Analyzer_cxx
Analyzer::Analyzer() : fTree(0)
{
   Xgen = 2*27.5; // cm
   Ygen = 2*37.5; // cm
   sigma = TMath::Pi(); // solid angle
}

Analyzer::~Analyzer()
{
  if (!fTree) return;
  delete fTree->GetCurrentFile();
}

void Analyzer::SetFile(TString fileName)
{
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.

  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fileName);
  if (!f || !f->IsOpen()) {
    f = new TFile(fileName);
  }
  fileName.Append(":/HEPD");
  TDirectory * dir = (TDirectory*)f->Get(fileName);
  dir->GetObject("EventTree",fTree);
  Init(fTree);  
  
}

void Analyzer::SetFileChain(TString fileName)
{
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  //fileName.Append(":/HEPD");

  TString HEPDPath = "/HEPD/EventTree";
  fTreeChain = new TChain(HEPDPath);

  std::ifstream File_Input(fileName);
  //  if (!File_Input){
  //    std::cout<<"List of ROOT file NOT found"<<std::endl;
  //    return;
  //  }
  while(!File_Input.eof()){
    TString singlefile = "noName";
    File_Input >> singlefile;
    if(singlefile!="noName"&&singlefile!=""){
    fTreeChain->Add(singlefile);
    printf("ajoute %s\n",singlefile.Data());
    }
  }
  File_Input.close();
  InitChain();  
}


Int_t Analyzer::GetEntry(Long64_t entry)
{
// Read contents of entry.

  if (!fTree) return 0;
  return fTree->GetEntry(entry);
}
Long64_t Analyzer::LoadTree(Long64_t entry)
{
// Set the environment to read one entry

//   printf("load tree\n");
   if (!fTreeChain) return -5;
   Long64_t centry = fTreeChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fTreeChain->GetTreeNumber() != fCurrent) {
      fCurrent = fTreeChain->GetTreeNumber();
      Notify();
   }
   //   printf("load tree\n",fTree);
   return centry;
}

void Analyzer::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  
  // Set branch addresses and branch pointers
  Event = 0;
  if (!tree) return;
  fTree = tree;
  fCurrent = -1;
  
  fTree->SetBranchAddress("Event", &Event, &b_Event);
  Notify();
}

void Analyzer::InitChain()
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  
  // Set branch addresses and branch pointers
  Event = 0;
  fCurrent = -1;
  printf("fTreeChain %p\n",fTreeChain);
  
  fTreeChain->SetBranchAddress("Event", &Event, &b_Event);
  printf("fTreeChain %p\n",fTreeChain);
  Notify();
}

Bool_t Analyzer::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void Analyzer::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry

  if (!fTree) return;
  fTree->Show(entry);
}
#endif // #ifdef Analyzer_cxx
