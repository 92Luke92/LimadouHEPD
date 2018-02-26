#include "LEvRec1.hh"
#include "LEvRec1File.hh"
#include "LTrackerSignal.hh"
#include "LTrackerTools.hh"
#include "LTrackerFittingTools.hh"
#include "LTrackletMaker.hh"
#include "LTracklet.hh"
#include "LOfflineTrigger.hh"
#include "analysis_const.hh"
// include ROOT native libraries 
#include "TProfile.h"
#include "TROOT.h"
#include "TH1.h"
// include C++/C++11 libraries
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

const double CUT_SN = 5.;

void L1_SilconCorrectedCounts( std::string namefile, std::string outputpath, std::string outputname, std::string pathLUT){

	// conversion factor from calibration
	double conversion_ADC_MEV[2][2];
	conversion_ADC_MEV[0][0] = 353.7;
	conversion_ADC_MEV[0][1] = 441.7;
	conversion_ADC_MEV[1][0] = 365.0;
	conversion_ADC_MEV[1][1] = 441.7;

	// histo initialization
	TH1F * h_sumcounts_distribution_corr[2][2];
	TH1F * h_dedx_spectrum[2][2];
	std::string namehisto;

	for(int ladder = 0; ladder < 2; ++ladder){
		for(int side = 0; side < 2; ++side){
			namehisto ="h_sumcounts_distribution_corr"+std::to_string(ladder+2)+"_side_"+std::to_string(side);
			h_sumcounts_distribution_corr[ladder][side] = new TH1F(namehisto.c_str(),namehisto.c_str(),150,0,150);
			namehisto="h_dedx_spectrum_"+std::to_string(ladder+2)+"_side_"+std::to_string(side);
			h_dedx_spectrum[ladder][side] = new TH1F(namehisto.c_str(),namehisto.c_str(),70,0,0.5);
		}
	}




	//********** Using the look up table histograms ************

	TH2F * LUT_histo[2][2];
	TFile * LUT_inputfile = new TFile(pathLUT.c_str(),"READ");
	std::string inputhisto_name;

	for( int ladder = 0 ; ladder< 2 ; ++ladder){
		for( int side =0 ; side<N_SIDES ; ++side ){
			LUT_histo[ladder][side]=0;
			inputhisto_name = "LUT_ladder_"+std::to_string(ladder+2)+"_side_"+std::to_string(side);
			std::cout<<inputhisto_name<<std::endl;
			gDirectory->GetObject(inputhisto_name.c_str(),LUT_histo[ladder][side]);
		}
	}

	LEvRec1File input(namefile.c_str(),"READ");
  	LEvRec1 ev;
  	input.SetTheEventPointer(ev);
  	const int MAXEVENTS = input.GetEntries();

  	for(int iev=0; iev<MAXEVENTS; ++iev){

		input.GetEntry(iev);
		if(ev.tracker.cls.size()<1) continue;
   		//ev.tracker.HoldTimeCorrectionSig();
		LTrackletMaker maker;
		LTracklet track = maker.GetTracklet(&ev,true);

		for (int icluster = 0; icluster<TRACKERCLUSTERS; ++icluster){
			int seed = track.tracker_cl[icluster].seed;
			if (seed==-999) continue;
			if(track.tracker_cl[icluster].GetEtaSN()<CUT_SN) continue;
			double eta = track.tracker_cl[icluster].GetEta();
			if(eta<0.||eta>1.) eta = std::abs(1 - std::abs(eta));
			int side = ChanToSide(seed);
			int ladder = ChanToLadder(seed);
			//ladder switched off check
			if((ladder!=2)&&(ladder!=3)) continue;
			double corrected_counts = 0;
			double sumclscount = 0;

			sumclscount = track.tracker_cl[icluster].GetEtaCounts();
			// apply correction just if the variable is true or if parabola is positive
	      	corrected_counts = LUT_histo[ladder-2][side]->GetBinContent(1+int(eta/(LUT_histo[ladder-2][side]->GetXaxis()->GetBinWidth(1))),1+int(sumclscount/LUT_histo[ladder-2][side]->GetYaxis()->GetBinWidth(1)));

	      	if(corrected_counts<sumclscount) corrected_counts = sumclscount;
			h_sumcounts_distribution_corr[ladder-2][side]->Fill(corrected_counts);
			h_dedx_spectrum[ladder-2][side]->Fill(corrected_counts/conversion_ADC_MEV[ladder-2][side]);
		}
	}
}

int main(int argc, char **argv){
  if(argc!=5){
    std::cout<< "Error! Wrong number of parameter!"<<std::endl
	     <<"Please type:"<<std::endl
	     <<"user> ./analysis_follega_bethe <inputfile> <outputpath> <outputname> <pathLUT>"<<std::endl
	     <<"This works on EvRec1 files!!!"<<std::endl;
    return 0;
  }
  else
    L1_SilconCorrectedCounts(argv[1],argv[2],argv[3],argv[4]);
  return 1;
}




