#include <TH1.h>
#include <TF1.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TMath.h>
#include <vector>

// used in function figs()
#define n_z1 3
#define n_z2 3
#define n_z3 7
#define n_z4 7
#define n_z5 6
#define n_z6 7
#define n_z7 8
#define n_z8 7
#define n_z9 8
#define n_z10 3
#define n_z11 5
#define n_z12 4
#define n_z13 4
#define n_z14 5
#define n_z15 1
#define n_z29 1
#define n_z34 1
#define n_z36 1
#define n_z37 6
#define n_z38 7
#define n_z39 7
#define n_z40 5
#define n_z69 12
#define n_z70 12
#define n_z71 15
#define n_z72 14


void figs_3HAD_cmp() 
{
  /*
  This function displays the track multiplicity in the context of three different hadronic models 
  and compares the results
  */

  char chist[180];
  char cchist[180];
  char ctext[180];
  char cfich[3][180];

  //// case centre: the beam is centered in (x = 0, y = 0) with angle 0 deg
  //sprintf(cfich[0],"hepd_bic_68MeV_proton_centre_hist.root");
  //sprintf(cfich[1],"hepd_qmd_68MeV_proton_centre_hist.root");
  //sprintf(cfich[2],"hepd_incl_68MeV_proton_centre_hist.root");

  // case 550: the beam is at the position 5 5 cm (x y) 0 deg
  sprintf(cfich[0],"hepd_bic_68MeV_proton_550_hist.root");
  sprintf(cfich[1],"hepd_qmd_68MeV_proton_550_hist.root");
  sprintf(cfich[2],"hepd_incl_68MeV_proton_550_hist.root");

  TFile* fich[3];
  TH1D *hntrace[3];

  for (int f=0; f<3; f++) {
    printf("fichier %d %s\n",f,cfich[f]);
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hntrace");
    sprintf(cchist,"hntrace%d",f);
    hntrace[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hntrace[f]->GetYaxis()->SetLabelOffset(0.005);
    hntrace[f]->GetYaxis()->SetLabelSize(0.050);
    hntrace[f]->GetYaxis()->SetTitle("Entries");
    hntrace[f]->GetYaxis()->SetTitleSize(0.055);
    hntrace[f]->GetYaxis()->SetTitleOffset(1.25);
    hntrace[f]->GetYaxis()->CenterTitle();
    hntrace[f]->GetXaxis()->SetLabelOffset(0.010);
    hntrace[f]->GetXaxis()->SetLabelSize(0.050);
    hntrace[f]->GetXaxis()->SetTitle("Number Tracks / Event");
    hntrace[f]->GetXaxis()->SetTitleSize(0.055);
    hntrace[f]->GetXaxis()->SetTitleOffset(1.1);
    hntrace[f]->GetXaxis()->CenterTitle();
    hntrace[f]->SetMinimum(0.5);
    if (f == 0) hntrace[f]->SetLineColor(kBlack);
    if (f == 1) hntrace[f]->SetLineColor(kGreen+2);
    if (f == 2) hntrace[f]->SetLineColor(kMagenta+2);
  }

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  gStyle->SetFrameFillColor(10);


  TPad* ptpad;
  char cpad[50];

  TCanvas *c1 = new TCanvas( "c1","tmult", 0, 0, 600, 400 );

  c1->Divide(1,1,0.001,0.001);
  for (int i=1; i<2; i++) {
    sprintf(cpad,"c1_%d",i);
    ptpad = (TPad*) c1->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c1->cd(i);
    switch(i) {
    case 1: {
              hntrace[2]->Draw(); 
              hntrace[1]->Draw("samehist"); 
              hntrace[0]->Draw("samehist"); 
              TLegend *legend_c1 = new TLegend(0.7,0.5,0.83,0.8);
              legend_c1->AddEntry(hntrace[0],"BIC","l");
              legend_c1->AddEntry(hntrace[1],"QMD","l");
              legend_c1->AddEntry(hntrace[2],"INCL++","l");
              legend_c1->Draw();
              break;
            }
    default: break;
    } 
  }

  return;

}

void figs_3HAD_ELoss_cmp() 
{
  /*
  This function displays the energy lost by primary and/or secondary particles in the tracker or/and in the Calo 
  in the context of three different hadronic models and compares the results
  */

  char chist[180];
  char cchist[180];
  char ctext[180];
  char cfich[3][180];

  TH1D *hELossPrimaryEvent[3];
  TH1D *hELossSecondaryEvent[3];
  TH1D *hELossPrimaryTracker[3];
  TH1D *hELossSecondaryTracker[3];
  TH1D *hELossPrimaryCalo[3];
  TH1D *hELossSecondaryCalo[3];
  TH1D *hELossEvent[3];
  TH1D *hELossTracker[3];
  TH1D *hELossCalo[3];

  //// case centre: the beam is centered in (x = 0, y = 0) with angle 0 deg
  //sprintf(cfich[0],"hepd_bic_68MeV_proton_centre_hist.root");
  //sprintf(cfich[1],"hepd_qmd_68MeV_proton_centre_hist.root");
  //sprintf(cfich[2],"hepd_incl_68MeV_proton_centre_hist.root");

  // case 550: the beam is at the position 5 5 cm (x y) 0 deg
  sprintf(cfich[0],"hepd_bic_68MeV_proton_550_hist.root");
  sprintf(cfich[1],"hepd_qmd_68MeV_proton_550_hist.root");
  sprintf(cfich[2],"hepd_incl_68MeV_proton_550_hist.root");

  TFile* fich[3];


  // ELoss Primary Event (2)
  for (int f=0; f<3; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossPrimaryEvent");
    sprintf(cchist,"hELossPrimaryEvent%d",f);
    hELossPrimaryEvent[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossPrimaryEvent[f]->SetTitle("Primary Events");
    hELossPrimaryEvent[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossPrimaryEvent[f]->GetYaxis()->SetLabelSize(0.050);
    hELossPrimaryEvent[f]->GetYaxis()->SetTitle("Entries");
    hELossPrimaryEvent[f]->GetYaxis()->SetTitleSize(0.055);
    hELossPrimaryEvent[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossPrimaryEvent[f]->GetYaxis()->CenterTitle();
    hELossPrimaryEvent[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossPrimaryEvent[f]->GetXaxis()->SetLabelSize(0.050);
    hELossPrimaryEvent[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossPrimaryEvent[f]->GetXaxis()->SetTitleSize(0.055);
    hELossPrimaryEvent[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossPrimaryEvent[f]->GetXaxis()->CenterTitle();
    hELossPrimaryEvent[f]->SetMinimum(0.5);
    if (f == 0) hELossPrimaryEvent[f]->SetLineColor(kRed+2);
    if (f == 1) hELossPrimaryEvent[f]->SetLineColor(kRed);
    if (f == 2) hELossPrimaryEvent[f]->SetLineColor(kOrange+2);
  }

  // ELoss Secondary Event (2)
  for (int f=0; f<3; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossSecondaryEvent");
    sprintf(cchist,"hELossSecondaryEvent%d",f);
    hELossSecondaryEvent[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossSecondaryEvent[f]->SetTitle("Secondary Events");
    hELossSecondaryEvent[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossSecondaryEvent[f]->GetYaxis()->SetLabelSize(0.050);
    hELossSecondaryEvent[f]->GetYaxis()->SetTitle("Entries");
    hELossSecondaryEvent[f]->GetYaxis()->SetTitleSize(0.055);
    hELossSecondaryEvent[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossSecondaryEvent[f]->GetYaxis()->CenterTitle();
    hELossSecondaryEvent[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossSecondaryEvent[f]->GetXaxis()->SetLabelSize(0.050);
    hELossSecondaryEvent[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossSecondaryEvent[f]->GetXaxis()->SetTitleSize(0.055);
    hELossSecondaryEvent[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossSecondaryEvent[f]->GetXaxis()->CenterTitle();
    hELossSecondaryEvent[f]->SetMinimum(0.5);
    if (f == 0) hELossSecondaryEvent[f]->SetLineColor(kBlue+2);
    if (f == 1) hELossSecondaryEvent[f]->SetLineColor(kBlue-1);
    if (f == 2) hELossSecondaryEvent[f]->SetLineColor(kCyan+2);
  }

  // ELoss Event (2)
  for (int f=0; f<3; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossEvent");
    sprintf(cchist,"hELossEvent%d",f);
    hELossEvent[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossEvent[f]->SetTitle("Events");
    hELossEvent[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossEvent[f]->GetYaxis()->SetLabelSize(0.050);
    hELossEvent[f]->GetYaxis()->SetTitle("Entries");
    hELossEvent[f]->GetYaxis()->SetTitleSize(0.055);
    hELossEvent[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossEvent[f]->GetYaxis()->CenterTitle();
    hELossEvent[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossEvent[f]->GetXaxis()->SetLabelSize(0.050);
    hELossEvent[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossEvent[f]->GetXaxis()->SetTitleSize(0.055);
    hELossEvent[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossEvent[f]->GetXaxis()->CenterTitle();
    hELossEvent[f]->SetMinimum(0.5);
    if (f == 0) hELossEvent[f]->SetLineColor(kBlack);
    if (f == 1) hELossEvent[f]->SetLineColor(kGreen+2);
    if (f == 2) hELossEvent[f]->SetLineColor(kMagenta+2);
  }

  // ELoss Primary Tracker (3)
  for (int f=0; f<3; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossPrimaryTracker");
    sprintf(cchist,"hELossPrimaryTracker%d",f);
    hELossPrimaryTracker[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossPrimaryTracker[f]->SetTitle("Primary Tracker Events");
    hELossPrimaryTracker[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossPrimaryTracker[f]->GetYaxis()->SetLabelSize(0.050);
    hELossPrimaryTracker[f]->GetYaxis()->SetTitle("Entries");
    hELossPrimaryTracker[f]->GetYaxis()->SetTitleSize(0.055);
    hELossPrimaryTracker[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossPrimaryTracker[f]->GetYaxis()->CenterTitle();
    hELossPrimaryTracker[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossPrimaryTracker[f]->GetXaxis()->SetLabelSize(0.050);
    hELossPrimaryTracker[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossPrimaryTracker[f]->GetXaxis()->SetTitleSize(0.055);
    hELossPrimaryTracker[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossPrimaryTracker[f]->GetXaxis()->CenterTitle();
    hELossPrimaryTracker[f]->SetMinimum(0.5);
    if (f == 0) hELossPrimaryTracker[f]->SetLineColor(kRed+2);
    if (f == 1) hELossPrimaryTracker[f]->SetLineColor(kRed);
    if (f == 2) hELossPrimaryTracker[f]->SetLineColor(kOrange+2);
  }

  // ELoss Secondary Tracker (3)
  for (int f=0; f<3; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossSecondaryTracker");
    sprintf(cchist,"hELossSecondaryTracker%d",f);
    hELossSecondaryTracker[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossSecondaryTracker[f]->SetTitle("Secondary Traker Events");
    hELossSecondaryTracker[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossSecondaryTracker[f]->GetYaxis()->SetLabelSize(0.050);
    hELossSecondaryTracker[f]->GetYaxis()->SetTitle("Entries");
    hELossSecondaryTracker[f]->GetYaxis()->SetTitleSize(0.055);
    hELossSecondaryTracker[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossSecondaryTracker[f]->GetYaxis()->CenterTitle();
    hELossSecondaryTracker[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossSecondaryTracker[f]->GetXaxis()->SetLabelSize(0.050);
    hELossSecondaryTracker[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossSecondaryTracker[f]->GetXaxis()->SetTitleSize(0.055);
    hELossSecondaryTracker[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossSecondaryTracker[f]->GetXaxis()->CenterTitle();
    hELossSecondaryTracker[f]->SetMinimum(0.5);
    if (f == 0) hELossSecondaryTracker[f]->SetLineColor(kBlue+2);
    if (f == 1) hELossSecondaryTracker[f]->SetLineColor(kBlue-1);
    if (f == 2) hELossSecondaryTracker[f]->SetLineColor(kCyan+2);
  }

  // ELoss Tracker (4)
  for (int f=0; f<3; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossTracker");
    sprintf(cchist,"hELossTracker%d",f);
    hELossTracker[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossTracker[f]->SetTitle("Traker Events");
    hELossTracker[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossTracker[f]->GetYaxis()->SetLabelSize(0.050);
    hELossTracker[f]->GetYaxis()->SetTitle("Entries");
    hELossTracker[f]->GetYaxis()->SetTitleSize(0.055);
    hELossTracker[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossTracker[f]->GetYaxis()->CenterTitle();
    hELossTracker[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossTracker[f]->GetXaxis()->SetLabelSize(0.050);
    hELossTracker[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossTracker[f]->GetXaxis()->SetTitleSize(0.055);
    hELossTracker[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossTracker[f]->GetXaxis()->CenterTitle();
    hELossTracker[f]->SetMinimum(0.5);
    if (f == 0) hELossTracker[f]->SetLineColor(kBlack);
    if (f == 1) hELossTracker[f]->SetLineColor(kGreen+2);
    if (f == 2) hELossTracker[f]->SetLineColor(kMagenta+2);
  }

  // ELoss Primary Calo (3)
  for (int f=0; f<3; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossPrimaryCalo");
    sprintf(cchist,"hELossPrimaryCalo%d",f);
    hELossPrimaryCalo[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossPrimaryCalo[f]->SetTitle("Primary Calo Events");
    hELossPrimaryCalo[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossPrimaryCalo[f]->GetYaxis()->SetLabelSize(0.050);
    hELossPrimaryCalo[f]->GetYaxis()->SetTitle("Entries");
    hELossPrimaryCalo[f]->GetYaxis()->SetTitleSize(0.055);
    hELossPrimaryCalo[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossPrimaryCalo[f]->GetYaxis()->CenterTitle();
    hELossPrimaryCalo[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossPrimaryCalo[f]->GetXaxis()->SetLabelSize(0.050);
    hELossPrimaryCalo[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossPrimaryCalo[f]->GetXaxis()->SetTitleSize(0.055);
    hELossPrimaryCalo[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossPrimaryCalo[f]->GetXaxis()->CenterTitle();
    hELossPrimaryCalo[f]->SetMinimum(0.5);
    if (f == 0) hELossPrimaryCalo[f]->SetLineColor(kRed+2);
    if (f == 1) hELossPrimaryCalo[f]->SetLineColor(kRed);
    if (f == 2) hELossPrimaryCalo[f]->SetLineColor(kOrange+2);
  }

  // ELoss Secondary Calo (3)
  for (int f=0; f<3; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossSecondaryCalo");
    sprintf(cchist,"hELossSecondaryCalo%d",f);
    hELossSecondaryCalo[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossSecondaryCalo[f]->SetTitle("Secondary Calo Events");
    hELossSecondaryCalo[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossSecondaryCalo[f]->GetYaxis()->SetLabelSize(0.050);
    hELossSecondaryCalo[f]->GetYaxis()->SetTitle("Entries");
    hELossSecondaryCalo[f]->GetYaxis()->SetTitleSize(0.055);
    hELossSecondaryCalo[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossSecondaryCalo[f]->GetYaxis()->CenterTitle();
    hELossSecondaryCalo[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossSecondaryCalo[f]->GetXaxis()->SetLabelSize(0.050);
    hELossSecondaryCalo[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossSecondaryCalo[f]->GetXaxis()->SetTitleSize(0.055);
    hELossSecondaryCalo[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossSecondaryCalo[f]->GetXaxis()->CenterTitle();
    hELossSecondaryCalo[f]->SetMinimum(0.5);
    if (f == 0) hELossSecondaryCalo[f]->SetLineColor(kBlue+2);
    if (f == 1) hELossSecondaryCalo[f]->SetLineColor(kBlue-1);
    if (f == 2) hELossSecondaryCalo[f]->SetLineColor(kCyan+2);
  }

  // ELoss Calo (4)
  for (int f=0; f<3; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossCalo");
    sprintf(cchist,"hELossCalo%d",f);
    hELossCalo[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossCalo[f]->SetTitle("Calo Events");
    hELossCalo[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossCalo[f]->GetYaxis()->SetLabelSize(0.050);
    hELossCalo[f]->GetYaxis()->SetTitle("Entries");
    hELossCalo[f]->GetYaxis()->SetTitleSize(0.055);
    hELossCalo[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossCalo[f]->GetYaxis()->CenterTitle();
    hELossCalo[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossCalo[f]->GetXaxis()->SetLabelSize(0.050);
    hELossCalo[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossCalo[f]->GetXaxis()->SetTitleSize(0.055);
    hELossCalo[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossCalo[f]->GetXaxis()->CenterTitle();
    hELossCalo[f]->SetMinimum(0.5);
    if (f == 0) hELossCalo[f]->SetLineColor(kBlack);
    if (f == 1) hELossCalo[f]->SetLineColor(kGreen+2);
    if (f == 2) hELossCalo[f]->SetLineColor(kMagenta+2);
  }


  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  gStyle->SetFrameFillColor(10);

  TPad* ptpad;
  char cpad[50];


  TCanvas *c2 = new TCanvas( "c2","hELossEvent Primary/Secondary + Sum", 0, 0, 1200, 600 );

  c2->Divide(3,1,0.001,0.001);
  for (int i=1; i<4; i++) {
    sprintf(cpad,"c2_%d",i);
    ptpad = (TPad*) c2->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c2->cd(i);
    switch(i) {
    case 1: {
              hELossPrimaryEvent[0]->Draw(); 
              hELossPrimaryEvent[1]->Draw("samehist"); 
              hELossPrimaryEvent[2]->Draw("samehist"); 
              TLegend *legend_c1_1 = new TLegend(0.20,0.65,0.50,0.85);
              legend_c1_1->AddEntry(hELossPrimaryEvent[0],"BIC","l");
              legend_c1_1->AddEntry(hELossPrimaryEvent[1],"QMD","l");
              legend_c1_1->AddEntry(hELossPrimaryEvent[2],"INCL++","l");
              legend_c1_1->Draw();
              break;
            }
    case 2: {
              hELossSecondaryEvent[0]->Draw(); 
              hELossSecondaryEvent[1]->Draw("samehist"); 
              hELossSecondaryEvent[2]->Draw("samehist"); 
              TLegend *legend_c1_2 = new TLegend(0.50,0.65,0.80,0.85);
              legend_c1_2->AddEntry(hELossSecondaryEvent[0],"BIC","l");
              legend_c1_2->AddEntry(hELossSecondaryEvent[1],"QMD","l");
              legend_c1_2->AddEntry(hELossSecondaryEvent[2],"INCL++","l");
              legend_c1_2->Draw();
              break;
            }
    case 3: {
              hELossEvent[0]->Draw(); 
              hELossEvent[1]->Draw("samehist"); 
              hELossEvent[2]->Draw("samehist"); 
              TLegend *legend_c1_3 = new TLegend(0.20,0.65,0.50,0.85);
              legend_c1_3->AddEntry(hELossEvent[0],"BIC","l");
              legend_c1_3->AddEntry(hELossEvent[1],"QMD","l");
              legend_c1_3->AddEntry(hELossEvent[2],"INCL++","l");
              legend_c1_3->Draw();
              break;
            }
    default: break;
    } 
  }

  TCanvas *c3 = new TCanvas( "c3","hELossPrimaryTracker/Calo - hELossSecondaryTracker/Calo", 0, 0, 1200, 800 );

  c3->Divide(2,2,0.001,0.001);
  for (int i=1; i<5; i++) {
    sprintf(cpad,"c3_%d",i);
    ptpad = (TPad*) c3->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c3->cd(i);
    switch(i) {
    case 1: {
              hELossPrimaryTracker[0]->Draw(); 
              hELossPrimaryTracker[1]->Draw("samehist"); 
              hELossPrimaryTracker[2]->Draw("samehist"); 
              TLegend *legend_c2_1 = new TLegend(0.61,0.52,0.81,0.82);
              legend_c2_1->AddEntry(hELossPrimaryTracker[0],"BIC","l");
              legend_c2_1->AddEntry(hELossPrimaryTracker[1],"QMD","l");
              legend_c2_1->AddEntry(hELossPrimaryTracker[2],"INCL++","l");
              legend_c2_1->Draw();
              break;
            }
    case 2: {
              hELossSecondaryTracker[0]->Draw(); 
              hELossSecondaryTracker[1]->Draw("samehist"); 
              hELossSecondaryTracker[2]->Draw("samehist"); 
              TLegend *legend_c2_2 = new TLegend(0.61,0.52,0.81,0.82);
              legend_c2_2->AddEntry(hELossSecondaryTracker[0],"BIC","l");
              legend_c2_2->AddEntry(hELossSecondaryTracker[1],"QMD","l");
              legend_c2_2->AddEntry(hELossSecondaryTracker[2],"INCL++","l");
              legend_c2_2->Draw();
              break;
            }
    case 3: {
              hELossPrimaryCalo[0]->Draw(); 
              hELossPrimaryCalo[1]->Draw("samehist"); 
              hELossPrimaryCalo[2]->Draw("samehist"); 
              TLegend *legend_c2_3 = new TLegend(0.19,0.52,0.39,0.82);
              legend_c2_3->AddEntry(hELossPrimaryCalo[0],"BIC","l");
              legend_c2_3->AddEntry(hELossPrimaryCalo[1],"QMD","l");
              legend_c2_3->AddEntry(hELossPrimaryCalo[2],"INCL++","l");
              legend_c2_3->Draw();
              break;
            }
    case 4: {
              hELossSecondaryCalo[0]->Draw(); 
              hELossSecondaryCalo[1]->Draw("samehist"); 
              hELossSecondaryCalo[2]->Draw("samehist"); 
              TLegend *legend_c2_4 = new TLegend(0.61,0.52,0.81,0.82);
              legend_c2_4->AddEntry(hELossSecondaryCalo[0],"BIC","l");
              legend_c2_4->AddEntry(hELossSecondaryCalo[1],"QMD","l");
              legend_c2_4->AddEntry(hELossSecondaryCalo[2],"INCL++","l");
              legend_c2_4->Draw();
              break;
            }
    default: break;
    } 
  }

  TCanvas *c4 = new TCanvas( "c4","hELossTracker - hELossCalo", 0, 0, 1200, 600 );

  c4->Divide(2,1,0.001,0.001);
  for (int i=1; i<3; i++) {
    sprintf(cpad,"c4_%d",i);
    ptpad = (TPad*) c4->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c4->cd(i);
    switch(i) {
    case 1: {
              hELossTracker[0]->Draw(); 
              hELossTracker[1]->Draw("samehist"); 
              hELossTracker[2]->Draw("samehist"); 
              TLegend *legend_c4_1 = new TLegend(0.61,0.53,0.81,0.83);
              legend_c4_1->AddEntry(hELossTracker[0],"BIC","l");
              legend_c4_1->AddEntry(hELossTracker[1],"QMD","l");
              legend_c4_1->AddEntry(hELossTracker[2],"INCL++","l");
              legend_c4_1->Draw();
              break;
            }
    case 2: {
              hELossCalo[0]->Draw(); 
              hELossCalo[1]->Draw("samehist"); 
              hELossCalo[2]->Draw("samehist"); 
              TLegend *legend_c4_2 = new TLegend(0.19,0.53,0.39,0.83);
              legend_c4_2->AddEntry(hELossCalo[0],"BIC","l");
              legend_c4_2->AddEntry(hELossCalo[1],"QMD","l");
              legend_c4_2->AddEntry(hELossCalo[2],"INCL++","l");
              legend_c4_2->Draw();
              break;
            }
    default: break;
    } 
  }
  
  return;

}

void figs_4pos_cmp() 
{
  /*
  This function displays the track multiplicity for the QMD hadronic model in the case 
  of 4 different beam positions (the four around the center: 3C, 4C, 3A and 4A) and compares the results
  */

  char chist[180];
  char cchist[180];
  char ctext[180];
  char cfich[4][180];

  sprintf(cfich[0],"hepd_qmd_68MeV_proton_3C0_hist.root"); // 3C: -1.75 -1.8 cm (x y) 0 deg
  sprintf(cfich[1],"hepd_qmd_68MeV_proton_4C0_hist.root"); // 4C: -1.75 1.8 cm (x y) 0 deg
  sprintf(cfich[2],"hepd_qmd_68MeV_proton_3A0_hist.root"); // 3A: 1.75 -1.8 cm (x y) 0 deg
  sprintf(cfich[3],"hepd_qmd_68MeV_proton_4A0_hist.root"); // 4A: 1.75 1.8 cm (x y) 0 deg 

  TFile* fich[4];

  TH1D *hntrace[4];

  for (int f=0; f<4; f++) {
    printf("fichier %d %s\n",f,cfich[f]);
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hntrace");
    sprintf(cchist,"hntrace%d",f);
    hntrace[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hntrace[f]->GetYaxis()->SetLabelOffset(0.005);
    hntrace[f]->GetYaxis()->SetLabelSize(0.050);
    hntrace[f]->GetYaxis()->SetTitle("Entries");
    hntrace[f]->GetYaxis()->SetTitleSize(0.055);
    hntrace[f]->GetYaxis()->SetTitleOffset(1.25);
    hntrace[f]->GetYaxis()->CenterTitle();
    hntrace[f]->GetXaxis()->SetLabelOffset(0.010);
    hntrace[f]->GetXaxis()->SetLabelSize(0.050);
    hntrace[f]->GetXaxis()->SetTitle("Number Tracks / Event");
    hntrace[f]->GetXaxis()->SetTitleSize(0.055);
    hntrace[f]->GetXaxis()->SetTitleOffset(1.1);
    hntrace[f]->GetXaxis()->CenterTitle();
    hntrace[f]->SetMinimum(0.5);
    if (f == 0) hntrace[f]->SetLineColor(kBlack);
    if (f == 1) hntrace[f]->SetLineColor(kGreen+2);
    if (f == 2) hntrace[f]->SetLineColor(kMagenta-7);
    if (f == 3) hntrace[f]->SetLineColor(kViolet+8);
  }

  sprintf(ctext,"%s","QMD model");
  TLatex* tqmd = new TLatex(0.45,0.83,ctext);
  tqmd->SetNDC();
  tqmd->SetTextSize(0.030);
  tqmd->SetTextColor(kBlack);

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  gStyle->SetFrameFillColor(10);

  TPad* ptpad;
  char cpad[50];


  TCanvas *c1 = new TCanvas( "c1","tmult", 0, 0, 600, 400 );

  c1->Divide(1,1,0.001,0.001);
  for (int i=1; i<2; i++) {
    sprintf(cpad,"c1_%d",i);
    ptpad = (TPad*) c1->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c1->cd(i);
    switch(i) {
    case 1: {
              hntrace[3]->Draw(); 
              hntrace[2]->Draw("samehist"); 
              hntrace[1]->Draw("samehist");
              hntrace[0]->Draw("samehist"); 
              tqmd->Draw(); 
              TLegend *legend_c1 = new TLegend(0.67,0.5,0.82,0.8);
              legend_c1->AddEntry(hntrace[0],"3C","l");
              legend_c1->AddEntry(hntrace[1],"4C","l");
              legend_c1->AddEntry(hntrace[2],"3A","l");
              legend_c1->AddEntry(hntrace[3],"4A","l");
              legend_c1->Draw();
              break;
            }
    default: break;
    } 
  }

  return;

}

void figs_4pos_ELoss_cmp() 
{
  /*
  This function displays the energy lost by primary or/and secondary particles in the tracker or/and in the calo for the QMD hadronic model
  in the case of 4 different beam positions (the four around the center: 3C, 4C, 3A and 4A) and compares the results
  */

  char chist[180];
  char cchist[180];
  char ctext[180];
  char cfich[4][180];

  sprintf(cfich[0],"hepd_qmd_68MeV_proton_3C0_hist.root"); // 3C: -1.75 -1.8 cm (x y) 0 deg
  sprintf(cfich[1],"hepd_qmd_68MeV_proton_4C0_hist.root"); // 4C: -1.75 1.8 cm (x y) 0 deg
  sprintf(cfich[2],"hepd_qmd_68MeV_proton_3A0_hist.root"); // 3A: 1.75 -1.8 cm (x y) 0 deg
  sprintf(cfich[3],"hepd_qmd_68MeV_proton_4A0_hist.root"); // 4A: 1.75 1.8 cm (x y) 0 deg 

  TFile* fich[4];

  TH1D *hELossPrimaryEvent[4];
  TH1D *hELossSecondaryEvent[4];
  TH1D *hELossPrimaryTracker[4];
  TH1D *hELossSecondaryTracker[4];
  TH1D *hELossPrimaryCalo[4];
  TH1D *hELossSecondaryCalo[4];
  TH1D *hELossEvent[4];
  TH1D *hELossTracker[4];
  TH1D *hELossCalo[4];

  sprintf(ctext,"%s","QMD model");
  TLatex* tqmd = new TLatex(0.45,0.83,ctext);
  tqmd->SetNDC();
  tqmd->SetTextSize(0.030);
  tqmd->SetTextColor(kBlack);


  // ELoss Primary Event (2)
  for (int f=0; f<4; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossPrimaryEvent");
    sprintf(cchist,"hELossPrimaryEvent%d",f);
    hELossPrimaryEvent[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossPrimaryEvent[f]->SetTitle("Primary Events");
    hELossPrimaryEvent[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossPrimaryEvent[f]->GetYaxis()->SetLabelSize(0.050);
    hELossPrimaryEvent[f]->GetYaxis()->SetTitle("Entries");
    hELossPrimaryEvent[f]->GetYaxis()->SetTitleSize(0.055);
    hELossPrimaryEvent[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossPrimaryEvent[f]->GetYaxis()->CenterTitle();
    hELossPrimaryEvent[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossPrimaryEvent[f]->GetXaxis()->SetLabelSize(0.050);
    hELossPrimaryEvent[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossPrimaryEvent[f]->GetXaxis()->SetTitleSize(0.055);
    hELossPrimaryEvent[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossPrimaryEvent[f]->GetXaxis()->CenterTitle();
    hELossPrimaryEvent[f]->SetMinimum(0.5);
    if (f == 0) hELossPrimaryEvent[f]->SetLineColor(kRed+2);
    if (f == 1) hELossPrimaryEvent[f]->SetLineColor(kRed);
    if (f == 2) hELossPrimaryEvent[f]->SetLineColor(kOrange+2);
    if (f == 3) hELossPrimaryEvent[f]->SetLineColor(kOrange-2);
  }

  // ELoss Secondary Event (2)
  for (int f=0; f<4; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossSecondaryEvent");
    sprintf(cchist,"hELossSecondaryEvent%d",f);
    hELossSecondaryEvent[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossSecondaryEvent[f]->SetTitle("Secondary Events");
    hELossSecondaryEvent[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossSecondaryEvent[f]->GetYaxis()->SetLabelSize(0.050);
    hELossSecondaryEvent[f]->GetYaxis()->SetTitle("Entries");
    hELossSecondaryEvent[f]->GetYaxis()->SetTitleSize(0.055);
    hELossSecondaryEvent[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossSecondaryEvent[f]->GetYaxis()->CenterTitle();
    hELossSecondaryEvent[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossSecondaryEvent[f]->GetXaxis()->SetLabelSize(0.050);
    hELossSecondaryEvent[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossSecondaryEvent[f]->GetXaxis()->SetTitleSize(0.055);
    hELossSecondaryEvent[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossSecondaryEvent[f]->GetXaxis()->CenterTitle();
    hELossSecondaryEvent[f]->SetMinimum(0.5);
    if (f == 0) hELossSecondaryEvent[f]->SetLineColor(kBlue);
    if (f == 1) hELossSecondaryEvent[f]->SetLineColor(kBlue-7);
    if (f == 2) hELossSecondaryEvent[f]->SetLineColor(kCyan+2);
    if (f == 3) hELossSecondaryEvent[f]->SetLineColor(kCyan-7);
  }

  // ELoss Event (2)
  for (int f=0; f<4; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossEvent");
    sprintf(cchist,"hELossEvent%d",f);
    hELossEvent[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossEvent[f]->SetTitle("Events");
    hELossEvent[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossEvent[f]->GetYaxis()->SetLabelSize(0.050);
    hELossEvent[f]->GetYaxis()->SetTitle("Entries");
    hELossEvent[f]->GetYaxis()->SetTitleSize(0.055);
    hELossEvent[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossEvent[f]->GetYaxis()->CenterTitle();
    hELossEvent[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossEvent[f]->GetXaxis()->SetLabelSize(0.050);
    hELossEvent[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossEvent[f]->GetXaxis()->SetTitleSize(0.055);
    hELossEvent[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossEvent[f]->GetXaxis()->CenterTitle();
    hELossEvent[f]->SetMinimum(0.5);
    if (f == 0) hELossEvent[f]->SetLineColor(kBlack);
    if (f == 1) hELossEvent[f]->SetLineColor(kGreen+2);
    if (f == 2) hELossEvent[f]->SetLineColor(kMagenta-7);
    if (f == 3) hELossEvent[f]->SetLineColor(kViolet+8);
  }


  // ELoss Primary Tracker (3)
  for (int f=0; f<4; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossPrimaryTracker");
    sprintf(cchist,"hELossPrimaryTracker%d",f);
    hELossPrimaryTracker[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossPrimaryTracker[f]->SetTitle("Primary Tracker Events");
    hELossPrimaryTracker[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossPrimaryTracker[f]->GetYaxis()->SetLabelSize(0.050);
    hELossPrimaryTracker[f]->GetYaxis()->SetTitle("Entries");
    hELossPrimaryTracker[f]->GetYaxis()->SetTitleSize(0.055);
    hELossPrimaryTracker[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossPrimaryTracker[f]->GetYaxis()->CenterTitle();
    hELossPrimaryTracker[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossPrimaryTracker[f]->GetXaxis()->SetLabelSize(0.050);
    hELossPrimaryTracker[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossPrimaryTracker[f]->GetXaxis()->SetTitleSize(0.055);
    hELossPrimaryTracker[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossPrimaryTracker[f]->GetXaxis()->CenterTitle();
    hELossPrimaryTracker[f]->SetMinimum(0.5);
    if (f == 0) hELossPrimaryTracker[f]->SetLineColor(kRed+2);
    if (f == 1) hELossPrimaryTracker[f]->SetLineColor(kRed);
    if (f == 2) hELossPrimaryTracker[f]->SetLineColor(kOrange+2);
    if (f == 3) hELossPrimaryTracker[f]->SetLineColor(kOrange-2);
  }

  // ELoss Secondary Tracker (3)
  for (int f=0; f<4; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossSecondaryTracker");
    sprintf(cchist,"hELossSecondaryTracker%d",f);
    hELossSecondaryTracker[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossSecondaryTracker[f]->SetTitle("Secondary Traker Events");
    hELossSecondaryTracker[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossSecondaryTracker[f]->GetYaxis()->SetLabelSize(0.050);
    hELossSecondaryTracker[f]->GetYaxis()->SetTitle("Entries");
    hELossSecondaryTracker[f]->GetYaxis()->SetTitleSize(0.055);
    hELossSecondaryTracker[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossSecondaryTracker[f]->GetYaxis()->CenterTitle();
    hELossSecondaryTracker[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossSecondaryTracker[f]->GetXaxis()->SetLabelSize(0.050);
    hELossSecondaryTracker[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossSecondaryTracker[f]->GetXaxis()->SetTitleSize(0.055);
    hELossSecondaryTracker[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossSecondaryTracker[f]->GetXaxis()->CenterTitle();
    hELossSecondaryTracker[f]->SetMinimum(0.5);
    if (f == 0) hELossSecondaryTracker[f]->SetLineColor(kBlue);
    if (f == 1) hELossSecondaryTracker[f]->SetLineColor(kBlue-7);
    if (f == 2) hELossSecondaryTracker[f]->SetLineColor(kCyan+2);
    if (f == 3) hELossSecondaryTracker[f]->SetLineColor(kCyan-7);
  }

  // ELoss Primary Calo (3)
  for (int f=0; f<4; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossPrimaryCalo");
    sprintf(cchist,"hELossPrimaryCalo%d",f);
    hELossPrimaryCalo[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossPrimaryCalo[f]->SetTitle("Primary Calo Events");
    hELossPrimaryCalo[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossPrimaryCalo[f]->GetYaxis()->SetLabelSize(0.050);
    hELossPrimaryCalo[f]->GetYaxis()->SetTitle("Entries");
    hELossPrimaryCalo[f]->GetYaxis()->SetTitleSize(0.055);
    hELossPrimaryCalo[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossPrimaryCalo[f]->GetYaxis()->CenterTitle();
    hELossPrimaryCalo[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossPrimaryCalo[f]->GetXaxis()->SetLabelSize(0.050);
    hELossPrimaryCalo[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossPrimaryCalo[f]->GetXaxis()->SetTitleSize(0.055);
    hELossPrimaryCalo[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossPrimaryCalo[f]->GetXaxis()->CenterTitle();
    hELossPrimaryCalo[f]->SetMinimum(0.5);
    if (f == 0) hELossPrimaryCalo[f]->SetLineColor(kRed+2);
    if (f == 1) hELossPrimaryCalo[f]->SetLineColor(kRed);
    if (f == 2) hELossPrimaryCalo[f]->SetLineColor(kOrange+2);
    if (f == 3) hELossPrimaryCalo[f]->SetLineColor(kOrange-2);
  }

  // ELoss Secondary Calo (3)
  for (int f=0; f<4; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossSecondaryCalo");
    sprintf(cchist,"hELossSecondaryCalo%d",f);
    hELossSecondaryCalo[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossSecondaryCalo[f]->SetTitle("Secondary Calo Events");
    hELossSecondaryCalo[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossSecondaryCalo[f]->GetYaxis()->SetLabelSize(0.050);
    hELossSecondaryCalo[f]->GetYaxis()->SetTitle("Entries");
    hELossSecondaryCalo[f]->GetYaxis()->SetTitleSize(0.055);
    hELossSecondaryCalo[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossSecondaryCalo[f]->GetYaxis()->CenterTitle();
    hELossSecondaryCalo[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossSecondaryCalo[f]->GetXaxis()->SetLabelSize(0.050);
    hELossSecondaryCalo[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossSecondaryCalo[f]->GetXaxis()->SetTitleSize(0.055);
    hELossSecondaryCalo[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossSecondaryCalo[f]->GetXaxis()->CenterTitle();
    hELossSecondaryCalo[f]->SetMinimum(0.5);
    if (f == 0) hELossSecondaryCalo[f]->SetLineColor(kBlue);
    if (f == 1) hELossSecondaryCalo[f]->SetLineColor(kBlue-7);
    if (f == 2) hELossSecondaryCalo[f]->SetLineColor(kCyan+2);
    if (f == 3) hELossSecondaryCalo[f]->SetLineColor(kCyan-7);
  }


  // ELoss Tracker (4)
  for (int f=0; f<4; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossTracker");
    sprintf(cchist,"hELossTracker%d",f);
    hELossTracker[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossTracker[f]->SetTitle("Traker Events");
    hELossTracker[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossTracker[f]->GetYaxis()->SetLabelSize(0.050);
    hELossTracker[f]->GetYaxis()->SetTitle("Entries");
    hELossTracker[f]->GetYaxis()->SetTitleSize(0.055);
    hELossTracker[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossTracker[f]->GetYaxis()->CenterTitle();
    hELossTracker[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossTracker[f]->GetXaxis()->SetLabelSize(0.050);
    hELossTracker[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossTracker[f]->GetXaxis()->SetTitleSize(0.055);
    hELossTracker[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossTracker[f]->GetXaxis()->CenterTitle();
    hELossTracker[f]->SetMinimum(0.5);
    if (f == 0) hELossTracker[f]->SetLineColor(kBlack);
    if (f == 1) hELossTracker[f]->SetLineColor(kGreen+2);
    if (f == 2) hELossTracker[f]->SetLineColor(kMagenta-7);
    if (f == 3) hELossTracker[f]->SetLineColor(kViolet+8);
  }

  // ELoss Calo (4)
  for (int f=0; f<4; f++) {
    fich[f] = new TFile(cfich[f]);
    sprintf(chist,"hELossCalo");
    sprintf(cchist,"hELossCalo%d",f);
    hELossCalo[f] = (TH1D*) fich[f]->Get(chist)->Clone(cchist);
    hELossCalo[f]->SetTitle("Calo Events");
    hELossCalo[f]->GetYaxis()->SetLabelOffset(0.005);
    hELossCalo[f]->GetYaxis()->SetLabelSize(0.050);
    hELossCalo[f]->GetYaxis()->SetTitle("Entries");
    hELossCalo[f]->GetYaxis()->SetTitleSize(0.055);
    hELossCalo[f]->GetYaxis()->SetTitleOffset(1.25);
    hELossCalo[f]->GetYaxis()->CenterTitle();
    hELossCalo[f]->GetXaxis()->SetLabelOffset(0.010);
    hELossCalo[f]->GetXaxis()->SetLabelSize(0.050);
    hELossCalo[f]->GetXaxis()->SetTitle("Energy Loss [MeV]");
    hELossCalo[f]->GetXaxis()->SetTitleSize(0.055);
    hELossCalo[f]->GetXaxis()->SetTitleOffset(1.1);
    hELossCalo[f]->GetXaxis()->CenterTitle();
    hELossCalo[f]->SetMinimum(0.5);
    if (f == 0) hELossCalo[f]->SetLineColor(kBlack);
    if (f == 1) hELossCalo[f]->SetLineColor(kGreen+2);
    if (f == 2) hELossCalo[f]->SetLineColor(kMagenta-7);
    if (f == 3) hELossCalo[f]->SetLineColor(kViolet+8);
  }


  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  gStyle->SetFrameFillColor(10);

  TPad* ptpad;
  char cpad[50];


  TCanvas *c2 = new TCanvas( "c2","hELossEvent Primary/Secondary + Sum", 0, 0, 1200, 600 );

  c2->Divide(3,1,0.001,0.001);
  for (int i=1; i<4; i++) {
    sprintf(cpad,"c2_%d",i);
    ptpad = (TPad*) c2->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c2->cd(i);
    switch(i) {
    case 1: {
              hELossPrimaryEvent[0]->Draw(); 
              hELossPrimaryEvent[1]->Draw("samehist"); 
              hELossPrimaryEvent[2]->Draw("samehist"); 
              hELossPrimaryEvent[3]->Draw("samehist");
              tqmd->Draw(); 
              TLegend *legend_c2_1 = new TLegend(0.22,0.55,0.37,0.80);
              legend_c2_1->AddEntry(hELossPrimaryEvent[0],"3C","l");
              legend_c2_1->AddEntry(hELossPrimaryEvent[1],"4C","l");
              legend_c2_1->AddEntry(hELossPrimaryEvent[2],"3A","l");
              legend_c2_1->AddEntry(hELossPrimaryEvent[3],"4A","l");
              legend_c2_1->Draw();
              break;
            }
    case 2: {
              hELossSecondaryEvent[0]->Draw(); 
              hELossSecondaryEvent[1]->Draw("samehist"); 
              hELossSecondaryEvent[2]->Draw("samehist"); 
              hELossSecondaryEvent[3]->Draw("samehist");
              tqmd->Draw(); 
              TLegend *legend_c2_2 = new TLegend(0.63,0.55,0.78,0.80);
              legend_c2_2->AddEntry(hELossSecondaryEvent[0],"3C","l");
              legend_c2_2->AddEntry(hELossSecondaryEvent[1],"4C","l");
              legend_c2_2->AddEntry(hELossSecondaryEvent[2],"3A","l");
              legend_c2_2->AddEntry(hELossSecondaryEvent[3],"4A","l");
              legend_c2_2->Draw();
              break;
            }
    case 3: {
              hELossEvent[0]->Draw(); 
              hELossEvent[1]->Draw("samehist"); 
              hELossEvent[2]->Draw("samehist"); 
              hELossEvent[3]->Draw("samehist");
              tqmd->Draw(); 
              TLegend *legend_c2_3 = new TLegend(0.22,0.55,0.37,0.80);
              legend_c2_3->AddEntry(hELossEvent[0],"3C","l");
              legend_c2_3->AddEntry(hELossEvent[1],"4C","l");
              legend_c2_3->AddEntry(hELossEvent[2],"3A","l");
              legend_c2_3->AddEntry(hELossEvent[3],"4A","l");
              legend_c2_3->Draw();
              break;
            }
    default: break;
    } 
  }

  TCanvas *c3 = new TCanvas( "c3","hELossPrimaryTracker/Calo - hELossSecondaryTracker/Calo", 0, 0, 1200, 800 );

  c3->Divide(2,2,0.001,0.001);
  for (int i=1; i<5; i++) {
    sprintf(cpad,"c3_%d",i);
    ptpad = (TPad*) c3->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c3->cd(i);
    switch(i) {
    case 1: {
              hELossPrimaryTracker[0]->Draw(); 
              hELossPrimaryTracker[1]->Draw("samehist"); 
              hELossPrimaryTracker[2]->Draw("samehist");
              hELossPrimaryTracker[3]->Draw("samehist");
              tqmd->Draw();
              TLegend *legend_c3_1 = new TLegend(0.65,0.55,0.80,0.80);
              legend_c3_1->AddEntry(hELossPrimaryTracker[0],"3C","l");
              legend_c3_1->AddEntry(hELossPrimaryTracker[1],"4C","l");
              legend_c3_1->AddEntry(hELossPrimaryTracker[2],"3A","l");
              legend_c3_1->AddEntry(hELossPrimaryTracker[3],"4A","l");
              legend_c3_1->Draw();
              break;
            }
    case 2: {
              hELossSecondaryTracker[0]->Draw(); 
              hELossSecondaryTracker[1]->Draw("samehist"); 
              hELossSecondaryTracker[2]->Draw("samehist"); 
              hELossSecondaryTracker[3]->Draw("samehist");
              tqmd->Draw(); 
              TLegend *legend_c3_2 = new TLegend(0.65,0.55,0.80,0.80);
              legend_c3_2->AddEntry(hELossSecondaryTracker[0],"3C","l");
              legend_c3_2->AddEntry(hELossSecondaryTracker[1],"4C","l");
              legend_c3_2->AddEntry(hELossSecondaryTracker[2],"3A","l");
              legend_c3_2->AddEntry(hELossSecondaryTracker[3],"4A","l");
              legend_c3_2->Draw();
              break;
            }
    case 3: {
              hELossPrimaryCalo[0]->Draw(); 
              hELossPrimaryCalo[1]->Draw("samehist"); 
              hELossPrimaryCalo[2]->Draw("samehist");
              hELossPrimaryCalo[3]->Draw("samehist"); 
              tqmd->Draw();
              TLegend *legend_c3_3 = new TLegend(0.20,0.55,0.35,0.80);
              legend_c3_3->AddEntry(hELossPrimaryCalo[0],"3C","l");
              legend_c3_3->AddEntry(hELossPrimaryCalo[1],"4C","l");
              legend_c3_3->AddEntry(hELossPrimaryCalo[2],"3A","l");
              legend_c3_3->AddEntry(hELossPrimaryCalo[3],"4A","l");
              legend_c3_3->Draw();
              break;
            }
    case 4: {
              hELossSecondaryCalo[0]->Draw(); 
              hELossSecondaryCalo[1]->Draw("samehist"); 
              hELossSecondaryCalo[2]->Draw("samehist");
              hELossSecondaryCalo[3]->Draw("samehist"); 
              tqmd->Draw();
              TLegend *legend_c3_4 = new TLegend(0.65,0.55,0.80,0.80);
              legend_c3_4->AddEntry(hELossSecondaryCalo[0],"3C","l");
              legend_c3_4->AddEntry(hELossSecondaryCalo[1],"4C","l");
              legend_c3_4->AddEntry(hELossSecondaryCalo[2],"3A","l");
              legend_c3_4->AddEntry(hELossSecondaryCalo[3],"4A","l");
              legend_c3_4->Draw();
              break;
            }
    default: break;
    } 
  }

  TCanvas *c4 = new TCanvas( "c4","hELossTracker - hELossCalo", 0, 0, 1200, 600 );

  c4->Divide(2,1,0.001,0.001);
  for (int i=1; i<3; i++) {
    sprintf(cpad,"c4_%d",i);
    ptpad = (TPad*) c4->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c4->cd(i);
    switch(i) {
    case 1: {
              hELossTracker[0]->Draw(); 
              hELossTracker[1]->Draw("samehist"); 
              hELossTracker[2]->Draw("samehist"); 
              hELossTracker[3]->Draw("samehist"); 
              tqmd->Draw();
              TLegend *legend_c4_1 = new TLegend(0.65,0.55,0.80,0.80);
              legend_c4_1->AddEntry(hELossTracker[0],"3C","l");
              legend_c4_1->AddEntry(hELossTracker[1],"4C","l");
              legend_c4_1->AddEntry(hELossTracker[2],"3A","l");
              legend_c4_1->AddEntry(hELossTracker[3],"4A","l");
              legend_c4_1->Draw();
              break;
            }
    case 2: {
              hELossCalo[0]->Draw(); 
              hELossCalo[1]->Draw("samehist"); 
              hELossCalo[2]->Draw("samehist");
              hELossCalo[3]->Draw("samehist"); 
              tqmd->Draw();
              TLegend *legend_c4_2 = new TLegend(0.20,0.55,0.35,0.80);
              legend_c4_2->AddEntry(hELossCalo[0],"3C","l");
              legend_c4_2->AddEntry(hELossCalo[1],"4C","l");
              legend_c4_2->AddEntry(hELossCalo[2],"3A","l");
              legend_c4_2->AddEntry(hELossCalo[3],"4A","l");
              legend_c4_2->Draw();
              break;
            }
    default: break;
    } 
  }
  
  return;

}

void figs() 
{
  /*
  This function displays the track multiplicity, the charge of secondary nuclei and the kinetic energy distribution of
  secondary nuclei
  */

  char cfich[180];
  char chist[180];
  char cchist[180];
  char ctext[180];

  char c_z1[n_z1][20] = { "p", "deuteron", "triton" };
  char c_z2[n_z2][20] = { "He3", "alpha", "He6" };
  char c_z3[n_z3][20] = { "Li3", "Li4", "Li5", "Li6", "Li7", "Li8", "Li9" };
  char c_z4[n_z4][20] = { "Be6", "Be7", "Be8", "Be9", "Be10", "Be11", "Be12" };
  char c_z5[n_z5][20] = { "B8", "B9", "B10", "B11", "B12", "B13" };
  char c_z6[n_z6][20] = { "C9", "C10", "C11", "C12", "C13", "C14", "C15" };
  char c_z7[n_z7][20] = { "N11", "N12", "N13", "N14", "N15", "N16", "N17", "N18" };
  char c_z8[n_z8][20] = { "O13", "O14", "O15", "O16", "O17", "O18", "O19" };
  char c_z9[n_z9][20] = { "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23" };
  char c_z10[n_z10][20] = { "Ne20", "Ne21", "Ne22" };
  char c_z11[n_z11][20] = { "Na21", "Na22", "Na23", "Na24", "Na25" };
  char c_z12[n_z12][20] = { "Mg23", "Mg24", "Mg25", "Mg26" };
  char c_z13[n_z13][20] = { "Al25", "Al26", "Al27", "Al28" };
  char c_z14[n_z14][20] = { "Si27", "Si28", "Si29", "Si30", "Si31" };
  char c_z15[n_z15][20] = { "P28" };
  char c_z29[n_z29][20] = { "Cu63" };
  char c_z34[n_z34][20] = { "Se77" };
  char c_z36[n_z36][20] = { "Kr79" };
  char c_z37[n_z37][20] = { "Rb81", "Rb82", "Rb83", "Rb84", "Rb85", "Rb86" };
  char c_z38[n_z38][20] = { "Sr82", "Sr83", "Sr84", "Sr85", "Sr86", "Sr87", "Sr88" };
  char c_z39[n_z39][20] = { "Y84", "Y85", "Y86", "Y87", "Y88", "Y89", "Y90" };
  char c_z40[n_z40][20] = { "Zr84", "Zr85", "Zr86", "Zr87", "Zr88" };
  char c_z69[n_z69][20] = { "Tm162", "Tm163", "Tm164", "Tm165", "Tm166", "Tm167", "Tm168", "Tm169", "Tm170", "Tm171", "Tm172", "Tm173" };
  char c_z70[n_z70][20] = { "Yb163", "Yb164", "Yb165", "Yb166", "Yb167", "Yb168", "Yb169", "Yb170", "Yb171", "Yb172", "Yb173", "Yb174" };
  char c_z71[n_z71][20] = { "Lu163", "Lu164", "Lu165", "Lu166", "Lu167", "Lu168", "Lu169", "Lu170", "Lu171", "Lu172", "Lu173", "Lu174", "Lu175", "Lu176", "Lu177" };
  char c_z72[n_z72][20] = { "Hf163", "Hf164", "Hf165", "Hf166", "Hf167", "Hf168", "Hf169", "Hf170", "Hf171", "Hf172", "Hf173", "Hf174", "Hf175", "Hf176" };


  //sprintf(cfich,"hepd_qmd_68MeV_proton_centre_hist.root");  // centre: 0 0 cm (x y) 0 deg
  //sprintf(cfich,"hepd_incl_68MeV_proton_centre_hist.root");
  //sprintf(cfich,"hepd_bic_68MeV_proton_centre_hist.root");

  //sprintf(cfich,"hepd_qmd_68MeV_proton_550_hist.root"); // 550: 5 5 cm (x y) 0 deg
  //sprintf(cfich,"hepd_incl_68MeV_proton_550_hist.root");
  //sprintf(cfich,"hepd_bic_68MeV_proton_550_hist.root");

  //sprintf(cfich,"hepd_qmd_68MeV_proton_3C0_hist.root"); // 3C: -1.75 -1.8 cm (x y) 0 deg
  //sprintf(cfich,"hepd_qmd_68MeV_proton_4C0_hist.root"); // 4C: -1.75 1.8 cm (x y) 0 deg
  //sprintf(cfich,"hepd_qmd_68MeV_proton_3A0_hist.root"); // 3A: 1.75 -1.8 cm (x y) 0 deg
  //sprintf(cfich,"hepd_qmd_68MeV_proton_4A0_hist.root"); // 4A: 1.75 1.8 cm (x y) 0 deg

  //sprintf(cfich,"hepd_qmd_68MeV_proton_TEST3C0_hist.root"); // 3C: -1.75 -1.8 cm (x y) 0 deg
  //sprintf(cfich,"hepd_qmd_202MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_36MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_50MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_100MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_125MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_154MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_174MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_228MeV_proton_TEST3C0_hist.root");

  sprintf(cfich,"hepd_qmd_32MeV_proton_3C0_tracks_hist.root");


  printf(" cfich %s\n",cfich);
  TFile *f = new TFile(cfich);

  f->ls();

  // hntrace hist (1)
  sprintf(chist,"hntrace");
  TH1D *hntrace = (TH1D*) f->Get(chist)->Clone();
  hntrace->GetYaxis()->SetLabelOffset(0.005);
  hntrace->GetYaxis()->SetLabelSize(0.050);
  hntrace->GetYaxis()->SetTitle("Entries");
  hntrace->GetYaxis()->SetTitleSize(0.055);
  hntrace->GetYaxis()->SetTitleOffset(1.25);
  hntrace->GetYaxis()->CenterTitle();
  hntrace->GetXaxis()->SetLabelOffset(0.010);
  hntrace->GetXaxis()->SetLabelSize(0.050);
  hntrace->GetXaxis()->SetTitle("Number Tracks / Event");
  hntrace->GetXaxis()->SetTitleSize(0.055);
  hntrace->GetXaxis()->SetTitleOffset(1.1);
  hntrace->GetXaxis()->CenterTitle();
  hntrace->SetMinimum(0.5);
  hntrace->SetLineColor(kBlack);

  TAxis *xaxis = hntrace->GetXaxis();
  Double_t nent = hntrace->GetEntries();
  Double_t mult1 = 0;
  Int_t nbinsx = hntrace->GetNbinsX();
  Double_t tot = 0;
  for (int b=1; b<=nbinsx; b++) {
    Double_t binCenter = xaxis->GetBinCenter(b);
    if (binCenter == 1) mult1=hntrace->GetBinContent(b);
    tot += hntrace->GetBinContent(b);    
    printf("bin %d mult %2.0lf nombre %6.0lf \n",b,binCenter,hntrace->GetBinContent(b)); 
  } 
  printf("tot %5.0lf \n",tot);
  printf("frac 1 %5.4lf \n",mult1/nent);

  sprintf(ctext,"frac of mult >1: %7.3lf",(1-mult1/nent));
  TLatex* tfrac_gt1 = new TLatex(0.300,0.735,ctext);
  tfrac_gt1->SetNDC();
  tfrac_gt1->SetTextSize(0.0500);

  // charge sec hist (1)
  sprintf(chist,"hcsec");
  TH1D *hcsec = (TH1D*) f->Get(chist)->Clone();
  hcsec->GetYaxis()->SetLabelOffset(0.005);
  hcsec->GetYaxis()->SetLabelSize(0.050);
  hcsec->GetYaxis()->SetTitle("Entries");
  hcsec->GetYaxis()->SetTitleSize(0.055);
  hcsec->GetYaxis()->SetTitleOffset(1.25);
  hcsec->GetYaxis()->CenterTitle();
  hcsec->GetXaxis()->SetLabelOffset(0.010);
  hcsec->GetXaxis()->SetLabelSize(0.050);
  hcsec->GetXaxis()->SetTitle("Charge of Secondary Nuclei");
  hcsec->GetXaxis()->SetTitleSize(0.055);
  hcsec->GetXaxis()->SetTitleOffset(1.1);
  hcsec->GetXaxis()->CenterTitle();
  hcsec->SetMinimum(0.5);
  hcsec->SetLineColor(kBlack);


  // ce charge sec hist (2)
  Int_t couleur[28] = {kBlack,kBlue+1,kBlue-6,kCyan,kCyan-9,kGreen+3,kGreen+1,kGreen-9,kYellow+1,kOrange,kOrange-3,kOrange+9,kRed-4,kViolet+1,kMagenta-7,kMagenta-10,kMagenta-5,kCyan-9,kGreen+3,kGreen+1,kGreen-9,kYellow+1,kOrange,kOrange-3,kOrange+9,kRed-4,kBlack,kBlack};

  TH1D* hcesec[27];
  for (int i=0; i<27; i++) {
    sprintf(chist,"hcesec%d",i+1);
    hcesec[i] = (TH1D*) f->Get(chist)->Clone();
    hcesec[i]->GetYaxis()->SetLabelOffset(0.005);
    hcesec[i]->GetYaxis()->SetLabelSize(0.050);
    hcesec[i]->GetYaxis()->SetTitle("Entries");
    hcesec[i]->GetYaxis()->SetTitleSize(0.055);
    hcesec[i]->GetYaxis()->SetTitleOffset(1.00);
    hcesec[i]->GetYaxis()->CenterTitle();
    hcesec[i]->GetXaxis()->SetLabelOffset(0.010);
    hcesec[i]->GetXaxis()->SetLabelSize(0.050);
    hcesec[i]->GetXaxis()->SetTitle("Kinetic Energy (MeV)");
    hcesec[i]->GetXaxis()->SetTitleSize(0.055);
    hcesec[i]->GetXaxis()->SetTitleOffset(1.1);
    hcesec[i]->GetXaxis()->CenterTitle();
    hcesec[i]->SetLineColor(couleur[i]);   
    hcesec[i]->Draw();
  }

  
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  gStyle->SetFrameFillColor(10);

  TPad* ptpad;
  char cpad[50];


  TCanvas *c1 = new TCanvas( "c1","ntrace - charge sec", 0, 0, 800, 400 );

  c1->Divide(2,1,0.001,0.001);
  for (int i=1; i<3; i++) {
    sprintf(cpad,"c1_%d",i);
    ptpad = (TPad*) c1->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c1->cd(i);
    switch(i) {
    case 1: {
              hntrace->Draw();
              tfrac_gt1->Draw();
              break;
            }
    case 2: {
              hcsec->Draw(); 
              break;
            }
    default: break;
    } 
  }

  TCanvas *c2 = new TCanvas( "c2","ce charge sec", 0, 0, 1200, 600 );

  c2->Divide(2,1,0.001,0.001);
  for (int i=1; i<3; i++) {
    sprintf(cpad,"c2_%d",i);
    ptpad = (TPad*) c2->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c2->cd(i);
    switch(i) {
    case 1: {
              hcesec[5]->Draw(); 
              hcesec[0]->Draw("samehist"); 
              hcesec[1]->Draw("samehist"); 
              hcesec[2]->Draw("samehist"); 
              hcesec[3]->Draw("samehist"); 
              hcesec[4]->Draw("samehist"); 
              hcesec[6]->Draw("samehist"); 
              hcesec[7]->Draw("samehist");
              hcesec[8]->Draw("samehist");
              hcesec[9]->Draw("samehist");
              hcesec[10]->Draw("samehist");
              hcesec[11]->Draw("samehist");
              hcesec[12]->Draw("samehist");
              hcesec[13]->Draw("samehist");
              hcesec[14]->Draw("samehist"); 
              TLegend *legend_c2_1 = new TLegend(0.73,0.20,0.83,0.85);
              legend_c2_1->AddEntry(hcesec[0],"p,d,t","l");
              legend_c2_1->AddEntry(hcesec[1],"He","l");
              legend_c2_1->AddEntry(hcesec[2],"Li","l");
              legend_c2_1->AddEntry(hcesec[3],"Be","l");
              legend_c2_1->AddEntry(hcesec[4],"B","l");
              legend_c2_1->AddEntry(hcesec[5],"C","l");
              legend_c2_1->AddEntry(hcesec[6],"N","l");
              legend_c2_1->AddEntry(hcesec[7],"O","l");
              legend_c2_1->AddEntry(hcesec[8],"F","l");
              legend_c2_1->AddEntry(hcesec[9],"Ne","l");
              legend_c2_1->AddEntry(hcesec[10],"Na","l");
              legend_c2_1->AddEntry(hcesec[11],"Mg","l");
              legend_c2_1->AddEntry(hcesec[12],"Al","l");
              legend_c2_1->AddEntry(hcesec[13],"Si","l");
              legend_c2_1->AddEntry(hcesec[14],"P","l");
              legend_c2_1->Draw();
              break;
            }
    case 2: {
              hcesec[26]->Draw();
              hcesec[15]->Draw("samehist");
              hcesec[16]->Draw("samehist");
              hcesec[17]->Draw("samehist");
              hcesec[18]->Draw("samehist");
              hcesec[19]->Draw("samehist");
              hcesec[20]->Draw("samehist");
              hcesec[21]->Draw("samehist");
              hcesec[22]->Draw("samehist");
              hcesec[23]->Draw("samehist");
              hcesec[24]->Draw("samehist");
              hcesec[25]->Draw("samehist");
              TLegend *legend_c2_2 = new TLegend(0.73,0.20,0.83,0.85);
              legend_c2_2->AddEntry(hcesec[26],"n","l");
              legend_c2_2->AddEntry(hcesec[15],"Cu","l");
              legend_c2_2->AddEntry(hcesec[16],"Se","l");
              legend_c2_2->AddEntry(hcesec[17],"Kr","l");
              legend_c2_2->AddEntry(hcesec[18],"Rb","l");
              legend_c2_2->AddEntry(hcesec[19],"Sr","l");
              legend_c2_2->AddEntry(hcesec[20],"Y","l");
              legend_c2_2->AddEntry(hcesec[21],"Zr","l");
              legend_c2_2->AddEntry(hcesec[22],"Tm","l");
              legend_c2_2->AddEntry(hcesec[23],"Yb","l");
              legend_c2_2->AddEntry(hcesec[24],"Lu","l");
              legend_c2_2->AddEntry(hcesec[25],"Hf","l");
              legend_c2_2->Draw();
              break;
            }
    default: break;
    } 
  }

  return;

}

void figs_ELoss() 
{
  /*
  This function displays the energy lost by primary or/and secondary particles in the tracker or/and in the calo
  */

  char cfich[180];
  char chist[180];
  char cchist[180];
  char ctext[180];

  //sprintf(cfich,"hepd_qmd_68MeV_proton_centre_hist.root");  // centre: 0 0 cm (x y) 0 deg
  //sprintf(cfich,"hepd_incl_68MeV_proton_centre_hist.root");
  //sprintf(cfich,"hepd_bic_68MeV_proton_centre_hist.root");

  //sprintf(cfich,"hepd_qmd_68MeV_proton_550_hist.root"); // 550: 5 5 cm (x y) 0 deg
  //sprintf(cfich,"hepd_incl_68MeV_proton_550_hist.root");
  //sprintf(cfich,"hepd_bic_68MeV_proton_550_hist.root");

  //sprintf(cfich,"hepd_qmd_68MeV_proton_3C0_hist.root"); // 3C: -1.75 -1.8 cm (x y) 0 deg
  //sprintf(cfich,"hepd_qmd_68MeV_proton_4C0_hist.root"); // 4C: -1.75 1.8 cm (x y) 0 deg
  //sprintf(cfich,"hepd_qmd_68MeV_proton_3A0_hist.root"); // 3A: 1.75 -1.8 cm (x y) 0 deg
  //sprintf(cfich,"hepd_qmd_68MeV_proton_4A0_hist.root"); // 4A: 1.75 1.8 cm (x y) 0 deg

  sprintf(cfich,"hepd_qmd_68MeV_proton_TEST3C0_hist.root"); // 3C: -1.75 -1.8 cm (x y) 0 deg
  //sprintf(cfich,"hepd_qmd_202MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_36MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_50MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_100MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_125MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_154MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_174MeV_proton_TEST3C0_hist.root");
  //sprintf(cfich,"hepd_qmd_228MeV_proton_TEST3C0_hist.root");


  printf(" cfich %s\n",cfich);
  TFile *f = new TFile(cfich);

  f->ls();


  // hELossPrimaryEvent (3)
  sprintf(chist,"hELossPrimaryEvent");
  TH1D *hELossPrimaryEvent = (TH1D*) f->Get(chist)->Clone();
  hELossPrimaryEvent->GetYaxis()->SetLabelOffset(0.005);
  hELossPrimaryEvent->GetYaxis()->SetLabelSize(0.050);
  hELossPrimaryEvent->GetYaxis()->SetTitle("Entries");
  hELossPrimaryEvent->GetYaxis()->SetTitleSize(0.055);
  hELossPrimaryEvent->GetYaxis()->SetTitleOffset(1.25);
  hELossPrimaryEvent->GetYaxis()->CenterTitle();
  hELossPrimaryEvent->GetXaxis()->SetLabelOffset(0.010);
  hELossPrimaryEvent->GetXaxis()->SetLabelSize(0.050);
  hELossPrimaryEvent->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossPrimaryEvent->GetXaxis()->SetTitleSize(0.055);
  hELossPrimaryEvent->GetXaxis()->SetTitleOffset(1.1);
  hELossPrimaryEvent->GetXaxis()->CenterTitle();
  hELossPrimaryEvent->SetMinimum(0.5);
  hELossPrimaryEvent->SetLineColor(kRed);

  // hELossSecondaryEvent (3)
  sprintf(chist,"hELossSecondaryEvent");
  TH1D *hELossSecondaryEvent = (TH1D*) f->Get(chist)->Clone();
  hELossSecondaryEvent->GetYaxis()->SetLabelOffset(0.005);
  hELossSecondaryEvent->GetYaxis()->SetLabelSize(0.050);
  hELossSecondaryEvent->GetYaxis()->SetTitle("Entries");
  hELossSecondaryEvent->GetYaxis()->SetTitleSize(0.055);
  hELossSecondaryEvent->GetYaxis()->SetTitleOffset(1.25);
  hELossSecondaryEvent->GetYaxis()->CenterTitle();
  hELossSecondaryEvent->GetXaxis()->SetLabelOffset(0.010);
  hELossSecondaryEvent->GetXaxis()->SetLabelSize(0.050);
  hELossSecondaryEvent->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossSecondaryEvent->GetXaxis()->SetTitleSize(0.055);
  hELossSecondaryEvent->GetXaxis()->SetTitleOffset(1.1);
  hELossSecondaryEvent->GetXaxis()->CenterTitle();
  hELossSecondaryEvent->SetMinimum(0.5);
  hELossSecondaryEvent->SetLineColor(kBlue);

  // hELossEvent (3)
  sprintf(chist,"hELossEvent");
  TH1D *hELossEvent = (TH1D*) f->Get(chist)->Clone();
  hELossEvent->GetYaxis()->SetLabelOffset(0.005);
  hELossEvent->GetYaxis()->SetLabelSize(0.050);
  hELossEvent->GetYaxis()->SetTitle("Entries");
  hELossEvent->GetYaxis()->SetTitleSize(0.055);
  hELossEvent->GetYaxis()->SetTitleOffset(1.25);
  hELossEvent->GetYaxis()->CenterTitle();
  hELossEvent->GetXaxis()->SetLabelOffset(0.010);
  hELossEvent->GetXaxis()->SetLabelSize(0.050);
  hELossEvent->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossEvent->GetXaxis()->SetTitleSize(0.055);
  hELossEvent->GetXaxis()->SetTitleOffset(1.1);
  hELossEvent->GetXaxis()->CenterTitle();
  hELossEvent->SetMinimum(0.5);
  hELossEvent->SetLineColor(kBlack);


  // hELossPrimaryTracker (4)
  sprintf(chist,"hELossPrimaryTracker");
  TH1D *hELossPrimaryTracker = (TH1D*) f->Get(chist)->Clone();
  hELossPrimaryTracker->GetYaxis()->SetLabelOffset(0.005);
  hELossPrimaryTracker->GetYaxis()->SetLabelSize(0.050);
  hELossPrimaryTracker->GetYaxis()->SetTitle("Entries");
  hELossPrimaryTracker->GetYaxis()->SetTitleSize(0.055);
  hELossPrimaryTracker->GetYaxis()->SetTitleOffset(1.25);
  hELossPrimaryTracker->GetYaxis()->CenterTitle();
  hELossPrimaryTracker->GetXaxis()->SetLabelOffset(0.010);
  hELossPrimaryTracker->GetXaxis()->SetLabelSize(0.050);
  hELossPrimaryTracker->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossPrimaryTracker->GetXaxis()->SetTitleSize(0.055);
  hELossPrimaryTracker->GetXaxis()->SetTitleOffset(1.1);
  hELossPrimaryTracker->GetXaxis()->CenterTitle();
  hELossPrimaryTracker->SetMinimum(0.5);
  hELossPrimaryTracker->SetLineColor(kRed);

  // hELossSecondaryTracker (4)
  sprintf(chist,"hELossSecondaryTracker");
  TH1D *hELossSecondaryTracker = (TH1D*) f->Get(chist)->Clone();
  hELossSecondaryTracker->GetYaxis()->SetLabelOffset(0.005);
  hELossSecondaryTracker->GetYaxis()->SetLabelSize(0.050);
  hELossSecondaryTracker->GetYaxis()->SetTitle("Entries");
  hELossSecondaryTracker->GetYaxis()->SetTitleSize(0.055);
  hELossSecondaryTracker->GetYaxis()->SetTitleOffset(1.25);
  hELossSecondaryTracker->GetYaxis()->CenterTitle();
  hELossSecondaryTracker->GetXaxis()->SetLabelOffset(0.010);
  hELossSecondaryTracker->GetXaxis()->SetLabelSize(0.050);
  hELossSecondaryTracker->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossSecondaryTracker->GetXaxis()->SetTitleSize(0.055);
  hELossSecondaryTracker->GetXaxis()->SetTitleOffset(1.1);
  hELossSecondaryTracker->GetXaxis()->CenterTitle();
  hELossSecondaryTracker->SetMinimum(0.5);
  hELossSecondaryTracker->SetLineColor(kBlue);

  // hELossTracker (4)
  sprintf(chist,"hELossTracker");
  TH1D *hELossTracker = (TH1D*) f->Get(chist)->Clone();
  hELossTracker->GetYaxis()->SetLabelOffset(0.005);
  hELossTracker->GetYaxis()->SetLabelSize(0.050);
  hELossTracker->GetYaxis()->SetTitle("Entries");
  hELossTracker->GetYaxis()->SetTitleSize(0.055);
  hELossTracker->GetYaxis()->SetTitleOffset(1.25);
  hELossTracker->GetYaxis()->CenterTitle();
  hELossTracker->GetXaxis()->SetLabelOffset(0.010);
  hELossTracker->GetXaxis()->SetLabelSize(0.050);
  hELossTracker->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossTracker->GetXaxis()->SetTitleSize(0.055);
  hELossTracker->GetXaxis()->SetTitleOffset(1.1);
  hELossTracker->GetXaxis()->CenterTitle();
  hELossTracker->SetMinimum(0.5);
  hELossTracker->SetLineColor(kGreen+2);


  // hELossPrimaryCalo (4)
  sprintf(chist,"hELossPrimaryCalo");
  TH1D *hELossPrimaryCalo = (TH1D*) f->Get(chist)->Clone();
  hELossPrimaryCalo->GetYaxis()->SetLabelOffset(0.005);
  hELossPrimaryCalo->GetYaxis()->SetLabelSize(0.050);
  hELossPrimaryCalo->GetYaxis()->SetTitle("Entries");
  hELossPrimaryCalo->GetYaxis()->SetTitleSize(0.055);
  hELossPrimaryCalo->GetYaxis()->SetTitleOffset(1.25);
  hELossPrimaryCalo->GetYaxis()->CenterTitle();
  hELossPrimaryCalo->GetXaxis()->SetLabelOffset(0.010);
  hELossPrimaryCalo->GetXaxis()->SetLabelSize(0.050);
  hELossPrimaryCalo->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossPrimaryCalo->GetXaxis()->SetTitleSize(0.055);
  hELossPrimaryCalo->GetXaxis()->SetTitleOffset(1.1);
  hELossPrimaryCalo->GetXaxis()->CenterTitle();
  hELossPrimaryCalo->SetMinimum(0.5);
  hELossPrimaryCalo->SetLineColor(kRed);

  // hELossSecondaryCalo (4)
  sprintf(chist,"hELossSecondaryCalo");
  TH1D *hELossSecondaryCalo = (TH1D*) f->Get(chist)->Clone();
  hELossSecondaryCalo->GetYaxis()->SetLabelOffset(0.005);
  hELossSecondaryCalo->GetYaxis()->SetLabelSize(0.050);
  hELossSecondaryCalo->GetYaxis()->SetTitle("Entries");
  hELossSecondaryCalo->GetYaxis()->SetTitleSize(0.055);
  hELossSecondaryCalo->GetYaxis()->SetTitleOffset(1.25);
  hELossSecondaryCalo->GetYaxis()->CenterTitle();
  hELossSecondaryCalo->GetXaxis()->SetLabelOffset(0.010);
  hELossSecondaryCalo->GetXaxis()->SetLabelSize(0.050);
  hELossSecondaryCalo->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossSecondaryCalo->GetXaxis()->SetTitleSize(0.055);
  hELossSecondaryCalo->GetXaxis()->SetTitleOffset(1.1);
  hELossSecondaryCalo->GetXaxis()->CenterTitle();
  hELossSecondaryCalo->SetMinimum(0.5);
  hELossSecondaryCalo->SetLineColor(kBlue);

  // hELossCalo (4)
  sprintf(chist,"hELossCalo");
  TH1D *hELossCalo = (TH1D*) f->Get(chist)->Clone();
  hELossCalo->GetYaxis()->SetLabelOffset(0.005);
  hELossCalo->GetYaxis()->SetLabelSize(0.050);
  hELossCalo->GetYaxis()->SetTitle("Entries");
  hELossCalo->GetYaxis()->SetTitleSize(0.055);
  hELossCalo->GetYaxis()->SetTitleOffset(1.25);
  hELossCalo->GetYaxis()->CenterTitle();
  hELossCalo->GetXaxis()->SetLabelOffset(0.010);
  hELossCalo->GetXaxis()->SetLabelSize(0.050);
  hELossCalo->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossCalo->GetXaxis()->SetTitleSize(0.055);
  hELossCalo->GetXaxis()->SetTitleOffset(1.1);
  hELossCalo->GetXaxis()->CenterTitle();
  hELossCalo->SetMinimum(0.5);
  hELossCalo->SetLineColor(kMagenta+2);


  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  gStyle->SetFrameFillColor(10);

  TPad* ptpad;
  char cpad[50];


  TCanvas *c3 = new TCanvas( "c3","ELossPrimaryEvent - hELossSecondaryEvent", 0, 0, 1280, 600 );

  c3->Divide(2,1,0.001,0.001);
  for (int i=1; i<3; i++) {
    sprintf(cpad,"c3_%d",i);
    ptpad = (TPad*) c3->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c3->cd(i);
    switch(i) {
      case 1: {
                hELossSecondaryEvent->Draw();
                hELossPrimaryEvent->Draw("samehist");
                TLegend *legend_c3_1 = new TLegend(0.50,0.72,0.80,0.82);
                legend_c3_1->AddEntry(hELossPrimaryEvent,"Primary","l");
                legend_c3_1->AddEntry(hELossSecondaryEvent,"Secondary","l");
                legend_c3_1->Draw();
                break;
              }
      case 2: {
                hELossEvent->Draw();
                TLegend *legend_c3_2 = new TLegend(0.40,0.72,0.80,0.82);
                legend_c3_2->AddEntry(hELossEvent,"Primary + Secondary","l");
                legend_c3_2->Draw();
                break;
              }
      default: break;
    } 
  }

  TCanvas *c4 = new TCanvas( "c4","ELossPrimaryTracker/Calo - hELossSecondaryTracker/Calo - Sum", 0, 0, 1280, 500 );

  c4->Divide(3,1,0.001,0.001);
  for (int i=1; i<4; i++) {
    sprintf(cpad,"c4_%d",i);
    ptpad = (TPad*) c4->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c4->cd(i);
    switch(i) {
      case 1: {
                hELossSecondaryTracker->Draw();
                hELossPrimaryTracker->Draw("samehist"); 
                TLegend *legend_c4_1 = new TLegend(0.30,0.62,0.80,0.82);
                legend_c4_1->AddEntry(hELossPrimaryTracker,"Tracker Primary","l");
                legend_c4_1->AddEntry(hELossSecondaryTracker,"Tracker Secondary","l");
                legend_c4_1->Draw();
                break;
              }
      case 2: {
                hELossSecondaryCalo->Draw();
                hELossPrimaryCalo->Draw("samehist");
                TLegend *legend_c4_2 = new TLegend(0.40,0.67,0.80,0.82);
                legend_c4_2->AddEntry(hELossPrimaryCalo,"Calo Primary","l");
                legend_c4_2->AddEntry(hELossSecondaryCalo,"Calo Secondary","l");
                legend_c4_2->Draw();
                break;
              }
      case 3: {
                hELossCalo->Draw("hist");
                hELossTracker->Draw("samehist");
                TLegend *legend_c4_3 = new TLegend(0.55,0.67,0.80,0.82);
                legend_c4_3->AddEntry(hELossTracker,"Tracker","l");
                legend_c4_3->AddEntry(hELossCalo,"Calo","l");
                legend_c4_3->Draw();
                break;
              }
      default: break;
    } 
  }

  
  return;

}



// TRACKER //

// *** Langau (taken from: https://root.cern.ch/root/html/tutorials/fit/langaus.C.html) *** //
Double_t LangauFun(Double_t *x, Double_t *par) 
{
  /* 
  Fit parameters:
  * par[0] = Width (scale) parameter of Landau density
  * par[1] = Most Probable (MP, location) parameter of Landau density
  * par[2] = Total area (integral -inf to inf, normalization constant)
  * par[3] = Width (sigma) of convoluted Gaussian function

  In the Landau distribution (represented by the CERNLIB approximation), the maximum is located at x=-0.22278298 with the location parameter=0.
  This shift is corrected within this function, so that the actual maximum is identical to the MP parameter.
  */

  // Numeric constants
  Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
  Double_t mpshift  = -0.22278298;       // Landau maximum location

  // Control constants
  Double_t np = 100.0;      // number of convolution steps
  Double_t sc = 5.0;      // convolution extends to +-sc Gaussian sigmas

  // Variables
  Double_t xx;
  Double_t mpc;
  Double_t fland;
  Double_t sum = 0.0;
  Double_t xlow, xupp;
  Double_t step;
  Double_t i;

  // MP shift correction
  mpc = par[1] - mpshift * par[0];

  // Range of convolution integral
  xlow = x[0] - sc * par[3];
  xupp = x[0] + sc * par[3];

  step = (xupp - xlow) / np;

  // Convolution integral of Landau and Gaussian by sum
  for(i=1.0; i<= np/2; i++) {
    xx = xlow + (i - 0.5) * step;
    fland = TMath::Landau(xx, mpc, par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0], xx, par[3]);

    xx = xupp - (i - 0.5) * step;
    fland = TMath::Landau(xx, mpc, par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0], xx, par[3]);
  }

  return (par[2] * step * sum * invsq2pi / par[3]);

}


Double_t xLangauFun(Double_t *x, Double_t *par) 
{
  /*
  Same parameters as in LangauFun().
  It is simply the numerator of the definition of the mean for any distribution (in this case the Langau)
  */

  return x[0] * LangauFun(x, par);

}


// *** LangauFit (taken from: https://root.cern.ch/root/html/tutorials/fit/langaus.C.html) *** //
TF1 *LangauFit(TH1D *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF) 
{
  /* 
  Variables for langaufit call:
  * his             histogram to fit
  * fitrange[2]     lo and hi boundaries of fit range
  * startvalues[4]  reasonable start values for the fit
  * parlimitslo[4]  lower parameter limits
  * parlimitshi[4]  upper parameter limits
  * fitparams[4]    returns the final fit parameters
  * fiterrors[4]    returns the final fit errors
  * ChiSqr          returns the chi square
  * NDF             returns ndf
  */

  Char_t FunName[100];
  sprintf(FunName, "Fitfcn_%s", his->GetName());

  TF1 *ffitold = (TF1*)gROOT->GetListOfFunctions()->FindObject(FunName);
  if (ffitold) delete ffitold;

  TF1 *ffit = new TF1(FunName, LangauFun, fitrange[0], fitrange[1], 4);
  ffit->SetParameters(startvalues);
  ffit->SetParNames("Width", "MP", "Area", "GSigma");
  ffit->SetLineColor(kGreen);
  ffit->SetLineStyle(1);

  for (int i = 0; i < 4; i++) {
    ffit->SetParLimits(i, parlimitslo[i], parlimitshi[i]);
  }

  his->Fit(FunName,"RB");   // fit within specified range ("R"), use ParLimits ("B"), plot (otherwise put "0")

  ffit->GetParameters(fitparams);    // obtain fit parameters
  for (int i = 0; i < 4; i++) {
    fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
  }
  ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
  NDF[0] = ffit->GetNDF();           // obtain ndf

  return (ffit);              // return fit function

}


Double_t LangauMean(Double_t *fitrange, Double_t *fitparams) 
{
  /*
  This function calculates the mean of the langau which is not properly defined.
  It is obtained by simply using the definition of the mean value for a generic distribution:
  mean := [int x*func(x) dx] / [int func(x) dx]
  */

  Double_t epsrel = 1.e-6;  // precision step in the integral
  
  TF1 *ffit = new TF1("LangauFit_m", LangauFun, fitrange[0], fitrange[1], 4);
  ffit->SetParameters(fitparams);
  for (int i = 0; i < 4; i++)
    ffit->FixParameter(i,fitparams[i]);

  TF1 *xffit = new TF1("xLangauFit_m", xLangauFun, fitrange[0], fitrange[1], 4);
  xffit->SetParameters(fitparams);
  for (int i = 0; i < 4; i++)
    xffit->FixParameter(i,fitparams[i]);

  //Double_t IntNum = xffit->Integral(fitrange[0], fitrange[1], fitparams, epsrel);
  //Double_t IntDen = ffit->Integral(fitrange[0], fitrange[1], fitparams, epsrel);
  Double_t IntNum = xffit->Integral(fitrange[0], fitrange[1], epsrel);
  Double_t IntDen = ffit->Integral(fitrange[0], fitrange[1], epsrel);

  Double_t langaumean = IntNum / IntDen;

  return langaumean;  

}


Double_t LangauSigma(Double_t *fitrange, Double_t *fitparams, Double_t *fiterrors) 
{
  /*
  This function allows to compute an estimation of the uncertainty associated to the Langau mean.
  It is obtained by spanning all the 4-parameter space and calculating the LangauMean in each point and then taking
  the maximum difference between these outcomes and the real mean obtained by the LangauMean with the "correct" parameters
  (same procedure adopted by Francesco in the analysis of real data)
  */

  const int dim = 5;  // 10
  Double_t epsrel = 1.e-3;
  Double_t langaumean_real = LangauMean(fitrange, fitparams);

  TF1 *ffit = new TF1("LangauFit_s", LangauFun, fitrange[0], fitrange[1], 4);
  TF1 *xffit = new TF1("xLangauFit_s", xLangauFun, fitrange[0], fitrange[1], 4);

  std::vector<Double_t> Width_values;
  std::vector<Double_t> MP_values;
  std::vector<Double_t> Area_values;
  std::vector<Double_t> GSigma_values;
  for (int j = 1; j <= dim; j++) {  // divide by dim the interval for each parameter corresponding to twice its uncertainty 
    Width_values.push_back(fitparams[0] - fiterrors[0] + j * (2 * fiterrors[0] / dim));
    MP_values.push_back(fitparams[1] - fiterrors[1] + j * (2 * fiterrors[1] / dim));
    Area_values.push_back(fitparams[2] - fiterrors[2] + j * (2 * fiterrors[2] / dim));
    GSigma_values.push_back(fitparams[3] - fiterrors[3] + j * (2 * fiterrors[3] / dim));
  }

  std::vector<Double_t> sigmas;
  for (int k = 0; k < dim; k++)
    for (int l = 0; l < dim; l++)
      for (int m = 0; m < dim; m++)
        for (int n = 0; n < dim; n++) {
          Double_t params_tmp[4] = {Width_values[k], MP_values[l], Area_values[m], GSigma_values[n]};
          //Double_t IntNum_tmp = xffit->Integral(fitrange[0], fitrange[1], params_tmp, epsrel);
          //Double_t IntDen_tmp = ffit->Integral(fitrange[0], fitrange[1], params_tmp, epsrel);
	        ffit->SetParameters(params_tmp);
          for (int i = 0; i < 4; i++)
            ffit->FixParameter(i,params_tmp[i]);
          xffit->SetParameters(params_tmp);
          for (int i = 0; i < 4; i++)
            xffit->FixParameter(i,params_tmp[i]);
          Double_t IntNum_tmp = xffit->Integral(fitrange[0], fitrange[1], epsrel);
          Double_t IntDen_tmp = ffit->Integral(fitrange[0], fitrange[1], epsrel);

          Double_t mean_tmp = IntNum_tmp / IntDen_tmp;
          sigmas.push_back(TMath::Abs(langaumean_real - mean_tmp));
        }

  Double_t langausigma = *std::max_element(sigmas.begin(), sigmas.end()); 
  //Int_t sigmas_size = sigmas.size();  // alternative way to compute the maximum of the sigmas in the vector (slower)
  //Double_t langausigma = sigmas[0];
  //for(int j = 1; j < sigmas_size; j++)
  //  if (sigmas[j] > langausigma)
  //    langausigma = sigmas[j];

  return langausigma;

}


void figs_Tracker_ELoss() 
{
  /*
  This function displays the energy lost in the tracker. The histograms are fitted by a Langau = Landau * Gaussian
  distribution and the mean and its related uncertainty are computed. The last two informations (for different beam energies)
  are saved manually in "aiglon_g4_Bethe.C"
  */

  char cfich[180];
  char chist[180];
  char cchist[180];
  char ctext[180];

  //sprintf(cfich,"hepd_qmd_36MeV_proton_3C0_ELoss_Trk_hist.root"); // 3C: -1.75 -1.8 cm (x y) 0 deg
  //sprintf(cfich,"hepd_qmd_50MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_68MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_100MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_125MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_154MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_174MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_202MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_228MeV_proton_3C0_ELoss_Trk_hist.root");

  //sprintf(cfich,"hepd_qmd_32MeV_proton_3C0_ELoss_Trk_hist.root"); // air is taken into account!
  //sprintf(cfich,"hepd_qmd_47MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_98MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_123MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_152MeV_proton_3C0_ELoss_Trk_hist.root");
  //sprintf(cfich,"hepd_qmd_173MeV_proton_3C0_ELoss_Trk_hist.root");
  sprintf(cfich,"hepd_qmd_227MeV_proton_3C0_ELoss_Trk_hist.root");


  printf(" cfich %s\n",cfich);
  TFile *f = new TFile(cfich);

  f->ls();


  // hELossTrackerLayer1 (1)
  sprintf(chist,"hELossTrackerLayer1");
  TH1D *hELossTrackerLayer1 = (TH1D*) f->Get(chist)->Clone();
  hELossTrackerLayer1->GetYaxis()->SetLabelOffset(0.005);
  hELossTrackerLayer1->GetYaxis()->SetLabelSize(0.050);
  hELossTrackerLayer1->GetYaxis()->SetTitle("Entries");
  hELossTrackerLayer1->GetYaxis()->SetTitleSize(0.055);
  hELossTrackerLayer1->GetYaxis()->SetTitleOffset(1.25);
  hELossTrackerLayer1->GetYaxis()->CenterTitle();
  hELossTrackerLayer1->GetXaxis()->SetLabelOffset(0.010);
  hELossTrackerLayer1->GetXaxis()->SetLabelSize(0.050);
  hELossTrackerLayer1->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossTrackerLayer1->GetXaxis()->SetTitleSize(0.055);
  hELossTrackerLayer1->GetXaxis()->SetTitleOffset(1.1);
  hELossTrackerLayer1->GetXaxis()->CenterTitle();
  hELossTrackerLayer1->SetMinimum(0.5);
  hELossTrackerLayer1->SetLineColor(kRed);

  // hELossTrackerLayer2 (1)
  sprintf(chist,"hELossTrackerLayer2");
  TH1D *hELossTrackerLayer2 = (TH1D*) f->Get(chist)->Clone();
  hELossTrackerLayer2->GetYaxis()->SetLabelOffset(0.005);
  hELossTrackerLayer2->GetYaxis()->SetLabelSize(0.050);
  hELossTrackerLayer2->GetYaxis()->SetTitle("Entries");
  hELossTrackerLayer2->GetYaxis()->SetTitleSize(0.055);
  hELossTrackerLayer2->GetYaxis()->SetTitleOffset(1.25);
  hELossTrackerLayer2->GetYaxis()->CenterTitle();
  hELossTrackerLayer2->GetXaxis()->SetLabelOffset(0.010);
  hELossTrackerLayer2->GetXaxis()->SetLabelSize(0.050);
  hELossTrackerLayer2->GetXaxis()->SetTitle("Energy Loss [MeV]");
  hELossTrackerLayer2->GetXaxis()->SetTitleSize(0.055);
  hELossTrackerLayer2->GetXaxis()->SetTitleOffset(1.1);
  hELossTrackerLayer2->GetXaxis()->CenterTitle();
  hELossTrackerLayer2->SetMinimum(0.5);
  hELossTrackerLayer2->SetLineColor(kBlue);

  // hMomentumLayer1 (1)
  sprintf(chist,"hBetaGammaLayer1");
  TH1D *hBetaGammaLayer1 = (TH1D*) f->Get(chist)->Clone();
  hBetaGammaLayer1->GetYaxis()->SetLabelOffset(0.005);
  hBetaGammaLayer1->GetYaxis()->SetLabelSize(0.050);
  hBetaGammaLayer1->GetYaxis()->SetTitle("Entries");
  hBetaGammaLayer1->GetYaxis()->SetTitleSize(0.055);
  hBetaGammaLayer1->GetYaxis()->SetTitleOffset(1.25);
  hBetaGammaLayer1->GetYaxis()->CenterTitle();
  hBetaGammaLayer1->GetXaxis()->SetLabelOffset(0.010);
  hBetaGammaLayer1->GetXaxis()->SetLabelSize(0.050);
  hBetaGammaLayer1->GetXaxis()->SetTitle("#beta#gamma");
  hBetaGammaLayer1->GetXaxis()->SetTitleSize(0.055);
  hBetaGammaLayer1->GetXaxis()->SetTitleOffset(1.1);
  hBetaGammaLayer1->GetXaxis()->CenterTitle();
  hBetaGammaLayer1->SetMinimum(0.5);
  hBetaGammaLayer1->SetLineColor(kRed);

  // hMomentumLayer2 (1)
  sprintf(chist,"hBetaGammaLayer2");
  TH1D *hBetaGammaLayer2 = (TH1D*) f->Get(chist)->Clone();
  hBetaGammaLayer2->GetYaxis()->SetLabelOffset(0.005);
  hBetaGammaLayer2->GetYaxis()->SetLabelSize(0.050);
  hBetaGammaLayer2->GetYaxis()->SetTitle("Entries");
  hBetaGammaLayer2->GetYaxis()->SetTitleSize(0.055);
  hBetaGammaLayer2->GetYaxis()->SetTitleOffset(1.25);
  hBetaGammaLayer2->GetYaxis()->CenterTitle();
  hBetaGammaLayer2->GetXaxis()->SetLabelOffset(0.010);
  hBetaGammaLayer2->GetXaxis()->SetLabelSize(0.050);
  hBetaGammaLayer2->GetXaxis()->SetTitle("#beta#gamma");
  hBetaGammaLayer2->GetXaxis()->SetTitleSize(0.055);
  hBetaGammaLayer2->GetXaxis()->SetTitleOffset(1.1);
  hBetaGammaLayer2->GetXaxis()->CenterTitle();
  hBetaGammaLayer2->SetMinimum(0.5);
  hBetaGammaLayer2->SetLineColor(kBlue);


  // Set fit range (= fr) and declare start value (= sv), parameter limits (= pllo, plhi), chi square and number of d.o.f.
  Double_t fr[2];
  fr[0] = 0;
  fr[1] = 600;

  Double_t sv[4], pllo[4], plhi[4];
  Double_t chisqr;
  Int_t ndf;


  // hADCLayer1_pch (2)
  sprintf(chist,"hADCLayer1_pch");
  TH1D *hADCLayer1_pch = (TH1D*) f->Get(chist)->Clone();
  hADCLayer1_pch->GetYaxis()->SetLabelOffset(0.005);
  hADCLayer1_pch->GetYaxis()->SetLabelSize(0.050);
  hADCLayer1_pch->GetYaxis()->SetTitle("Entries");
  hADCLayer1_pch->GetYaxis()->SetTitleSize(0.055);
  hADCLayer1_pch->GetYaxis()->SetTitleOffset(1.25);
  hADCLayer1_pch->GetYaxis()->CenterTitle();
  hADCLayer1_pch->GetXaxis()->SetLabelOffset(0.010);
  hADCLayer1_pch->GetXaxis()->SetLabelSize(0.050);
  hADCLayer1_pch->GetXaxis()->SetTitle("Energy Loss [ADC]");
  hADCLayer1_pch->GetXaxis()->SetTitleSize(0.055);
  hADCLayer1_pch->GetXaxis()->SetTitleOffset(1.1);
  hADCLayer1_pch->GetXaxis()->CenterTitle();
  hADCLayer1_pch->SetMinimum(0.5);
  hADCLayer1_pch->SetLineColor(kRed);

  Double_t fp_Layer1_pch[4], fpe_Layer1_pch[4];

  sv[0] = 5.0;      sv[1] = hADCLayer1_pch->GetMean();    sv[2] = 2500.0;     sv[3] = 5.0;
  pllo[0] = 0.5;    pllo[1] = 20.0;                       pllo[2] = 1.0;      pllo[3] = 0.5;
  plhi[0] = 50.0;   plhi[1] = 600.0;                      plhi[2] = 5500.0;   plhi[3] = 100.0;

  // do the Langau fit with the above paramters
  TF1 *fitADC_Layer1_pch = LangauFit(hADCLayer1_pch, fr, sv, pllo, plhi, fp_Layer1_pch, fpe_Layer1_pch, &chisqr, &ndf);


  // hADCLayer2_pch (2)
  sprintf(chist,"hADCLayer2_pch");
  TH1D *hADCLayer2_pch = (TH1D*) f->Get(chist)->Clone();
  hADCLayer2_pch->GetYaxis()->SetLabelOffset(0.005);
  hADCLayer2_pch->GetYaxis()->SetLabelSize(0.050);
  hADCLayer2_pch->GetYaxis()->SetTitle("Entries");
  hADCLayer2_pch->GetYaxis()->SetTitleSize(0.055);
  hADCLayer2_pch->GetYaxis()->SetTitleOffset(1.25);
  hADCLayer2_pch->GetYaxis()->CenterTitle();
  hADCLayer2_pch->GetXaxis()->SetLabelOffset(0.010);
  hADCLayer2_pch->GetXaxis()->SetLabelSize(0.050);
  hADCLayer2_pch->GetXaxis()->SetTitle("Energy Loss [ADC]");
  hADCLayer2_pch->GetXaxis()->SetTitleSize(0.055);
  hADCLayer2_pch->GetXaxis()->SetTitleOffset(1.1);
  hADCLayer2_pch->GetXaxis()->CenterTitle();
  hADCLayer2_pch->SetMinimum(0.5);
  hADCLayer2_pch->SetLineColor(kBlue);

  Double_t fp_Layer2_pch[4], fpe_Layer2_pch[4];

  sv[0] = 5.0;      sv[1] = hADCLayer2_pch->GetMean();    sv[2] = 2500.0;     sv[3] = 5.0;
  pllo[0] = 0.5;    pllo[1] = 20.0;                       pllo[2] = 1.0;      pllo[3] = 0.5;
  plhi[0] = 50.0;   plhi[1] = 600.0;                      plhi[2] = 5500.0;   plhi[3] = 100.0;

  TF1 *fitADC_Layer2_pch = LangauFit(hADCLayer2_pch, fr, sv, pllo, plhi, fp_Layer2_pch, fpe_Layer2_pch, &chisqr, &ndf);


  // hADCLayer1_nch (2)
  sprintf(chist,"hADCLayer1_nch");
  TH1D *hADCLayer1_nch = (TH1D*) f->Get(chist)->Clone();
  hADCLayer1_nch->GetYaxis()->SetLabelOffset(0.005);
  hADCLayer1_nch->GetYaxis()->SetLabelSize(0.050);
  hADCLayer1_nch->GetYaxis()->SetTitle("Entries");
  hADCLayer1_nch->GetYaxis()->SetTitleSize(0.055);
  hADCLayer1_nch->GetYaxis()->SetTitleOffset(1.25);
  hADCLayer1_nch->GetYaxis()->CenterTitle();
  hADCLayer1_nch->GetXaxis()->SetLabelOffset(0.010);
  hADCLayer1_nch->GetXaxis()->SetLabelSize(0.050);
  hADCLayer1_nch->GetXaxis()->SetTitle("Energy Loss [ADC]");
  hADCLayer1_nch->GetXaxis()->SetTitleSize(0.055);
  hADCLayer1_nch->GetXaxis()->SetTitleOffset(1.1);
  hADCLayer1_nch->GetXaxis()->CenterTitle();
  hADCLayer1_nch->SetMinimum(0.5);
  hADCLayer1_nch->SetLineColor(kRed);

  Double_t fp_Layer1_nch[4], fpe_Layer1_nch[4];

  sv[0] = 5.0;      sv[1] = hADCLayer1_nch->GetMean();    sv[2] = 2500.0;     sv[3] = 5.0;
  pllo[0] = 0.5;    pllo[1] = 20.0;                       pllo[2] = 1.0;      pllo[3] = 0.5;
  plhi[0] = 50.0;   plhi[1] = 600.0;                      plhi[2] = 5500.0;   plhi[3] = 100.0;

  TF1 *fitADC_Layer1_nch = LangauFit(hADCLayer1_nch, fr, sv, pllo, plhi, fp_Layer1_nch, fpe_Layer1_nch, &chisqr, &ndf);


  // hADCLayer2_nch (2)
  sprintf(chist,"hADCLayer2_nch");
  TH1D *hADCLayer2_nch = (TH1D*) f->Get(chist)->Clone();
  hADCLayer2_nch->GetYaxis()->SetLabelOffset(0.005);
  hADCLayer2_nch->GetYaxis()->SetLabelSize(0.050);
  hADCLayer2_nch->GetYaxis()->SetTitle("Entries");
  hADCLayer2_nch->GetYaxis()->SetTitleSize(0.055);
  hADCLayer2_nch->GetYaxis()->SetTitleOffset(1.25);
  hADCLayer2_nch->GetYaxis()->CenterTitle();
  hADCLayer2_nch->GetXaxis()->SetLabelOffset(0.010);
  hADCLayer2_nch->GetXaxis()->SetLabelSize(0.050);
  hADCLayer2_nch->GetXaxis()->SetTitle("Energy Loss [ADC]");
  hADCLayer2_nch->GetXaxis()->SetTitleSize(0.055);
  hADCLayer2_nch->GetXaxis()->SetTitleOffset(1.1);
  hADCLayer2_nch->GetXaxis()->CenterTitle();
  hADCLayer2_nch->SetMinimum(0.5);
  hADCLayer2_nch->SetLineColor(kBlue);

  Double_t fp_Layer2_nch[4], fpe_Layer2_nch[4];

  sv[0] = 5.0;      sv[1] = hADCLayer2_nch->GetMean();    sv[2] = 2500.0;     sv[3] = 5.0;
  pllo[0] = 0.5;    pllo[1] = 20.0;                       pllo[2] = 1.0;      pllo[3] = 0.5;
  plhi[0] = 50.0;   plhi[1] = 600.0;                      plhi[2] = 5500.0;   plhi[3] = 100.0;

  TF1 *fitADC_Layer2_nch = LangauFit(hADCLayer2_nch, fr, sv, pllo, plhi, fp_Layer2_nch, fpe_Layer2_nch, &chisqr, &ndf);



  // hADCLayer1_pch_noise (3)
  sprintf(chist,"hADCLayer1_pch_noise");
  TH1D *hADCLayer1_pch_noise = (TH1D*) f->Get(chist)->Clone();
  hADCLayer1_pch_noise->GetYaxis()->SetLabelOffset(0.005);
  hADCLayer1_pch_noise->GetYaxis()->SetLabelSize(0.050);
  hADCLayer1_pch_noise->GetYaxis()->SetTitle("Entries");
  hADCLayer1_pch_noise->GetYaxis()->SetTitleSize(0.055);
  hADCLayer1_pch_noise->GetYaxis()->SetTitleOffset(1.25);
  hADCLayer1_pch_noise->GetYaxis()->CenterTitle();
  hADCLayer1_pch_noise->GetXaxis()->SetLabelOffset(0.010);
  hADCLayer1_pch_noise->GetXaxis()->SetLabelSize(0.050);
  hADCLayer1_pch_noise->GetXaxis()->SetTitle("Energy Loss [ADC]");
  hADCLayer1_pch_noise->GetXaxis()->SetTitleSize(0.055);
  hADCLayer1_pch_noise->GetXaxis()->SetTitleOffset(1.1);
  hADCLayer1_pch_noise->GetXaxis()->CenterTitle();
  hADCLayer1_pch_noise->SetMinimum(0.5);
  hADCLayer1_pch_noise->SetLineColor(kRed);

  Double_t fp_Layer1_pch_noise[4], fpe_Layer1_pch_noise[4];

  sv[0] = 5.0;      sv[1] = hADCLayer1_pch_noise->GetMean();    sv[2] = 2500.0;     sv[3] = 5.0;
  pllo[0] = 0.5;    pllo[1] = 20.0;                             pllo[2] = 1.0;      pllo[3] = 0.5;
  plhi[0] = 50.0;   plhi[1] = 600.0;                            plhi[2] = 5500.0;   plhi[3] = 100.0;

  TF1 *fitADC_Layer1_pch_noise = LangauFit(hADCLayer1_pch_noise, fr, sv, pllo, plhi, fp_Layer1_pch_noise, fpe_Layer1_pch_noise, &chisqr, &ndf);


  // hADCLayer2_pch_noise (3)
  sprintf(chist,"hADCLayer2_pch_noise");
  TH1D *hADCLayer2_pch_noise = (TH1D*) f->Get(chist)->Clone();
  hADCLayer2_pch_noise->GetYaxis()->SetLabelOffset(0.005);
  hADCLayer2_pch_noise->GetYaxis()->SetLabelSize(0.050);
  hADCLayer2_pch_noise->GetYaxis()->SetTitle("Entries");
  hADCLayer2_pch_noise->GetYaxis()->SetTitleSize(0.055);
  hADCLayer2_pch_noise->GetYaxis()->SetTitleOffset(1.25);
  hADCLayer2_pch_noise->GetYaxis()->CenterTitle();
  hADCLayer2_pch_noise->GetXaxis()->SetLabelOffset(0.010);
  hADCLayer2_pch_noise->GetXaxis()->SetLabelSize(0.050);
  hADCLayer2_pch_noise->GetXaxis()->SetTitle("Energy Loss [ADC]");
  hADCLayer2_pch_noise->GetXaxis()->SetTitleSize(0.055);
  hADCLayer2_pch_noise->GetXaxis()->SetTitleOffset(1.1);
  hADCLayer2_pch_noise->GetXaxis()->CenterTitle();
  hADCLayer2_pch_noise->SetMinimum(0.5);
  hADCLayer2_pch_noise->SetLineColor(kBlue);

  Double_t fp_Layer2_pch_noise[4], fpe_Layer2_pch_noise[4];

  sv[0] = 5.0;      sv[1] = hADCLayer2_pch_noise->GetMean();    sv[2] = 2500.0;     sv[3] = 5.0;
  pllo[0] = 0.5;    pllo[1] = 20.0;                             pllo[2] = 1.0;      pllo[3] = 0.5;
  plhi[0] = 50.0;   plhi[1] = 600.0;                            plhi[2] = 5500.0;   plhi[3] = 100.0;

  TF1 *fitADC_Layer2_pch_noise = LangauFit(hADCLayer2_pch_noise, fr, sv, pllo, plhi, fp_Layer2_pch_noise, fpe_Layer2_pch_noise, &chisqr, &ndf);


  // hADCLayer1_nch_noise (3)
  sprintf(chist,"hADCLayer1_nch_noise");
  TH1D *hADCLayer1_nch_noise = (TH1D*) f->Get(chist)->Clone();
  hADCLayer1_nch_noise->GetYaxis()->SetLabelOffset(0.005);
  hADCLayer1_nch_noise->GetYaxis()->SetLabelSize(0.050);
  hADCLayer1_nch_noise->GetYaxis()->SetTitle("Entries");
  hADCLayer1_nch_noise->GetYaxis()->SetTitleSize(0.055);
  hADCLayer1_nch_noise->GetYaxis()->SetTitleOffset(1.25);
  hADCLayer1_nch_noise->GetYaxis()->CenterTitle();
  hADCLayer1_nch_noise->GetXaxis()->SetLabelOffset(0.010);
  hADCLayer1_nch_noise->GetXaxis()->SetLabelSize(0.050);
  hADCLayer1_nch_noise->GetXaxis()->SetTitle("Energy Loss [ADC]");
  hADCLayer1_nch_noise->GetXaxis()->SetTitleSize(0.055);
  hADCLayer1_nch_noise->GetXaxis()->SetTitleOffset(1.1);
  hADCLayer1_nch_noise->GetXaxis()->CenterTitle();
  hADCLayer1_nch_noise->SetMinimum(0.5);
  hADCLayer1_nch_noise->SetLineColor(kRed);

  Double_t fp_Layer1_nch_noise[4], fpe_Layer1_nch_noise[4];

  sv[0] = 5.0;      sv[1] = hADCLayer1_nch_noise->GetMean();    sv[2] = 2500.0;     sv[3] = 5.0;
  pllo[0] = 0.5;    pllo[1] = 20.0;                             pllo[2] = 1.0;      pllo[3] = 0.5;
  plhi[0] = 50.0;   plhi[1] = 600.0;                            plhi[2] = 5500.0;   plhi[3] = 100.0;

  TF1 *fitADC_Layer1_nch_noise = LangauFit(hADCLayer1_nch_noise, fr, sv, pllo, plhi, fp_Layer1_nch_noise, fpe_Layer1_nch_noise, &chisqr, &ndf);


  // hADCLayer2_nch_noise (3)
  sprintf(chist,"hADCLayer2_nch_noise");
  TH1D *hADCLayer2_nch_noise = (TH1D*) f->Get(chist)->Clone();
  hADCLayer2_nch_noise->GetYaxis()->SetLabelOffset(0.005);
  hADCLayer2_nch_noise->GetYaxis()->SetLabelSize(0.050);
  hADCLayer2_nch_noise->GetYaxis()->SetTitle("Entries");
  hADCLayer2_nch_noise->GetYaxis()->SetTitleSize(0.055);
  hADCLayer2_nch_noise->GetYaxis()->SetTitleOffset(1.25);
  hADCLayer2_nch_noise->GetYaxis()->CenterTitle();
  hADCLayer2_nch_noise->GetXaxis()->SetLabelOffset(0.010);
  hADCLayer2_nch_noise->GetXaxis()->SetLabelSize(0.050);
  hADCLayer2_nch_noise->GetXaxis()->SetTitle("Energy Loss [ADC]");
  hADCLayer2_nch_noise->GetXaxis()->SetTitleSize(0.055);
  hADCLayer2_nch_noise->GetXaxis()->SetTitleOffset(1.1);
  hADCLayer2_nch_noise->GetXaxis()->CenterTitle();
  hADCLayer2_nch_noise->SetMinimum(0.5);
  hADCLayer2_nch_noise->SetLineColor(kBlue);

  Double_t fp_Layer2_nch_noise[4], fpe_Layer2_nch_noise[4];

  sv[0] = 5.0;      sv[1] = hADCLayer2_nch_noise->GetMean();    sv[2] = 2500.0;     sv[3] = 5.0;
  pllo[0] = 0.5;    pllo[1] = 20.0;                             pllo[2] = 1.0;      pllo[3] = 0.5;
  plhi[0] = 50.0;   plhi[1] = 600.0;                            plhi[2] = 5500.0;   plhi[3] = 100.0;

  TF1 *fitADC_Layer2_nch_noise = LangauFit(hADCLayer2_nch_noise, fr, sv, pllo, plhi, fp_Layer2_nch_noise, fpe_Layer2_nch_noise, &chisqr, &ndf);


  gStyle->SetOptTitle(0);
  //gStyle->SetOptStat(0);
  gStyle->SetOptStat(1111); // it displays the name of the histogram, the mean value, the RMS and the best-fit parameter values
  gStyle->SetOptFit(111); // display (0->no, 1->yes): first->Prob (set to 0), second->Chi^2, third->Err, fourth->Par_value
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  gStyle->SetFrameFillColor(10);

  TPad* ptpad;
  char cpad[50];


  TCanvas *c1 = new TCanvas( "c1","hELossTracker/BetaGamma Layer1 - Layer2", 0, 0, 1200, 800 );

  c1->Divide(2,2,0.001,0.001);
  for (int i=1; i<5; i++) {
    sprintf(cpad,"c1_%d",i);
    ptpad = (TPad*) c1->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c1->cd(i);
    switch(i) {
    case 1: {
              hELossTrackerLayer1->Draw(); 
              TLegend *legend_c1_1 = new TLegend(0.62,0.7,0.84,0.85);
              legend_c1_1->AddEntry(hELossTrackerLayer1,"Tracker Layer 1","l");
              legend_c1_1->Draw();
              break;
            }
    case 2: {
              hELossTrackerLayer2->Draw(); 
              TLegend *legend_c1_2 = new TLegend(0.62,0.7,0.84,0.85);
              legend_c1_2->AddEntry(hELossTrackerLayer2,"Tracker Layer 2","l");
              legend_c1_2->Draw();
              break;
            }
    case 3: {
              hBetaGammaLayer1->Draw(); 
              TLegend *legend_c1_3 = new TLegend(0.62,0.7,0.84,0.85);
              legend_c1_3->AddEntry(hBetaGammaLayer1,"Tracker Layer 1","l");
              legend_c1_3->Draw();
              break;
            }
    case 4: {
              hBetaGammaLayer2->Draw();
              TLegend *legend_c1_4 = new TLegend(0.62,0.7,0.84,0.85);
              legend_c1_4->AddEntry(hBetaGammaLayer2,"Tracker Layer 2","l");
              legend_c1_4->Draw();
              break;
            }
    default: break;
    } 
  }

  TCanvas *c2 = new TCanvas( "c2","hADCLayer1_pch/nch - hADCLayer2_pch/nch", 0, 0, 1200, 800 );

  c2->Divide(2,2,0.001,0.001);
  for (int i=1; i<5; i++) {
    sprintf(cpad,"c2_%d",i);
    ptpad = (TPad*) c2->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c2->cd(i);
    switch(i) {
    case 1: {
              hADCLayer1_pch->Draw();
              fitADC_Layer1_pch->Draw("lsame"); 
              TLegend *legend_c2_1 = new TLegend(0.62,0.3,0.84,0.45);
              legend_c2_1->AddEntry(hADCLayer1_pch,"Tracker Layer 1","l");
              legend_c2_1->Draw();
              break;
            }
    case 2: {
              hADCLayer2_pch->Draw();
              fitADC_Layer2_pch->Draw("lsame"); 
              TLegend *legend_c2_2 = new TLegend(0.62,0.3,0.84,0.45);
              legend_c2_2->AddEntry(hADCLayer2_pch,"Tracker Layer 2","l");
              legend_c2_2->Draw();
              break;
            }
    case 3: {
              hADCLayer1_nch->Draw();
              fitADC_Layer1_nch->Draw("lsame"); 
              TLegend *legend_c2_3 = new TLegend(0.62,0.3,0.84,0.45);
              legend_c2_3->AddEntry(hADCLayer1_nch,"Tracker Layer 1","l");
              legend_c2_3->Draw();
              break;
            }
    case 4: {
              hADCLayer2_nch->Draw();
              fitADC_Layer2_nch->Draw("lsame"); 
              TLegend *legend_c2_4 = new TLegend(0.62,0.3,0.84,0.45);
              legend_c2_4->AddEntry(hADCLayer2_nch,"Tracker Layer 2","l");
              legend_c2_4->Draw();
              break;
            }
    default: break;
    } 
  }

  TCanvas *c3 = new TCanvas( "c3","hADCLayer1_pch_noise/nch_noise - hADCLayer2_pch_noise/nch_noise", 0, 0, 1200, 800 );

  c3->Divide(2,2,0.001,0.001);
  for (int i=1; i<5; i++) {
    sprintf(cpad,"c3_%d",i);
    ptpad = (TPad*) c3->FindObject(cpad);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.15);
    ptpad->SetTopMargin(0.125);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogy(1);
    c3->cd(i);
    switch(i) {
    case 1: {
              hADCLayer1_pch_noise->Draw();
              fitADC_Layer1_pch_noise->Draw("lsame"); 
              TLegend *legend_c3_1 = new TLegend(0.62,0.3,0.84,0.45);
              legend_c3_1->AddEntry(hADCLayer1_pch_noise,"Tracker Layer 1","l");
              legend_c3_1->Draw();
              break;
            }
    case 2: {
              hADCLayer2_pch_noise->Draw();
              fitADC_Layer2_pch_noise->Draw("lsame"); 
              TLegend *legend_c3_2 = new TLegend(0.62,0.3,0.84,0.45);
              legend_c3_2->AddEntry(hADCLayer2_pch_noise,"Tracker Layer 2","l");
              legend_c3_2->Draw();
              break;
            }
    case 3: {
              hADCLayer1_nch_noise->Draw();
              fitADC_Layer1_nch_noise->Draw("lsame"); 
              TLegend *legend_c3_3 = new TLegend(0.62,0.3,0.84,0.45);
              legend_c3_3->AddEntry(hADCLayer1_nch_noise,"Tracker Layer 1","l");
              legend_c3_3->Draw();
              break;
            }
    case 4: {
              hADCLayer2_nch_noise->Draw();
              fitADC_Layer2_nch_noise->Draw("lsame"); 
              TLegend *legend_c3_4 = new TLegend(0.62,0.3,0.84,0.45);
              legend_c3_4->AddEntry(hADCLayer2_nch_noise,"Tracker Layer 2","l");
              legend_c3_4->Draw();
              break;
            }
    default: break;
    } 
  }


  // Compute the Langau_mean and _sigma 
  // (this piece of code is put here because pressing "Ctrl+C" after ROOT-minuit has done, the plots are shown 
  // ... method used to avoid to wait lots of time before seeing the plots)
  printf("\n");
  printf("hELossTrackerLayer1 Mean = %f MeV\n", hELossTrackerLayer1->GetMean());
  printf("hELossTrackerLayer2 Mean = %f MeV\n", hELossTrackerLayer2->GetMean());
  printf("hBetaGammaLayer1 Mean = %f\n", hBetaGammaLayer1->GetMean());
  printf("hBetaGammaLayer2 Mean = %f\n", hBetaGammaLayer2->GetMean());
  printf("\n");
  

  printf("\n");
  Double_t langaumean_Layer1_pch = LangauMean(fr, fp_Layer1_pch);
  printf("langaumean_Layer1_pch = %f ADC\n", langaumean_Layer1_pch);
  Double_t langausigma_Layer1_pch = LangauSigma(fr, fp_Layer1_pch, fpe_Layer1_pch);
  printf("langausigma_Layer1_pch = %f ADC\n", langausigma_Layer1_pch);
  printf("\n");

  Double_t langaumean_Layer2_pch = LangauMean(fr, fp_Layer2_pch);
  printf("langaumean_Layer2_pch = %f ADC\n", langaumean_Layer2_pch);
  Double_t langausigma_Layer2_pch = LangauSigma(fr, fp_Layer2_pch, fpe_Layer2_pch);
  printf("langausigma_Layer2_pch = %f ADC\n", langausigma_Layer2_pch);
  printf("\n");

  Double_t langaumean_Layer1_nch = LangauMean(fr, fp_Layer1_nch);
  printf("langaumean_Layer1_nch = %f ADC\n", langaumean_Layer1_nch);
  Double_t langausigma_Layer1_nch = LangauSigma(fr, fp_Layer1_nch, fpe_Layer1_nch);
  printf("langausigma_Layer1_nch = %f ADC\n", langausigma_Layer1_nch);
  printf("\n");

  Double_t langaumean_Layer2_nch = LangauMean(fr, fp_Layer2_nch);
  printf("langaumean_Layer2_nch = %f ADC\n", langaumean_Layer2_nch);
  Double_t langausigma_Layer2_nch = LangauSigma(fr, fp_Layer2_nch, fpe_Layer2_nch);
  printf("langausigma_Layer2_nch = %f ADC\n", langausigma_Layer2_nch);
  printf("\n");

  
  Double_t langaumean_Layer1_pch_noise = LangauMean(fr, fp_Layer1_pch_noise);
  printf("langaumean_Layer1_pch_noise = %f ADC\n", langaumean_Layer1_pch_noise);
  Double_t langausigma_Layer1_pch_noise = LangauSigma(fr, fp_Layer1_pch_noise, fpe_Layer1_pch_noise);
  printf("langausigma_Layer1_pch_noise = %f ADC\n", langausigma_Layer1_pch_noise);
  printf("\n");

  Double_t langaumean_Layer2_pch_noise = LangauMean(fr, fp_Layer2_pch_noise);
  printf("langaumean_Layer2_pch_noise = %f ADC\n", langaumean_Layer2_pch_noise);
  Double_t langausigma_Layer2_pch_noise = LangauSigma(fr, fp_Layer2_pch_noise, fpe_Layer2_pch_noise);
  printf("langausigma_Layer2_pch_noise = %f ADC\n", langausigma_Layer2_pch_noise);
  printf("\n");

  Double_t langaumean_Layer1_nch_noise = LangauMean(fr, fp_Layer1_nch_noise);
  printf("langaumean_Layer1_nch_noise = %f ADC\n", langaumean_Layer1_nch_noise);
  Double_t langausigma_Layer1_nch_noise = LangauSigma(fr, fp_Layer1_nch_noise, fpe_Layer1_nch_noise);
  printf("langausigma_Layer1_nch_noise = %f ADC\n", langausigma_Layer1_nch_noise);
  printf("\n");

  Double_t langaumean_Layer2_nch_noise = LangauMean(fr, fp_Layer2_nch_noise);
  printf("langaumean_Layer2_nch_noise = %f ADC\n", langaumean_Layer2_nch_noise);
  Double_t langausigma_Layer2_nch_noise = LangauSigma(fr, fp_Layer2_nch_noise, fpe_Layer2_nch_noise);
  printf("langausigma_Layer2_nch_noise = %f ADC\n", langausigma_Layer2_nch_noise);
  printf("\n");

  return;

}
