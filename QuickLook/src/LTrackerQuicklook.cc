#include "detector_const.hh"
#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"
#include "TH2D.h"
#include "TError.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TProfile.h>
#include <TStyle.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "LEvRec0File.hh"
#include "LEvRec0.hh"

#include "LTrackerTools.hh"
#include "LTrackerCalibrationManager.hh"
#include "LTrackerCalibration.hh"
#include "LTrackerCluster.hh"
#include "LTrackerQuicklookPlots.hh"
#include "LTrackerQuicklook.hh"
#include "LTrackerMask.hh"


int TrackerQuickLook(TString namefile, TString outPath){
   //gROOT->Reset();
   //gDirectory->GetList()->Delete();
   gErrorIgnoreLevel = 5000 ;
   
   gStyle->SetPalette(1);
   
   std::cout << "Root file = " << namefile << std::endl;
  
   /*TSystem filename_nopath;
     const char * nopath = filename_nopath.BaseName((const char*)namefile.c_str());
   */  const char *nopath = "StringaIntrodottaDaRoma2";
   TString outname = outPath;
   TString _temp= namefile;

   _temp.Replace(0, _temp.Last('/'), "");
   outname += "/";
   outname += _temp;

   TString outnameStart;
   TString outnameEnd;
   outname.ReplaceAll(".root", 5, "_TrackerQL.pdf", 14);
   outnameStart = outname+"(";
   outnameEnd = outname+")";
  
  
   TH2D *sigmaraw[N_SIDES][N_LADDER];
   TH2D *pedestal[N_SIDES][N_LADDER];
   TH2D *sigmaped[N_SIDES][N_LADDER];
   TH2D *NGindex[N_SIDES][N_LADDER];
   TH2D *CN[N_SIDES][N_LADDER];
  
   TH2D *counts_clean[N_SIDES][N_LADDER];
   TH2D *signal_noise[N_SIDES][N_LADDER];

   TH2D *temp_map[N_SIDES][N_LADDER];
   TH2D *gaus_map[N_SIDES][N_LADDER];

   TH1D *clustersize[N_SIDES][N_LADDER];
   TH1D *landau[N_SIDES][N_LADDER];

   for(int side=0;side<N_SIDES;++side){
      for(int ld=0;ld<N_LADDER;++ld){
         sigmaraw[side][ld]=NULL;
         pedestal[side][ld]=0;
         sigmaped[side][ld]=0;
         NGindex[side][ld]=0;
         CN[side][ld]=0;
         counts_clean[side][ld]=0;
         signal_noise[side][ld]=0;
         temp_map[side][ld]=0;
         gaus_map[side][ld]=0;
         clustersize[side][ld]=0;
         landau[side][ld]=0;     
      }
   }
   
   for(int side=0;side<N_SIDES;++side){
      char plan=(side==0 ? 'n' : 'p');
      for(int ld=0;ld<N_LADDER;++ld){

         sigmaraw[side][ld]=new TH2D(
            Form("sigma_raw_%c_%d",plan,ld),
            Form("sigma_raw_%c_%d;chan;ADC",plan,ld),
            SIDE_CHAN,0,SIDE_CHAN,100,0,50);
      
         pedestal[side][ld]=new TH2D(
            Form("pedestal_%c_%d",plan,ld),
            Form("pedestal_%c_%d;chan;ADC",plan,ld),SIDE_CHAN,0,SIDE_CHAN,100,1000,2500);
        
         sigmaped[side][ld]=new TH2D(
            Form("sigma_pedestal_%c_%d",plan,ld),
            Form("sigma_pedestal_%c_%d;chan;ADC",plan,ld),SIDE_CHAN,0,SIDE_CHAN,100,0,50);

         NGindex[side][ld]=new TH2D(
            Form("Non_gaussianty_%c_%d",plan,ld),
            Form("Non_gaussianty_%c_%d;chan;NG index",plan,ld),SIDE_CHAN,0,SIDE_CHAN,50,-8,8);

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
            Form("non_gaussianity_map_%c_%d;channel;VA",plan,ld),VA_CHAN,0,VA_CHAN,SIDE_VA,0,SIDE_VA);
      
         clustersize[side][ld]=new TH1D(
            Form("Clustersize_%c_%d",plan,ld),
            Form("Clustersize_%c_%d;clustersize;counts",plan,ld),6,0,6);
      
         landau[side][ld]=new TH1D(
            Form("Eta_counts_distribution_%c_%d",plan,ld),
            Form("Eta_counts_distribution_%c_%d;ADC;counts",plan,ld),100,0,300);
        
      }
   }
    
   LEvRec0File input(namefile.Data());
   LEvRec0 ev;
   LEvRec0Md metaData;
  
   input.SetTheEventPointer(ev);
   input.SetTmdPointer(metaData);
   input.GetEntry(0);
  
   const int MAXEVENTS = input.GetEntries();
   int N_PKG = MAXEVENTS/NCALIBEVENTS_QL;
  
   if (N_PKG == 0)
      N_PKG = 1;
  
  
   if (ev.IsVirgin())
      std::cout << "Virgin Run " << std::endl;
   else
   {
      std::cout << "Not Virgin Run " << std::endl;
      int res= CompressedTrackerQL(input,outname);
      return res;
   }
    
   //Calibration on file
   LTrackerCalibrationManager::GetInstance().LoadRun(namefile.Data());
   LTrackerCalibration * cal = LTrackerCalibrationManager::
      GetInstance().Calibrate(NCALIBEVENTS_QL, 0);
   //int slots=cal->GetNSlots();
    
   for(int ipk = 0; ipk < N_PKG; ++ipk){

      const double *sigmaraw_chan = cal->GetSigmaRaw(ipk);
      const double *sigma_chan = cal->GetSigma(ipk);
      const double *mean_chan = cal->GetPedestal(ipk);
      const double *NGindex_chan = cal->GetNGIndex(ipk);
      const bool *CNmask = cal->GetCNMask(ipk);
      LTrackerMask hotchan_tmp = cal->GetMaskOnSigma(ipk, -999., HOTCHANNELTHRESHOLD);
      LTrackerMask coldchan_tmp = cal->GetMaskOnSigma(ipk, COLDCHANNELTHRESHOLD, 999.);
      LTrackerMask nongauschan_tmp = cal->GetMaskOnNGI(ipk, -999., GAUSCHANNELTHRESHOLD);
      
      LTrackerMask evmask=(hotchan_tmp&&nongauschan_tmp);
    
      short data[NCHAN];
      double CN_va[N_VA];
      double counts_clean_chan[NCHAN];
      double sign_noise[NCHAN];
    
    
      for(int iev = 0; iev < NCALIBEVENTS_QL; ++iev){
         input.GetEntry(NCALIBEVENTS_QL*ipk + iev);
         for(int ichan=0; ichan < NCHAN; ichan++){
            data[ichan] = ev.strip[ichan];
         }
         ComputeCN(data,mean_chan,&evmask,CN_va);
        
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
	 /*
	 //std::cout<<"Clusterfinding algorithm running"<<std::endl;
         std::vector<LTrackerCluster> signal2=GetClusters(counts_clean_chan,sigma_chan,&evmask);
	 std::cout<<"signal size= "<< &signal2<<std::endl << std::flush;
	 std::vector<LTrackerCluster> signal = signal2;
	 
	 
	 for(int cl=0;cl<signal.size();++cl){
      
	     int seed=signal.at(cl).seed;
    
           int clusterside=ChanToSide(seed);
           int clusterladder=ChanToLadder(seed);
           double etacounts=signal.at(cl).GetEtaCounts();
           double clsize=signal.at(cl).ClusterSize(3.);
     
           clustersize[clusterside][clusterladder]->Fill(clsize);
	   landau[clusterside][clusterladder]->Fill(etacounts);
    
           }
	 */	 
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
         sigmaped[chanside][chanladder]->Fill(chanonside,sigma_chan[ichan]);
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
   input.GetTmdEntry(0);
   ss << metaData.run_id;
   // ss << input.GetRunId();
   TString runID = ss.str();
   runID += "-";
   ss.str("");
   input.GetTmdEntry(input.GetTmdEntries() -1);
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
   drawing6_chan_2D(sigmaped[0])->Print(outname,"pdf");
   drawing6_chan_2D(sigmaped[1])->Print(outname,"pdf");
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

   gErrorIgnoreLevel = 1;

   return 0;
}

int CompressedTrackerQL(LEvRec0File input,TString outname){

  LEvRec0 ev;
  LEvRec0Md metaData;

  input.SetTheEventPointer(ev);
  input.SetTmdPointer(metaData);
  //  input.GetEntry(0);

  const int MAXEVENTS = input.GetEntries();
  //std::cout<<"The events are "<<MAXEVENTS<<std::endl;
  TH2D* CountsPosition[N_SIDES][N_LADDER];
  //TH1D* ClusterSize[N_SIDES][N_LADDER];
  TH1D* SeedCounts[N_SIDES][N_LADDER];

  for(int iSide=0;iSide<N_SIDES;++iSide){
    char plan=(iSide==0 ? 'p' : 'n');
    for(int iLd=0;iLd<N_LADDER;++iLd){
      CountsPosition[iSide][iLd]=new TH2D(Form("Counts_clean_%c_%d",plan,iLd),Form("Counts_clean_%c_%d;chan;ADC",plan,iLd),SIDE_CHAN,0,SIDE_CHAN,200,-200,2000);
      //ClusterSize[iSide][iLd]=new TH1D(Form("Clustersize_%c_%d",plan,iLd),Form("Clustersize_%c_%d;clustersize;counts",plan,iLd),6,0,6);
      SeedCounts[iSide][iLd]=new TH1D(Form("Cluster_counts_distribution_%c_%d",plan,iLd),Form("Cluster_counts_distribution_%c_%d;ADC;counts",plan,iLd),100,1,2000);
    }
  }
  double data[NCHAN];
  //  double fakesigma[NCHAN];
  for(int iChan=0;iChan<NCHAN;++iChan){
    data[iChan]=0;
    //fakesigma[iChan]=1.;
  }
  //  std::vector<LTrackerCluster> clusters;
  for(int iEv=0;iEv<MAXEVENTS;++iEv){
    //    std::cout<<"Processing event "<<iEv<<std::endl;
    input.GetEntry(iEv);
    for(int ichan=0; ichan < NCHAN; ichan++){
      data[ichan] = ev.strip[ichan];
      CountsPosition[ChanToSide(ichan)][ChanToLadder(ichan)]->Fill(ChanToSideChan(ichan),data[ichan]);
    }
    for(int iChan=0;iChan<NCHAN;++iChan){
      if(data[iChan]!=0.){
	double evdata[CLUSTERCHANNELS];
	double fakesigma[CLUSTERCHANNELS];
	double sum=0;
	for(int iCl=0; iCl<CLUSTERCHANNELS;++iCl){
	  evdata[iCl]=data[iChan+iCl];
	  fakesigma[iCl]=1.;
	  sum+=evdata[iCl];
	}
	LTrackerCluster evcluster(iChan+2,evdata,fakesigma);
	SeedCounts[ChanToSide(iChan+2)][ChanToLadder(iChan+2)]->Fill(sum);
	iChan+=CLUSTERCHANNELS-1;
      }
    }

  }
  /*
  for(int iCl=0;iCl<static_cast<int>(clusters.size());++iCl){
    int seed=clusters.at(iCl).seed;
    SeedCounts[ChanToSide(seed)][ChanToLadder(seed)]->Fill(clusters.at(iCl).GetCounts(0));
  }
  */
  TString outnameStart;
  TString outnameEnd;
  outname.ReplaceAll(".root", 5, "_TrackerQL.pdf", 14);
  outnameStart = outname+"(";
  outnameEnd = outname+")";


  TCanvas *output=new TCanvas();
  TString nameOutFile(outname);
  //  unsigned int run_in, boot_nr;                                                                                                                        
  std::stringstream ss;
  ss << input.GetEntries();
  TString numEvents = ss.str();
  ss.str("");
  input.GetTmdEntry(0);
  ss << metaData.run_id;
  // ss << input.GetRunId();                                                                                                                               
  TString runID = ss.str();
  runID += "-";
  ss.str("");
  input.GetTmdEntry(input.GetTmdEntries() -1);
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

  drawing6_chan_2D(CountsPosition[0])->Print(outname,"pdf");
  drawing6_chan_2D(CountsPosition[1])->Print(outname,"pdf");
  drawing6_chan_1D(SeedCounts[0],true,false)->Print(outname,"pdf");
  drawing6_chan_1D(SeedCounts[1],true,false)->Print(outname,"pdf");
  output->Print(outnameEnd,"pdf");

  gErrorIgnoreLevel = 1;

  return 1;
}
