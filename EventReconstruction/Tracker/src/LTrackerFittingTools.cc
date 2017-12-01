#include "LEvRec0.hh"
#include "LTrackerTools.hh"
#include "LTrackerCluster.hh"
#include "LTrackerFittingTools.hh"
#include "analysis_const.hh"

#include <iostream>
#include "TFile.h"
#include "TLatex.h"
#include "TH1.h"
#include "TMath.h"
#include "TAxis.h"
#include "TF1.h"
#include "TH2.h"
#include "TPad.h"
#include "string.h"
#include "TTree.h"
#include "TROOT.h"
#include "TLegend.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPad.h"
#include "TLine.h"
#include <string>
#include "TPaveLabel.h"
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <string>
#include "string.h"


double Correction_1(double eta, double par0, double par1, double par2){
	double result=-999;
	double fitting = par0+par1+par2;
	//if(eta<-0.1||eta>1.1) eta = eta - int(eta);
	eta = eta - int(eta);
	result = par0+par1*eta+par2*eta*eta;
	result /= fitting;
	return 1./result; 
}
// final correction function, based on the parabolic fit parameters that are readed from "fitting_parameters... .txt"
double Correction_2(TProfile * histo, double x){
	double pos_x=-999;
	double Nbin = histo->GetNbinsX();
	double max = histo->GetBinContent(1);
	double min = histo->GetBinContent(Nbin+1);
	double first_corr = (max+min)/2.;
	histo->Scale(1/first_corr);
	pos_x = histo->GetBin(x);
	return 1/(histo->GetBinContent(pos_x));
}
// it fits the counts vs. eta profile and it gets the parabola parameters
void Fitting_func(TProfile *histo[6], double par0[6], double par1[6], double par2[6]){
	for (int i=0; i< 6; i++){
		TF1 * myfit= new TF1("myfit","[0]+[1]*x+[2]*x*x", 0., 1.);
		//myfit->FixParameter(1,0.);
		histo[i]->Fit("myfit");
		par0[i] = myfit->GetParameter("p0");
		par1[i] = myfit->GetParameter("p1");
		par2[i] = myfit->GetParameter("p2");
	}
	return;
}

void Fitting_func_sym(TProfile *histo[6], double par0[6], double par1[6], double par2[6]/*, double errpar2[6]*/){
	for (int i=0; i< 6; i++){
		TF1 * myfit= new TF1("myfit","([0]+[1]/4)+[1]*(x*x-x)", 0., 1.);
		//myfit->FixParameter(1,0.);
		histo[i]->Fit("myfit","R");
		par0[i] = myfit->GetParameter("p0")+myfit->GetParameter("p1")/4;
		par1[i] = -(myfit->GetParameter("p1"));
		par2[i] = myfit->GetParameter("p1");
		//errpar2[i] = myfit->GetParError("p1");
	}
	return;
}

// langaufunction taken from root examples
Double_t intlangaufun(Double_t *x, Double_t *par) {

	//Fit parameters:
    //par[0]=Width (scale) parameter of Landau density
    //par[1]=Most Probable (MP, location) parameter of Landau density
    //par[2]=Total area (integral -inf to inf, normalization constant)
    //par[3]=Width (sigma) of convoluted Gaussian function
    //
    //In the Landau distribution (represented by the CERNLIB approximation),
    //the maximum is located at x=-0.22278298 with the location parameter=0.
    //This shift is corrected within this function, so that the actual

    //maximum is identical to the MP parameter.
      // Numeric constants
      Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
      Double_t mpshift  = -0.22278298;       // Landau maximum location

      // Control constants
      Double_t np = 100.0;      // number of convolution steps
      Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

      // Variables
      Double_t xx;
      Double_t mpc;
      Double_t fland;
      Double_t sum = 0.0;
      Double_t xlow,xupp;
      Double_t step;
      Double_t i;

      // MP shift correction
      mpc = par[1] - mpshift * par[0];

      // Range of convolution integral
      xlow = x[0] - sc * par[3];
      xupp = x[0] + sc * par[3];

      step = (xupp-xlow) / np;

      // Convolution integral of Landau and Gaussian + a Gaussian
      for(i=1.0; i<=np/2; i++) {
         xx = xlow + (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);

         xx = xupp - (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);
      }

      return (par[2] * step * sum * invsq2pi / par[3]);
}


