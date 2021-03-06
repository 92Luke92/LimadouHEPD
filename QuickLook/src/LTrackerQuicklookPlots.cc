#include "detector_const.hh"

#include "LTrackerQuicklookPlots.hh"

#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"
#include "TROOT.h"
#include "TRint.h"
#include "TStyle.h"
#include "TLine.h"

TCanvas *drawing6_chan_1D(TH1D *ladder[N_LADDER],const bool log,const bool logx){
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
    if(log) gPad->SetLogy();
    if(logx) gPad->SetLogx();
    ladder[i]->SetStats(0);
    ladder[i]->Draw();
  }
  return display;
}

TCanvas *drawing6_chan_2D(TH2D *ladder[N_LADDER]){
  TCanvas *display=new TCanvas();
  TLine *line_adc[N_LADDER];
  TLine *line_va[N_LADDER][SIDE_VA-1];
  display->Divide(3,2);
  int pad;
  double ymin=ladder[0]->GetYaxis()->GetXmin();
  double ymax=ladder[0]->GetYaxis()->GetXmax();
  
  for(int i=0;i<N_LADDER;++i){
    if (i==0) pad=1;
    else if(i==1) pad=4;
    else if(i==2) pad=2;
    else if(i==3) pad=5;
    else if(i==4) pad=3;
    else if(i==5) pad=6;
    
    line_adc[i]=new TLine((double)ADC_CHAN,ymin,(double)ADC_CHAN,ymax);
    line_adc[i]->SetLineColor(1);
    line_adc[i]->SetLineStyle(1);
    ladder[i]->GetYaxis()->SetRangeUser(ymin-ymin*0.2, ymax+ymax*0.2); 
    
    display->cd(pad);
    gPad->cd(pad);
    gPad->SetLogz();
    ladder[i]->SetStats(0);
    ladder[i]->Draw("zcol");
    line_adc[i]->Draw("same");

    for(int iva=0;iva<SIDE_VA;++iva){
      line_va[i][iva]=new TLine(VA_CHAN+iva*VA_CHAN,ymin,VA_CHAN+iva*VA_CHAN,ymax);
      line_va[i][iva]->SetLineColor(20);
      line_va[i][iva]->SetLineStyle(2);

      line_va[i][iva]->Draw("same");
    }
  }
  return display;
}

TCanvas *drawing6_va_2D(TH2D *ladder[N_LADDER]){
  TCanvas *display=new TCanvas();
  TLine *line_adc[N_LADDER];
  TLine *line_va[N_LADDER][SIDE_VA-1];
  display->Divide(3,2);
  int pad;
  double ymin=ladder[0]->GetYaxis()->GetXmin();
  double ymax=ladder[0]->GetYaxis()->GetXmax();
  
  for(int i=0;i<N_LADDER;++i){
    if (i==0) pad=1;
    else if(i==1) pad=4;
    else if(i==2) pad=2;
    else if(i==3) pad=5;
    else if(i==4) pad=3;
    else if(i==5) pad=6;
    
    line_adc[i]=new TLine((double)SIDE_VA/2,ymin,(double)SIDE_VA/2,ymax);
    line_adc[i]->SetLineColor(1);
    line_adc[i]->SetLineStyle(1);
    
    display->cd(pad);
    gPad->cd(pad);
    gPad->SetLogz();
    ladder[i]->SetStats(0);
    ladder[i]->Draw("zcol");
    line_adc[i]->Draw("same");
    for(int iva=0;iva<SIDE_VA;++iva){
      line_va[i][iva]=new TLine(iva,ymin,iva,ymax);
      line_va[i][iva]->SetLineColor(20);
      line_va[i][iva]->SetLineStyle(2);

      line_va[i][iva]->Draw("same");
    }
  }
  return display;
}


TCanvas *drawing12_2D(TH2D *pside[N_LADDER],TH2D *nside[N_LADDER]){
   TCanvas *display=new TCanvas();
   display->Divide(3,4);
   int pad1;
   int pad2;
   for(int i=0;i<N_LADDER;++i){
      if(i==0){pad1=1; pad2=4;}
      else if(i==1){pad1=7; pad2=10;}
      else if(i==2){pad1=2; pad2=5;}
      else if(i==3){pad1=8; pad2=11;}
      else if(i==4){pad1=3; pad2=6;}
      else if(i==5){pad1=9; pad2=12;}
      display->cd(pad1);
      gPad->cd(pad1);
      //gPad->SetLogz();
      pside[i]->SetStats(0);
      pside[i]->Draw("zcol");
      display->cd(pad2);
      gPad->cd(pad2);
      //gPad->SetLogz();
      nside[i]->SetStats(0);
      nside[i]->Draw("zcol");
     
   }
   return display;
}
