
/**
* =============================================================================
*
* Created by Cinzia De Donato @ INFN Roma2:
* cinzia.dedonato@roma2.infn.it
*
* FILENAME:       PlotRateQL.cc
*
* DESCRIPTION:    Script to generate Acquired Event Rate Quicklook pdf file starting 
*                 from HEPD L0 ROOT file 
*		  Plots of Event Rate vs CPU time, latitude and longitude
*
* DATE:           February 22, 2018
*     
* =============================================================================
**/



#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>

#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TPaveText.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TError.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include "LEvRec0File.hh"
#include "LEvRec0.hh"

#define INTEG_TIME 10000//integration time in ms
#define VERBOSE true
#define DEBUG false	
using namespace std;

TCanvas *earth();

const TString Get_VetoConf_Name(const int V);
const TString Get_TriggerMask_Name(const int TM);


void EventRateL0(TString rootname, TString outPath){

   gErrorIgnoreLevel = 5000 ;
    cout << "\n--------------------------------------------\n";
   
   cout << "\nQuickLook -> Running  EventRateL0\n";

   TString _temp = rootname ;
   cout << "\nInput ROOT  File: " <<rootname;

   _temp.Replace(0, _temp.Last('/')+1, "");

   TString input_name;
   input_name=_temp;

   TString outname=outPath;
   outname+="/";
   outname+= _temp;
   outname.ReplaceAll(".root", 5, "_EventRateQL.pdf", 16);

   cout<< "\n-----------------------------------------------\n";
   cout << "\nDIRECTORY: " <<outPath<<"\n";
   cout << "\nInput  ROOT File: " <<input_name<<"\n";
   cout << "\nOutput PDF File: " << outname <<"\n";
   cout << "\nRate Integration time= " << INTEG_TIME <<"\n";
   cout<< "\n-----------------------------------------------\n";

// data interfacing 

  LEvRec0File File(rootname.Data());

  LEvRec0 cev;
  LEvRec0Md cmd;

  File.SetTmdPointer(cmd);
  int N2_Tmd=File.GetTmdEntries();
  int N_Tmd=N2_Tmd/2;

  int Tmd_BootN[N_Tmd];
  int BOOT_temp[N_Tmd];
  int Tmd_runN[N_Tmd];

  double Tmd_run_time[N_Tmd];
  double CPU_run_time[N_Tmd][2];

  int Tmd_run_Nevents[N_Tmd];
  int Tmd_run_orbit[N_Tmd][2];
  double Tmd_run_long[N_Tmd][2];
  double Tmd_run_lat[N_Tmd][2];
  int Tmd_run_TriggerMask[N_Tmd];
  int Tmd_run_Veto[N_Tmd];

  TString Orbit_Zone[6];
  Orbit_Zone[0]="SAA";
  Orbit_Zone[1]="Equatorial";
  Orbit_Zone[2]="South Polar";
  Orbit_Zone[3]="North Polar";
  Orbit_Zone[4]="Default";
  Orbit_Zone[5]="STDBY";

  TString TriggerMask[9];
  for (int iTM=0; iTM<9; iTM++)    TriggerMask[iTM]=Get_TriggerMask_Name(iTM);

  TString Veto[4];
  for (int iV=0; iV<4; iV++)    Veto[iV]=Get_VetoConf_Name(iV);

  for (int ir=0; ir<N_Tmd; ir++){
	Tmd_BootN[ir]=0;
        BOOT_temp[ir]=0;
	Tmd_runN[ir]=0;
	Tmd_run_time[ir]=0;
	CPU_run_time[ir][0]=0;
	CPU_run_time[ir][1]=0;
	Tmd_run_Nevents[ir]=0;
	Tmd_run_orbit[ir][0]=0;//first: calculated orbital zone
	Tmd_run_orbit[ir][1]=0;//second: applied orbital zone
	Tmd_run_long[ir][0]=0;
	Tmd_run_long[ir][1]=0;
	Tmd_run_lat[ir][0]=0;
	Tmd_run_lat[ir][1]=0;
  	Tmd_run_TriggerMask[ir]=0;
  	Tmd_run_Veto[ir]=0;
  }
  

  int NN_boot=0;// boot counter 
  for(int ir=0; ir<N2_Tmd; ++ir) {
  	File.GetTmdEntry(ir); 
  	if (ir%2==0){
		Tmd_BootN[ir/2]=cmd.boot_nr;
        	bool boot2=true;
		if (ir>0){
			for (int irr=0; irr<ir/2; irr++){
				if (Tmd_BootN[ir/2]==Tmd_BootN[irr]) boot2=false;
			}
		}	
		else boot2=true;// (Tmd_BootN[0])
		if (boot2==true) {
			BOOT_temp[NN_boot]=Tmd_BootN[ir/2];
			NN_boot++;
		}
	
		Tmd_runN[ir/2]=cmd.run_id;
		Tmd_run_long[ir/2][0]=(cmd.broadcast.GPS.lon)*pow(10,-7);
		Tmd_run_lat[ir/2][0]=(cmd.broadcast.GPS.lat)*pow(10,-7);
		Tmd_run_orbit[ir/2][0]=(cmd.orbit_Zone>>8);//Calculated
		Tmd_run_orbit[ir/2][1]=(cmd.orbit_Zone& 0x00FF);//applied
		Tmd_run_TriggerMask[ir/2]=cmd.trigger_mask[1];
		Tmd_run_Veto[ir/2]=cmd.trigger_mask[0];
	}
	if (ir%2!=0){
		CPU_run_time[(ir-1)/2][0]=cmd.CPU_time[0];
		CPU_run_time[(ir-1)/2][1]=cmd.CPU_time[1];//ms
		Tmd_run_long[(ir-1)/2][1]=(cmd.broadcast.GPS.lon)*pow(10,-7);
		Tmd_run_lat[(ir-1)/2][1]=(cmd.broadcast.GPS.lat)*pow(10,-7);
	}
 	Tmd_run_time[ir/2]=(cmd.CPU_time[1]-cmd.CPU_time[0])/1000;//seconds
  }//end loop on Tmd
  

  cout<< "\n\tTotal number of boots="<< NN_boot;
  for (int nb=0; nb< NN_boot;nb++)        std::cout<< "\n\tBoot N["<<nb<<"] = "<<BOOT_temp[nb];
  cout << "\n\tTMd entries = "<< N2_Tmd<<" -> Run entries = "<< N_Tmd;
  File.SetTheEventPointer(cev);
  int nEntries=File.GetEntries();
  cout<< "\n\tEvent Entries="<<nEntries<<"\n";

  if (VERBOSE==true){
	for (int ir=0; ir<N_Tmd; ir++){

		cout << "\n***********************************************\n";
		cout << "TMd entry "<< ir<<": boot_nr "<< Tmd_BootN[ir]<<"\n";
		cout << "TMd entry "<< ir<<": run_nr "<<Tmd_runN[ir]<<"\n";
		cout << "TMd entry "<< ir<<": start time "<<CPU_run_time[ir][0]<<" ms\n";
  		cout << "TMd entry "<< ir<<": stop time "<< CPU_run_time[ir][1]<<" ms\n";
  		cout << "TMd entry "<< ir<<": run time dt "<< Tmd_run_time[ir]<<" s\n";
 		cout << "Tmd entry "<< ir<<": Orbit zone: Calculated-Applied "<< Tmd_run_orbit[ir][0] <<" - " << Tmd_run_orbit[ir][1]<<"\n";
		cout << "Tmd entry "<< ir<<": Orbit zone: Applied Orbit_Zone = "<< Orbit_Zone[Tmd_run_orbit[ir][1]]<<"\n";
		cout << "Tmd entry "<< ir<<": Orbit zone: Trigger Mask "<< Tmd_run_TriggerMask[ir] <<" - Veto " << Tmd_run_Veto[ir]<<"\n";
		cout << "Tmd entry "<< ir<<": Orbit zone latitude = [ "<< Tmd_run_lat[ir][0]<< " -> "<< Tmd_run_lat[ir][1]<<" ]\n";
		cout << "Tmd entry "<< ir<<": Orbit zone longitude = ["<< Tmd_run_long[ir][0]<< " -> "<< Tmd_run_long[ir][1]<<" ]\n";
	}
  }  
  
  if (nEntries==0) {
        std::cout<< "\n\n--------------------- TOTAL NUMBER OF ACQUIRED EVENTS=0! ---------------------\n\n";
        std::cout<< "NO EVENT RATE QL PRODUCED!!!";
        std::cout<< "\n\n------------------------------------------------------------------------------\n\n";

        return;
  }
        

/// PDF OUTPUT ////////////

   TString outnameStart;
   TString outnameEnd;
   TCanvas *cRun = new TCanvas("Run");
   TPaveText *pt = new TPaveText(.1,.1,.9,.9);
   pt->AddText("DIRECTORY:");
   pt->AddText(outPath);
   pt->AddText("INPUT ROOT file:");
   pt->AddText(input_name);
   pt->AddText(Form("Total number of boots= %d", NN_boot));
   pt->AddText(Form("TMd entries = %d -> Run entries = %d", N2_Tmd, N_Tmd));
   pt->AddText(Form("N events= %d", File.GetEntries()));
   pt->SetTextSize(0.02);
   pt->Draw();
   outnameStart = outname+"(";
   cRun->Print(outnameStart);
 int Boot_evEntries[NN_boot];

  for (int nb=0; nb< NN_boot; nb++) 	Boot_evEntries[nb]=0;
  for (int ir=0; ir<N_Tmd; ir++){
	if (ir%2==0){
		cRun= new TCanvas("Run");
		cRun->Divide (1, 2);
		pt= new TPaveText(.1,.55,.9,.9);
	}
	if (ir%2!=0)	pt= new TPaveText(.1,.1,.9,.45);
	pt->SetTextSize(0.03);
	pt->AddText(Form("BootN= %d, RunN= %d", Tmd_BootN[ir], Tmd_runN[ir] ));
	pt->AddText(Form("CPU start-stop time (s): [%4.3f - %4.3f]", CPU_run_time[ir][0]/1000, CPU_run_time[ir][1]/1000));
	pt->AddText(Form("Orbit zone: Calculated-Applied %d - %d",  Tmd_run_orbit[ir][0],  Tmd_run_orbit[ir][1]));
	pt->AddText(Form("Applied Orbit Zone Configuration = ")+  Orbit_Zone[Tmd_run_orbit[ir][1]]);
	pt->AddText(Form("    TriggerMask [%d] -> ", Tmd_run_TriggerMask[ir])+TriggerMask[Tmd_run_TriggerMask[ir]]);
	pt->AddText(Form("    Veto  [%d] -> ", Tmd_run_Veto[ir])+ Veto[Tmd_run_Veto[ir]]);
	pt->AddText(Form("Latitude [ %4.3f -> %4.3f ]", Tmd_run_lat[ir][0],  Tmd_run_lat[ir][1]));
	pt->AddText(Form("Longitude [ %4.3f -> %4.3f ]", Tmd_run_long[ir][0],  Tmd_run_long[ir][1]));
	cRun->cd();
	pt->Draw();
	if (ir%2!=0 || ir==N_Tmd-1) cRun->Print(outname);
}
/////////////////////////////////////GRAPH INITIALIZATION *************************
  for (int nb=0; nb< NN_boot; nb++){

	TGraph *average_rate[5];
	const char *gname[5];
	const char *gtitle[5];

	gname[0]=Form("BootN_%d_Applied_SAA_rate_time", BOOT_temp[nb]);
	gname[1]=Form("BootN_%d_Applied_Eq_rate_time", BOOT_temp[nb]);
	gname[2]=Form("BootN_%d_Applied_SouthPol_rate_time", BOOT_temp[nb]);
	gname[3]=Form("BootN_%d_Applied_NorthPol_rate_time", BOOT_temp[nb]);
	gname[4]=Form("BootN_%d_Applied_Default_rate_time", BOOT_temp[nb]);//no broadcast available
	
	gtitle[0]=Form("Boot_%d_Applied_Config_SAA", BOOT_temp[nb]);
	gtitle[1]=Form("Boot_%d_Applied_Config_Eq", BOOT_temp[nb]);
	gtitle[2]=Form("Boot_%d_Applied_Config_SouthPol", BOOT_temp[nb]);
	gtitle[3]=Form("Boot_%d_Applied_Config_NorthPol", BOOT_temp[nb]);
	gtitle[4]=Form("Boot_%d_Applied_Config_DefaultZone", BOOT_temp[nb]);//no broadcast available


	TGraph *average_rate2[5];


	const char *gname2[5];
	const char *gtitle2[5];

	gname2[0]=Form("BootN_%d_Applied_SAA_rate_lat", BOOT_temp[nb]);
	gname2[1]=Form("BootN_%d_Applied_Eq_rate_lat", BOOT_temp[nb]);
	gname2[2]=Form("BootN_%d_Applied_SouthPol_rate_lat", BOOT_temp[nb]);
	gname2[3]=Form("BootN_%d_Applied_NorthPol_rate_lat", BOOT_temp[nb]);
	gname2[4]=Form("BootN_%d_Applied_Default_rate_lat", BOOT_temp[nb]);//no broadcast available

 	for (int ig=0; ig<5; ig++){	
		average_rate[ig]= new TGraph();
		average_rate2[ig]= new TGraph();
 	}
        TH2D *h_events_orbit;
        const char *h2_name;
        h2_name=Form("BootN_%d_Orbit_events", BOOT_temp[nb]);
        h_events_orbit=new TH2D(h2_name, h2_name,361,-180,+180, 181, -90, +90);


//////////////////////////////////////////////////////////////////////////////

// event loop
  	UInt_t event_time;
	Int_t time_flag=0;
  	Int_t time_flag_old=0;

	Int_t numevent_int=0;
	double event_lat=0;
	double event_long=0;
	int zone=-1;
        double MaxY=0;
	double lat_dt=0;
        double long_dt=0;

	Int_t check_events=0;
  	for(int ie=0; ie<nEntries; ++ie) {
        	File.GetEntry(ie);
        	for (int ir=0; ir<N_Tmd; ir++){
			 if (Tmd_BootN[ir]==BOOT_temp[nb] && cev.run_id==Tmd_runN[ir] && cev.boot_nr==Tmd_BootN[ir]){
		  		event_time = CPU_run_time[ir][0]+ cev.hepd_time/1e+2; //unit = ms
				if ( Tmd_run_orbit[ir][0]!=0xAA  ) {//if no broadcast no latitude-longitude
					lat_dt= (Tmd_run_lat[ir][1]- Tmd_run_lat[ir][0])/(CPU_run_time[ir][1]-CPU_run_time[ir][0]);
					long_dt= (Tmd_run_long[ir][1]- Tmd_run_long[ir][0]);
					if (long_dt > 180)
	                                   long_dt-= 360;
        	                        else if (long_dt < -180)
                	                   long_dt+= 360;
					long_dt/=(CPU_run_time[ir][1]-CPU_run_time[ir][0]);
			
					event_lat =  Tmd_run_lat[ir][0]+(event_time-CPU_run_time[ir][0])*lat_dt;
					event_long = Tmd_run_long[ir][0]+(event_time-CPU_run_time[ir][0])*long_dt;
					if (event_long>=180.) event_long-=360.;
					if (event_long<-180.) event_long+=360.;
					h_events_orbit->Fill(event_long, event_lat);
				}
      				if (ie == 0)    {
					time_flag = CPU_run_time[ir][0]+ INTEG_TIME;
					time_flag_old = CPU_run_time[ir][0];
					zone= Tmd_run_orbit[ir][1];		
				}

				if (event_time>=time_flag_old && event_time   < time_flag)     {
					numevent_int++;
					zone= Tmd_run_orbit[ir][1];		
				}
       				else {
					if ((time_flag-time_flag_old)>0) {
						average_rate[zone]->SetPoint(average_rate[zone]->GetN(), time_flag/1.e3, numevent_int/((time_flag-time_flag_old)/1.e3));
						MaxY=max(MaxY,numevent_int/((time_flag-time_flag_old)/1.e3)); 
						check_events+=numevent_int;
						if (DEBUG==true){
							std::cout<< "\nset Point "<< average_rate[zone]->GetN()<<" : zone="<<zone<<"\n\tnumevent_int= "<< numevent_int<< ", dt (s)="<<(time_flag-time_flag_old)/1.e3<<  "\n\tx=time_flag (s)="<<time_flag/1.e3<< ", y=rate(Hz)="<<  numevent_int/((time_flag-time_flag_old)/1.e3);
						}
						if ( Tmd_run_orbit[ir][0]!=0xAA  ) {//if no broadcast no latitude-longitude 
						 //lat and long correspondent to time_flag+INTEG_TIME
                                        	        event_long=Tmd_run_long[ir][0]+(time_flag-CPU_run_time[ir][0])*long_dt;
                                                	if (event_long>=180.) event_long-=360.;
                                                	if (event_long<-180.) event_long+=360.;
                                                	event_lat=Tmd_run_lat[ir][0]+(time_flag-CPU_run_time[ir][0])*lat_dt;
 							average_rate2[zone]->SetPoint(average_rate2[zone]->GetN(), event_lat, numevent_int/((time_flag-time_flag_old)/1.e3));
						}
					}
					zone= Tmd_run_orbit[ir][1];		
					if (event_time<CPU_run_time[ir][1]) time_flag_old= event_time;
					else time_flag_old=CPU_run_time[ir][1];						
					if (event_time + INTEG_TIME<CPU_run_time[ir][1]) time_flag = event_time + INTEG_TIME;
					else {
						time_flag = CPU_run_time[ir][1]+50;
					}
					numevent_int = 1;
				
         			}
				if (ie==nEntries-1){//last events
					average_rate[zone]->SetPoint(average_rate[zone]->GetN(), time_flag/1.e3, numevent_int/((time_flag-time_flag_old)/1.e3));
					MaxY=max(MaxY,numevent_int/((time_flag-time_flag_old)/1.e3));
					check_events+=numevent_int;
					if (DEBUG==true){
						std::cout<< "\nset LAST Point "<< average_rate[zone]->GetN()<<": zone="<<zone<<"\n\tnumevent_int= "<< numevent_int<< ", dt (s)="<<(time_flag-time_flag_old)/1.e3<<  "\n\tx=time_flag (s)="<<time_flag/1.e3<< ", y=rate(Hz)="<<  numevent_int/((time_flag-time_flag_old)/1.e3);
					}

                                        if ( Tmd_run_orbit[ir][0]!=0xAA  ) {//if no broadcast no latitude-longitude 
                                                 //lat and long correspondent to time_flag+INTEG_TIME
	                                        event_long=Tmd_run_long[ir][0]+(time_flag-CPU_run_time[ir][0])*long_dt;
                                                if (event_long>=180.) event_long-=360.;
                                                if (event_long<-180.) event_long+=360.;
                                                event_lat=Tmd_run_lat[ir][0]+(time_flag-CPU_run_time[ir][0])*lat_dt;
                                                average_rate2[zone]->SetPoint(average_rate2[zone]->GetN(), event_lat, numevent_int/((time_flag-time_flag_old)/1.e3));
                                        }
				}
      			}
                }//Tmd loop
      }// event loop
      Boot_evEntries[nb]=check_events;
      if (VERBOSE){
	      std::cout<< "\n------------------------------------------------------------------------------\n";
	      std::cout<< "\nBOOT N. "<<BOOT_temp[nb];
	      std::cout<< "\nTotal number of events="<< nEntries<< "\t BOOT N."<<BOOT_temp[nb]<< " number of events="<<Boot_evEntries[nb] ;
	      std::cout<< "\n------------------------------------------------------------------------------\n";
	}
//  output
//AVERAGE RATE L0 as a function of RunID and calculated orbital zone

      if (Boot_evEntries[nb]>0){

 	for (int ig=0; ig<5; ig++){
 		average_rate[ig]->GetYaxis()->SetTitle("Rate (Hz)");
		average_rate[ig]->GetXaxis()->SetTitle("CPU time (s)");
		average_rate[ig]->SetMarkerStyle(20);
		average_rate[ig]->SetMarkerSize(0.4);

	 	average_rate[ig]->SetTitle(gtitle[ig]);
		average_rate[ig]->SetName(gname[ig]);
		average_rate[ig]->SetFillStyle(0);
		average_rate[ig]->SetDrawOption("AP");	
		average_rate[ig]->SetLineWidth(0);

 		average_rate2[ig]->GetYaxis()->SetTitle("Rate (Hz)");
		average_rate2[ig]->GetXaxis()->SetTitle("latitude");
		average_rate2[ig]->SetMarkerStyle(20);
		average_rate2[ig]->SetMarkerSize(0.4);

	 	average_rate2[ig]->SetTitle(gtitle[ig]);
		average_rate2[ig]->SetName(gname2[ig]);
		average_rate2[ig]->SetFillStyle(0);
		average_rate2[ig]->SetDrawOption("AP");	
		average_rate2[ig]->SetLineWidth(0);
	}
	average_rate[0]->SetMarkerColor(kRed);
	average_rate[1]->SetMarkerColor(kBlack);
 	average_rate[2]->SetMarkerColor(kBlue);
	average_rate[3]->SetMarkerColor(kGreen);
	average_rate[4]->SetMarkerColor(kMagenta);

	average_rate2[0]->SetMarkerColor(kRed);
	average_rate2[1]->SetMarkerColor(kBlack);
 	average_rate2[2]->SetMarkerColor(kBlue);
	average_rate2[3]->SetMarkerColor(kGreen);
	average_rate2[4]->SetMarkerColor(kMagenta);

 	TCanvas *crate=new TCanvas();
	TString mg_name=Form("BootN_%d_orbital_rate_time", BOOT_temp[nb]);
	crate->SetTitle(mg_name);
	crate->SetName(mg_name);

   
	TMultiGraph *mg = new TMultiGraph();
	mg->SetTitle(mg_name);
	mg->SetName(mg_name);


 	for (int ig=0; ig<5; ig++){
        	if (average_rate[ig]->GetN()>0)	mg->Add(average_rate[ig]); 
 	}
 
	crate->cd();
	mg->SetTitle(mg_name + Form(" - rate (Hz) vs CPU time [Integ_time=%d s]",INTEG_TIME/1000));
	mg->Draw("AP");
	mg->GetYaxis()->SetTitle("Rate (Hz)");
	mg->GetXaxis()->SetTitle("CPU time (s)");
	mg->GetYaxis()->SetRangeUser(0, MaxY*1.5);
	crate-> BuildLegend();	
	crate->Print(outname);


 	TCanvas *crate2=new TCanvas();
	TString mg_name2=Form("BootN_%d_orbital_rate_lat", BOOT_temp[nb]);
	crate2->SetTitle(mg_name2);
	crate2->SetName(mg_name2);
   
	TMultiGraph *mg2 = new TMultiGraph();
	mg2->SetTitle(mg_name2);
	mg2->SetName(mg_name2);


 	for (int ig=0; ig<5; ig++){
        	if (average_rate2[ig]->GetN()>0) mg2->Add(average_rate2[ig]); 
 	}
 
	crate2->cd();
	mg2->SetTitle(mg_name2 + Form(" - rate (Hz) vs latitude [Integ_time=%d s]",INTEG_TIME/1000));
	mg2->Draw("AP");
	mg2->GetYaxis()->SetTitle("Rate (Hz)");
	mg2->GetXaxis()->SetTitle("latitude");
	mg2->GetYaxis()->SetRangeUser(0, MaxY*1.5);

        crate2-> BuildLegend();
	crate2->Print(outname);


	TCanvas *crate3= earth();//new TCanvas(); 
	TString mg_name3=Form("BootN_%d_orbital_events_lat_long", BOOT_temp[nb]);
	crate3->SetTitle(mg_name3);
	crate3->SetName(mg_name3);
	crate3->cd();
	h_events_orbit->GetYaxis()->SetTitle("Latitude");
        h_events_orbit->GetXaxis()->SetTitle("Longitude");
        h_events_orbit->GetZaxis()->SetTitle("#events");

        h_events_orbit->SetTitle(h2_name);
        h_events_orbit->SetName(h2_name);
        h_events_orbit->SetDrawOption("COLZ");
	gStyle->SetOptStat(0);
	h_events_orbit->Draw("SAME, COLZ");
	crate3->Print(outname);
	TFile *filetest = new TFile("filetest.root","recreate");
        crate3->Write();
        filetest->Close();

 	}//end if Boot_evEntries[nb]>0

}//end loop nb
   pt = new TPaveText(.1,.1,.9,.9);
   cRun = new TCanvas("Run");
   pt->AddText(Form("Total number of boots= %d", NN_boot));
   pt->AddText(Form("Total N. events= %d", File.GetEntries()));
   for (int nb=0; nb<NN_boot; nb++)  pt->AddText(Form("%d) BOOT N. %d -> Number of events  = %d ", nb+1, BOOT_temp[nb], Boot_evEntries[nb]));
   pt->SetTextSize(0.02);

   pt->Draw();
   outnameEnd = outname+")";
   cRun->Print(outnameEnd);


  File.Close();
  cout << "\nEventRateL0 ... done \nOutput PDF File: " << outname << "\n\n";
    cout << "\n--------------------------------------------\n";

  gErrorIgnoreLevel = 1;

}