// langaufunction taken from root examples
Double_t langaufun(Double_t *x, Double_t *par) {

	//Fit parameters:
    //par[0]=Width (scale) parameter of Landau density
    //par[1]=Most Probable (MP, location) parameter of Landau density
    //par[2]=Total area (integral -inf to inf, normalization constant)
    //par[3]=Width (sigma) of convoluted Gaussian function
    //
    //In the Landau distribution (represented by the CERNLIB approximation),
    //the maximum is located at x=-0.22278298 with the location parameter=0.
    //This shift is corrected within this function, so that the actual

    //maximum is identical to the MP parameter.
      // Numeric constants
      Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
      Double_t mpshift  = -0.22278298;       // Landau maximum location

      // Control constants
      Double_t np = 100.0;      // number of convolution steps
      Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

      // Variables
      Double_t xx;
      Double_t mpc;
      Double_t fland;
      Double_t sum = 0.0;
      Double_t xlow,xupp;
      Double_t step;
      Double_t i;

      // MP shift correction
      mpc = par[1] - mpshift * par[0];

      // Range of convolution integral
      xlow = x[0] - sc * par[3];
      xupp = x[0] + sc * par[3];

      step = (xupp-xlow) / np;

      // Convolution integral of Landau and Gaussian + a Gaussian
      for(i=1.0; i<=np/2; i++) {
         xx = xlow + (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);

         xx = xupp - (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);
      }

      return (par[2] * step * sum * invsq2pi / par[3]);//+par[4]*TMath::Gaus(x[0],par[5],par[3]);
}

Double_t langaufunmean(Double_t *x, Double_t *par) {
      // Numeric constants
      Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
      Double_t mpshift  = -0.22278298;       // Landau maximum location

      // Control constants
      Double_t np = 100.0;      // number of convolution steps
      Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

      // Variables
      Double_t xx;
      Double_t mpc;
      Double_t fland;
      Double_t sum = 0.0;
      Double_t xlow,xupp;
      Double_t step;
      Double_t i;

      // MP shift correction
      mpc = par[1] - mpshift * par[0];

      // Range of convolution integral
      xlow = x[0] - sc * par[3];
      xupp = x[0] + sc * par[3];

      step = (xupp-xlow) / np;

      // Convolution integral of Landau and Gaussian by sum
      for(i=1.0; i<=np/2; i++) {
         xx = xlow + (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);

         xx = xupp - (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);
      }

      return (x[0]*par[2] * step * sum * invsq2pi / par[3]);
}
// langau fit "ad hoc" for protons
void Langau(TH1F *histo[6], double langau_Mean[6], double langau_Sigma[6], double chi2red[6], double inf, double sup, Double_t values[6][4], std::string check, std::string typeofpart){
	for (int i=0; i< 6; i++){
		if (((i!=2)&&(i!=3)&&(check=="p"))||((i!=2)&&(check=="n"))){
			langau_Mean[i]=0;
			langau_Sigma[i]=0;
			chi2red[i]=0;
		} else{
		//inf_prov and sup_prov in order to fit better the distribution around the  
		float max_bin = histo[i]->GetMaximumBin();
		//float mean_histo = histo[i]->GetMean();
		float max_center = histo[i]->GetBinCenter(max_bin);
		float integral = histo[i]->Integral(inf,sup);
		float std_histo = histo[i]->GetStdDev();
		float inf_prov,sup_prov;
		if(typeofpart=="muon"){
			inf_prov = max_center-std_histo;
			sup_prov = max_center+1.5*std_histo;
		}
		else{
			inf_prov = max_center-std_histo;
			sup_prov = max_center+1.5*std_histo;	
		}
		std::cout<<"max_bin="<<max_center<<" std="<<std_histo<<std::endl;
		std::cout<<"inf_prov="<<inf_prov<<" sup_prov="<<sup_prov<<std::endl;
		TF1 *langau = new TF1("langau",langaufun,inf_prov,sup_prov,4);
		//Double_t values[4];
		values[i][0]=6; 
		values[i][1]=max_center; 
		values[i][2]=integral;
		values[i][3]=4;
		langau->SetParameters(6.,max_center,integral,4.,integral/20,25.);
		if(typeofpart=="muon"){
			langau->SetParLimits(0,0.,60.); //Width
			langau->SetParLimits(1,0.,200.); //MPV
			langau->SetParLimits(2,0.,100000.); //Area langau muon
			langau->SetParLimits(3,0.,60.); //sigma gauss
		}
		else{
			langau->SetParLimits(0,0.,60.); //Width
			langau->SetParLimits(1,0.,600.); //MPV
			langau->SetParLimits(2,0.,100000.); //Area langau proton
			langau->SetParLimits(3,0.,60.); //sigma gauss
		}
		//langau->SetParLimits(4,20.,1000000.); // Area gauss
		//langau->SetParLimits(5,15.,35.); // Mean gauss
		histo[i]->Fit("langau","","",inf_prov,sup_prov);
		values[i][0] = langau->GetParameter("p0");
		values[i][1] = langau->GetParameter("p1");
		values[i][2] = langau->GetParameter("p2");
		values[i][3] = langau->GetParameter("p3");

		TF1 *langaumean = new TF1("langaumean",langaufunmean,inf,sup,4);
		langaumean->SetParameters(values[i]);
		TF1 * intlangaumean = new TF1("intlangaumean",intlangaufun,inf,sup,4);
		intlangaumean->SetParameters(values[i]);
		//double fitintegral = langau->Integral(inf,sup);
		double fitintegral = intlangaumean->Integral(inf,sup);
		double fitmean = langaumean->Integral(inf,sup);

		std::cout<<"------------------------>"<<i<<check
				 <<fitmean/fitintegral<<std::endl;
		//Test line
		langau_Mean[i] = fitmean/fitintegral;
		langau_Sigma[i]=GetMeanUncertanty(langau_Mean[i],langau->GetParameter(1),langau->GetParError(1),langau->GetParameter(0),langau->GetParError(0),langau->GetParameter(3),langau->GetParError(3),langau->GetParameter(2),langau->GetParError(2), inf, sup);
		chi2red[i]=langau->GetChisquare()/langau->GetNDF();
		std::cout<<"**********ladder_"<<i<<" "<<langau_Mean[i]<<std::endl;
	}
	}
	return;
}

