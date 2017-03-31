/**
* =============================================================================
*
* Created by Francesco Palma @ INFN Roma2: francesco.palma@roma2.infn.it
*
* FILENAME:       PMTScan.C
*
* DESCRIPTION:    Script to generate Scintillator Quicklook pdf file starting 
*                 from HEPD L0 ROOT file 
*
* INSTRUCTIONS:   g++ -Wall PMTScan.C -o PMTScan `root-config --cflags --libs`
*                 ./PMTScan [ROOT file name]  
*
* DATE:           March 10, 2017
*     
* =============================================================================
**/

#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH2D.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TProfile.h>
#include <TStyle.h>
#include <iostream>
#include <sstream>
#include "LEvRec0File.hh"
#include "LEvRec0.hh"

#define EASIROC_CH 64
#define EASIROC_HALF_CH 32
#define SILSTRIP 4608
#define TRIG_PAD 6
#define CALO_PL 16
#define LYSO_CU 9
#define VETO_PL 5

using namespace std;

void PMTScan(TString rootname)
{
   //cout << "Rootname:" << rootname << endl;
  
  TString outname = rootname;
  TString outnameStart;
  TString outnameEnd;
  outname.ReplaceAll(".root", 5, "_ScintillatorQL.pdf", 19);


  UShort_t first_run_nr;
  UShort_t last_run_nr;

  Double_t max_c = 1000;
  
   LEvRec0File rootfile(rootname.Data());
   LEvRec0 ev;
   LEvRec0Md metaData;
   rootfile.SetTheEventPointer(ev);
   rootfile.SetTmdPointer(metaData);

  // Metadata
  int Tmd_entries = rootfile.GetTmdEntries();
  cout << "Number of Tmd entries: " << Tmd_entries << endl;

  first_run_nr = rootfile.GetRunID(0);
  last_run_nr = rootfile.GetRunID(Tmd_entries-1);

  // Events
  int nEvents = rootfile.GetEntries();
  cout << "Number of events: " << nEvents << endl;
  
  stringstream ss;
  ss << nEvents;
  TString numEvents = ss.str(); 
  ss.str("");
  ss << metaData.boot_nr;  
  TString bootNR = ss.str();
  ss.str("");
  ss << first_run_nr;
  TString FirstRunNR = ss.str();
  ss.str("");
  ss << last_run_nr;
  TString LastRunNR = ss.str(); 
  
  TCanvas *Run = new TCanvas("Run");
  TPaveText *pt = new TPaveText(.25,.2,.75,.6);
  pt->AddText("ROOT file: ");
  TString nameFile(rootname);
  pt->AddText(nameFile );
  pt->AddText("Boot Number:");
  pt->AddText(bootNR);
  pt->AddText("Run Number:");
  pt->AddText(FirstRunNR + "-" + LastRunNR);
  pt->AddText("Number of events: ");
  pt->AddText(numEvents);
  pt->Draw();
  outnameStart = outname+"(";
  Run->Print(outnameStart); 
  
  const char *subdetector[EASIROC_CH] = {"T1e","T2e","T3e","T4e","T5e","T6e",
				   "P1se","P2sw","P3se","P4sw","P5se","P6sw","P7se","P8sw","P9se","P10sw","P11se","P12sw","P13se","P14sw","P15se","P16sw",
				   "VNu","VEu","VSu","VWu","VBne",
				   "L9sw","L7nw","L1ne","L8w","L5c", 
				   "T1w","T2w","T3w","T4w","T5w","T6w",  
				   "P1nw","P2ne","P3nw","P4ne","P5nw","P6ne","P7nw","P8ne","P9nw","P10ne","P11nw","P12ne","P13nw","P14ne","P15nw","P16ne", 
				   "VNd","VEd","VSd","VWd","VBsw", 
				   "L3se","L2e","L6s","L4n","N.C."};
  
  TProfile *prof_pmt_high = new TProfile("PMTProfHigh","PMT Profile - High Gain",64, 0, 64);
  prof_pmt_high->GetYaxis()->SetTitleOffset(1.5);  
  prof_pmt_high->GetYaxis()->SetTitle("ADC counts");
  //prof_pmt_high->GetYaxis()->SetRangeUser(0,max_c); 
  
  TProfile *prof_pmt_low = new TProfile("PMTProfLow","PMT Profile - Low Gain",64, 0, 64);
  prof_pmt_low->GetYaxis()->SetTitleOffset(1.5);
  prof_pmt_low->GetYaxis()->SetTitle("ADC counts");
  //prof_pmt_low->GetYaxis()->SetRangeUser(0,max_c);
  
  for(int fl=0;fl<EASIROC_CH;fl++){
    prof_pmt_high->GetXaxis()->SetBinLabel(fl+1,subdetector[fl]);
    prof_pmt_low->GetXaxis()->SetBinLabel(fl+1,subdetector[fl]);
  }
  
  TH2D *h_pmt_comp_low[TRIG_PAD+CALO_PL+VETO_PL];
  TH2D *h_pmt_comp_high[TRIG_PAD+CALO_PL+VETO_PL];
  
  for(int kk=0;kk<TRIG_PAD+CALO_PL+VETO_PL;kk++) {
    TString hname1_low = "PMT Low Gain Ch. ";
    hname1_low += kk+EASIROC_HALF_CH;
    hname1_low += " vs ";
    hname1_low += kk;
    
    TString hname1_high = "PMT High Gain Ch. ";
    hname1_high += kk+EASIROC_HALF_CH;
    hname1_high += " vs ";
    hname1_high += kk;
    
    h_pmt_comp_high[kk] = new TH2D("",hname1_high,100,0,5*max_c,100,0,5*max_c);
    TString xt="Ch ";
    xt += kk;
    h_pmt_comp_high[kk]->GetXaxis()->SetTitle(xt);
    TString yt="Ch ";
    yt += kk+EASIROC_HALF_CH;
    h_pmt_comp_high[kk]->GetYaxis()->SetTitle(yt);
    h_pmt_comp_high[kk]->GetYaxis()->SetTitleOffset(1.5);
    
    h_pmt_comp_low[kk] = new TH2D("",hname1_low,100,0,max_c,100,0,max_c);
    h_pmt_comp_low[kk]->GetXaxis()->SetTitle(xt);
    h_pmt_comp_low[kk]->GetYaxis()->SetTitle(yt);
    h_pmt_comp_low[kk]->GetYaxis()->SetTitleOffset(1.5);
  }
  
  TH1D* h_pmt_low[EASIROC_CH];
  TH1D* h_pmt_high[EASIROC_CH];
  
  for(int k=0;k<EASIROC_CH;k++) {
    TString hname_low = "PMT Low Gain Ch. ";
    TString hname_high = "PMT High Gain Ch.";
    hname_low += k;
    hname_high += k;
    h_pmt_low[k] = new TH1D("",hname_low, 500,0,max_c);
    h_pmt_high[k] = new TH1D("",hname_high, 300,0,4*max_c);
  }
  
  TCanvas *c1 = new TCanvas("c1"," ",1200,600);
  gPad->SetGrid();
  c1->Divide(2,1);
  
  for(int i=0;i<nEvents;i++) //Event loop
    {
      rootfile.GetEntry(i);
      
     if(metaData.run_type == 0x634E) // mixed virgin
	 continue;

     for(int ch=0;ch<EASIROC_CH;ch++) //PMT channel loop ch[0-63]
	{

	  h_pmt_low[ch]->Fill(ev.pmt_low[ch]);  
	  h_pmt_high[ch]->Fill(ev.pmt_high[ch]);
	  
	  prof_pmt_low->Fill(ch, ev.pmt_low[ch]);
	  prof_pmt_high->Fill(ch, ev.pmt_high[ch]);
	}
      
      for(int n=0;n<TRIG_PAD+CALO_PL+VETO_PL;n++)
	{
	  h_pmt_comp_high[n]->Fill(ev.pmt_high[n], ev.pmt_high[n+EASIROC_HALF_CH]);
	  h_pmt_comp_low[n]->Fill(ev.pmt_low[n], ev.pmt_low[n+EASIROC_HALF_CH]);
	}
      
    } //End of event loop
  
  TCanvas *cp = new TCanvas("cp","",1200,600);
  gPad->SetGrid();
  
  prof_pmt_high->SetLineWidth(2);  
  prof_pmt_high->Draw();
  
  TLine *line_high = new TLine(EASIROC_HALF_CH, prof_pmt_high->GetMinimum(),
			       EASIROC_HALF_CH, prof_pmt_high->GetMaximum());
  line_high->SetLineColor(2);
  line_high->SetLineWidth(2);
  line_high->Draw("same");
  
  cp->Print(outname); 
  prof_pmt_low->SetLineWidth(2);   
  prof_pmt_low->Draw();

  TLine *line_low=new TLine(EASIROC_HALF_CH, prof_pmt_low->GetMinimum(),
			    EASIROC_HALF_CH, prof_pmt_low->GetMaximum());
  line_low->SetLineColor(2);
  line_low->SetLineWidth(2);
  line_low->Draw("same");
  line_low->SetLineColor(2);
  line_low->SetLineWidth(2);
  line_low->Draw("same");
  cp->Print(outname); 
  
  //Trigger Plane (T1-T3) - High gain
  TCanvas* c_trig_T1_T3_high = new TCanvas("c_trig_T1_T3_high"," ",1200,600);
  c_trig_T1_T3_high->Divide(3,2);

  for(int p=0;p<6;p++) {
    c_trig_T1_T3_high->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_high[p]->SetTitle("T1e (CH0) - High Gain");
    if(p==1) h_pmt_high[p]->SetTitle("T2e (CH1) - High Gain");
    if(p==2) h_pmt_high[p]->SetTitle("T3e (CH2) - High Gain");
    h_pmt_high[p]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_high[p]->GetYaxis()->SetTitle("Events");
    h_pmt_high[p]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_high[p]->Draw();
    if(p>2){
      if(p==3) h_pmt_high[p+29]->SetTitle("T1w (CH32) - High Gain");
      if(p==4) h_pmt_high[p+29]->SetTitle("T2w (CH33) - High Gain");
      if(p==5) h_pmt_high[p+29]->SetTitle("T3w (CH34) - High Gain");
      h_pmt_high[p+29]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_high[p+29]->GetYaxis()->SetTitle("Events");
      h_pmt_high[p+29]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_high[p+29]->Draw();
    }
  }
  c_trig_T1_T3_high->Print(outname);

  //Trigger Plane (T4-T6) - High gain
  TCanvas* c_trig_T4_T6_high = new TCanvas("c_trig_T4_T6_high"," ",1200,600);
  c_trig_T4_T6_high->Divide(3,2);

  for(int p=0;p<6;p++) {
    c_trig_T4_T6_high->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_high[p+3]->SetTitle("T4e (CH3) - High Gain");
    if(p==1) h_pmt_high[p+3]->SetTitle("T5e (CH4) - High Gain");
    if(p==2) h_pmt_high[p+3]->SetTitle("T6e (CH5) - High Gain");
    h_pmt_high[p+3]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_high[p+3]->GetYaxis()->SetTitle("Events");
    h_pmt_high[p+3]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_high[p+3]->Draw();
    if(p>2){
      if(p==3) h_pmt_high[p+32]->SetTitle("T4w (CH35) - High Gain");
      if(p==4) h_pmt_high[p+32]->SetTitle("T5w (CH36) - High Gain");
      if(p==5) h_pmt_high[p+32]->SetTitle("T6w (CH37) - High Gain");
      h_pmt_high[p+32]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_high[p+32]->GetYaxis()->SetTitle("Events");
      h_pmt_high[p+32]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_high[p+32]->Draw();
    }
  }
  c_trig_T4_T6_high->Print(outname);

  //Trigger Plane (T1-T3) - Low gain
  TCanvas* c_trig_T1_T3_low = new TCanvas("c_trig_T1_T3_low"," ",1200,600);
  c_trig_T1_T3_low->Divide(3,2);

  for(int p=0;p<6;p++) {
    c_trig_T1_T3_low->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_low[p]->SetTitle("T1e (CH0) - Low Gain");
    if(p==1) h_pmt_low[p]->SetTitle("T2e (CH1) - Low Gain");
    if(p==2) h_pmt_low[p]->SetTitle("T3e (CH2) - Low Gain");
    h_pmt_low[p]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_low[p]->GetYaxis()->SetTitle("Events");
    h_pmt_low[p]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_low[p]->Draw();
    if(p>2){
      if(p==3) h_pmt_low[p+29]->SetTitle("T1w (CH32) - Low Gain");
      if(p==4) h_pmt_low[p+29]->SetTitle("T2w (CH33) - Low Gain");
      if(p==5) h_pmt_low[p+29]->SetTitle("T3w (CH34) - Low Gain");
      h_pmt_low[p+29]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_low[p+29]->GetYaxis()->SetTitle("Events");
      h_pmt_low[p+29]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_low[p+29]->Draw();
    }
  }
  c_trig_T1_T3_low->Print(outname);

  //Trigger Plane (T4-T6) - Low gain
  TCanvas* c_trig_T4_T6_low = new TCanvas("c_trig_T4_T6_low"," ",1200,600);
  c_trig_T4_T6_low->Divide(3,2);

  for(int p=0;p<6;p++) {
    c_trig_T4_T6_low->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_low[p+3]->SetTitle("T4e (CH3) - Low Gain");
    if(p==1) h_pmt_low[p+3]->SetTitle("T5e (CH4) - Low Gain");
    if(p==2) h_pmt_low[p+3]->SetTitle("T6e (CH5) - Low Gain");
    h_pmt_low[p+3]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_low[p+3]->GetYaxis()->SetTitle("Events");
    h_pmt_low[p+3]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_low[p+3]->Draw();
    if(p>2){
      if(p==3) h_pmt_low[p+32]->SetTitle("T4w (CH35) - Low Gain");
      if(p==4) h_pmt_low[p+32]->SetTitle("T5w (CH36) - Low Gain");
      if(p==5) h_pmt_low[p+32]->SetTitle("T6w (CH37) - Low Gain");
      h_pmt_low[p+32]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_low[p+32]->GetYaxis()->SetTitle("Events");
      h_pmt_low[p+32]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_low[p+32]->Draw();
    }
  }
  c_trig_T4_T6_low->Print(outname);

  //Trigger Plane Correlation Plots (T1-T6) - High gain
  TCanvas* c_trig_corr_high = new TCanvas("c_trig_corr_high"," ",1200,600);
  c_trig_corr_high->Divide(3,2);

  for(int p=0;p<6;p++) {
    c_trig_corr_high->cd(p+1);
    gPad->SetGrid();
    gStyle->SetOptStat(0);  
    gPad->SetLogz();
    if(p==0) h_pmt_comp_high[p]->SetTitle("T1w vs T1e (CH32 vs CH0) - High Gain");
    if(p==1) h_pmt_comp_high[p]->SetTitle("T2w vs T2e (CH33 vs CH1) - High Gain");
    if(p==2) h_pmt_comp_high[p]->SetTitle("T3w vs T3e (CH34 vs CH2) - High Gain");
    if(p==3) h_pmt_comp_high[p]->SetTitle("T4w vs T4e (CH35 vs CH3) - High Gain");
    if(p==4) h_pmt_comp_high[p]->SetTitle("T5w vs T5e (CH36 vs CH4) - High Gain");
    if(p==5) h_pmt_comp_high[p]->SetTitle("T6w vs T6e (CH37 vs CH5) - High Gain");
    h_pmt_comp_high[p]->Draw("colz");
  }
  c_trig_corr_high->Print(outname);

  //Trigger Plane Correlation Plots (T1-T6) - Low gain
  TCanvas* c_trig_corr_low = new TCanvas("c_trig_corr_low"," ",1200,600);
  c_trig_corr_low->Divide(3,2);

  for(int p=0;p<6;p++) {
    c_trig_corr_low->cd(p+1);
    gPad->SetGrid();
    gStyle->SetOptStat(0);  
    gPad->SetLogz();
    if(p==0) h_pmt_comp_low[p]->SetTitle("T1w vs T1e (CH32 vs CH0) - Low Gain");
    if(p==1) h_pmt_comp_low[p]->SetTitle("T2w vs T2e (CH33 vs CH1) - Low Gain");
    if(p==2) h_pmt_comp_low[p]->SetTitle("T3w vs T3e (CH34 vs CH2) - Low Gain");
    if(p==3) h_pmt_comp_low[p]->SetTitle("T4w vs T4e (CH35 vs CH3) - Low Gain");
    if(p==4) h_pmt_comp_low[p]->SetTitle("T5w vs T5e (CH36 vs CH4) - Low Gain");
    if(p==5) h_pmt_comp_low[p]->SetTitle("T6w vs T6e (CH37 vs CH5) - Low Gain");
    h_pmt_comp_low[p]->Draw("colz");
  }
  c_trig_corr_low->Print(outname);

  //Calorimeter (P1-P4) - High gain
  TCanvas* c_calo_P1_P4_high = new TCanvas("c_calo_P1_P4_high"," ",1200,600);
  c_calo_P1_P4_high->Divide(4,2);

  for(int p=0;p<8;p++) {
    c_calo_P1_P4_high->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_high[p+6]->SetTitle("P1se (CH6) - High Gain");
    if(p==1) h_pmt_high[p+6]->SetTitle("P2sw (CH7) - High Gain");
    if(p==2) h_pmt_high[p+6]->SetTitle("P3se (CH8) - High Gain");
    if(p==3) h_pmt_high[p+6]->SetTitle("P4sw (CH9) - High Gain");
    h_pmt_high[p+6]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_high[p+6]->GetYaxis()->SetTitle("Events");
    h_pmt_high[p+6]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_high[p+6]->Draw();
    if(p>3){
      if(p==4) h_pmt_high[p+34]->SetTitle("P1nw (CH38) - High Gain");
      if(p==5) h_pmt_high[p+34]->SetTitle("P2ne (CH39) - High Gain");
      if(p==6) h_pmt_high[p+34]->SetTitle("P3nw (CH40) - High Gain");
      if(p==7) h_pmt_high[p+34]->SetTitle("P4ne (CH41) - High Gain");
      h_pmt_high[p+34]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_high[p+34]->GetYaxis()->SetTitle("Events");
      h_pmt_high[p+34]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_high[p+34]->Draw();
    }
  }
  c_calo_P1_P4_high->Print(outname);

  //Calorimeter (P5-P8) - High gain
  TCanvas* c_calo_P5_P8_high = new TCanvas("c_calo_P5_P8_high"," ",1200,600);
  c_calo_P5_P8_high->Divide(4,2);

  for(int p=0;p<8;p++) {
    c_calo_P5_P8_high->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_high[p+10]->SetTitle("P5se (CH10) - High Gain");
    if(p==1) h_pmt_high[p+10]->SetTitle("P6sw (CH11) - High Gain");
    if(p==2) h_pmt_high[p+10]->SetTitle("P7se (CH12) - High Gain");
    if(p==3) h_pmt_high[p+10]->SetTitle("P8sw (CH13) - High Gain");
    h_pmt_high[p+10]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_high[p+10]->GetYaxis()->SetTitle("Events");
    h_pmt_high[p+10]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_high[p+10]->Draw();
    if(p>3){
      if(p==4) h_pmt_high[p+38]->SetTitle("P5nw (CH42) - High Gain");
      if(p==5) h_pmt_high[p+38]->SetTitle("P6ne (CH43) - High Gain");
      if(p==6) h_pmt_high[p+38]->SetTitle("P7nw (CH44) - High Gain");
      if(p==7) h_pmt_high[p+38]->SetTitle("P8ne (CH45) - High Gain");
      h_pmt_high[p+38]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_high[p+38]->GetYaxis()->SetTitle("Events");
      h_pmt_high[p+38]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_high[p+38]->Draw();
    }
  }
  c_calo_P5_P8_high->Print(outname);

  //Calorimeter (P9-P12) - High gain
  TCanvas* c_calo_P9_P12_high = new TCanvas("c_calo_P9_P12_high"," ",1200,600);
  c_calo_P9_P12_high->Divide(4,2);

  for(int p=0;p<8;p++) {
    c_calo_P9_P12_high->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_high[p+14]->SetTitle("P9se (CH14) - High Gain");
    if(p==1) h_pmt_high[p+14]->SetTitle("P10sw (CH15) - High Gain");
    if(p==2) h_pmt_high[p+14]->SetTitle("P11se (CH16) - High Gain");
    if(p==3) h_pmt_high[p+14]->SetTitle("P12sw (CH17) - High Gain");
    h_pmt_high[p+14]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_high[p+14]->GetYaxis()->SetTitle("Events");
    h_pmt_high[p+14]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_high[p+14]->Draw();
    if(p>3){
      if(p==4) h_pmt_high[p+42]->SetTitle("P9nw (CH46) - High Gain");
      if(p==5) h_pmt_high[p+42]->SetTitle("P10ne (CH47) - High Gain");
      if(p==6) h_pmt_high[p+42]->SetTitle("P11nw (CH48) - High Gain");
      if(p==7) h_pmt_high[p+42]->SetTitle("P12ne (CH49) - High Gain");
      h_pmt_high[p+42]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_high[p+42]->GetYaxis()->SetTitle("Events");
      h_pmt_high[p+42]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_high[p+42]->Draw();
    }
  }
  c_calo_P9_P12_high->Print(outname);

  //Calorimeter (P13-P16) - High gain
  TCanvas* c_calo_P13_P16_high = new TCanvas("c_calo_P13_P16_high"," ",1200,600);
  c_calo_P13_P16_high->Divide(4,2);

  for(int p=0;p<8;p++) {
    c_calo_P13_P16_high->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_high[p+18]->SetTitle("P13se (CH18) - High Gain");
    if(p==1) h_pmt_high[p+18]->SetTitle("P14sw (CH19) - High Gain");
    if(p==2) h_pmt_high[p+18]->SetTitle("P15se (CH20) - High Gain");
    if(p==3) h_pmt_high[p+18]->SetTitle("P16sw (CH21) - High Gain");
    h_pmt_high[p+18]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_high[p+18]->GetYaxis()->SetTitle("Events");
    h_pmt_high[p+18]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_high[p+18]->Draw();
    if(p>3){
      if(p==4) h_pmt_high[p+46]->SetTitle("P13nw (CH50) - High Gain");
      if(p==5) h_pmt_high[p+46]->SetTitle("P14ne (CH51) - High Gain");
      if(p==6) h_pmt_high[p+46]->SetTitle("P15nw (CH52) - High Gain");
      if(p==7) h_pmt_high[p+46]->SetTitle("P16ne (CH53) - High Gain");
      h_pmt_high[p+46]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_high[p+46]->GetYaxis()->SetTitle("Events");
      h_pmt_high[p+46]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_high[p+46]->Draw();
    }
  }
  c_calo_P13_P16_high->Print(outname);

  //Calorimeter (P1-P4) - Low gain
  TCanvas* c_calo_P1_P4_low = new TCanvas("c_calo_P1_P4_low"," ",1200,600);
  c_calo_P1_P4_low->Divide(4,2);

  for(int p=0;p<8;p++) {
    c_calo_P1_P4_low->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_low[p+6]->SetTitle("P1se (CH6) - Low Gain");
    if(p==1) h_pmt_low[p+6]->SetTitle("P2sw (CH7) - Low Gain");
    if(p==2) h_pmt_low[p+6]->SetTitle("P3se (CH8) - Low Gain");
    if(p==3) h_pmt_low[p+6]->SetTitle("P4sw (CH9) - Low Gain");
    h_pmt_low[p+6]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_low[p+6]->GetYaxis()->SetTitle("Events");
    h_pmt_low[p+6]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_low[p+6]->Draw();
    if(p>3){
      if(p==4) h_pmt_low[p+34]->SetTitle("P1nw (CH38) - Low Gain");
      if(p==5) h_pmt_low[p+34]->SetTitle("P2ne (CH39) - Low Gain");
      if(p==6) h_pmt_low[p+34]->SetTitle("P3nw (CH40) - Low Gain");
      if(p==7) h_pmt_low[p+34]->SetTitle("P4ne (CH41) - Low Gain");
      h_pmt_low[p+34]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_low[p+34]->GetYaxis()->SetTitle("Events");
      h_pmt_low[p+34]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_low[p+34]->Draw();
    }
  }
  c_calo_P1_P4_low->Print(outname);

  //Calorimeter (P5-P8) - Low gain
  TCanvas* c_calo_P5_P8_low = new TCanvas("c_calo_P5_P8_low"," ",1200,600);
  c_calo_P5_P8_low->Divide(4,2);

  for(int p=0;p<8;p++) {
    c_calo_P5_P8_low->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_low[p+10]->SetTitle("P5se (CH10) - Low Gain");
    if(p==1) h_pmt_low[p+10]->SetTitle("P6sw (CH11) - Low Gain");
    if(p==2) h_pmt_low[p+10]->SetTitle("P7se (CH12) - Low Gain");
    if(p==3) h_pmt_low[p+10]->SetTitle("P8sw (CH13) - Low Gain");
    h_pmt_low[p+10]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_low[p+10]->GetYaxis()->SetTitle("Events");
    h_pmt_low[p+10]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_low[p+10]->Draw();
    if(p>3){
      if(p==4) h_pmt_low[p+38]->SetTitle("P5nw (CH42) - Low Gain");
      if(p==5) h_pmt_low[p+38]->SetTitle("P6ne (CH43) - Low Gain");
      if(p==6) h_pmt_low[p+38]->SetTitle("P7nw (CH44) - Low Gain");
      if(p==7) h_pmt_low[p+38]->SetTitle("P8ne (CH45) - Low Gain");
      h_pmt_low[p+38]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_low[p+38]->GetYaxis()->SetTitle("Events");
      h_pmt_low[p+38]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_low[p+38]->Draw();
    }
  }
  c_calo_P5_P8_low->Print(outname);

  //Calorimeter (P9-P12) - Low gain
  TCanvas* c_calo_P9_P12_low = new TCanvas("c_calo_P9_P12_low"," ",1200,600);
  c_calo_P9_P12_low->Divide(4,2);

  for(int p=0;p<8;p++) {
    c_calo_P9_P12_low->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_low[p+14]->SetTitle("P9se (CH14) - Low Gain");
    if(p==1) h_pmt_low[p+14]->SetTitle("P10sw (CH15) - Low Gain");
    if(p==2) h_pmt_low[p+14]->SetTitle("P11se (CH16) - Low Gain");
    if(p==3) h_pmt_low[p+14]->SetTitle("P12sw (CH17) - Low Gain");
    h_pmt_low[p+14]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_low[p+14]->GetYaxis()->SetTitle("Events");
    h_pmt_low[p+14]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_low[p+14]->Draw();
    if(p>3){
      if(p==4) h_pmt_low[p+42]->SetTitle("P9nw (CH46) - Low Gain");
      if(p==5) h_pmt_low[p+42]->SetTitle("P10ne (CH47) - Low Gain");
      if(p==6) h_pmt_low[p+42]->SetTitle("P11nw (CH48) - Low Gain");
      if(p==7) h_pmt_low[p+42]->SetTitle("P12ne (CH49) - Low Gain");
      h_pmt_low[p+42]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_low[p+42]->GetYaxis()->SetTitle("Events");
      h_pmt_low[p+42]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_low[p+42]->Draw();
    }
  }
  c_calo_P9_P12_low->Print(outname);

  //Calorimeter (P13-P16) - Low gain
  TCanvas* c_calo_P13_P16_low = new TCanvas("c_calo_P13_P16_low"," ",1200,600);
  c_calo_P13_P16_low->Divide(4,2);

  for(int p=0;p<8;p++) {
    c_calo_P13_P16_low->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_low[p+18]->SetTitle("P13se (CH18) - Low Gain");
    if(p==1) h_pmt_low[p+18]->SetTitle("P14sw (CH19) - Low Gain");
    if(p==2) h_pmt_low[p+18]->SetTitle("P15se (CH20) - Low Gain");
    if(p==3) h_pmt_low[p+18]->SetTitle("P16sw (CH21) - Low Gain");
    h_pmt_low[p+18]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_low[p+18]->GetYaxis()->SetTitle("Events");
    h_pmt_low[p+18]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_low[p+18]->Draw();
    if(p>3){
      if(p==4) h_pmt_low[p+46]->SetTitle("P13nw (CH50) - Low Gain");
      if(p==5) h_pmt_low[p+46]->SetTitle("P14ne (CH51) - Low Gain");
      if(p==6) h_pmt_low[p+46]->SetTitle("P15nw (CH52) - Low Gain");
      if(p==7) h_pmt_low[p+46]->SetTitle("P16ne (CH53) - Low Gain");
      h_pmt_low[p+46]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_low[p+46]->GetYaxis()->SetTitle("Events");
      h_pmt_low[p+46]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_low[p+46]->Draw();
    }
  }
  c_calo_P13_P16_low->Print(outname);

  //Calorimeter Correlation Plots (P1-P8) - High gain
  TCanvas* c_calo_corr_P1_P8_high = new TCanvas("c_calo_corr_P1_P8_high"," ",1200,600);
  c_calo_corr_P1_P8_high->Divide(4,2);

  for(int p=6;p<14;p++) {
    c_calo_corr_P1_P8_high->cd(p-5);
    gPad->SetGrid();
    gStyle->SetOptStat(0);  
    gPad->SetLogz();
    if(p==6) h_pmt_comp_high[p]->SetTitle("P1nw vs P1se (CH38 vs CH6) - High Gain");
    if(p==7) h_pmt_comp_high[p]->SetTitle("P2ne vs P2sw (CH39 vs CH7) - High Gain");
    if(p==8) h_pmt_comp_high[p]->SetTitle("P3nw vs P3se (CH40 vs CH8) - High Gain");
    if(p==9) h_pmt_comp_high[p]->SetTitle("P4ne vs P4sw (CH41 vs CH9) - High Gain");
    if(p==10) h_pmt_comp_high[p]->SetTitle("P5nw vs P5se (CH42 vs CH10) - High Gain");
    if(p==11) h_pmt_comp_high[p]->SetTitle("P6ne vs P6sw (CH43 vs CH11) - High Gain");
    if(p==12) h_pmt_comp_high[p]->SetTitle("P7nw vs P7se (CH44 vs CH12) - High Gain");
    if(p==13) h_pmt_comp_high[p]->SetTitle("P8ne vs P8sw (CH45 vs CH13) - High Gain");
    h_pmt_comp_high[p]->Draw("colz");
  }
  c_calo_corr_P1_P8_high->Print(outname);

  //Calorimeter Correlation Plots (P9-P16) - High gain
  TCanvas* c_calo_corr_P9_P16_high = new TCanvas("c_calo_corr_P9_P16_high"," ",1200,600);
  c_calo_corr_P9_P16_high->Divide(4,2);

  for(int p=14;p<22;p++) {
    c_calo_corr_P9_P16_high->cd(p-13);
    gPad->SetGrid();
    gStyle->SetOptStat(0);  
    gPad->SetLogz();
    if(p==14) h_pmt_comp_high[p]->SetTitle("P9nw vs P9se (CH46 vs CH14) - High Gain");
    if(p==15) h_pmt_comp_high[p]->SetTitle("P10ne vs P10sw (CH47 vs CH15) - High Gain");
    if(p==16) h_pmt_comp_high[p]->SetTitle("P11nw vs P11se (CH48 vs CH16) - High Gain");
    if(p==17) h_pmt_comp_high[p]->SetTitle("P12ne vs P12sw (CH49 vs CH17) - High Gain");
    if(p==18) h_pmt_comp_high[p]->SetTitle("P13nw vs P13se (CH50 vs CH18) - High Gain");
    if(p==19) h_pmt_comp_high[p]->SetTitle("P14ne vs P14sw (CH51 vs CH19) - High Gain");
    if(p==20) h_pmt_comp_high[p]->SetTitle("P15nw vs P15se (CH52 vs CH20) - High Gain");
    if(p==21) h_pmt_comp_high[p]->SetTitle("P16ne vs P16sw (CH53 vs CH21) - High Gain");
    h_pmt_comp_high[p]->Draw("colz");
  }
  c_calo_corr_P9_P16_high->Print(outname);

  //Calorimeter Correlation Plots (P1-P8) - Low gain
  TCanvas* c_calo_corr_P1_P8_low = new TCanvas("c_calo_corr_P1_P8_low"," ",1200,600);
  c_calo_corr_P1_P8_low->Divide(4,2);

  for(int p=6;p<14;p++) {
    c_calo_corr_P1_P8_low->cd(p-5);
    gPad->SetGrid();
    gStyle->SetOptStat(0);  
    gPad->SetLogz();
    if(p==6) h_pmt_comp_low[p]->SetTitle("P1nw vs P1se (CH38 vs CH6) - Low Gain");
    if(p==7) h_pmt_comp_low[p]->SetTitle("P2ne vs P2sw (CH39 vs CH7) - Low Gain");
    if(p==8) h_pmt_comp_low[p]->SetTitle("P3nw vs P3se (CH40 vs CH8) - Low Gain");
    if(p==9) h_pmt_comp_low[p]->SetTitle("P4ne vs P4sw (CH41 vs CH9) - Low Gain");
    if(p==10) h_pmt_comp_low[p]->SetTitle("P5nw vs P5se (CH42 vs CH10) - Low Gain");
    if(p==11) h_pmt_comp_low[p]->SetTitle("P6ne vs P6sw (CH43 vs CH11) - Low Gain");
    if(p==12) h_pmt_comp_low[p]->SetTitle("P7nw vs P7se (CH44 vs CH12) - Low Gain");
    if(p==13) h_pmt_comp_low[p]->SetTitle("P8ne vs P8sw (CH45 vs CH13) - Low Gain");
    h_pmt_comp_low[p]->Draw("colz");
  }
  c_calo_corr_P1_P8_low->Print(outname);

  //Calorimeter Correlation Plots (P9-P16) - Low gain
  TCanvas* c_calo_corr_P9_P16_low = new TCanvas("c_calo_corr_P9_P16_low"," ",1200,600);
  c_calo_corr_P9_P16_low->Divide(4,2);

  for(int p=14;p<22;p++) {
    c_calo_corr_P9_P16_low->cd(p-13);
    gPad->SetGrid();
    gStyle->SetOptStat(0);  
    gPad->SetLogz();
    if(p==14) h_pmt_comp_low[p]->SetTitle("P9nw vs P9se (CH46 vs CH14) - Low Gain");
    if(p==15) h_pmt_comp_low[p]->SetTitle("P10ne vs P10sw (CH47 vs CH15) - Low Gain");
    if(p==16) h_pmt_comp_low[p]->SetTitle("P11nw vs P11se (CH48 vs CH16) - Low Gain");
    if(p==17) h_pmt_comp_low[p]->SetTitle("P12ne vs P12sw (CH49 vs CH17) - Low Gain");
    if(p==18) h_pmt_comp_low[p]->SetTitle("P13nw vs P13se (CH50 vs CH18) - Low Gain");
    if(p==19) h_pmt_comp_low[p]->SetTitle("P14ne vs P14sw (CH51 vs CH19) - Low Gain");
    if(p==20) h_pmt_comp_low[p]->SetTitle("P15nw vs P15se (CH52 vs CH20) - Low Gain");
    if(p==21) h_pmt_comp_low[p]->SetTitle("P16ne vs P16sw (CH53 vs CH21) - Low Gain");
    h_pmt_comp_low[p]->Draw("colz");
  }
  c_calo_corr_P9_P16_low->Print(outname);

  //Lyso - High gain
  TCanvas* c_lyso_high = new TCanvas("c_lyso_high"," ",1200,600);
  c_lyso_high->Divide(5,2);

  for(int p=0;p<10;p++) {
    c_lyso_high->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_high[p+27]->SetTitle("L9sw (CH27) - High Gain");
    if(p==1) h_pmt_high[p+27]->SetTitle("L7nw (CH28) - High Gain");
    if(p==2) h_pmt_high[p+27]->SetTitle("L1ne (CH29) - High Gain");
    if(p==3) h_pmt_high[p+27]->SetTitle("L8w (CH30) - High Gain");
    if(p==4) h_pmt_high[p+27]->SetTitle("L5c (CH31) - High Gain");
    h_pmt_high[p+27]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_high[p+27]->GetYaxis()->SetTitle("Events");
    h_pmt_high[p+27]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_high[p+27]->Draw();
    if(p>4){
      if(p==5) h_pmt_high[p+54]->SetTitle("L3se (CH59) - High Gain");
      if(p==6) h_pmt_high[p+54]->SetTitle("L2e (CH60) - High Gain");
      if(p==7) h_pmt_high[p+54]->SetTitle("L6s (CH61) - High Gain");
      if(p==8) h_pmt_high[p+54]->SetTitle("L4n (CH62) - High Gain");
      if(p==9) h_pmt_high[p+54]->SetTitle("N.C. (CH63) - High Gain");
      h_pmt_high[p+54]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_high[p+54]->GetYaxis()->SetTitle("Events");
      h_pmt_high[p+54]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_high[p+54]->Draw();
    }
  }
  c_lyso_high->Print(outname);

  //Lyso - Low gain
  TCanvas* c_lyso_low = new TCanvas("c_lyso_low"," ",1200,600);
  c_lyso_low->Divide(5,2);

  for(int p=0;p<10;p++) {
    c_lyso_low->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_low[p+27]->SetTitle("L9sw (CH27) - Low Gain");
    if(p==1) h_pmt_low[p+27]->SetTitle("L7nw (CH28) - Low Gain");
    if(p==2) h_pmt_low[p+27]->SetTitle("L1ne (CH29) - Low Gain");
    if(p==3) h_pmt_low[p+27]->SetTitle("L8w (CH30) - Low Gain");
    if(p==4) h_pmt_low[p+27]->SetTitle("L5c (CH31) - Low Gain");
    h_pmt_low[p+27]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_low[p+27]->GetYaxis()->SetTitle("Events");
    h_pmt_low[p+27]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_low[p+27]->Draw();
    if(p>4){
      if(p==5) h_pmt_low[p+54]->SetTitle("L3se (CH59) - Low Gain");
      if(p==6) h_pmt_low[p+54]->SetTitle("L2e (CH60) - Low Gain");
      if(p==7) h_pmt_low[p+54]->SetTitle("L6s (CH61) - Low Gain");
      if(p==8) h_pmt_low[p+54]->SetTitle("L4n (CH62) - Low Gain");
      if(p==9) h_pmt_low[p+54]->SetTitle("N.C. (CH63) - Low Gain");
      h_pmt_low[p+54]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_low[p+54]->GetYaxis()->SetTitle("Events");
      h_pmt_low[p+54]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_low[p+54]->Draw();
    }
  }
  c_lyso_low->Print(outname);  

  //Veto - High gain
  TCanvas* c_veto_high = new TCanvas("c_veto_high"," ",1200,600);
  c_veto_high->Divide(5,2);

  for(int p=0;p<10;p++) {
    c_veto_high->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_high[p+22]->SetTitle("VNu (CH22) - High Gain");
    if(p==1) h_pmt_high[p+22]->SetTitle("VEu (CH23) - High Gain");
    if(p==2) h_pmt_high[p+22]->SetTitle("VSu (CH24) - High Gain");
    if(p==3) h_pmt_high[p+22]->SetTitle("VWu (CH25) - High Gain");
    if(p==4) h_pmt_high[p+22]->SetTitle("VBne (CH26) - High Gain");
    h_pmt_high[p+22]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_high[p+22]->GetYaxis()->SetTitle("Events");
    h_pmt_high[p+22]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_high[p+22]->Draw();
    if(p>4){
      if(p==5) h_pmt_high[p+49]->SetTitle("VNd (CH54) - High Gain");
      if(p==6) h_pmt_high[p+49]->SetTitle("VEd (CH55) - High Gain");
      if(p==7) h_pmt_high[p+49]->SetTitle("VSd (CH56) - High Gain");
      if(p==8) h_pmt_high[p+49]->SetTitle("VWd (CH57) - High Gain");
      if(p==9) h_pmt_high[p+49]->SetTitle("VBsw (CH58) - High Gain");
      h_pmt_high[p+49]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_high[p+49]->GetYaxis()->SetTitle("Events");
      h_pmt_high[p+49]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_high[p+49]->Draw();
    }
  }
  c_veto_high->Print(outname);

  //Veto - Low gain
  TCanvas* c_veto_low = new TCanvas("c_veto_low"," ",1200,600);
  c_veto_low->Divide(5,2);

  for(int p=0;p<10;p++) {
    c_veto_low->cd(p+1);
    gPad->SetGrid();
    gPad->SetLogy();
    if(p==0) h_pmt_low[p+22]->SetTitle("VNu (CH22) - Low Gain");
    if(p==1) h_pmt_low[p+22]->SetTitle("VEu (CH23) - Low Gain");
    if(p==2) h_pmt_low[p+22]->SetTitle("VSu (CH24) - Low Gain");
    if(p==3) h_pmt_low[p+22]->SetTitle("VWu (CH25) - Low Gain");
    if(p==4) h_pmt_low[p+22]->SetTitle("VBne (CH26) - Low Gain");
    h_pmt_low[p+22]->GetXaxis()->SetTitle("ADC counts");
    h_pmt_low[p+22]->GetYaxis()->SetTitle("Events");
    h_pmt_low[p+22]->GetYaxis()->SetTitleOffset(1.5);
    h_pmt_low[p+22]->Draw();
    if(p>4){
      if(p==5) h_pmt_low[p+49]->SetTitle("VNd (CH54) - Low Gain");
      if(p==6) h_pmt_low[p+49]->SetTitle("VEd (CH55) - Low Gain");
      if(p==7) h_pmt_low[p+49]->SetTitle("VSd (CH56) - Low Gain");
      if(p==8) h_pmt_low[p+49]->SetTitle("VWd (CH57) - Low Gain");
      if(p==9) h_pmt_low[p+49]->SetTitle("VBsw (CH58) - Low Gain");
      h_pmt_low[p+49]->GetXaxis()->SetTitle("ADC counts");
      h_pmt_low[p+49]->GetYaxis()->SetTitle("Events");
      h_pmt_low[p+49]->GetYaxis()->SetTitleOffset(1.5);
      h_pmt_low[p+49]->Draw();
    }
  }
  c_veto_low->Print(outname);  

  //Veto Correlation Plots - High gain
  TCanvas* c_veto_corr_high = new TCanvas("c_veto_corr_high"," ",1200,600);
  c_veto_corr_high->Divide(3,2);

  for(int p=22;p<27;p++) {
    c_veto_corr_high->cd(p-21);
    gPad->SetGrid();
    gStyle->SetOptStat(0);  
    gPad->SetLogz();
    if(p==22) h_pmt_comp_high[p]->SetTitle("VNd vs VNu (CH54 vs CH22) - High Gain");
    if(p==23) h_pmt_comp_high[p]->SetTitle("VEd vs VEu (CH55 vs CH23) - High Gain");
    if(p==24) h_pmt_comp_high[p]->SetTitle("VSd vs VSu (CH56 vs CH24) - High Gain");
    if(p==25) h_pmt_comp_high[p]->SetTitle("VWd vs VWu (CH57 vs CH25) - High Gain");
    if(p==26) h_pmt_comp_high[p]->SetTitle("VBsw vs VBne (CH58 vs CH26) - High Gain");
    h_pmt_comp_high[p]->Draw("colz");
  }
  c_veto_corr_high->Print(outname);

  //Veto Correlation Plots - Low gain
  TCanvas* c_veto_corr_low = new TCanvas("c_veto_corr_low"," ",1200,600);
  c_veto_corr_low->Divide(3,2);

  for(int p=22;p<27;p++) {
    c_veto_corr_low->cd(p-21);
    gPad->SetGrid();
    gStyle->SetOptStat(0);  
    gPad->SetLogz();
    if(p==22) h_pmt_comp_low[p]->SetTitle("VNd vs VNu (CH54 vs CH22) - Low Gain");
    if(p==23) h_pmt_comp_low[p]->SetTitle("VEd vs VEu (CH55 vs CH23) - Low Gain");
    if(p==24) h_pmt_comp_low[p]->SetTitle("VSd vs VSu (CH56 vs CH24) - Low Gain");
    if(p==25) h_pmt_comp_low[p]->SetTitle("VWd vs VWu (CH57 vs CH25) - Low Gain");
    if(p==26) h_pmt_comp_low[p]->SetTitle("VBsw vs VBne (CH58 vs CH26) - Low Gain");
    h_pmt_comp_low[p]->Draw("colz");

    if(p==26)   
      {   
	outnameEnd = outname+")";
	c_veto_corr_low->Print(outnameEnd);
      }
  }
}