TCanvas *earth(){
   gStyle->SetOptTitle(1);
   gStyle->SetOptStat(0);
   TCanvas *c1 = new TCanvas("c1","earth_projections");

  TString dat = gROOT->GetTutorialsDir();
   dat.Append("/graphics/world.dat");
   std::ifstream infile(dat.Data());
   if (infile){
      TGraph*g = new TGraph(dat);
      c1->cd();
      TH2F *hm = new TH2F("hm","",  180, -180, 180, 161, -80.5, 80.5);
      TH2F *hoverlay = new TH2F("hover","",  180, -180, 180, 161, -80.5, 80.5);
      hm->Draw("");
      g->Draw("p same");
      hm->GetYaxis()->SetTitleFont(132);
      hm->GetYaxis()->SetLabelFont(132);
      hm->GetXaxis()->SetTitleFont(132);
      hm->GetXaxis()->SetLabelFont(132);
      hm->GetXaxis()->SetTitle("Longitude (deg)");
      hm->GetYaxis()->SetTitle("Latitude (deg)");
   }
   else {
//      TH2F *ha = new TH2F("ha","Aitoff",    180, -180, 180, 179, -89.5, 89.5);
      TH2F *hm = new TH2F("hm","",  180, -180, 180, 161, -80.5, 80.5);
//      TH2F *hs = new TH2F("hs","Sinusoidal",180, -180, 180, 181, -90.5, 90.5);
//      TH2F *hp = new TH2F("hp","Parabolic", 180, -180, 180, 181, -90.5, 90.5);
      TString dat2 = gROOT->GetTutorialsDir();
      dat2.Append("/graphics/earth.dat");
      dat2.ReplaceAll("/./","/");
      ifstream in;
      in.open(dat2.Data());
      Float_t x,y;
      while (1) {
         in >> x >> y;
         if (!in.good()) break;
         // ha->Fill(x,y, 1);
         hm->Fill(x,y, 1);
         // hs->Fill(x,y, 1);
         // hp->Fill(x,y, 1);
      }
      in.close();
      // c1->cd(1); ha->Draw("aitoff");
      hm->SetLineColor(1);
      hm->SetLineWidth(1);
      hm->Draw("cont2");

      // c1->cd(3); hs->Draw("sinusoidal");
      // c1->cd(4); hp->Draw("parabolic");
   }
   return c1;
}