double GetMeanUncertanty(double mean, double MPV, double err_MPV, double sig_l, double err_sig_l, double sig_g, double err_sig_g, double area, double err_area, double inf, double sup){
	std::vector<double> resid;
	int dim1=10;
	int dim2=10;
	double MPV_L[dim1];
	double SIGMA_L[dim1];
	double SIGMA_G[dim2];
	double AREA_L[dim2];
	int cont = 0;
	while(cont<dim1){
		MPV_L[cont] = (MPV - err_MPV + 2*err_MPV/(dim1-1)*cont);
		SIGMA_L[cont] = (sig_l - err_sig_l + (2*err_sig_l/(dim1-1))*cont);
		++cont;
	}
	cont=0;
	while(cont<dim2){
		SIGMA_G[cont] = (sig_g - err_sig_g + (2*err_sig_g/(dim2-1))*cont);
		AREA_L[cont] = (area - err_area + (2*err_area/(dim2-1))*cont);
		++cont;
	}
	Double_t values[4];
	for(int i_mpv=0; i_mpv<dim1; ++i_mpv){
		values[1]=MPV_L[i_mpv];
		for(int i_sl=0; i_sl<dim1; ++i_sl){
			values[0]=SIGMA_L[i_sl];
			for(int i_sg=0; i_sg<dim2; ++i_sg){
				values[3]=SIGMA_G[i_sg];
				for(int i_area = 0; i_area<dim2; ++ i_area){
					values[2]=AREA_L[i_area];
					TF1 * mylangau = new TF1("mylangau",langaufun,inf,sup,4);
					mylangau->SetParameters(values);
					TF1 * mylangaumean = new TF1("mylangaumean",langaufunmean,inf,sup,4);
					mylangaumean->SetParameters(values);
					double myfitintegral = mylangau->Integral(inf,sup,1.e-3);
					double myfitmean = mylangaumean->Integral(inf,sup,1.e-3);
					//double myfitintegral = mylangau->Integral(inf,sup);
					//double myfitmean = mylangaumean->Integral(inf,sup);
					double tmp = myfitmean/myfitintegral;
					double unc = std::abs(tmp - mean);
					resid.push_back(unc);
					mylangaumean->Delete();
					mylangau->Delete();
				}
			}
		}
		std::cout<<float(i_mpv)/float(dim1)<< " " <<resid.back()<<std::endl;
	}
	double result = *std::max_element(resid.begin(),resid.end());
	std::cout<<"err "<<result<<std::endl;
	return result;
}


