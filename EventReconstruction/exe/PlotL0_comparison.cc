// here to start documentation
// author: de donato cinzia, cinzia.dedonato@roma2.infn.it
//
// class purpose: comparison PMT HG_histograms between 2 files  (L0 data)


#include "LEvRec0File.hh"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TStyle.h"
#include "TPaveText.h"

const int Get_Unit_PMT_ch(const int i, const int j);
const TString Get_Unit_Name(const int i);
const int GetTrigger_ch(const int i, const int j);
const int GetPlane_ch(const int i, const int j);
const int GetLyso_ch(const int i);
const int GetVeto_ch(const int i, const int j);
const TString Get_Unit_Name_PMT(const int channel);
const TString Get_VetoConf_Name(const int V);
const TString Get_TriggerMask_Name(const int TM);


//first and last unit to Print in pdf (in output root file all PMTs are plotted)  :
#define FIRST_UNIT 0	
#define LAST_UNIT 8
//0->5 Trigger (NTRIGSCINT=6)
//6->21 Calo Planes (NSCINTPLANES=16)
//22->26 Veto Planes  (NVETOSCINT=5)
//27->35 Lyso crystals (NLYSOCRYSTALS=9);


//--------------------------------------------------------------------------------------//
int main(int argc, char **argv) {

// input argument

  if(argc!=3) {

    std::cout << "Usage:" << " user> PlotL0_comparison <inputFile1> <inputFile2> " << std::endl;
	     
   return -999;
 
  }

   TString rootname1=argv[1];
   TString rootname2=argv[2];

   std::cout << "Input File1: " << rootname1 << "\n";
   std::cout << "Input File2: " << rootname2 << "\n";


  TString Unit_PMT[64];
  for (int ch=0; ch<64; ch++)  	Unit_PMT[ch]=Get_Unit_Name_PMT(ch);

  TString TriggerMask_name[9];
  for (int TM=0; TM<9; TM++) 	TriggerMask_name[TM]=Get_TriggerMask_Name(TM);
  
  TString VetoConf_name[4];
  for (int V=0; V<4; V++) 	VetoConf_name[V]=Get_VetoConf_Name(V);
 


// data interfacing 
  TH1D *h_pmt_high1[64];
  TH1D *h_pmt_high2[64];


///////////////////////   FILE 1 ////////////////////////
  LEvRec0File File1(rootname1);
  std::cout << "\n\n*************************** File1 ***************************\n\n" ;

  std::cout << "Input File1: " << rootname1 << "\n";

  LEvRec0Md cmd;
  File1.SetMdPointer(cmd);
  int N_Tmd1=File1.GetMDEntries();
  std::cout << "TMd entries = "<< N_Tmd1<<"\n";
  File1.GetMDEntry(0);
  int BootN1=cmd.boot_nr;
  int RunN1=cmd.run_id;
  int TriggerMask1=cmd.trigger_mask[1];
  int Veto1=cmd.trigger_mask[0];
  double Tmd_run_long1[2];
  double Tmd_run_lat1[2];
  Tmd_run_long1[0]=(cmd.broadcast.GPS.lon)*pow(10,-7);
  Tmd_run_lat1[0]=(cmd.broadcast.GPS.lat)*pow(10,-7);

  File1.GetMDEntry(1);

  int CPUtime1[2];

  CPUtime1[0]=cmd.CPU_time[0];
  CPUtime1[1]=cmd.CPU_time[1];
  Tmd_run_long1[1]=(cmd.broadcast.GPS.lon)*pow(10,-7);
  Tmd_run_lat1[1]=(cmd.broadcast.GPS.lat)*pow(10,-7);

  std::cout << "BootN= "<< BootN1<<"\t RunN= "<< RunN1<<"\n";
  std::cout << "TriggerMask= "<< TriggerMask1<<" -> "<< TriggerMask_name[TriggerMask1]<<"\n";
  std::cout << "Veto= "<< Veto1<<" -> "<<VetoConf_name[Veto1] << "\n";
  std::cout << "Run Duration (s)= "<< (CPUtime1[1]-CPUtime1[0])/1.e3<<" s\n";
  std::cout << "Latitude= [ "<< Tmd_run_lat1[0]<<" -> "<<Tmd_run_lat1[1]<<" ]\n";
  std::cout << "Longitude= [ "<< Tmd_run_long1[0]<<" -> "<<Tmd_run_long1[1]<<" ]\n";

  LEvRec0 cev;
  File1.SetTheEventPointer(cev);
  int nEntries1=File1.GetEntries();
  std::cout<< "\nnEntries= "<<nEntries1<<"\n";

  TString hname_high1;

for (int ch=0; ch<64; ch++){
	hname_high1 = Form("BootN_%d_RunN_%d_PMT_High_Gain_Ch_", BootN1, RunN1);
   	hname_high1 += ch;
   	h_pmt_high1[ch] = new TH1D(hname_high1,hname_high1,256,0,4096);
}

// event loop
  for(int ie=0; ie<nEntries1; ++ie) {
    File1.GetEntry(ie);
    for (int ch=0; ch<64; ch++){   
        h_pmt_high1[ch]->Fill(cev.pmt_high[ch]);
    }//channel loop
  }// event loop
 
///////////////////////   FILE 2 ////////////////////////

  std::cout << "\n\n*************************** File2 ***************************\n\n" ;

  LEvRec0File File2(rootname2);

  std::cout << "Input File2: " << rootname2 << "\n";
  
  File2.SetMdPointer(cmd);
  int N_Tmd2=File2.GetMDEntries();
  std::cout << "TMd entries= "<< N_Tmd2<<"\n";
  File2.GetMDEntry(0);
  int BootN2=cmd.boot_nr;
  int RunN2=cmd.run_id;
  int TriggerMask2=cmd.trigger_mask[1];
  int Veto2=cmd.trigger_mask[0];
  int CPUtime2[2];
  double Tmd_run_long2[2];
  double Tmd_run_lat2[2];

  Tmd_run_long2[0]=(cmd.broadcast.GPS.lon)*pow(10,-7);
  Tmd_run_lat2[0]=(cmd.broadcast.GPS.lat)*pow(10,-7);

  File2.GetMDEntry(1);
 
  CPUtime2[0]=cmd.CPU_time[0];
  CPUtime2[1]=cmd.CPU_time[1];
  Tmd_run_long2[1]=(cmd.broadcast.GPS.lon)*pow(10,-7);
  Tmd_run_lat2[1]=(cmd.broadcast.GPS.lat)*pow(10,-7);


  std::cout << "BootN= "<< BootN2<<"\t RunN= "<< RunN2<<"\n";
  std::cout << "TriggerMask= "<< TriggerMask2<<" -> "<< TriggerMask_name[TriggerMask2]<<"\n";
  std::cout << "Veto= "<< Veto2<<" -> "<<VetoConf_name[Veto2] << "\n";
  std::cout << "Run Duration (s)= "<< (CPUtime2[1]-CPUtime2[0])/1.e3<<" s\n";
  std::cout << "Latitude= [ "<< Tmd_run_lat2[0]<<" -> "<<Tmd_run_lat2[1]<<" ]\n";
  std::cout << "Longitude= [ "<< Tmd_run_long2[0]<<" -> "<<Tmd_run_long2[1]<<" ]\n";


  File2.SetTheEventPointer(cev);
  int nEntries2=File2.GetEntries();
  std::cout<< "\nnEntries= "<<nEntries2<<"\n";

  TString hname_high2;
  for (int ch=0; ch<64; ch++){
	hname_high2 = Form("BootN_%d_RunN_%d_PMT_High_Gain_Ch_", BootN2, RunN2);
   	hname_high2 += ch;
   	h_pmt_high2[ch] = new TH1D(hname_high2,hname_high2,256,0,4096);
  }

// event loop
  for(int ie=0; ie<nEntries2; ++ie) {
    File2.GetEntry(ie);
    for (int ch=0; ch<64; ch++){   
        h_pmt_high2[ch]->Fill(cev.pmt_high[ch]);
    }//channel loop
  }// event loop

 

//OUTPUT FILE
  std::cout << "\n\n*************************** Output Files ***************************\n\n" ;

   TString outname=Form("Comparison_BootN_%d_runN_%d_BootN_%d_runN_%d.root", BootN1, RunN1, BootN2, RunN2);
   std::cout << "\nOutput Root File:" << outname << "\n";
  TFile fout(outname, "new");
  outname.ReplaceAll(".root", 5, "_ScintillatorQL.pdf", 19);
  std::cout << "\nOutput PDF File:" << outname << "\n\n";



   TString outnameStart;
   TString outnameEnd;

   TCanvas *Run = new TCanvas("Run");
   TPaveText *pt = new TPaveText(.1,.1,.9,.9);

   pt->AddText("ROOT files: ");
   
   TString nameFile1(rootname1);
   pt->AddText(nameFile1);
   pt->AddText(Form("BootN= %d - RunN= %d", BootN1, RunN1));
   pt->AddText(Form("TriggerMask= %d -> ", TriggerMask1)+ TriggerMask_name[TriggerMask1]);
   pt->AddText(Form("Veto= %d -> ", Veto1)+ VetoConf_name[Veto1]);

   pt->AddText(Form("N Entries=%d",nEntries1));
   pt->AddText(Form("Run duration (s)=%4.3f",(CPUtime1[1]-CPUtime1[0])/1.e3));
   pt->AddText(Form("Latitude= [ %4.3f -> %4.3f ]",Tmd_run_lat1[0], Tmd_run_lat1[1]));
   pt->AddText(Form("Longitude=[ %4.3f -> %4.3f ]",Tmd_run_long1[0], Tmd_run_long1[1]));
  

   TString nameFile2(rootname2);
   pt->AddText(nameFile2 );
   pt->AddText(Form("BootN= %d - RunN= %d", BootN2, RunN2));
   pt->AddText(Form("TriggerMask= %d -> ", TriggerMask2)+ TriggerMask_name[TriggerMask2]);
   pt->AddText(Form("Veto= %d -> ", Veto2)+ VetoConf_name[Veto2]);
   pt->AddText(Form("N Entries=%d",nEntries2));
   pt->AddText(Form("Run duration (s)=%4.3f",(CPUtime2[1]-CPUtime2[0])/1.e3));
   pt->AddText(Form("Latitude= [ %4.3f -> %4.3f ]",Tmd_run_lat2[0], Tmd_run_lat2[1]));
   pt->AddText(Form("Longitude= [ %4.3f -> %4.3f ]",Tmd_run_long2[0], Tmd_run_long2[1]));

   pt->SetTextSize(0.02);
   pt->Draw();
   outnameStart = outname+"(";
   Run->Print(outnameStart);


for (int ch=0; ch<32; ch++){   
	for (int ip=0; ip<2; ip++){

		h_pmt_high1[ch+ip*32]->SetLineColor(kRed);
		h_pmt_high1[ch+ip*32]->SetLineWidth(2);
		h_pmt_high2[ch+ip*32]->SetLineColor(kBlack);
		h_pmt_high2[ch+ip*32]->SetLineWidth(2);
		h_pmt_high1[ch+ip*32]->GetXaxis()->SetTitle("ADC counts");
		h_pmt_high2[ch+ip*32]->GetXaxis()->SetTitle("ADC counts");

		h_pmt_high1[ch+ip*32]->SetNormFactor(1);
		h_pmt_high2[ch+ip*32]->SetNormFactor(1);

		hname_high1 = Form("BootN_%d_RunN_%d_PMT_High_Gain_Ch_", BootN1, RunN1);
		hname_high1 += (ch+ip*32);

		hname_high2 = Form("BootN_%d_RunN_%d_PMT_High_Gain_Ch_", BootN2, RunN2);
   		hname_high2 += (ch+ip*32);


		TCanvas *c=new TCanvas();
        	c->cd();
	        c->SetTitle(Unit_PMT[ch+ip*32]);
        	c->SetName(Unit_PMT[ch+ip*32]);
        	gStyle->SetOptStat(0);
        	h_pmt_high1[ch+ip*32]->Draw();
	        gStyle->SetOptStat(0);
        	h_pmt_high2[ch+ip*32]->Draw("SAME");
	        gStyle->SetOptStat(0);
        	c->BuildLegend();
        	c->SetLogy();
        	h_pmt_high1[ch+ip*32]->SetTitle(Unit_PMT[ch+ip*32]);
	        gStyle->SetOptStat(0);
        	c->Write(Unit_PMT[ch+ip*32]);
    		for (int iu=FIRST_UNIT; iu<=LAST_UNIT; iu++){		//PMT To be plotted on pdf
                        if  (Get_Unit_PMT_ch(iu, ip)==(ch+ip*32))  c->Print(outname);
		}
	}
}


File1.Close();
File2.Close();

Run = new TCanvas("Run");

pt->Draw();
outnameEnd = outname+")";
 Run->Print(outnameEnd);

fout.Close();
  return 0;
}

