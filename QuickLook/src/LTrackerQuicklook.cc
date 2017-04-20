#include "detector_const.hh"
#include "analysis_const.hh"

#include "LEvRec0File.hh"
#include "LEvRec0.hh"
#include "LTrackerTools.hh"
#include "LTrackerCalibrationManager.hh"
#include "LTrackerCalibration.hh"
#include "LTrackerCluster.hh"
#include "LTrackerQuicklookPlots.hh"
#include "LTrackerQuicklook.hh"
#include "LTrackerMask.hh"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>

#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"
#include "TROOT.h"
#include "TRint.h"
#include "TStyle.h"
#include "TLine.h"
#include <TSystem.h>
#include <TPaveText.h>



int TrackerQuickLook(std::string namefile){
  gROOT->Reset();
  gDirectory->GetList()->Delete();
  gStyle->SetPalette(1);

  std::cout << "Root file = " << namefile << std::endl;

  TSystem filename_nopath;
  const char * nopath = filename_nopath.BaseName((const char*)namefile.c_str());
  TString outname = nopath;
  TString outnameStart;
  TString outnameEnd;
  outname.ReplaceAll(".root", 5, "_TrackerQL.pdf", 14);
  outnameStart = outname+"(";
  outnameEnd = outname+")";
  TH2D *sigmaraw[N_SIDES][N_LADDER];
  TH2D *pedestal[N_SIDES][N_LADDER];
  TH2D *sigma[N_SIDES][N_LADDER];
  TH2D *NGindex[N_SIDES][N_LADDER];
  TH2D *CN[N_SIDES][N_LADDER];

  TH2D *counts_clean[N_SIDES][N_LADDER];
  TH2D *signal_noise[N_SIDES][N_LADDER];

  TH2D *temp_map[N_SIDES][N_LADDER];
  TH2D *gaus_map[N_SIDES][N_LADDER];

  TH1D *clustersize[N_SIDES][N_LADDER];
  TH1D *landau[N_SIDES][N_LADDER];

  LEvRec0File input(namefile.c_str());
  LEvRec0 ev;
  LEvRec0Md metaData;

  input.SetTheEventPointer(ev);
  input.SetTmdPointer(metaData);
  input.GetEntry(0);
  
  if (!ev.IsVirgin())
     return -1;
  
    for(int side=0;side<N_SIDES;++side){
    char plan=(side ? 'n' : 'p');
    for(int ld=0;ld<N_LADDER;++ld){
      sigmaraw[side][ld]=new TH2D(
	 Form("sigma_raw_%c_%d",plan,ld),
	 Form("sigma_raw_%c_%d;chan;ADC",plan,ld),SIDE_CHAN,0,SIDE_CHAN,100,0,50);
				  
      pedestal[side][ld]=new TH2D(
	 Form("pedestal_%c_%d",plan,ld),
	 Form("pedestal_%c_%d;chan;ADC",plan,ld),SIDE_CHAN,0,SIDE_CHAN,100,1000,2500);
      sigma[side][ld]=new TH2D(
	 Form("sigma_%c_%d",plan,ld),
	 Form("sigma_%c_%d;chan;ADC",plan,ld),SIDE_CHAN,0,SIDE_CHAN,100,0,50);
      NGindex[side][ld]=new TH2D(
	 Form("Non_gaussianty_%c_%d",plan,ld),
	 Form("Non_gaussianty_%c_%d;chan;NG index",plan,ld),
	 SIDE_CHAN,0,SIDE_CHAN,50,-10,10);
      CN[side][ld]=new TH2D(
	 Form("Common_Noise_%c_%d",plan,ld),
	 Form("Common_Noise_%c_%d;VA;ADC",plan,ld),SIDE_VA,0,SIDE_VA,200,-100,100);
      counts_clean[side][ld]=new TH2D(
	 Form("Counts_clean_%c_%d",plan,ld),
	 Form("Counts_clean_%c_%d;chan;ADC",plan,ld),SIDE_CHAN,0,SIDE_CHAN,200,-200,500);
     signal_noise[side][ld]=new TH2D(
	Form("signal_noise_%c_%d",plan,ld),
	Form("signal_noise_%c_%d;chan;S/N",plan,ld),SIDE_CHAN,0,SIDE_CHAN,100,-100,100);
     temp_map[side][ld]=new TH2D(
	Form("Hotness_map_%c_%d",plan,ld),
	Form("Hotness_map_%c_%d;channel;VA",plan,ld),VA_CHAN,0,VA_CHAN,SIDE_VA,0,SIDE_VA);
     gaus_map[side][ld]=new TH2D(
	Form("non_gaussianity_map_%c_%d",plan,ld),
	Form("non_gaussianity_map_%c_%d;channel;VA",plan,ld),
	VA_CHAN,0,VA_CHAN,SIDE_VA,0,SIDE_VA);
     clustersize[side][ld]=new TH1D(
	Form("Clustersize_%c_%d",plan,ld),
	Form("Clustersize_%c_%d;clustersize;counts",plan,ld),5,0,5);
     landau[side][ld]=new TH1D(
	Form("Eta_counts_distribution_%c_%d",plan,ld),
	Form("Eta_counts_distribution_%c_%d;ADC;counts",plan,ld),100,0,300);
    }
  }
  
  const int MAXEVENTS = input.GetEntries();
  int N_PKG = MAXEVENTS/NCALIBEVENTS_QL;

  if (N_PKG == 0)
     N_PKG = 1;
     
  
  //Calibration on file
  LTrackerCalibrationManager::GetInstance().LoadRun(namefile.c_str());
  LTrackerCalibration * cal = LTrackerCalibrationManager::
     GetInstance().Calibrate(NCALIBEVENTS_QL, 0);
  //int slots=cal->GetNSlots();
  
  for(int ipk = 0; ipk < N_PKG; ++ipk){
    //std::cout<<"Processing events "<< ipk<<std::endl;
    double *sigmaraw_chan = cal->GetSigmaRaw(ipk);
    double *sigma_chan = cal->GetSigma(ipk);
    double *mean_chan = cal->GetPedestal(ipk);
    double *NGindex_chan = cal->GetNGIndex(ipk);
    bool *CNmask = cal->GetCNMask(ipk);
    LTrackerMask hotchan_tmp = cal->GetMaskOnSigma(ipk, -999., HOTCHANNELTHRESHOLD);
    LTrackerMask coldchan_tmp = cal->GetMaskOnSigma(ipk, COLDCHANNELTHRESHOLD, 999.);
    LTrackerMask nongauschan_tmp = cal->GetMaskOnNGI(ipk, -999., GAUSCHANNELTHRESHOLD);

    bool *evmask=(hotchan_tmp&&nongauschan_tmp).GetBool();
    
    short data[NCHAN];
    double CN_va[N_VA];
    double counts_clean_chan[NCHAN];
    double sign_noise[NCHAN];
    
    
    for(int iev = 0; iev < NCALIBEVENTS_QL; ++iev){
      input.GetEntry(NCALIBEVENTS_QL*ipk + iev);
      for(int ichan=0; ichan < NCHAN; ichan++){
	data[ichan] = ev.strip[ichan];
      }
      ComputeCN(data,mean_chan,CNmask,CN_va);
      
      for(int ichan = 0; ichan < NCHAN; ++ichan){
	int chanva = ChanToVA(ichan);
	int chanside = ChanToSide(ichan);
	int chanladder = ChanToLadder(ichan);
	int chanonside = ChanToSideChan(ichan);
	int chanVAside = ChanToVA(ichan)%SIDE_VA;
	int chanonVA = ChanToVAChan(ichan);
  
	counts_clean_chan[ichan] = data[ichan] - CN_va[chanva] - mean_chan[ichan];
	sign_noise[ichan] = counts_clean_chan[ichan]/sigma_chan[ichan];
	counts_clean[chanside][chanladder]->Fill(chanonside, counts_clean_chan[ichan]);
	signal_noise[chanside][chanladder]->Fill(chanonside, sign_noise[ichan]);
      }

       std::vector<LTrackerCluster> *clusters=GetClusters(counts_clean_chan,sigma_chan,evmask);
       for(int cl=0;cl<clusters->size();++cl){
	 int seed=clusters->at(cl).seed;
	 int clusterside=ChanToSide(seed);
	 int clusterladder=ChanToLadder(seed);
	 double etacounts=clusters->at(cl).GetEtaCounts();
	 double clsize=clusters->at(cl).ClusterSize(3.);

	 clustersize[clusterside][clusterladder]->Fill(clsize);
	 landau[clusterside][clusterladder]->Fill(etacounts);
       }
       
      for(int ld=0;ld<N_LADDER;++ld){
	for(int sd=0;sd<N_SIDES;++sd){
	  for(int vaside=0;vaside<SIDE_VA;++vaside)
	    CN[sd][ld]->Fill(vaside,CN_va[vaside + sd*SIDE_VA + ld*LADDER_VA]);
	}
      }
    }
    bool *hot_bool=hotchan_tmp.GetBool();
    bool *cold_bool=coldchan_tmp.GetBool();
    bool *nongaus_bool=nongauschan_tmp.GetBool();    

    for(int ichan=0;ichan<NCHAN;++ichan){
      int chanside=ChanToSide(ichan);
      int chanladder=ChanToLadder(ichan);
      int chanonside=ChanToSideChan(ichan);
      int chanVAside=ChanToVA(ichan)%SIDE_VA;
      int chanonVA=ChanToVAChan(ichan);
      sigmaraw[chanside][chanladder]->Fill(chanonside,sigmaraw_chan[ichan]);
      pedestal[chanside][chanladder]->Fill(chanonside,mean_chan[ichan]);
      sigma[chanside][chanladder]->Fill(chanonside,sigma_chan[ichan]);
      NGindex[chanside][chanladder]->Fill(chanonside,NGindex_chan[ichan]);
      if(!hot_bool[ichan]) temp_map[chanside][chanladder]->Fill(chanonVA,chanVAside,+10);
      if(!cold_bool[ichan])temp_map[chanside][chanladder]->Fill(chanonVA,chanVAside,-10.);
      if(!nongaus_bool[ichan])gaus_map[chanside][chanladder]->Fill(chanonVA,chanVAside);
    }
  }

  TCanvas *output=new TCanvas();
  TString nameOutFile(outname);
//  unsigned int run_in, boot_nr;
  std::stringstream ss;
  ss << input.GetEntries();
  TString numEvents = ss.str(); 
  ss.str("");
  input.GetEntry(0);
  ss << metaData.run_id;
  // ss << input.GetRunId();
  TString runID = ss.str();
  runID += "-";
  ss.str("");
  input.GetEntry(input.GetTmdEntries() -1);
  ss << metaData.run_id;
  runID += ss.str();
  // ss << input.GetBootNr();
  ss.str("");
  ss << metaData.boot_nr;
  TString bootNr = ss.str();

  
  TPaveText *pt = new TPaveText(.25,.2,.75,.6);
  pt->AddText("ROOT file: ");
  pt->AddText(nameOutFile);
  pt->AddText("Boot Nr: ");
  pt->AddText(bootNr);
  pt->AddText("Run Nr: ");
  pt->AddText(runID);
  pt->AddText("Number of events: ");
  pt->AddText(numEvents);
  pt->Draw();
  output->Print(outnameStart,"pdf");
  drawing6_chan_2D(sigmaraw[0])->Print(outname,"pdf");
  drawing6_chan_2D(sigmaraw[1])->Print(outname,"pdf"); 
  drawing6_chan_2D(sigma[0])->Print(outname,"pdf");
  drawing6_chan_2D(sigma[1])->Print(outname,"pdf");
  drawing6_chan_2D(pedestal[0])->Print(outname,"pdf");
  drawing6_chan_2D(pedestal[1])->Print(outname,"pdf");
  drawing6_va_2D(CN[0])->Print(outname,"pdf");
  drawing6_va_2D(CN[1])->Print(outname,"pdf");
  drawing6_chan_2D(NGindex[0])->Print(outname,"pdf");
  drawing6_chan_2D(NGindex[1])->Print(outname);
  drawing6_chan_2D(counts_clean[0])->Print(outname,"pdf");
  drawing6_chan_2D(counts_clean[1])->Print(outname,"pdf");
  drawing6_chan_2D(signal_noise[0])->Print(outname,"pdf");
  drawing6_chan_2D(signal_noise[1])->Print(outname,"pdf");
  drawing6_chan_1D(clustersize[0],true)->Print(outname,"pdf");
  drawing6_chan_1D(clustersize[1],true)->Print(outname,"pdf");
  drawing6_chan_1D(landau[0],false)->Print(outname,"pdf");
  drawing6_chan_1D(landau[1],false)->Print(outname,"pdf");
  drawing12_2D(temp_map[0],temp_map[1])->Print(outname,"pdf");
  drawing12_2D(gaus_map[0],gaus_map[1])->Print(outname,"pdf");
  output->Print(outnameEnd,"pdf");

  return 0;
}
