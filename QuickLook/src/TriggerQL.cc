/**
 * =============================================================================
 *
 * Created by Francesco Palma @ INFN Roma2: francesco.palma@roma2.infn.it
 *
 * FILENAME:       TriggerScan.C
 *
 * DESCRIPTION:    Script to generate Trigger Quicklook pdf file starting 
 *                 from HEPD L0 ROOT file 
 *
 * INSTRUCTIONS:   g++ -Wall TriggerScan.C -o TriggerScan `root-config --cflags --libs`
 *                 ./TriggerScan [ROOT file name]  
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
#include <TTree.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TStyle.h>
#include <TPaveText.h>
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

void TriggerScan(TString rootname)
{
  
   TString outname = rootname;
   TString outnameStart;
   TString outnameEnd;
   outname.ReplaceAll(".root", 5, "_TriggerQL.pdf", 14);
  
 
   UShort_t first_run_nr;
   UShort_t last_run_nr;
   UInt_t event_time;

   LEvRec0File rootfile(rootname.Data());
   LEvRec0 ev;
   LEvRec0Md metaData;
   rootfile.SetTheEventPointer(ev);
   rootfile.SetTmdPointer(metaData);
  
   // Metadata
   int Tmd_entries =  rootfile.GetTmdEntries();
   if (Tmd_entries % 2)
      Tmd_entries -= 1;
   cout << "Number of Tmd entries: " << Tmd_entries << endl;


   first_run_nr = rootfile.GetRunID(0);
   last_run_nr = rootfile.GetRunID(Tmd_entries-1);
   // cout << "First run id: " << first_run_nr << endl;
   // cout << "Last run id: " <<  last_run_nr << endl;

   // Events
   int nEvents = rootfile.GetEntries();
   cout << "Number of events: " << nEvents << endl;
  
   stringstream ss;
   ss << nEvents;
   TString numEvents = ss.str(); 
   ss.str("");
   rootfile.GetTmdEntry(0);
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
  
  
   TH2I *h_lost_triggers_vs_event = new TH2I("h_lost_triggers_vs_event","Lost triggers vs event time",1000,0,2000000,1000,0,5);
   h_lost_triggers_vs_event->GetXaxis()->SetTitle("Event time (ms)");
   h_lost_triggers_vs_event->GetYaxis()->SetTitle("Lost triggers");
   //h_lost_triggers_vs_event->GetYaxis()->SetTitleOffset(1.5);
  
   TH2D *h_alive_time_vs_event = new TH2D("h_alive_time","Alive time vs event time",
					  1000,0,2000000,100,0,5000);
   h_alive_time_vs_event->GetXaxis()->SetTitle("Event time (ms)");
   h_alive_time_vs_event->GetYaxis()->SetTitle("Alive time (ms)");
   h_alive_time_vs_event->GetYaxis()->SetTitleOffset(1.5);
  
   TH2D *h_dead_time_vs_event = new TH2D("h_dead_time","Dead time vs event time",
					 1000,0,2000000,100,0,10);
   h_dead_time_vs_event->GetXaxis()->SetTitle("Event time (ms)");
   h_dead_time_vs_event->GetYaxis()->SetTitle("Dead time (ms)");
   h_dead_time_vs_event->GetYaxis()->SetTitleOffset(1.5);
  
   const char *subdetector[EASIROC_CH] = {"T1e","T2e","T3e","T4e","T5e","T6e",
					  "P1se","P2sw","P3se","P4sw","P5se","P6sw",
					  "P7se","P8sw","P9se","P10sw","P11se","P12sw",
					  "P13se","P14sw","P15se","P16sw",
					  "VNu","VEu","VSu","VWu","VBne",
					  "L9sw","L7nw","L1ne","L8w","L5c", 
					  "T1w","T2w","T3w","T4w","T5w","T6w",  
					  "P1nw","P2ne","P3nw","P4ne","P5nw","P6ne","P7nw",
					  "P8ne","P9nw","P10ne","P11nw","P12ne","P13nw",
					  "P14ne","P15nw","P16ne", 
					  "VNd","VEd","VSd","VWd","VBsw", 
					  "L3se","L2e","L6s","L4n","N.C."};
  
   TH1D *h_FlagCount_vs_Ch =
     new TH1D("TrigFlag1Count vs Ch","Counts for trigger flag = 1 vs PMT channel",64,0,64);
  
   for(int fl=0;fl<EASIROC_CH;fl++)
      h_FlagCount_vs_Ch->GetXaxis()->SetBinLabel(fl+1,subdetector[fl]);
  
   TH2D *h_rate_meter[9];
   TString hname_rate_meter;
  
   for(int kk=0;kk<9;kk++) {
      if (kk==0)
	 hname_rate_meter = "Trigger MASK 0 [T]";
      if (kk==1)
	 hname_rate_meter = "Trigger MASK 1 [T & P1]";
      if (kk==2)
	 hname_rate_meter = "Trigger MASK 2 [T & (P1 || P2)]";
      if (kk==3)
	 hname_rate_meter = "Trigger MASK 3 [(T3 || T4) & (P1 || P2)]";
      if (kk==4)
	 hname_rate_meter = "Trigger MASK 4 [T & P1 & P2]";
      if (kk==5)
	 hname_rate_meter = "Trigger MASK 5 [T & P1 & P2 & P3]";
      if (kk==6)
	 hname_rate_meter = "Trigger MASK 6 [T & (P1 & P2) & (P15 || P16)]";
      if (kk==7)
	 hname_rate_meter = "Trigger MASK 7 [T & (P1 || P2) & L]";
      if (kk==8)
	 hname_rate_meter = "Trigger MASK 8 [Generic Trigger Mask]";
    
      h_rate_meter[kk] = new TH2D("",hname_rate_meter,1000,0,2000000,100,0,200);
    
      // TString hname_rate_meter = "Rate Meter of Trigger Mask ";
      // hname_rate_meter += kk;
      // h_rate_meter[kk] = new TH2D("",hname_rate_meter,100,0,500,100,0,200);
      h_rate_meter[kk]->GetXaxis()->SetTitle("Event time (ms)");
      h_rate_meter[kk]->GetYaxis()->SetTitle("Rate meter (Hz)");
      h_rate_meter[kk]->GetYaxis()->SetTitleOffset(1.5);
      // h_rate_meter[kk]->SetTitleSize(0.1);
   }
 
   TH2D *h_pmt_rate_meter[65];
   TString hname_pmt_rate_meter;
   for(int kk=0;kk<65;kk++) {
      TString hname_pmt_rate_meter = "Rate Meter of PMT ";
      hname_pmt_rate_meter += kk;
      h_pmt_rate_meter[kk] = new TH2D("",hname_pmt_rate_meter,1000,0,2000000,100,0,50);
      h_pmt_rate_meter[kk]->GetXaxis()->SetTitle("Event time (ms)");
      h_pmt_rate_meter[kk]->GetYaxis()->SetTitle("PMT rate meter (Hz)");
      h_pmt_rate_meter[kk]->GetYaxis()->SetTitleOffset(1.5);
   }

   Int_t cpu_startRunTime_vect[100];

   for(int j=1; j<Tmd_entries; j+=2) //Tmd loop
   {
      rootfile.GetTmdEntry(j); 
      cpu_startRunTime_vect[(j-1)/2] = metaData.CPU_time[0];
   }      

   

   
   for(int i = 0; i < nEvents; i++) //Event loop
   {
      rootfile.GetEntry(i);

      if (ev.run_id > last_run_nr) // to avoid processing run without tail
	 break;
      
      if(metaData.run_type == 0x634E) // to skip mixed virgin event
	 continue;

      
      event_time = cpu_startRunTime_vect[ev.run_id - first_run_nr] + ev.hepd_time/1e+2; //unit = ms
       
      h_lost_triggers_vs_event->Fill(event_time, ev.lost_trigger);
      h_alive_time_vs_event->Fill(event_time, ev.alive_time*0.005);
      h_dead_time_vs_event->Fill(event_time, ev.dead_time*0.005);
	
      for(int kk=0;kk<9;kk++) 
	 h_rate_meter[kk]->Fill(event_time, ev.rate_meter[kk]);

      for(int kk=0;kk<65;kk++) 
	 h_pmt_rate_meter[kk]->Fill(event_time, metaData.PMT_rate_meter[kk]);
	 
      for(int ch=0;ch<EASIROC_CH;ch++) //PMT channel loop ch[0-63]
      {
	 if(ev.trigger_flag[ch]>0) h_FlagCount_vs_Ch->Fill(ch);
      }
	
   } //End of event loop
     
  
   TCanvas *c_lost_triggers = new TCanvas("c_lost_triggers","",1200,600);
   gPad->SetGrid();
   h_lost_triggers_vs_event->SetMarkerStyle(20);
   h_lost_triggers_vs_event->Draw();
   c_lost_triggers->Print(outname);
 
   TCanvas *c_alive_dead_time = new TCanvas("c_alive_dead_time","",1200,600);
   c_alive_dead_time->Divide(2,1);
   c_alive_dead_time->cd(1);
   gPad->SetGrid();

   h_alive_time_vs_event->Draw();
   c_alive_dead_time->cd(2);
   gPad->SetGrid();
   h_dead_time_vs_event->Draw();
   c_alive_dead_time->Print(outname);
  
   TCanvas *c_trig_flag= new TCanvas("c_trig_flag"," ",1200,600);
   c_trig_flag->cd();
   c_trig_flag->SetGrid();
   h_FlagCount_vs_Ch->SetLineColor(kBlack);
   h_FlagCount_vs_Ch->SetLineWidth(2);
   h_FlagCount_vs_Ch->GetYaxis()->SetTitle("Counts");
  
   gStyle->SetOptStat(0);
   h_FlagCount_vs_Ch->Draw();
  
   TLine *line = new TLine(32, h_FlagCount_vs_Ch->GetMinimum(),
			   32, h_FlagCount_vs_Ch->GetMaximum());
   line->SetLineColor(2);
   line->SetLineWidth(2);
   line->Draw("same");
   c_trig_flag->Print(outname);
  
   TCanvas* c_rate_meter_trig_mask_0_3 = new TCanvas("c_rate_meter_trig_mask_0_3"," ",
						     1200,600);
   c_rate_meter_trig_mask_0_3->Divide(3,1);
  
   for(int p=0;p<3;p++) {
      c_rate_meter_trig_mask_0_3->cd(p+1);
      gPad->SetGrid();
      h_rate_meter[p]->Draw();
    
   }
   c_rate_meter_trig_mask_0_3->Print(outname);
  
   TCanvas* c_rate_meter_trig_mask_4_6 = new TCanvas("c_rate_meter_trig_mask_4_6"," ",
						     1200,600);
   c_rate_meter_trig_mask_4_6->Divide(3,1);
  
   for(int p=3;p<6;p++) {
      c_rate_meter_trig_mask_4_6->cd(p-2);
      gPad->SetGrid();
      h_rate_meter[p]->Draw();
    
   }
   c_rate_meter_trig_mask_4_6->Print(outname);
  
   TCanvas* c_rate_meter_trig_mask_7_9 = new TCanvas("c_rate_meter_trig_mask_7_9"," ",
						     1200,600);
   c_rate_meter_trig_mask_7_9->Divide(3,1);
  
   for(int p=6;p<9;p++)
   {
      c_rate_meter_trig_mask_7_9->cd(p-5);
      gPad->SetGrid();
      h_rate_meter[p]->Draw();
      
      if(p==8)   
      {   
	 outnameEnd = outname+")";
	 c_rate_meter_trig_mask_7_9->Print(outnameEnd);
      }   
   }
   //c_rate_meter_trig_mask_7_9->Print(outname);
   
   // TCanvas* c_rate_meter_pmt_0_8 = new TCanvas("c_rate_meter_pmt_0_8"," ",1200,600);
   // c_rate_meter_pmt_0_8->Divide(3,3);
  
   // for(int p=0;p<9;p++)
   // {
   //    c_rate_meter_pmt_0_8->cd(p+1);
   //    gPad->SetGrid();
   //    h_pmt_rate_meter[p]->Draw();
  
   //    if(p==8)   
   //    {   
   // 	 outnameEnd = outname+")";
   // 	 c_rate_meter_pmt_0_8->Print(outnameEnd);
   //    }
   // }
   
}