//------------------------------------------ functions ---------------------------------//

const int Get_Unit_PMT_ch(const int i, const int j){
        if(i<0 || i>=(NTRIGSCINT+NSCINTPLANES+ NVETOSCINT+NLYSOCRYSTALS)) return -999;
        if (i>=0 && i<NTRIGSCINT) return GetTrigger_ch(i, j);//Trigger counters
        if (i>=NTRIGSCINT && i<NTRIGSCINT+NSCINTPLANES) return GetPlane_ch((i-NTRIGSCINT), j);//Calo planes
        if (i>=NTRIGSCINT+NSCINTPLANES && i< NTRIGSCINT+NSCINTPLANES+ NVETOSCINT ) return GetVeto_ch((i-NTRIGSCINT- NSCINTPLANES), j);//Veto planes
        if (i>= NTRIGSCINT+NSCINTPLANES+ NVETOSCINT && i<NTRIGSCINT+NSCINTPLANES+ NVETOSCINT+NLYSOCRYSTALS ) return GetLyso_ch((i-NTRIGSCINT- NSCINTPLANES-NVETOSCINT));//LYSO crystals
}

const int GetTrigger_ch(const int i, const int j)  {
  if(i<0 || i>=NTRIGSCINT) return -999;
  if(j<0 || j>=2) return -999;
  return (i+j*NPMT/2);
}

