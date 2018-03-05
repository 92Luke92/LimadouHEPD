#include "LEvRec0File.hh"
#include "LEvRec0.hh"
#include <iostream>
#include "TH1D.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TFile.h"
#include "TString.h"
#include "LOfflineL0Selector.hh"
#include "LCalibration.hh"

const int NBINS = 4096/4;
const double XMIN = 0.;
const double XMAX = 4096.;

int main(int argc, char **argv) {

   std::cout << std::endl
      << "Usage: CalibCheck <cal1.root> <cal2.root> ... <calN.root>  <outFile.root> "
      << std::endl;

   double   *calo_pedestal[NPMT];
   double   *calo_sigma[NPMT]; 
   int       index[NPMT];
   int       index2[NCHAN];

   int       ped_gr[NPMT];
   int       sig_gr[NPMT];

   double   *sil_pedestal[NCHAN];
   double   *sil_sigma[NCHAN]; 
   int       sil_ped_gr[NCHAN];
   int       sil_sig_gr[NCHAN];
 

   LCalibration   **calib = new LCalibration*;
   TGraph         *ped_graph;
   TGraph         *sig_graph;
   TMultiGraph    *mgrped = new TMultiGraph();
   TMultiGraph    *mgrsig = new TMultiGraph();

   TGraph         *sil_ped_graph;
   TGraph         *sil_sig_graph;
   TMultiGraph    *sil_mgrped = new TMultiGraph();
   TMultiGraph    *sil_mgrsig = new TMultiGraph();

   int            sil_ped_diff[NCHAN]; 
   int            sil_sigma_diff[NCHAN]; 
  

   for (int arg = 0; arg<(argc-1); arg++)
      calib[arg] = LCalibration::ReadROOT(argv[arg+1]);
   
   std::cout << "\nCalibration files loaded = " << argc-1  << std::endl << std::endl;

   for (int i = 0; i<NPMT; i++)
   {
      calo_pedestal[i] = new double[argc-1];
      calo_sigma[i] = new double[argc-1];
      index[i] = i;
   }

   for (int i = 0; i<NCHAN; i++)
   {
      sil_pedestal[i] = new double[argc-1];
      sil_sigma[i] = new double[argc-1];
      index2[i] = i;
   }

// TFile *fout = new TFile(argv[argc-1],"recreate");
   TFile *fout = new TFile("output.root","recreate");
   
   for (int j = 0; j<(argc-1); j++)
   {
      for (int i = 0; i<NPMT; i++)
      {
	 calo_pedestal[i][j] = calib[j]->GetCaloHGCalibration()->GetPedestal()[i];
	 calo_sigma[i][j] = calib[j]->GetCaloHGCalibration()->GetSigma()[i];
	 ped_gr[i] = calo_pedestal[i][j];
	 sig_gr[i] = calo_sigma[i][j];
	 // std::cout << "ped = " << calo_pedestal[i][j] << " sig = "
	 //   << calo_sigma[i][j] << std::endl;
      }

      for (int l = 0; l<NCHAN; l++)
      {
      	 sil_pedestal[l][j] = calib[j]->GetTrackerCalibration()->GetPedestal(0)[l];
	 sil_sigma[l][j] = calib[j]->GetTrackerCalibration()->GetSigma(0)[l];
	 sil_ped_gr[l] = sil_pedestal[l][j];
	 sil_sig_gr[l] = sil_sigma[l][j];
	 // if (l > 1536 && l <1600)std::cout << "ped = " << sil_pedestal[l][j] << " sig = "
	 // 	   << sil_sigma[l][j] << std::endl;
      }
      // std::cout << std::endl;


      ped_graph = new TGraph(NPMT, index, ped_gr);
      ped_graph->SetMarkerStyle(20);
      ped_graph->SetName("ped");
      ped_graph->SetTitle(argv[j+1]);
      ped_graph->GetXaxis()->SetTitle("PMT");
      ped_graph->GetYaxis()->SetTitle("ADC counts");
      ped_graph->SetMarkerColor(j+1);
      //ped_graph->SetLineWidth(0);
      ped_graph->SetLineColor(j+1);
      mgrped->Add(ped_graph);

      sig_graph = new TGraph(NPMT, index, sig_gr);
      sig_graph->SetMarkerStyle(20);
      sig_graph->SetName("sig");
      sig_graph->SetTitle(argv[j+1]);
      sig_graph->GetXaxis()->SetTitle("PMT");
      sig_graph->GetYaxis()->SetTitle("ADC counts");
      sig_graph->SetMarkerColor(j+1);
      //sig_graph->SetLineWidth(0);
      sig_graph->SetLineColor(j+1);
      mgrsig->Add(sig_graph);

      ped_graph->Write();
      sig_graph->Write();
      
      sil_ped_graph = new TGraph(COLUMN_CHAN, &index2[COLUMN_CHAN],
				 &sil_ped_gr[COLUMN_CHAN]);
      sil_ped_graph->SetMarkerStyle(20);
      sil_ped_graph->SetName("ped");
      sil_ped_graph->SetTitle(argv[j+1]);
      sil_ped_graph->GetXaxis()->SetTitle("Channel");
      sil_ped_graph->GetYaxis()->SetTitle("ADC counts");
      sil_ped_graph->SetMarkerColor(j+1);
      //sil_ped_graph->SetLineWidth(0);
      sil_ped_graph->SetLineColor(j+1);
      sil_mgrped->Add(sil_ped_graph);

      sil_sig_graph = new TGraph(COLUMN_CHAN, &index2[COLUMN_CHAN],
				 &sil_sig_gr[COLUMN_CHAN]);
      
      sil_sig_graph->SetMarkerStyle(20);
      sil_sig_graph->SetName("sig");
      sil_sig_graph->SetTitle(argv[j+1]);
      sil_sig_graph->GetXaxis()->SetTitle("Channel");
      sil_sig_graph->GetYaxis()->SetTitle("ADC counts");
      sil_sig_graph->SetMarkerColor(j+1);
      //sil_sig_graph->SetLineWidth(0);
      sil_sig_graph->SetLineColor(j+1);
      sil_mgrsig->Add(sil_sig_graph);

      sil_ped_graph->Write();
      sil_sig_graph->Write();
   }

   for (int l = 0; l<NCHAN; l++)
   {
      sil_ped_diff[l] = sil_pedestal[l][argc-2] - sil_pedestal[l][1];
      // std::cout << "argc = "<< argc << "  last cal ped = "
      // 		<< sil_pedestal[l][argc-2] <<" first cal ped = "
      // 		<< sil_pedestal[l][1] << std::endl;
      sil_sigma_diff[l] = sil_sigma[l][argc-2]  - sil_sigma[l][1] ;

   }
   TGraph  *sil_ped_diff_gr =  new TGraph(COLUMN_CHAN, &index2[COLUMN_CHAN],
				       &sil_ped_diff[COLUMN_CHAN]);
   TGraph  *sil_sig_diff_gr =  new TGraph(COLUMN_CHAN, &index2[COLUMN_CHAN],
				       &sil_sigma_diff[COLUMN_CHAN]);

   sil_ped_diff_gr->SetTitle("pedestal diff");
   sil_ped_diff_gr->SetName("pedestal diff");
   sil_ped_diff_gr->GetXaxis()->SetTitle("Channel");
   sil_ped_diff_gr->GetYaxis()->SetTitle("ADC counts");
   sil_ped_diff_gr->Write();

   sil_sig_diff_gr->SetTitle("sigma diff");
   sil_sig_diff_gr->SetName("sigma diff");
   sil_sig_diff_gr->GetXaxis()->SetTitle("Channel");
   sil_sig_diff_gr->GetYaxis()->SetTitle("ADC counts");
   sil_sig_diff_gr->Write();
   
   mgrped->SetName("mutliPed");
   mgrped->Draw("AP");
   mgrped->GetXaxis()->SetTitle("PMT");
   mgrped->GetYaxis()->SetTitle("ADC counts");
   mgrped->Write();
   
   mgrsig->SetName("mutliSig");
   mgrsig->Draw("AP");
   mgrsig->GetXaxis()->SetTitle("PMT");
   mgrsig->GetYaxis()->SetTitle("ADC counts");
   mgrsig->Write();

   
   sil_mgrped->SetName("mutliPed");
   sil_mgrped->Draw("AP");
   sil_mgrped->GetXaxis()->SetTitle("Channel");
   sil_mgrped->GetYaxis()->SetTitle("ADC counts");
   sil_mgrped->Write();
   
   sil_mgrsig->SetName("mutliSig");
   sil_mgrsig->Draw("AP");
   sil_mgrsig->GetXaxis()->SetTitle("Channel");
   sil_mgrsig->GetYaxis()->SetTitle("ADC counts");
   sil_mgrsig->Write();

   fout->Close();

   
   return 0;
}
