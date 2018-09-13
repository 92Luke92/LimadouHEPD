
/**
* =============================================================================
*
* Created by Cinzia De Donato @ INFN Roma2:
* cinzia.dedonato@roma2.infn.it
*
* FILENAME:       PlotRateTM_EGSE_QL.cc
*
* DESCRIPTION:    Script to generate Acquired Event Rate Quicklook pdf file starting 
*                 from HEPD L0 ROOT file for EGSE acquisition (NO BROADCASTS)
*		  Plots of Event Rate and Trigger MAsk Rate Meter vs CPU time
*
* DATE:           July 5, 2018
*     
* =============================================================================
**/



#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>
#include <TDatime.h>

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

#define OBDH_TO_EPOCH_OFFSET_SEC 1230768000
#define INTEG_TIME 10000//integration time in ms
#define RUN_DT 600 //seconds; if tail missing CPU_run_time set as [0,600] seconds

#define VERBOSE true
#define DEBUG false

#define PRINT_METADATA true
#define output_PDF true
#define output_DAT false
using namespace std;

const TString Get_VetoConf_Name(const int V);
const TString Get_TriggerMask_Name(const int TM);


void EventRateTM_EGSE(TString rootname, TString outPath){

   gErrorIgnoreLevel = 5000 ;
    cout << "\n--------------------------------------------\n";
   
   cout << "\nQuickLook -> Running  EventRateTM_EGSE\n";
   const char * _temp = rootname;
    cout << "\ninput ROOT  File : " << rootname ;
    cout << "\nRate Integration time= " << INTEG_TIME << "\n";


//PDF output
    TString outname=outPath;
    outname+="/";
    outname+= basename(_temp);
    outname.ReplaceAll(".root", 5, "_EventRateQL_TM.pdf", 19);
    if (output_PDF)    cout << "\nOutput PDF File : " << outname << "\n";


//DAT OUTPUT
    FILE * fout;
    TString datname;
    datname = outname;
    datname.ReplaceAll(".pdf", 4, ".dat", 4);
    if (output_DAT) {
	 cout<< "\nDATA FILE name="<< datname;
   	 fout = fopen (datname,"w");
    }

//ROOT OUTPUT //NEW
    TString outroot;
    outroot=outname;
    outroot.ReplaceAll(".pdf", 4, ".root", 5);
    std::cout << "\nOutput Root File : " << outroot << "\n";

/////////////////////////////////////////////////////////////////////////////////
// data interfacing 

  LEvRec0File File(rootname.Data());

  LEvRec0 cev;
  LEvRec0Md cmd;

  File.SetTmdPointer(cmd);
  int N2_Tmd=File.GetTmdEntries();
  int N_Tmd=N2_Tmd/2;

  if (N2_Tmd%2!=0) {
	std::cout<< "\nNumber of Tmd ODD!!! Missing Header or Tail in some run!";
	N_Tmd=(N2_Tmd+1)/2;
	if  ( N2_Tmd>1) {//per il momento se c'è più di un run e manca un header o tail esco
		std:: cout<< "\n\tNumber of runs >1 -> \tExit!!!";
		std::cout<< "\n---------------------------------------------------------------------------\n";
		return ;
	}
	else {
		std:: cout<< "\n\tNumber of runs =1 ->\tContinue...";
		std::cout<< "\n---------------------------------------------------------------------------\n";
	}
  }



  int Tmd_BootN[N_Tmd];
  int BOOT_temp[N_Tmd];
  int Tmd_runN[N_Tmd];

  double Tmd_run_time[N_Tmd];
  double CPU_run_time[N_Tmd][2];
  int Tmd_run_Nevents[N_Tmd];
  int Tmd_run_orbit[N_Tmd][2];
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
  	Tmd_run_TriggerMask[ir]=0;
  	Tmd_run_Veto[ir]=0;
  }
  

  int NN_boot=0;// boot counter 
  for(int ir=0; ir<N2_Tmd; ++ir) {
  	File.GetTmdEntry(ir); 
  	if (ir%2==0){//run header
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
		Tmd_run_orbit[ir/2][0]=(cmd.orbit_Zone>>8);//Calculated
		Tmd_run_orbit[ir/2][1]=(cmd.orbit_Zone& 0x00FF);//applied
		Tmd_run_TriggerMask[ir/2]=cmd.trigger_mask[1];
		Tmd_run_Veto[ir/2]=cmd.trigger_mask[0];
		CPU_run_time[ir/2][0]=cmd.CPU_time[0];//in case of header this should be =0; in case of missing header-> Tmd=tail CPU_run_time!=0
		CPU_run_time[ir/2][1]=cmd.CPU_time[1];//ms

	}
	if (ir%2!=0){
		if (cmd.boot_nr==Tmd_BootN[(ir-1)/2] && cmd.run_id==Tmd_runN[(ir-1)/2]){//tail correspondent to previous header
			CPU_run_time[(ir-1)/2][0]=cmd.CPU_time[0];
			CPU_run_time[(ir-1)/2][1]=cmd.CPU_time[1];//ms
			Tmd_run_time[(ir-1)/2]=(cmd.CPU_time[1]-cmd.CPU_time[0])/1000;//seconds
		}
		else {
			std::cout<< "\nNumber of Tmd EVEN but missing more than 1 header or tail!!!\n";
			if  ( N2_Tmd>1) {//per il momento se c'è più di un run e manca un header o tail esco
                		std:: cout<< "\n\tNumber of runs >1 -> \tExit!!!";
                		std::cout<< "\n---------------------------------------------------------------------------\n";
				return;
			}
		}
	}
  }//end loop on Tmd



  if (N2_Tmd!= 2*N_Tmd){//con le condizioni iniziali imposte (se c'è più di un run e manca almeno un tail o header) la correzione avviene solo nei casi in cui si ha un solo run; 
	std::cout<< "\n------------------ Missing Run Header or Tail!!! ----------------------\n";
	for(int ir=0; ir<N_Tmd; ir++){
		if (CPU_run_time[ir][0]==CPU_run_time[ir][1]){
			std::cout<< "\nBOOT N.="<< Tmd_BootN[ir]<< "\nRUN N.="<< Tmd_runN[ir]<< " -> Missing Tail!!!\n";
			CPU_run_time[ir][1]=CPU_run_time[ir][0]+RUN_DT*1e3;
			Tmd_run_time[ir]=(CPU_run_time[ir][1]-CPU_run_time[ir][0])/1000;//seconds
			std::cout<< "\nCPU_run_time[0] set = "<<CPU_run_time[ir][0] << " ms \nCPU_run-time[1] set = "<<  CPU_run_time[ir][1]<< " ms";
			std::cout<< "\nRun duration set = "<<Tmd_run_time[ir] << "s\n";
			std::cout<< "\n---------------------------------------------------------------------------\n";
		}
		else { 
			std::cout<< "\nBOOT N.="<< Tmd_BootN[ir]<< "\nRUN N.="<< Tmd_runN[ir]<< " -> Missing Header!!!\n";
		}
	}	
  } 


  cout<< "\n\tTotal number of boots="<< NN_boot;
  for (int nb=0; nb< NN_boot;nb++)	std::cout<< "\n\tBoot N["<<nb<<"] = "<<BOOT_temp[nb];
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
		if (Tmd_run_orbit[ir][0]==0xAA) cout << "Tmd entry "<< ir<<": Orbit zone: Calculated-Applied "<< "N/A" <<" - " << Tmd_run_orbit[ir][1]<<"\n";
                else    cout << "Tmd entry "<< ir<<": Orbit zone: Calculated-Applied "<< Tmd_run_orbit[ir][0] <<" - " << Tmd_run_orbit[ir][1]<<"\n";
		cout << "Tmd entry "<< ir<<": Orbit zone: Applied Orbit_Zone = "<< Orbit_Zone[Tmd_run_orbit[ir][1]]<<"\n";
		cout << "Tmd entry "<< ir<<": Orbit zone: Trigger Mask "<< Tmd_run_TriggerMask[ir] <<" - Veto " << Tmd_run_Veto[ir]<<"\n";
	}

  }  
  if (nEntries==0) {
	std::cout<< "\n\n--------------------- TOTAL NUMBER OF ACQUIRED EVENTS=0! ---------------------\n\n";
	std::cout<< "NO EVENT RATE QL PRODUCED!!!";
	std::cout<< "\n\n------------------------------------------------------------------------------\n\n";

	return;
  }

 if (output_DAT) {
      fprintf(fout, "\nInputFile=%s", rootname.Data());
      fprintf(fout, "\nIntegrationTime=%4.3f (s)", INTEG_TIME/1e3);
      fprintf(fout, "\nTotal number of boots= %d", NN_boot);
      for (int nb=0; nb< NN_boot;nb++)      fprintf(fout, "\n\t- Boot N[%d]=%d", nb, BOOT_temp[nb]);
      fprintf(fout, "\nTotal number of Runs= %d", N_Tmd);
      fprintf(fout, "\nTotal number of Events= %d", nEntries);
      fprintf(fout, "\n------------------------------------------------------\n");
      fprintf(fout, "\nBootN\tRunN\tAppliedZone\tTM_id\tVeto_id\tRate_CPU_time (s) \tRateMeter(Hz)\tAcquired Rate(Hz)\t Ratio\n");//NEW
}