const int GetPlane_ch(const int i, const int j)  {
  if(i<0 || i>=NSCINTPLANES) return -999;
  if(j<0 || j>=2) return -999;
  return (NTRIGSCINT+i+j*NPMT/2);
}

const int GetLyso_ch(const int i)  {
  if(i<0 || i>=NLYSOCRYSTALS) return -999;
  if(i==0) return 28; // North West
  if(i==1) return 62; // North
  if(i==2) return 29; // North East
  if(i==3) return 30; // West
  if(i==4) return 31; // Center
  if(i==5) return 60; // East
  if(i==6) return 27; // South West
  if(i==7) return 61; // South
  if(i==8) return 59; // South East
}

const int GetVeto_ch(const int i, const int j)  {
  if(i<0 || i>=NVETOSCINT) return -999;
  if(j<0 || j>=2) return -999;
  return (NTRIGSCINT+NSCINTPLANES +i+j*NPMT/2);
}

//-------------------------------------------------------------------------//


const TString Get_Unit_Name_PMT(const int channel){
  TString Unit_PMT[64];
  for (int ch=0; ch<27; ch++){
  	if (ch<6){
		Unit_PMT[ch]=Form("Trigger_T%d_PMT%d",ch+1,  0);
		Unit_PMT[ch+32]=Form("Trigger_T%d_PMT%d",ch+1,  1);
  	}
  	if (ch>5&&ch<22) {
		Unit_PMT[ch]=Form("Calo_Plane_P%d_PMT%d",ch-5,  0);
		Unit_PMT[ch+32]=Form("Calo_Plane_P%d_PMT%d",ch-5,  1);
	}
   	if (ch>21&&ch<27) {
		if (ch==26) {
			Unit_PMT[ch]=Form("Bottom_Veto_V%d_PMT%d",ch-21,  0);
			Unit_PMT[ch+32]=Form("Bottom_Veto_V%d_PMT%d",ch-21,  1);
		}
		if (ch==22) {
			Unit_PMT[ch]=Form("Lateral_Veto_North_V%d_PMT%d",ch-21,  0);
			Unit_PMT[ch+32]=Form("Lateral_Veto_North_V%d_PMT%d",ch-21,  1);
		}
		if (ch==23) {
			Unit_PMT[ch]=Form("Lateral_Veto_East_V%d_PMT%d",ch-21,  0);
			Unit_PMT[ch+32]=Form("Lateral_Veto_East_V%d_PMT%d",ch-21,  1);
		}
		if (ch==24) {
			Unit_PMT[ch]=Form("Lateral_Veto_South_V%d_PMT%d",ch-21,  0);
			Unit_PMT[ch+32]=Form("Lateral_Veto_South_V%d_PMT%d",ch-21,  1);
		}
		if (ch==25) {
			Unit_PMT[ch]=Form("Lateral_Veto_West_V%d_PMT%d",ch-21,  0);
			Unit_PMT[ch+32]=Form("Lateral_Veto_West_V%d_PMT%d",ch-21,  1);
		}
	}	
  }	
  Unit_PMT[27]=Form("Lyso_SW_L%d_PMT%d",7, 0);
  Unit_PMT[28]=Form("Lyso_NW_L%d_PMT%d",1, 0);
  Unit_PMT[29]=Form("Lyso_NE_L%d_PMT%d",3, 0);
  Unit_PMT[30]=Form("Lyso_W_L%d_PMT%d",4,  0);
  Unit_PMT[31]=Form("Lyso_C_L%d_PMT%d",5,  0);
  Unit_PMT[59]=Form("Lyso_SE_L%d_PMT%d",9, 0);
  Unit_PMT[60]=Form("Lyso_E_L%d_PMT%d",6,  0);
  Unit_PMT[61]=Form("Lyso_S_L%d_PMT%d",8,  0);
  Unit_PMT[62]=Form("Lyso_N_L%d_PMT%d",2,  0);
  Unit_PMT[63]="EMPTY";


  return Unit_PMT[channel];

}
//-------------------------------------------------------------------------//

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

