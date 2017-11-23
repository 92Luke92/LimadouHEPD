#include "LCalibrationMonitor.hh"
#include "detector_const.hh"
#include "LCalibration.hh"
#include "LTrackerTools.hh"

#include <string>
#include <random>
#include <fstream>
#include <cmath>
#include <iostream>
#include <chrono>

#include "TCanvas.h"
#include <string>
#include <fstream>
#include <vector>

#include "TH1.h"
#include "TGraphErrors.h"

LCalibration* CalculateWindow(const char * infile,double *err_sigma_LG,double *err_sigma_HG,double *err_sigma_TRK){
  std::ifstream input(infile,std::ifstream::in);
  std::vector<std::string> readfile;
  while(!input.eof()){
    std::string buffer;
    getline(input, buffer);
    if(buffer.size()>3)readfile.push_back(buffer);         
  }     
  LCalibration* output=LCalibration::Read(readfile.at(readfile.size()-1).c_str());

  int wsize=readfile.size()>WINDOW_LEN ? readfile.size()-WINDOW_LEN : 0;
  LCalibration* calbuffer[WINDOW_LEN];
  
  int islot=0;
  for(int iw=readfile.size()-2;iw>=wsize;--iw){
    calbuffer[islot]=LCalibration::Read(readfile.at(iw).c_str());
    (*output) += (*calbuffer[islot]);
    ++islot;
    
  }
  
  //sigma of sigmas calculation
  //Mean calculation
  double PMTsigma_meanLG[NPMT];
  double PMTsigma_meanHG[NPMT];
  double TRKsigma_mean[NCHAN];

  for(int iPmt=0;iPmt<NPMT;++iPmt){
    PMTsigma_meanLG[iPmt]=0;
    PMTsigma_meanHG[iPmt]=0;
  }

  for(int iChan=0;iChan<NCHAN;++iChan)
    TRKsigma_mean[iChan]=0;

  const double* sigmaHG[WINDOW_LEN];
  const double* sigmaLG[WINDOW_LEN];
  const double* sigmaTRK[WINDOW_LEN];
  
  for(int iw=0;iw<islot;++iw){
    sigmaHG[iw]=calbuffer[iw]->GetCaloHGCalibration()->GetSigma();
    sigmaLG[iw]=calbuffer[iw]->GetCaloLGCalibration()->GetSigma();
    sigmaTRK[iw]=calbuffer[iw]->GetTrackerCalibration()->GetSigma(0);
    for(int iPmt=0;iPmt<NPMT;++iPmt){
      PMTsigma_meanLG[iPmt]+=sigmaLG[iw][iPmt];
      PMTsigma_meanHG[iPmt]+=sigmaHG[iw][iPmt];
    }
    for(int iChan=0;iChan<NCHAN;++iChan)
      TRKsigma_mean[iChan]+=sigmaTRK[iw][iChan];
  }
  for(int iPmt=0;iPmt<NPMT;++iPmt){
 
  PMTsigma_meanLG[iPmt]/=static_cast<double>(islot);
  PMTsigma_meanHG[iPmt]/=static_cast<double>(islot);
  }
  for(int iChan=0;iChan<NCHAN;++iChan)
    TRKsigma_mean[iChan]/=static_cast<double>(islot);
  double PMTsigma_errLG[NPMT];
  double PMTsigma_errHG[NPMT];
  double TRKsigma_err[NCHAN];
  for(int iPmt=0;iPmt<NPMT;++iPmt){
    PMTsigma_errLG[iPmt]=0;
    PMTsigma_errHG[iPmt]=0;
  }

  for(int iChan=0;iChan<NCHAN;++iChan)
    TRKsigma_err[iChan]=0;

  for(int iw=0;iw<islot;++iw){
   
    for(int iPmt=0;iPmt<NPMT;++iPmt){
      PMTsigma_errLG[iPmt]+=(sigmaLG[iw][iPmt]-PMTsigma_meanLG[iPmt])*(sigmaLG[iw][iPmt]-PMTsigma_meanLG[iPmt]);
      PMTsigma_errHG[iPmt]+=(sigmaHG[iw][iPmt]-PMTsigma_meanHG[iPmt])*(sigmaHG[iw][iPmt]-PMTsigma_meanHG[iPmt]);
    }
    for(int iChan=0;iChan<NCHAN;++iChan)
      TRKsigma_err[iChan]+=(sigmaTRK[iw][iChan]-TRKsigma_mean[iChan])*(sigmaTRK[iw][iChan]-TRKsigma_mean[iChan]);
  }
  for(int iChan=0;iChan<NCHAN;++iChan){
    TRKsigma_err[iChan]/=static_cast<double>(islot-1);
    err_sigma_TRK[iChan]=sqrt(TRKsigma_err[iChan]);
  }
  for(int iPmt=0;iPmt<NPMT;++iPmt){
  PMTsigma_errHG[iPmt]/=static_cast<double>(islot-1);
  PMTsigma_errLG[iPmt]/=static_cast<double>(islot-1);
  err_sigma_HG[iPmt]=sqrt(PMTsigma_errHG[iPmt]);
  err_sigma_LG[iPmt]=sqrt(PMTsigma_errLG[iPmt]);
  }
  
  /*
  for(auto fit : readfile) {
    calbuffer=LCalibration::Read(fit.c_str());
    //calbuffer->WriteTXT("testOther.txt"); 
    (*output) += (*calbuffer);
    calbuffer=NULL; 
  }
  */
  (*output)/=readfile.size();
  return output;
}