//ROOT OUTPUT
  TFile froot(outroot, "recreate");
  std::cout<< "\n\nOutput file: "<< outroot<<"\n\n";

/// PDF OUTPUT ////////////

   TString outnameStart;
   TString outnameEnd;
   TCanvas *cRun = new TCanvas("Run");
	
   TPaveText *pt = new TPaveText(.1,.1,.9,.9);
   pt->AddText("DIRECTORY:");
   pt->AddText(outPath);
   pt->AddText("ROOT file:");
   pt->AddText(basename(rootname));
   pt->AddText(Form("Total number of boots= %d", NN_boot));
   pt->AddText(Form("TMd entries = %d -> Run entries = %d", N2_Tmd, N_Tmd));
   pt->AddText(Form("N events= %d", File.GetEntries()));
   pt->SetTextSize(0.02);
   pt->Draw();
   outnameStart = outname+"(";
    if (output_PDF)     cRun->Print(outnameStart);
   int Boot_evEntries[NN_boot];

  for (int nb=0; nb< NN_boot; nb++) 	Boot_evEntries[nb]=0;

  if (PRINT_METADATA && output_PDF ){
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
	pt->AddText(Form("RUN duration (s) = %4.3f ", Tmd_run_time[ir]));
        if (Tmd_run_orbit[ir][0]==0xAA) 	pt->AddText(Form("Orbit zone: Calculated-Applied N/A - %d",    Tmd_run_orbit[ir][1]));
	else	pt->AddText(Form("Orbit zone: Calculated-Applied %d - %d",  Tmd_run_orbit[ir][0],  Tmd_run_orbit[ir][1]));
	pt->AddText(Form("Applied Orbit Zone Configuration = ")+  Orbit_Zone[Tmd_run_orbit[ir][1]]);
	pt->AddText(Form("    TriggerMask [%d] -> ", Tmd_run_TriggerMask[ir])+TriggerMask[Tmd_run_TriggerMask[ir]]);
	pt->AddText(Form("    Veto  [%d] -> ", Tmd_run_Veto[ir])+ Veto[Tmd_run_Veto[ir]]);
	cRun->cd();
	pt->Draw();
	if (ir%2!=0 || ir==N_Tmd-1) cRun->Print(outname);
   }
}