//////////////////////////////////////////////////////////////

const TString Get_TriggerMask_Name(const int TM){

  if (TM<0 || TM>8) {
        std::cout<< "\nTrigger Mask Config "<< TM<< " inexistent!\n";
        return "NA";
  }
  TString TriggerMask_name[9];
  TriggerMask_name[0]="T";
  TriggerMask_name[1]="T & P1";
  TriggerMask_name[2]="T & (P1||P2)";
  TriggerMask_name[3]="(T3||T4) & (P1||P2)";
  TriggerMask_name[4]="T & P1 & P2";
  TriggerMask_name[5]="T & P1 & P2 & P3";
  TriggerMask_name[6]="T & (P1||P2) & (P15||P16)";
  TriggerMask_name[7]="T & (P1||P2) & L";
  TriggerMask_name[8]="Generic";
  return TriggerMask_name[TM];
}
//-------------------------------------------------------------------------//
const TString Get_VetoConf_Name(const int V){
  if (V<0 || V>3) {
        std::cout<< "\nVeto Config "<< V<< " inexistent!\n";
        return "NA";
  }
  TString Veto_name[4];
  Veto_name[0]="No Veto";
  Veto_name[1]="Lateral Veto";
  Veto_name[2]="Bottom Veto";
  Veto_name[3]="Lateral+ Bottom Veto";
  return Veto_name[V];
}

//-------------------------------------------------------------------------//