bool IsGood(const LCalibration *reference,const double* err_sigma_LG,const double* err_sigma_HG,const double* err_sigma_trk,const LCalibration *latest){
  bool result;

  bool Trackercheck= CompareTracker(reference,err_sigma_trk,latest);
  bool PMTcheck= ComparePMT(latest,err_sigma_LG,err_sigma_HG,reference);
  return Trackercheck && PMTcheck;
}

bool ComparePMT(const LCalibration *first, const double* err_sigma_LG,const double* err_sigma_HG,const LCalibration *second){
  bool result=true;
  const double *PMTmean1HG=first->GetCaloHGCalibration()->GetPedestal();
  const double *PMTmean2HG=second->GetCaloHGCalibration()->GetPedestal();
  const double *PMTsigma1HG=first->GetCaloHGCalibration()->GetSigma();
  const double *PMTsigma2HG=second->GetCaloHGCalibration()->GetSigma();

  const double *PMTmean1LG=first->GetCaloLGCalibration()->GetPedestal();
  const double *PMTmean2LG=second->GetCaloLGCalibration()->GetPedestal();
  const double *PMTsigma1LG=first->GetCaloLGCalibration()->GetSigma();
  const double *PMTsigma2LG=second->GetCaloLGCalibration()->GetSigma(); 

  int wrongPedHG=0;
  int wrongSigmaHG=0;
  int wrongPedLG=0;
  int wrongSigmaLG=0;
  for(int iPmt=0;iPmt<NPMT;++iPmt){
    if(std::abs(PMTmean1HG[iPmt]-PMTmean2HG[iPmt])>PMT_PED_THRESHOLD*PMTsigma1HG[iPmt]) ++wrongPedHG;
    if(std::abs(PMTsigma1HG[iPmt]-PMTsigma2HG[iPmt])>PMT_SIGMA_THRESHOLD*err_sigma_HG[iPmt]) ++wrongSigmaHG;
    if(std::abs(PMTmean1LG[iPmt]-PMTmean2LG[iPmt])>PMT_PED_THRESHOLD*PMTsigma1LG[iPmt]) ++wrongPedLG;
    if(std::abs(PMTsigma1LG[iPmt]-PMTsigma2LG[iPmt])>PMT_SIGMA_THRESHOLD*err_sigma_LG[iPmt]) ++wrongSigmaLG;
  }
  if(wrongPedHG>PMT_MAX_FAIL_CHAN || wrongSigmaHG>PMT_MAX_FAIL_CHAN) result=false;
  if(wrongPedLG>PMT_MAX_FAIL_CHAN || wrongSigmaLG>PMT_MAX_FAIL_CHAN) result=false;
  std::cout<<"wrong PMT LG ped: "<<wrongPedLG<<" wrong PMT LG sigma: "<<wrongSigmaLG<<std::endl;
  std::cout<<"wrong PMT HG ped: "<<wrongPedLG<<" wrong PMT HG sigma: "<<wrongSigmaHG<<std::endl;
  return result;
} 