/////////////////////////////////////GRAPH INITIALIZATION *************************
 for (int nb=0; nb< NN_boot; nb++){

	TGraph *average_rate;
	const char *gname;
	const char *gtitle;

	gname=Form("BootN_%d_Applied_Default_rate_time", BOOT_temp[nb]);//no broadcast available	
	gtitle=Form("Boot_%d_Applied_Config_DefaultZone", BOOT_temp[nb]);//no broadcast available

	average_rate= new TGraph();
 	


 	TGraph *rate_meter_vs_time[9];
	for(int kk=0;kk<9;kk++) {
      		rate_meter_vs_time[kk] = new TGraph();
      		rate_meter_vs_time[kk]->SetTitle(Form("TriggerMask_%d_%s_RateMeterT", kk, TriggerMask[kk].Data()));
       		rate_meter_vs_time[kk]->SetName(Form("TriggerMask_%d_%s_RateMeterT", kk, TriggerMask[kk].Data()));
	}

//////////////////////////////////////////////////////////////////////////////

// event loop
  	UInt_t event_time;
  	UInt_t time_flag=0;
  	UInt_t time_flag_old=0;

	Int_t sum[9];
  	for(int kk=0;kk<9;kk++)	sum[kk] = 0;

	Int_t numevent_int=0;
	int zone=-1;
        double MaxY=0;
        double MaxY2=0;
       	int TM=-1;

	Int_t check_events=0;
  	for(int ie=0; ie<nEntries; ++ie) {
        	File.GetEntry(ie);
        	for (int ir=0; ir<N_Tmd; ir++){
                	if (Tmd_BootN[ir]==BOOT_temp[nb] && cev.run_id==Tmd_runN[ir] && cev.boot_nr==Tmd_BootN[ir]){
				Boot_evEntries[nb]++;	
				event_time = CPU_run_time[ir][0]+ cev.hepd_time/1e+2; //unit = ms
      				if (ie == 0)    {
					time_flag = CPU_run_time[ir][0]+ INTEG_TIME;
                                        time_flag_old = CPU_run_time[ir][0];
                                        zone= Tmd_run_orbit[ir][1];
					TM=Tmd_run_TriggerMask[ir];
				}

				if (event_time>=time_flag_old && event_time   < time_flag)     {
                                        numevent_int++;
                                        zone= Tmd_run_orbit[ir][1];
					TM=Tmd_run_TriggerMask[ir];
					for(int kk=0;kk<9;kk++)
						sum[kk] += cev.rate_meter[kk];	
    				}
       				else {
					if ((time_flag-time_flag_old)>0) {
						average_rate->SetPoint(average_rate->GetN(), time_flag/1.e3, numevent_int/((time_flag-time_flag_old)/1.e3));

                                                MaxY=max(MaxY,numevent_int/((time_flag-time_flag_old)/1.e3));
                                                check_events+=numevent_int;

						rate_meter_vs_time[TM]->SetPoint(rate_meter_vs_time[TM]->GetN(), (time_flag)/1.e3, (double)sum[TM]/numevent_int);
						MaxY2=max(MaxY2,(double)sum[TM]/numevent_int);
						if (DEBUG==true){
                                                        std::cout<< "\nEvent Rate, set Point "<< average_rate->GetN()<<" : zone="<<zone<<"\n\tnumevent_int= "<< numevent_int<< ", dt (s)="<<(time_flag-time_flag_old)/1.e3<<  "\n\tx=time_flag (s)="<<time_flag/1.e3<< ", y=rate(Hz)="<<  numevent_int/((time_flag-time_flag_old)/1.e3);

                                                        std::cout<< "\nRate Meter TM["<< TM<<"], set Point "<< rate_meter_vs_time[TM]->GetN()<<"\n\tsum= "<< sum[TM]<<", numevent_int= "<< numevent_int<<  "\n\tx=time_flag (s)="<<time_flag/1.e3<< ", y=rate(Hz)="<< (double)sum[TM]/numevent_int;

                                                }
					}
					zone= Tmd_run_orbit[ir][1];
					TM=Tmd_run_TriggerMask[ir];
					if (event_time<CPU_run_time[ir][1]) {
						time_flag_old= event_time;
                                        	if (event_time + INTEG_TIME<CPU_run_time[ir][1]) time_flag = event_time + INTEG_TIME;
						else {
							time_flag = CPU_run_time[ir][1];
							if (DEBUG==true) std::cout<< "\nEventTime+ INTEG>CPU run time:\tEventTime= "<< event_time<< "\tCPUTime="<<CPU_run_time[ir][1];
						}
					}
                                        else {
						if (DEBUG==true) std::cout<< "\nEventTime> CPU run time:\tEventTime= "<< event_time<< "\tCPUTime="<<CPU_run_time[ir][1];
						time_flag_old=CPU_run_time[ir][1];
						time_flag=CPU_run_time[ir][1];
					}
                                        numevent_int = 1;
         				for(int kk=0;kk<9;kk++) sum[kk] = 0;
					sum[TM] += cev.rate_meter[TM];
         			}//end else
				if (ie==nEntries-1 ){//last events
					time_flag=min(event_time, time_flag);//NEW
					if ((time_flag-time_flag_old)>0){
                                        	average_rate->SetPoint(average_rate->GetN(), time_flag/1.e3, numevent_int/((time_flag-time_flag_old)/1.e3));
	                                        MaxY=max(MaxY,numevent_int/((time_flag-time_flag_old)/1.e3));
        	                                check_events+=numevent_int;
                	                        rate_meter_vs_time[TM]->SetPoint(rate_meter_vs_time[TM]->GetN(), (time_flag)/1.e3, (double)sum[TM]/numevent_int);
                        	                MaxY2=max(MaxY2,(double)sum[TM]/numevent_int);
                                        	if (DEBUG==true){
                                                        std::cout<< "\nEvent Rate, set LAST Point "<< average_rate->GetN()<<" : zone="<<zone<<"\n\tnumevent_int= "<< numevent_int<< ", dt (s)="<<(time_flag-time_flag_old)/1.e3<<  "\n\tx=time_flag (s)="<<time_flag/1.e3<< ", y=rate(Hz)="<<  numevent_int/((time_flag-time_flag_old)/1.e3);

                                                        std::cout<< "\nRate Meter TM["<< TM<<"], set LAST Point "<< rate_meter_vs_time[TM]->GetN()<<"\n\tsum= "<< sum[TM]<<"numevent_int= "<< numevent_int<<  "\n\tx=time_flag (s)="<<time_flag/1.e3<< ", y=rate(Hz)="<< (double)sum[TM]/numevent_int;
                                        	}
					}
                                }

      			}
                }//Tmd loop
	}// event loop


	//Boot_evEntries[nb]=check_events;
	std:: cout<< "\ncheck_events="<< check_events<<"\tBoot_evEntries["<<nb<<"]="<<Boot_evEntries[nb];
	std::cout<< "\n\n------------------------------------------------------------------------------\n\n";
	std::cout<< "\nBOOT N. "<<BOOT_temp[nb];
	std::cout<< "\nTotal number of events="<< nEntries<< "\t BOOT N. events="<<Boot_evEntries[nb] ;
	std::cout<< "\n\n------------------------------------------------------------------------------\n\n";

//  output
//AVERAGE RATE L0 as a function of RunID and calculated orbital zone

	if (Boot_evEntries[nb]>0){

	Color_t color[5];
	color[0]=kRed;
        color[1]=kBlack;
        color[2]=kBlue;
        color[3]=kGreen;
        color[4]=kMagenta;
	average_rate->GetYaxis()->SetTitle("Rate (Hz)");
	average_rate->GetXaxis()->SetTitle("CPU time (s)");
	average_rate->SetMarkerStyle(20);
	average_rate->SetMarkerSize(0.4);
	average_rate->SetTitle(gtitle);
	average_rate->SetName(gname);
	average_rate->SetFillStyle(0);
	average_rate->SetDrawOption("AP");	
	average_rate->SetLineWidth(0);
	average_rate->SetMarkerColor(color[2]);

	for(int kk=0;kk<9;kk++){
		rate_meter_vs_time[kk]->GetYaxis()->SetTitle("TM Rate (Hz)");
		rate_meter_vs_time[kk]->GetXaxis()->SetTitle("CPU time (s)");
		rate_meter_vs_time[kk]->SetMarkerStyle(20+kk);
		rate_meter_vs_time[kk]->SetMarkerSize(0.4);
		rate_meter_vs_time[kk]->SetTitle(Form("TriggerMask_%d_%s_RateMeterT", kk, TriggerMask[kk].Data()));
                rate_meter_vs_time[kk]->SetName(Form("TriggerMask_%d_%s_RateMeterT", kk, TriggerMask[kk].Data()));
                rate_meter_vs_time[kk]->SetFillStyle(0);
                rate_meter_vs_time[kk]->SetDrawOption("AP");	
                rate_meter_vs_time[kk]->SetLineWidth(0);
	  	rate_meter_vs_time[kk]->SetMarkerColor(kBlack);
	
	}
////////////////////////////////// EVENT RATE VS CPU TIME/////////////////////


 	TCanvas *crate=new TCanvas();

	TString mg_name=Form("BootN_%d_orbital_rate_time", BOOT_temp[nb]);
	gPad->SetGrid();
	crate->SetTitle(mg_name);
	crate->SetName(mg_name);

	TMultiGraph *mg = new TMultiGraph();
	mg->SetTitle(mg_name);
	mg->SetName(mg_name);
	TMultiGraph *mg2 = new TMultiGraph();

 	/*for (int ig=0; ig<5; ig++){
        	if (average_rate[ig]->GetN()>0)	mg->Add(average_rate[ig]); 
	}*/

       	if (average_rate->GetN()>0)	mg->Add(average_rate); 
	
	
 
	crate->cd();
	mg->SetTitle(mg_name + Form(" - Acquired rate (Hz) vs CPU time [Integ_time=%d s]",INTEG_TIME/1000));
	mg->Draw("AP");
	mg->GetYaxis()->SetTitle("Rate (Hz)");
	mg->GetXaxis()->SetTitle("CPU time (s)");
	mg->GetYaxis()->SetRangeUser(0, MaxY*1.5);
	crate-> BuildLegend();
	crate->Write(mg_name);//NEW
	if (output_PDF)	crate->Print(outname);	


////////////////////////////////// EVENT RATE and TRIGGER MASK RATE METER  VS CPU TIME/////////////////////
 	crate=new TCanvas();
	mg_name=Form("BootN_%d_orbital_rate_TM_time", BOOT_temp[nb]);
	crate->SetTitle(mg_name);
	crate->SetName(mg_name);
   
	mg = new TMultiGraph();
	mg->SetTitle(mg_name);
	mg->SetName(mg_name);

	bool drawn[9];
	for (int kk=0; kk<9; kk++) drawn[kk]=false;
        if (average_rate->GetN()>0)	mg->Add(average_rate); 
	for (int ir=0; ir<N_Tmd; ir++){
		if (Tmd_run_orbit[ir][1]==4 ){//no broadcast, default zone configuration
			if (drawn[Tmd_run_TriggerMask[ir]]==false) {
				rate_meter_vs_time[Tmd_run_TriggerMask[ir]]->SetMarkerColor(color[0]);
				mg->Add(rate_meter_vs_time[Tmd_run_TriggerMask[ir]]);
 				drawn[Tmd_run_TriggerMask[ir]]=true;
			}
         	}	
 	}

	crate->cd();
	mg->SetTitle(mg_name + Form(" - rate (Hz) vs CPU time [Integ_time=%d s]",INTEG_TIME/1000));
	mg->Draw("AP");
	mg->GetYaxis()->SetTitle("Rate (Hz)");
	mg->GetXaxis()->SetTitle("CPU time (s)");
	mg->GetYaxis()->SetRangeUser(0, MaxY2*1.5);
	if (MaxY2/MaxY>100) {
		mg->GetYaxis()->SetRangeUser(1, MaxY2*100);
		gPad->SetLogy();
	}
	gPad->SetGrid();
	crate-> BuildLegend();
	crate->Write(mg_name);
        if (output_PDF)	crate->Print(outname);


///////////////////////////////// EVENT RATE / TRIGGER MASK RATE METER ////////////////////////

 	TCanvas *crate2=new TCanvas();
 	TCanvas *crate3=new TCanvas();
	crate=new TCanvas();

	TString mg_name2;
	TString mg_name3;

	mg_name=Form("BootN_%d_orbital_rate_TM_ratio_time", BOOT_temp[nb]);
	mg_name2=Form("BootN_%d_orbital_rate_TM_ratio", BOOT_temp[nb]);
	mg_name3=Form("BootN_%d_orbital_rate_TM_rate", BOOT_temp[nb]);

	crate->SetTitle(mg_name);
	crate->SetName(mg_name);
	crate2->SetTitle(mg_name2);
	crate2->SetName(mg_name2);
	crate3->SetTitle(mg_name3);
	crate3->SetName(mg_name3);

	TGraph *gr;
	TGraph *gr2;
	TGraph *gr3;

        gtitle=Form("Boot_%d_Applied_Config_DefaultZone", BOOT_temp[nb]);//no broadcast available
     

	double MinY=10000;
	double MinY2=10000;

       gr =new TGraph();
       gr->SetTitle(gtitle);
       gr->SetName(gname);
       gr->SetMarkerStyle(20);
       gr->SetMarkerSize(0.4);
       gr->SetFillStyle(0);


       gr2 =new TGraph();
       gr2->SetTitle(gtitle);
       gr2->SetName(gname);
       gr2->SetMarkerStyle(20);
       gr2->SetMarkerSize(0.4);
       gr2->SetFillStyle(0);
       

       gr3 =new TGraph();
       gr3->SetTitle(gtitle);
       gr3->SetName(gname);
       gr3->SetMarkerStyle(20);
       gr3->SetMarkerSize(0.4);
       gr3->SetFillStyle(0);

       if (average_rate->GetN()>0){
		Double_t *nt = average_rate->GetX();//CPU times	//Added
	   	Double_t *ny = average_rate->GetY();//acquired rate
		Int_t N_ig=average_rate->GetN();

		Double_t *nt2;// CPU times rate meter
		Double_t *ny2;//rate meters
		Int_t N_ir;

		if (DEBUG==true) std::cout <<"\nZone["<<4<<"], N points="<<N_ig;

  		for (int ir=0; ir<N_Tmd; ir++) {
			if (Tmd_run_orbit[ir][1]==4 ){//DEFAULT ZONE, NO BROADCAST
				N_ir= rate_meter_vs_time[Tmd_run_TriggerMask[ir]]->GetN();
	                        ny2 = rate_meter_vs_time[Tmd_run_TriggerMask[ir]]->GetY();
				nt2 = rate_meter_vs_time[Tmd_run_TriggerMask[ir]]->GetX();
				for (int ii=0; ii<N_ig; ii++){
					MinY=min(MinY,ny[ii]);
					for (int jj=0; jj<N_ir; jj++){
						MinY2=min(MinY2,ny2[jj]);
						if (nt[ii]==nt2[jj] ){ 
							if (DEBUG==true) {
								std::cout<< "\nPoint "<<ii<<"\ttime nt["<<ii<<"]="<<nt[ii]<< "\ttime nt2["<<jj<<"]="<<nt2[jj];
								std::cout<< "\t\trate ny["<<ii<<"]="<<ny[ii]<< "\trate meter ny2["<<jj<<"]="<<ny2[jj];
							}
							gr->SetPoint(gr->GetN(), nt[ii], ny[ii]/ny2[jj]);
							gr2->SetPoint(gr2->GetN(), ny2[jj], ny[ii]/ny2[jj]);
							gr3->SetPoint(gr3->GetN(), ny2[jj], ny[ii]);
							if (output_DAT) {
							  //    fprintf(fout, "\nBootN\tRunN\tAppliedZone\tTM_id\tVeto_id\tRate_CPU_time (s) \tRateMeter(Hz)\tAcquired Rate(Hz)\t Ratio\n");//NEW
									fprintf(fout, "\n%d \t%d \t%d\t%d\t%d\t%4.3f\t%4.3f\t%4.3f\t%4.3f",
	                Tmd_BootN[ir],Tmd_runN[ir],  Tmd_run_orbit[ir][1], Tmd_run_TriggerMask[ir], Tmd_run_Veto[ir], nt[ii], ny2[jj],ny[ii], ny[ii]/ny2[jj]);
							}
						}
//						else std::cout<< "\ncheck: times don't match! nt="<<nt[ii]<<"\tnt2="<<nt2[jj];
					}
				}
				if (DEBUG==true)	std::cout <<"\n ------------------------------------------------------------------\n";
                       	}
                }
	}

  	mg = new TMultiGraph();
        mg->SetTitle(mg_name);
        mg->SetName(mg_name);

	mg2 = new TMultiGraph();
        mg2->SetTitle(mg_name2);
        mg2->SetName(mg_name2);

	TMultiGraph *mg3 = new TMultiGraph();
        mg3->SetTitle(mg_name3);
        mg3->SetName(mg_name3);

	gr->SetMarkerColor(color[2]);
	gr2->SetMarkerColor(color[2]);
	gr3->SetMarkerColor(color[2]);
        if (gr->GetN()>0) mg->Add(gr);
        if (gr2->GetN()>0) mg2->Add(gr2);
        if (gr3->GetN()>0) mg3->Add(gr3);

        

        crate3->cd();
	mg3->SetTitle(mg_name3 + Form(" - Acquired rate vs TM rate meter [Integ_time=%d s]",INTEG_TIME/1000));
        mg3->Draw("AP");
        mg3->GetYaxis()->SetTitle("Acquired RATE (Hz)");
        mg3->GetXaxis()->SetTitle("Trigger Mask rate meter(Hz)");
	mg3->GetYaxis()	->SetRangeUser(0, MaxY*1.5);
	if (MaxY2/MaxY>100) {
                gPad->SetLogx();
        }

	gPad->SetGrid();
	crate3-> BuildLegend();	
	crate3->Write(mg_name3);
        if (output_PDF)	crate3->Print(outname);

        crate->cd();
        mg->SetTitle(mg_name + Form(" - Acquired Rate/TM Rate  vs CPU time [Integ_time=%d s]",INTEG_TIME/1000));
        mg->Draw("AP");
        mg->GetYaxis()->SetTitle("RATE Ratio (acquired/TM rate meter)");
        mg->GetXaxis()->SetTitle("CPU time (s)");
	mg->GetYaxis()	->SetRangeUser(MinY/MaxY2*0.5, 1.5);
	if (MaxY2/MaxY>100) {
        	mg->GetYaxis()->SetRangeUser(MinY/MaxY2*0.5, MaxY/MinY2*10);
		gPad->SetLogy();
	}
	mg->GetXaxis()	->SetRangeUser(MinY2/2, MaxY2*2);
	gPad->SetGrid();

	crate-> BuildLegend();	
	crate->Write(mg_name);
        if (output_PDF) crate->Print(outname);


        crate2->cd();
        mg2->SetTitle(mg_name2 + Form(" - Event_Rate/TM_Rate  vs TM rate meter [Integ_time=%d s]",INTEG_TIME/1000));
        mg2->Draw("AP");
        mg2->GetYaxis()->SetTitle("RATE Ratio (acquired/TM rate meter)");
        mg2->GetXaxis()->SetTitle("Trigger Mask rate meter(Hz)");
	mg2->GetYaxis()	->SetRangeUser(MinY/MaxY2*0.5, 1.5);
	if (MaxY2/MaxY>100) {
        	mg->GetYaxis()->SetRangeUser(MinY/MaxY2*0.5, MaxY/MinY2*10);
		gPad->SetLogy();
	}
	mg2->GetXaxis()	->SetRangeUser(MinY2/2, MaxY2*2);
	gPad->SetGrid();

	crate2-> BuildLegend();	
	crate2->Write(mg_name2);
        if (output_PDF) crate2->Print(outname);

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
   if (output_PDF)    cRun->Print(outnameEnd);
   froot.Close();
if (output_DAT)    fclose (fout);

  File.Close();
  cout << "\nEventRateL0 ... done \nOutput PDF File: " << outname << "\n\n";
    cout << "\n--------------------------------------------\n";

  gErrorIgnoreLevel = 1;

}

