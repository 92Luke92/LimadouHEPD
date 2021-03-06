/**
 * =============================================================================
 *
 * Created by Francesco Palma & Luca Carfora @ INFN Roma2:
 * francesco.palma@roma2.infn.it
 *
 * FILENAME:       TriggerQL.cc
 *
 * DESCRIPTION:    Script to generate Trigger Quicklook pdf file starting 
 *                 from HEPD L0 ROOT file 
 *
 *
 * DATE:          July 4, 2017
 *     
 * =============================================================================
 **/

#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH2D.h>
#include <TError.h>
#include <TGraph.h>
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
#define INTEGTIME 1000

using namespace std;

//int runIDtoStartTime(int (*matrix)[2], int size, int runID)
int runIDtoStartTime(int **matrix, int size, int runID)
{
   int ret = -1;

   for(int j=0; j<size; j++)
      if(matrix[j][1] == runID)
	 ret = matrix[j][0];
   return ret;
}

void TriggerScan(TString rootname, TString outPath )
{
   gErrorIgnoreLevel = 5000 ;

   // Page numbers
   char c[14]; int n = 1;
   TString outname = outPath;
   TString _temp = rootname ;
   _temp.Replace(0, _temp.Last('/')+1, "");
     
   outname+= "/";
   outname+= _temp;
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
   TString header = "1_header.png";
   TText l; l.SetTextSize(0.05); l.SetTextAlign(22); 
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   Run->SaveAs(header);

   TGraph *lost_triggers_vs_time = new TGraph();
   lost_triggers_vs_time->SetTitle("Lost triggers vs event time;Event time (s); Lost triggers");
   
   TGraph *alive_time_vs_time = new TGraph();
   alive_time_vs_time->SetTitle("Alive time vs event time; Event time (s); Alive time (ms)");
   
   TGraph *dead_time_vs_time = new TGraph();
   dead_time_vs_time->SetTitle("Dead time vs event time; Event time (s); Dead time (ms)");
   
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
   
   TGraph *rate_meter_vs_time[9];
   const char *name_rate_meter;
   
   for(int kk=0;kk<9;kk++) {

      rate_meter_vs_time[kk] = new TGraph();
     
      if (kk==0)
	 name_rate_meter = "Trigger MASK 0 [T]";
      if (kk==1)
	 name_rate_meter = "Trigger MASK 1 [T & P1]";
      if (kk==2)
	 name_rate_meter = "Trigger MASK 2 [T & (P1 || P2)]";
      if (kk==3)
	 name_rate_meter = "Trigger MASK 3 [(T3 || T4) & (P1 || P2)]";
      if (kk==4)
	 name_rate_meter = "Trigger MASK 4 [T & P1 & P2]";
      if (kk==5)
	 name_rate_meter = "Trigger MASK 5 [T & P1 & P2 & P3]";
      if (kk==6)
	 name_rate_meter = "Trigger MASK 6 [T & (P1 || P2) & (P15 || P16)]";
      if (kk==7)
	 name_rate_meter = "Trigger MASK 7 [T & (P1 || P2) & L]";
      if (kk==8)
	 name_rate_meter = "Trigger MASK 8 [Generic Trigger Mask]";

      rate_meter_vs_time[kk]->SetTitle(Form("%s; Event time (s); Rate meter (Hz) ", name_rate_meter));
   }

   TGraph *pmt_rate_meter_vs_time[65];
   for(int kk=0;kk<65;kk++) {
      pmt_rate_meter_vs_time[kk] = new TGraph();
   }
   for(int kk=0;kk<58;kk++) {
      pmt_rate_meter_vs_time[kk]->SetTitle(Form("Rate Meter of %s (CH%i); Run time (s); PMT rate meter (Hz)", subdetector[kk], kk));
   }
   pmt_rate_meter_vs_time[58]->SetTitle("Null PMT");
   for(int kk=59;kk<65;kk++) {
      pmt_rate_meter_vs_time[kk]->SetTitle(Form("Rate Meter of %s (CH%i); Run time (s); PMT rate meter (Hz)", subdetector[kk-1], kk-1));
   }
   
   // Int_t OBDH_timestamp[1000];
   // Int_t OBDH_time_sec[1000];
   // Int_t OBDH_time_ms[1000];

   //   int cpuStartTime[Tmd_entries/2][2];
   int **cpuStartTime = new int*[Tmd_entries/2] ;

   for(int l=0; l<Tmd_entries/2; l++)
      cpuStartTime[l] = new int[2];
	 
   for(int j=1; j<Tmd_entries; j+=2) //Tmd loop
   {
      rootfile.GetTmdEntry(j); 
      cpuStartTime[(j-1)/2][0] = metaData.CPU_time[0];
      cpuStartTime[(j-1)/2][1] = metaData.run_id;
      // OBDH_time_sec[(j-1)/2] = metaData.broadcast.OBDH.sec;
      // OBDH_time_ms[(j-1)/2] = metaData.broadcast.OBDH.ms;
      // OBDH_timestamp[(j-1)/2] = metaData.timestamp.OBDH;
      
      for(int kk=0; kk<65; kk++)
	pmt_rate_meter_vs_time[kk]->SetPoint(j, metaData.CPU_time[0]/1000., metaData.PMT_rate_meter[kk]);
      
   }      
   // TDatime da(2009,01,01,00,00,00);
   // gStyle->SetTimeOffset(da.Convert());
   
   
   Int_t time_flag = 0;
   Int_t numevent_int = 0;
   Int_t sum[9];
   Int_t totevents = 0;
   for(int kk=0;kk<9;kk++)
      sum[kk] = 0;

   for(int i = 0; i < nEvents; i++) //Event loop
   {
      rootfile.GetEntry(i);
	    
      if(metaData.run_type == 0x634E) // to skip mixed virgin event
	 continue;

      
      // event_time = 1230764400+OBDH_time_sec[ev.run_id - first_run_nr];
      //event_time += cpu_startRunTime_vect[ev.run_id - first_run_nr] - OBDH_timestamp[(ev.run_id - first_run_nr] ;// -  ev.hepd_time/1e+2; 

      event_time = runIDtoStartTime(&cpuStartTime[0], Tmd_entries/2, ev.run_id) + ev.hepd_time/1e+2; //unit = ms //TODO: add broadcast time

      lost_triggers_vs_time->SetPoint(i, event_time/1000., ev.lost_trigger);
      alive_time_vs_time->SetPoint(i, event_time/1000., ev.alive_time*0.005);
      dead_time_vs_time->SetPoint(i, event_time/1000., ev.dead_time*0.005);


      
      for(int ch=0;ch<EASIROC_CH;ch++) //PMT channel loop ch[0-63]
	 if(ev.trigger_flag[ch] > 0)
	    h_FlagCount_vs_Ch->Fill(ch);

      if (i == 0){
	 time_flag = event_time + INTEGTIME;
	 // cout << "start time = " << time_flag << endl;
      }
      
      if (event_time < time_flag)
      {
	 numevent_int++;
	 for(int kk=0;kk<9;kk++)
	    sum[kk] += ev.rate_meter[kk];
      }
      else
      {
	 //cout << "num events = "<< numevent_int << endl;
	 for(int kk=0;kk<9;kk++)
	 {
	   
 	    rate_meter_vs_time[kk]->SetPoint(rate_meter_vs_time[kk]->GetN(), (time_flag-INTEGTIME/2.)/1000., (double)sum[kk]/numevent_int);

	    if(kk == 5)
	       totevents += ((sum[kk]/numevent_int));
	    sum[kk] = 0;
	 }
	 // cout << "start time = " << time_flag << endl;


	    
	 numevent_int = 1;
	 time_flag = event_time + INTEGTIME;
      }	 
   } //End of event loop
   std::cout << "totevents =  " << totevents*INTEGTIME << std::endl;
   
   TCanvas *c_lost_triggers = new TCanvas("c_lost_triggers","",1200,600);
   gPad->SetGrid();
   lost_triggers_vs_time->SetMarkerStyle(7);
   TString lost_trig_fig = "2_lost_triggers.png"; 
   lost_triggers_vs_time->Draw("AP");
   l.SetTextSize(0.04);
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   c_lost_triggers->SaveAs(lost_trig_fig);


   TCanvas *c_alive_dead_time = new TCanvas("c_alive_dead_time","",1200,600);
   c_alive_dead_time->Divide(2,1);
   c_alive_dead_time->cd(1);
   gPad->SetGrid();
   alive_time_vs_time->SetMarkerStyle(7);
   alive_time_vs_time->Draw("AP");
   // alive_time_vs_time->GetXaxis()->SetTimeDisplay(1);
   // alive_time_vs_time->GetXaxis()->SetTimeFormat("%H:%M:%s");
   c_alive_dead_time->cd(2);
   gPad->SetGrid();
   dead_time_vs_time->SetMarkerStyle(7);
   // dead_time_vs_time->GetXaxis()->SetTimeDisplay(1);
   // dead_time_vs_time->GetXaxis()->SetTimeFormat("%H:%M:%s");
   dead_time_vs_time->Draw("AP");
   TString alive_dead_time_fig = "3_alive_dead_time.png";    
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   c_alive_dead_time->SaveAs(alive_dead_time_fig);

  
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
   TString trig_flag_fig = "4_trigger_flag.png";    
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   c_trig_flag->SaveAs(trig_flag_fig);


   // Rate meter trigger mask
   TCanvas* c_rate_meter_trig_mask_0_3 = new TCanvas("c_rate_meter_trig_mask_0_3"," ",
						     1200,600);
   c_rate_meter_trig_mask_0_3->Divide(3,1);
  
   for(int p=0;p<3;p++) {
      c_rate_meter_trig_mask_0_3->cd(p+1);
      gPad->SetGrid();
      rate_meter_vs_time[p]->SetMarkerStyle(7);
      rate_meter_vs_time[p]->Draw("AP");
      //rate_meter_vs_time[p]->GetXaxis()->SetTimeDisplay(1);
   }
   TString rate_meter_trig_mask_0_3_fig = "5_rate_meter_trig_mask_0_3.png";    
   l.SetTextSize(0.06);
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   c_rate_meter_trig_mask_0_3->SaveAs(rate_meter_trig_mask_0_3_fig); 
  
   TCanvas* c_rate_meter_trig_mask_4_6 = new TCanvas("c_rate_meter_trig_mask_4_6"," ",
						     1200,600);
   c_rate_meter_trig_mask_4_6->Divide(3,1);
  
   for(int p=3;p<6;p++) {
      c_rate_meter_trig_mask_4_6->cd(p-2);
      gPad->SetGrid();
      rate_meter_vs_time[p]->SetMarkerStyle(7);
      rate_meter_vs_time[p]->Draw("AP");
      //rate_meter_vs_time[p]->GetXaxis()->SetTimeDisplay(1);
   }
   TString rate_meter_trig_mask_4_6_fig = "6_rate_meter_trig_mask_4_6.png";    
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   c_rate_meter_trig_mask_4_6->SaveAs(rate_meter_trig_mask_4_6_fig); 
  
   TCanvas* c_rate_meter_trig_mask_7_9 = new TCanvas("c_rate_meter_trig_mask_7_9"," ",
						     1200,600);
   c_rate_meter_trig_mask_7_9->Divide(3,1);
  
   for(int p=6;p<9;p++)
   {
      c_rate_meter_trig_mask_7_9->cd(p-5);
      gPad->SetGrid();
      rate_meter_vs_time[p]->SetMarkerStyle(7);
      rate_meter_vs_time[p]->Draw("AP");
      //rate_meter_vs_time[p]->GetXaxis()->SetTimeDisplay(1);
   }

   TString rate_meter_trig_mask_7_9_fig = "7_rate_meter_trig_mask_7_9.png";    
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   c_rate_meter_trig_mask_7_9->SaveAs(rate_meter_trig_mask_7_9_fig);


   //PMT rate meter
   TCanvas* c_rate_meter_pmt_1_9 = new TCanvas("c_rate_meter_pmt_1_9"," ",1200,600);
   c_rate_meter_pmt_1_9->Divide(3,3);
  
   for(int p=0;p<9;p++)
   {
      c_rate_meter_pmt_1_9->cd(p+1);
      gPad->SetGrid();
      pmt_rate_meter_vs_time[p]->SetMarkerStyle(7);
      pmt_rate_meter_vs_time[p]->Draw("AP");
   }

   TString rate_meter_pmt_1_9 = "8_pmt_rate_meter_1_9.png";
   l.SetTextSize(0.12); 
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c);  
   c_rate_meter_pmt_1_9->SaveAs(rate_meter_pmt_1_9);
   /*
   TCanvas* myc = new TCanvas("myc"," ",1200,600);
   pmt_rate_meter_vs_time[8]->SetMarkerStyle(20);
   pmt_rate_meter_vs_time[8]->Draw("AP");
   TString mys = "Trigger_example_pmtratemeter.png";
   myc->SaveAs(mys);
   */
   TCanvas* c_rate_meter_pmt_10_18 = new TCanvas("c_rate_meter_pmt_10_18"," ",1200,600);
   c_rate_meter_pmt_10_18->Divide(3,3);
  
   for(int p=9;p<18;p++)
   {
      c_rate_meter_pmt_10_18->cd(p-8);
      gPad->SetGrid();
      pmt_rate_meter_vs_time[p]->SetMarkerStyle(7);
      pmt_rate_meter_vs_time[p]->Draw("AP");
   }

   TString rate_meter_pmt_10_18 = "9_pmt_rate_meter_10_18.png";
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c);  
   c_rate_meter_pmt_10_18->SaveAs(rate_meter_pmt_10_18);

   TCanvas* c_rate_meter_pmt_19_27 = new TCanvas("c_rate_meter_pmt_19_27"," ",1200,600);
   c_rate_meter_pmt_19_27->Divide(3,3);
  
   for(int p=18;p<27;p++)
   {
      c_rate_meter_pmt_19_27->cd(p-17);
      gPad->SetGrid();
      pmt_rate_meter_vs_time[p]->SetMarkerStyle(7);
      pmt_rate_meter_vs_time[p]->Draw("AP");
   }

   TString rate_meter_pmt_19_27 = "10_pmt_rate_meter_19_27.png";
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   c_rate_meter_pmt_19_27->SaveAs(rate_meter_pmt_19_27);

   TCanvas* c_rate_meter_pmt_28_36 = new TCanvas("c_rate_meter_pmt_28_36"," ",1200,600);
   c_rate_meter_pmt_28_36->Divide(3,3);
  
   for(int p=27;p<36;p++)
   {
      c_rate_meter_pmt_28_36->cd(p-26);
      gPad->SetGrid();
      pmt_rate_meter_vs_time[p]->SetMarkerStyle(7);
      pmt_rate_meter_vs_time[p]->Draw("AP");
   }

   TString rate_meter_pmt_28_36 = "11_pmt_rate_meter_28_36.png";
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   c_rate_meter_pmt_28_36->SaveAs(rate_meter_pmt_28_36);

   TCanvas* c_rate_meter_pmt_37_45 = new TCanvas("c_rate_meter_pmt_37_45"," ",1200,600);
   c_rate_meter_pmt_37_45->Divide(3,3);
  
   for(int p=36;p<45;p++)
   {
      c_rate_meter_pmt_37_45->cd(p-35);
      gPad->SetGrid();
      pmt_rate_meter_vs_time[p]->SetMarkerStyle(7);
      pmt_rate_meter_vs_time[p]->Draw("AP");
   }

   TString rate_meter_pmt_37_45 = "12_pmt_rate_meter_37_45.png";
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c);  
   c_rate_meter_pmt_37_45->SaveAs(rate_meter_pmt_37_45);

   TCanvas* c_rate_meter_pmt_46_54 = new TCanvas("c_rate_meter_pmt_46_54"," ",1200,600);
   c_rate_meter_pmt_46_54->Divide(3,3);
  
   for(int p=45;p<54;p++)
   {
      c_rate_meter_pmt_46_54->cd(p-44);
      gPad->SetGrid();
      pmt_rate_meter_vs_time[p]->SetMarkerStyle(7);
      pmt_rate_meter_vs_time[p]->Draw("AP");
   }

   TString rate_meter_pmt_46_54 = "13_pmt_rate_meter_46_54.png";
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   c_rate_meter_pmt_46_54->SaveAs(rate_meter_pmt_46_54);

   TCanvas* c_rate_meter_pmt_55_64 = new TCanvas("c_rate_meter_pmt_55_64"," ",1200,600);
   c_rate_meter_pmt_55_64->Divide(3,3);
  
   for(int p=54;p<59;p++)
   {
      c_rate_meter_pmt_55_64->cd(p-53);
      gPad->SetGrid();
      pmt_rate_meter_vs_time[p]->SetMarkerStyle(7);
      pmt_rate_meter_vs_time[p]->Draw("AP");
   }
  
   for(int p=59;p<64;p++)
   {
      c_rate_meter_pmt_55_64->cd(p-54);
      gPad->SetGrid();
      pmt_rate_meter_vs_time[p]->SetMarkerStyle(7);
      pmt_rate_meter_vs_time[p]->Draw("AP");
   }

   TString rate_meter_pmt_55_64 = "14_pmt_rate_meter_55_64.png";
   sprintf(c,"%d",n++); l.DrawTextNDC(.98, 0.03, c); 
   c_rate_meter_pmt_55_64->SaveAs(rate_meter_pmt_55_64);


   const char *char_outname = outname;
   gROOT->ProcessLine(Form(".!convert `ls -v *.png` %s",char_outname));
   gROOT->ProcessLine(".!rm *.png");


   rootfile.Close();
   gErrorIgnoreLevel = 1;
   
}