bool CompareTracker(const LCalibration *first,const double* err_sigma_trk,const  LCalibration *second){
  bool result = true;
  
  const double *TRKmean1=first->GetTrackerCalibration()->GetPedestal(0);
  const double *TRKmean2=second->GetTrackerCalibration()->GetPedestal(0);
  const double *TRKsigma1=first->GetTrackerCalibration()->GetSigma(0);
  const double *TRKsigma2=second->GetTrackerCalibration()->GetSigma(0);
  
  
  int wrongPed=0;
  int wrongSigma=0;
  for(int iChan=0;iChan<NCHAN;++iChan){
    if(std::abs(TRKmean1[iChan]-TRKmean2[iChan])>TRK_PED_THRESHOLD*TRKsigma1[iChan]) ++wrongPed;
    if(std::abs(TRKsigma1[iChan]-TRKsigma2[iChan])>TRK_SIGMA_THRESHOLD*err_sigma_trk[iChan]) {++wrongSigma;
      std::cout<<"Chan= "<<iChan<<" sigma ref= "<<TRKsigma1[iChan]<<" new sigma= "<<TRKsigma2[iChan]<<" Max dist= "<< TRK_SIGMA_THRESHOLD*err_sigma_trk[iChan]<<std::endl;
    }  
  }  

  std::cout<<"wrong TRK ped: "<<wrongPed<<" wrong TRK sigma: "<<wrongSigma<<std::endl;
  if(wrongPed> TRK_MAX_FAIL_CHAN || wrongSigma>TRK_MAX_FAIL_CHAN) result = false;
  
  return result;
}

