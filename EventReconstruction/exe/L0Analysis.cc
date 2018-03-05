#include "LEvRec0File.hh"
#include "LEvRec0.hh"
#include <iostream>
#include "TH1D.h"
#include "TFile.h"
#include "TString.h"
#include "LOfflineL0Selector.hh"
#include "LCalibration.hh"

const int NBINS = 4096/4;
const double XMIN = 0.;
const double XMAX = 4096.;

int main(int argc, char **argv) {
	LEvRec0 ev;
	LEvRec0File fin(argv[1]);
	fin.SetTheEventPointer(ev);

	TFile fout(argv[2],"update");

	TString hbaseName("counts");

	TH1D ***hTrigScint_Raw = new TH1D**[NTRIGSCINT];
	TH1D ***hTrigScint_Mask3 = new TH1D**[NTRIGSCINT];
	TH1D ***hTrigScint_AntiMask3 = new TH1D**[NTRIGSCINT];
	TH1D ***hTrigScint_Planes34 = new TH1D**[NTRIGSCINT];
	for(int i=0; i<NTRIGSCINT; ++i) {
		hTrigScint_Raw[i] = new TH1D*[2];
		hTrigScint_Mask3[i] = new TH1D*[2];
		hTrigScint_AntiMask3[i] = new TH1D*[2];
		hTrigScint_Planes34[i] = new TH1D*[2];
		for(int j=0; j<2; ++j){
			TString name=hbaseName+TString::TString("_bar")+TString::Itoa(i,10)+TString::TString("_PMT")
				+TString::Itoa(j,10)+TString::TString("_Raw");
			hTrigScint_Raw[i][j] = new TH1D(name.Data(), "", NBINS,XMIN,XMAX);
			name=hbaseName+TString::TString("_bar")+TString::Itoa(i,10)+TString::TString("_PMT")
				+TString::Itoa(j,10)+TString::TString("_Mask3");
			hTrigScint_Mask3[i][j] = new TH1D(name.Data(), "", NBINS,XMIN,XMAX);
			name=hbaseName+TString::TString("_bar")+TString::Itoa(i,10)+TString::TString("_PMT")
				+TString::Itoa(j,10)+TString::TString("_AntiMask3");
			hTrigScint_AntiMask3[i][j] = new TH1D(name.Data(), "", NBINS,XMIN,XMAX);
			name=hbaseName+TString::TString("_bar")+TString::Itoa(i,10)+TString::TString("_PMT")
				+TString::Itoa(j,10)+TString::TString("_Planes34");
			hTrigScint_Planes34[i][j] = new TH1D(name.Data(), "", NBINS,XMIN,XMAX);
		}
	}

	TH1D ***hScintPlane_Raw = new TH1D**[NSCINTPLANES];
	TH1D ***hScintPlane_Mask3 = new TH1D**[NSCINTPLANES];
	TH1D ***hScintPlane_AntiMask3 = new TH1D**[NSCINTPLANES];
	TH1D ***hScintPlane_Planes34 = new TH1D**[NSCINTPLANES];
	for(int i=0; i<NSCINTPLANES; ++i) {
		hScintPlane_Raw[i] = new TH1D*[2];
		hScintPlane_Mask3[i] = new TH1D*[2];
		hScintPlane_AntiMask3[i] = new TH1D*[2];
		hScintPlane_Planes34[i] = new TH1D*[2];
		for(int j=0; j<2; ++j){
			TString name=hbaseName+TString::TString("_plane")+TString::Itoa(i,10)+TString::TString("_PMT")
				+TString::Itoa(j,10)+TString::TString("_Raw");
			hScintPlane_Raw[i][j] = new TH1D(name.Data(), "", NBINS,XMIN,XMAX);
			name=hbaseName+TString::TString("_plane")+TString::Itoa(i,10)+TString::TString("_PMT")
				+TString::Itoa(j,10)+TString::TString("_Mask3");
			hScintPlane_Mask3[i][j] = new TH1D(name.Data(), "", NBINS,XMIN,XMAX);
			name=hbaseName+TString::TString("_plane")+TString::Itoa(i,10)+TString::TString("_PMT")
				+TString::Itoa(j,10)+TString::TString("_AntiMask3");
			hScintPlane_AntiMask3[i][j] = new TH1D(name.Data(), "", NBINS,XMIN,XMAX);
			name=hbaseName+TString::TString("_plane")+TString::Itoa(i,10)+TString::TString("_PMT")
				+TString::Itoa(j,10)+TString::TString("_Planes34");
			hScintPlane_Planes34[i][j] = new TH1D(name.Data(), "", NBINS,XMIN,XMAX);
		}
	}

	TH1D hTimeMask3("hTimeMask3","",4000,0.,400.);
	TH1D hTimeAntiMask3("hTimeAntiMask3","",4000,0.,400.);
	TH1D hDiffTimeMask3("hDiffTimeMask3","",1000,0.,1000.);
	TH1D hDiffTimeAntiMask3("hDiffTimeAntiMask3","",1000,0.,1000.);

	int nent = fin.GetEntries();
	std::cout << "File " << argv[1] << " contains " << nent << " events" << std::endl;

	LCalibration *calib = LCalibration::ReadROOT("/Users/roberto/work/LIMADOU/COMMISSIONING/data/HEPD000770_20180207_074810_20180207_081601/L0_files/Data-20180207-075450-CalOn-00158_00027-p01_L0.root");
	LOfflineL0Selector AntiMask3;
	LOfflineL0Selector Mask3;
	LOfflineL0Selector Planes34;
	AntiMask3.LoadCalibration(calib);
	AntiMask3.LoadConfiguration("AntiMask3");
	AntiMask3.SetThreshold(5.);
	Mask3.LoadCalibration(calib);
	Mask3.LoadConfiguration("Mask3");
	Mask3.SetThreshold(5.);
	Planes34.LoadCalibration(calib);
	Planes34.LoadConfiguration("Planes34");
	Planes34.SetThreshold(5.);
	
	int oldTime=-9999;
	for(int iev=0; iev<nent; ++iev) {
		fin.GetEntry(iev);
		for(int n=0; n<NTRIGSCINT; ++n) {
			for(int j=0; j<2; ++j) {
				hTrigScint_Raw[n][j]->Fill(static_cast<double>(ev.trigger(n,j)));
				if(Mask3.Select(ev)) hTrigScint_Mask3[n][j]->Fill(static_cast<double>(ev.trigger(n,j)));
				if(AntiMask3.Select(ev)) hTrigScint_AntiMask3[n][j]->Fill(static_cast<double>(ev.trigger(n,j)));
				if(Planes34.Select(ev)) hTrigScint_Planes34[n][j]->Fill(static_cast<double>(ev.trigger(n,j)));
			}
		}
		for(int n=0;  n<NSCINTPLANES; ++n) {
			for(int j=0; j<2; ++j) {
				hScintPlane_Raw[n][j]->Fill(static_cast<double>(ev.plane(n,j)));
				if(Mask3.Select(ev)) hScintPlane_Mask3[n][j]->Fill(static_cast<double>(ev.plane(n,j)));
				if(AntiMask3.Select(ev)) hScintPlane_AntiMask3[n][j]->Fill(static_cast<double>(ev.plane(n,j)));
				if(Planes34.Select(ev)) hScintPlane_Planes34[n][j]->Fill(static_cast<double>(ev.plane(n,j)));
			}
		}
		int newTime=ev.hepd_time;
		if(Mask3.Select(ev)) hTimeMask3.Fill((newTime)/100000.);
		else hTimeAntiMask3.Fill((newTime)/100000.);
		if(iev>0) {
			if(Mask3.Select(ev)) hDiffTimeMask3.Fill((newTime-oldTime)/100.);
			else hDiffTimeAntiMask3.Fill((newTime-oldTime)/100.);
		}
		oldTime=newTime;
	}

	fout.Write();

	return 0;
}