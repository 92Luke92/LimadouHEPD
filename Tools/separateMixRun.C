/**
 * =============================================================================
 *
 * Created by Alessandro Sotgiu: alessandro.sotgiu@roma2.infn.it
 *
 * FILENAME :        separateMixRun.C
 *
 * DESCRIPTION :     Script to separate virgin/compressed events inside a mixed run
 *                   
 *
 * INSTRUCTIONS:
 *     compile:    g++ -Wall separateMixRun.C -o separate `root-config --cflags --libs`
 *     run:        ./separate <root file> <outdir>
 *
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <TVector3.h>
#include <TTimeStamp.h>
#include <TTree.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TBranch.h>
#include <iostream>
#include <string>

#include "bin2root.h"

using namespace std;

void separateMixedRun(string rootname, string outdir);


void separateMixedRun(string rootname, string outdir)
{

   tail_run_t tail_run;

   //T Tree
   UShort_t run_type;
   UShort_t ev_boot_nr;
   UShort_t ev_run_id;
   UInt_t event_index;
   UShort_t event_length;
   UInt_t hepd_time;
   UInt_t trigger_index;

   UInt_t PMTBoard_trigger_counter;
   UShort_t lost_trigger;
   UShort_t rate_meter[9];
   UShort_t pmt_high[EASIROC_CH];
   UShort_t pmt_low[EASIROC_CH];
   Bool_t trigger_flag[EASIROC_CH];
   Short_t strip[SILLEN];
   UInt_t alive_time;
   UInt_t dead_time;

   bool   pmt_mask[64];
   bool   hv_mask[12];
   bool   generic_trig_mask[18];

   UShort_t PMT_rate_meter[65];
   Short_t CPU_temp[2];
   Short_t PMT_temp[2];
   UInt_t CPU_time[2];


   //TFile *rootfile = new TFile(rootname,"read");
   TFile *rootfile = new TFile(rootname.c_str(),"read");

   TTree* Tmd = (TTree*)rootfile->Get("Tmd");
   Tmd->SetBranchAddress("boot_nr", &tail_run.boot_nr);
   Tmd->SetBranchAddress("run_id", &tail_run.run_id);
   Tmd->SetBranchAddress("run_type", &tail_run.run_type);
   Tmd->SetBranchAddress("run_duration", &tail_run.run_duration);
   Tmd->SetBranchAddress("orbitZone", &tail_run.orbit_Zone);

   Tmd->SetBranchAddress("silConfiguration", &tail_run.silConfig.ladder_on);

   Tmd->SetBranchAddress("trigger_mask[2]", &tail_run.trigger_mask[2]);
   Tmd->SetBranchAddress("easiroc_conf[60]", &tail_run.easiroc_config[0]); // todo: fix header/tail size
   Tmd->SetBranchAddress("PMT_mask[64]", &pmt_mask[0]);
   Tmd->SetBranchAddress("HV_mask[12]", &hv_mask[0]);
   Tmd->SetBranchAddress("HV_value[10]", &tail_run.HV_value[0]);
   Tmd->SetBranchAddress("gen_trig_mask[18]", &generic_trig_mask[0]);

   // broadcasta data
   Tmd->SetBranchAddress("OBDH_info", &tail_run.broadcast.OBDH.sec);
   Tmd->SetBranchAddress("GPS_info", &tail_run.broadcast.GPS.sec);
   Tmd->SetBranchAddress("AOCC_info", &tail_run.broadcast.AOCC.sec);
   Tmd->SetBranchAddress("star_sensor_info", &tail_run.broadcast.star_sensor.sec_ss11);
   
   // scinentific packet
   Tmd->SetBranchAddress("PMT_rate_meter[65]", &PMT_rate_meter[0]);
   Tmd->SetBranchAddress("CPU_temp_start_stop_Run[2]", &CPU_temp[0]);
   Tmd->SetBranchAddress("PMT_temp_start_stop_Run[2]", &PMT_temp[0]);
   Tmd->SetBranchAddress("CPU_time_start_stop_Run[2]", &CPU_time[0]);
   
   // timestamp CPU
   Tmd->SetBranchAddress("CPU_timestamp", &tail_run.timestamp.OBDH);
   
   // board status
   Tmd->SetBranchAddress("status_register", &tail_run.status_reg.statusDAQ);
   
   
   TTree* T = (TTree*)rootfile->Get("T");

   // event header
   T->SetBranchAddress("run_type", &run_type);
   T->SetBranchAddress("ev_boot_nr", &ev_boot_nr);
   T->SetBranchAddress("ev_run_id", &ev_run_id);
   T->SetBranchAddress("event_index", &event_index);     
   T->SetBranchAddress("event_length", &event_length);
   T->SetBranchAddress("trigger_index", &trigger_index);
   T->SetBranchAddress("hepd_time", &hepd_time);
   
   // trigger board data
   T->SetBranchAddress("pmt_high[64]", &pmt_high[0]); 
   T->SetBranchAddress("pmt_low[64]", &pmt_low[0]);
   T->SetBranchAddress("PMTBoard_trigger_counter",&PMTBoard_trigger_counter);
   T->SetBranchAddress("lost_trigger", &lost_trigger);
   T->SetBranchAddress("rate_meter[9]", &rate_meter[0]);  
   T->SetBranchAddress("trigger_flag[64]", &trigger_flag[0]);
   T->SetBranchAddress("alive_time", &alive_time);
   T->SetBranchAddress("dead_time", &dead_time);

   // silicon data
   T->SetBranchAddress("strip[4608]", &strip[0]);

   string outnameCOMP= outdir;
   outnameCOMP.append("/");
   outnameCOMP.append(rootname.substr(rootname.find_last_of("\\/")));
   
   outnameCOMP.erase(outnameCOMP.end()-5);
   outnameCOMP.append("_COMP.root");
   string outnameVIRGIN=outdir;
   outnameVIRGIN.append("/");
   outnameVIRGIN.append(rootname.substr(rootname.find_last_of("\\/")));
   
   outnameVIRGIN.erase(outnameVIRGIN.end()-5);
   outnameVIRGIN.append("_VIRGIN.root");
   
   Int_t nEvents = T->GetEntries();
   
   
   TFile *newfile2 = new TFile(outnameVIRGIN.c_str(),"recreate");
   TTree *virtree = T->CloneTree(0);
   TTree *newtmdv = Tmd->CloneTree(0);
   /*
   TFile *newfile1 = new TFile(outnameCOMP.c_str(),"recreate");
   TTree *newtmdc = Tmd->CloneTree(0);
   TTree *comptree = T->CloneTree(0);
   */
   
   for(int i=0;i<nEvents;i++)
   {
      //cout << "entry: " << i  << endl;
      T->GetEntry(i);

      if(run_type == 0x634E)
      	 virtree->Fill();
      /*
      if(run_type == 0x6336) 
	 comptree->Fill();
      */
   }

   for(int i=0; i<2; i++) 
   {
      Tmd->GetEntry(i);
      //newtmdc->Fill();
      newtmdv->Fill();
   }
   /*
   newfile1->cd();
   newtmdc->Write();
   comptree->Write();
   */
   
   
   newfile2->cd();
   newtmdv->Write();
   virtree->Write();
   
   //newfile1->Close(); 
   newfile2->Close();
   rootfile->Close();
}


int main(int argc, char** argv){
  separateMixedRun(argv[1],argv[2]);

   return 0;
}