void IsGoodGraphics(const LCalibration *reference,const double* err_sigma_LG,const double* err_sigma_HG,const double* err_sigma_trk,const LCalibration *latest){
  const double *TRKmean1=reference->GetTrackerCalibration()->GetPedestal(0);
  const double *TRKmean2=latest->GetTrackerCalibration()->GetPedestal(0);
  const double *TRKsigma1=reference->GetTrackerCalibration()->GetSigma(0);
  const double *TRKsigma2=latest->GetTrackerCalibration()->GetSigma(0);

  const double *PMTmean1HG=reference->GetCaloHGCalibration()->GetPedestal();
  const double *PMTmean2HG=latest->GetCaloHGCalibration()->GetPedestal();
  const double *PMTsigma1HG=reference->GetCaloHGCalibration()->GetSigma();
  const double *PMTsigma2HG=latest->GetCaloHGCalibration()->GetSigma();

  const double *PMTmean1LG=reference->GetCaloLGCalibration()->GetPedestal();
  const double *PMTmean2LG=latest->GetCaloLGCalibration()->GetPedestal();
  const double *PMTsigma1LG=reference->GetCaloLGCalibration()->GetSigma();
  const double *PMTsigma2LG=latest->GetCaloLGCalibration()->GetSigma();

  //PMTS 

  TGraphErrors *ref_ped_PMT_LG=new TGraphErrors(NPMT);
  TGraphErrors *ref_sigma_PMT_LG=new TGraphErrors(NPMT);
  TGraphErrors *ref_ped_PMT_HG=new TGraphErrors(NPMT);
  TGraphErrors *ref_sigma_PMT_HG=new TGraphErrors(NPMT);

  TGraphErrors *new_ped_PMT_LG=new TGraphErrors(NPMT);
  TGraphErrors *new_sigma_PMT_LG=new TGraphErrors(NPMT);
  TGraphErrors *new_ped_PMT_HG=new TGraphErrors(NPMT);
  TGraphErrors *new_sigma_PMT_HG=new TGraphErrors(NPMT);

  for(int iPmt=0;iPmt<NPMT;++iPmt){
    ref_ped_PMT_LG->SetPoint(iPmt,iPmt,PMTmean1LG[iPmt]);
    ref_ped_PMT_LG->SetPointError(iPmt,0,PMT_PED_THRESHOLD*PMTsigma1LG[iPmt]);
    ref_ped_PMT_HG->SetPoint(iPmt,iPmt,PMTmean1HG[iPmt]);
    ref_ped_PMT_HG->SetPointError(iPmt,0,PMT_PED_THRESHOLD*PMTsigma1HG[iPmt]);

    new_ped_PMT_LG->SetPoint(iPmt,iPmt,PMTmean2LG[iPmt]);
    new_ped_PMT_HG->SetPoint(iPmt,iPmt,PMTmean2HG[iPmt]);

    ref_sigma_PMT_LG->SetPoint(iPmt,iPmt,PMTsigma1LG[iPmt]);
    ref_sigma_PMT_LG->SetPointError(iPmt,0,PMT_SIGMA_THRESHOLD*err_sigma_LG[iPmt]);
    ref_sigma_PMT_HG->SetPoint(iPmt,iPmt,PMTsigma1HG[iPmt]);
    ref_sigma_PMT_HG->SetPointError(iPmt,0,PMT_SIGMA_THRESHOLD*err_sigma_HG[iPmt]);

    new_sigma_PMT_LG->SetPoint(iPmt,iPmt,PMTsigma2LG[iPmt]);
    new_sigma_PMT_HG->SetPoint(iPmt,iPmt,PMTsigma2HG[iPmt]);
  }

  //Tracker
  TGraphErrors *ref_ped_TRK[2][N_LADDER];
  TGraphErrors *ref_sigma_TRK[2][N_LADDER];

  TGraphErrors *new_ped_TRK[2][N_LADDER];
  TGraphErrors *new_sigma_TRK[2][N_LADDER];

  for(int iLd=0;iLd<N_LADDER;++iLd){
    for(int iSide=0;iSide<2;++iSide){
      ref_ped_TRK[iSide][iLd]=new TGraphErrors(SIDE_CHAN);
      ref_sigma_TRK[iSide][iLd]=new TGraphErrors(SIDE_CHAN);

      new_ped_TRK[iSide][iLd]=new TGraphErrors(SIDE_CHAN);
      new_sigma_TRK[iSide][iLd]=new TGraphErrors(SIDE_CHAN);
    }

  }

  for(int iChan=0;iChan<NCHAN;++iChan){
    ref_ped_TRK[ChanToSide(iChan)][ChanToLadder(iChan)]->SetPoint(ChanToSideChan(iChan),ChanToSideChan(iChan),TRKmean1[iChan]);
    ref_ped_TRK[ChanToSide(iChan)][ChanToLadder(iChan)]->SetPointError(ChanToSideChan(iChan),0,TRK_PED_THRESHOLD*TRKsigma1[iChan]);
    ref_sigma_TRK[ChanToSide(iChan)][ChanToLadder(iChan)]->SetPoint(ChanToSideChan(iChan),ChanToSideChan(iChan),TRKsigma1[iChan]);
    ref_sigma_TRK[ChanToSide(iChan)][ChanToLadder(iChan)]->SetPointError(ChanToSideChan(iChan),0,TRK_SIGMA_THRESHOLD*err_sigma_trk[iChan]);

    new_ped_TRK[ChanToSide(iChan)][ChanToLadder(iChan)]->SetPoint(ChanToSideChan(iChan),ChanToSideChan(iChan),TRKmean2[iChan]);
    new_sigma_TRK[ChanToSide(iChan)][ChanToLadder(iChan)]->SetPoint(ChanToSideChan(iChan),ChanToSideChan(iChan),TRKsigma2[iChan]);
  }

  TCanvas* can=new TCanvas();

  can->Print("test.pdf[","pdf");
  drawing_1D(ref_ped_PMT_HG,new_ped_PMT_HG,250,500,"Pedestal HG;PMT;ADC",false)->Print("test.pdf","pdf");
  drawing_1D(ref_sigma_PMT_HG,new_sigma_PMT_HG,0,20,"Sigma HG;PMT;ADC",false)->Print("test.pdf","pdf");
  drawing_1D(ref_ped_PMT_LG,new_ped_PMT_LG,150,500,"Pedestal LG;PMT;ADC",false)->Print("test.pdf","pdf");
  drawing_1D(ref_sigma_PMT_LG,new_sigma_PMT_LG,0,20,"Sigma LG;PMT;ADC",false)->Print("test.pdf","pdf");
  drawing6_chan_1D(ref_ped_TRK[0],new_ped_TRK[0],1200,1500,";chan;ADC",false)->Print("test.pdf","pdf");
  drawing6_chan_1D(ref_ped_TRK[1],new_ped_TRK[1],1200,1500,";chan;ADC",false)->Print("test.pdf","pdf");
  drawing6_chan_1D(ref_sigma_TRK[0],new_sigma_TRK[0],0,20,";chan;ADC",false)->Print("test.pdf","pdf");
  drawing6_chan_1D(ref_sigma_TRK[1],new_sigma_TRK[1],0,20,";chan;ADC",false)->Print("test.pdf","pdf");

  can->Print("test.pdf]","pdf");
  return;
}


void SlowDrift(const LCalibration* seed,const int nFiles,const double offset,char* label){
  std::cout<<"Creating fake calibrations"<<std::endl;
  for(int ir=1;ir<=nFiles;++ir){
    LCalibration*   out=LCalibration::CreateFakeCalibration(seed,ir*offset,ir*offset);
    std::string filename;
    filename.append(label);
    filename.append("_");
    filename.append(std::to_string(ir));
    filename.append(".txt");
    out->WriteTXT(filename.c_str());
    out=NULL;

  }
  return;
}

void Jump(const LCalibration* seed,const int nFiles,const double jump,char* label){
  typedef std::chrono::high_resolution_clock myclock;
  myclock::time_point beginning = myclock::now();
  std::default_random_engine generator;
  myclock::duration d = myclock::now() - beginning;
  unsigned seed2 = d.count();
  generator.seed(seed2);

  std::uniform_int_distribution<int> distribution(0,nFiles);
  int threshold=distribution(generator);
  for(int ir=0;ir<nFiles;++ir){
    LCalibration* out;
    if(ir<threshold) out=LCalibration::CreateFakeCalibration(seed);
    else  out=LCalibration::CreateFakeCalibration(seed,jump,jump);
    std::string filename;
    filename.append(label);
    filename.append("_");
    filename.append(std::to_string(ir));
    filename.append(".txt");
    out->WriteTXT(filename.c_str());
    out=NULL;


  }
  return;

}



TCanvas *drawing_1D(TGraphErrors *input,TGraphErrors *input2,double ymin,double ymax,const char* title,bool log){
  TCanvas* result=new TCanvas();
  input->SetMarkerColor(8);
  input->SetLineColor(8);
  input->SetLineWidth(5);
  input2->SetMarkerStyle(7);
  if(log) gPad->SetLogy();
  gPad->SetLeftMargin(0.15);
  TH1* bg=gPad->DrawFrame(0,ymin,NPMT,ymax);
  bg->SetTitle(title);
  result->Update();
  input->Draw("P same");
  input2->Draw("P same");
  return result;
}


TCanvas *drawing6_chan_1D(TGraphErrors *ladder[N_LADDER],TGraphErrors *ladder2[N_LADDER],double ymin,double ymax,const char* title,bool log){
  TCanvas *display=new TCanvas();
  display->Divide(3,2);

  int pad;
  for(int i=0;i<N_LADDER;++i){
    if (i==0) pad=1;
    else if(i==1) pad=4;
    else if(i==2) pad=2;
    else if(i==3) pad=5;
    else if(i==4) pad=3;
    else if(i==5) pad=6;

    display->cd(pad);
    gPad->cd(pad);
    gPad->SetLeftMargin(0.15);
   TH1* bg= gPad->DrawFrame(0,ymin,SIDE_CHAN,ymax);
   bg->SetTitle(title);
   display->Update();
    if(log) gPad->SetLogy();
    ladder[i]->SetMarkerColor(8);
    ladder[i]->SetLineColor(8);
    ladder[i]->SetLineWidth(5);
    ladder2[i]->SetMarkerStyle(7);
    ladder[i]->Draw("P same");
    ladder2[i]->Draw("P same");
  }
  return display;
}
