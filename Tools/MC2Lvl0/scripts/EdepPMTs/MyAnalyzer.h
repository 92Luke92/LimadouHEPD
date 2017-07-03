#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TString.h>
#include <TMath.h>
#include <iostream>
#include <fstream>
// Header file for the classes stored in the TTree if any.
#include "../source/Dataformats/include/RootEvent.hh"
#include "../source/Dataformats/include/RootTrack.hh"
#include "../source/Dataformats/include/RootVertex.hh"
#include "../source/Dataformats/include/RootCaloHit.hh"
#include "../source/Dataformats/include/RootTrackerHit.hh"

#include <TObject.h>
#include <TVector3.h>

#include <string.h>
#define ELECTRON "e-"
#define POSITRON "e+" //
#define MUON "mu-"  //
#define ANTI_MUON "mu+" //
#define ANTI_NU_E "anti_nu_e" //
#define NU_E "nu_e" //
#define ANTI_NU_MU "anti_nu_mu" //
#define NU_MU "nu_mu" //
#define GAMMA "gamma"
#define NEUTRON "neutron"
#define PROTON "proton"



class MyAnalyzer{

	private:
	TTree          *fTree;        //!pointer to the analyzed TTree
	TChain         *fTreeChain;   //!pointer to the analyzed TChain
	Int_t           fCurrent;     //!current Tree number in a TChain
	// Declaration of leaf types
	RootEvent   *Event;
	//
	TBranch        *b_Event;

	float Xgen; // cm
	float Ygen; // cm
	float sigma; // solid angle


	std::string outfilename;
	
	public:
	MyAnalyzer();

	std::string GetOutFileName(){return outfilename;};
	void SetFile(TString fileName);
	void SetFileChain(TString fileName);	
	Int_t GetEntry(Long64_t entry);
	Long64_t LoadTree(Long64_t entry);
	void Init(TTree *tree);
	void InitChain();	
	void Show(Long64_t entry);
	Bool_t Notify();
	void GenerateOutputFileName(std::string filename);	
	void Edep_Plots();
	void ImpactPoint_Plots();
	void LVL0Converter();
};


MyAnalyzer::MyAnalyzer() : fTree(0)
{
   Xgen = 2*27.5; // cm
   Ygen = 2*37.5; // cm
   sigma = TMath::Pi(); // solid angle
}

void MyAnalyzer::SetFile(TString fileName)
{
 TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fileName);
  if (!f || !f->IsOpen()) {
    f = new TFile(fileName);
  }
  fileName.Append(":/HEPD");
  TDirectory * dir = (TDirectory*)f->Get(fileName);
  dir->GetObject("EventTree",fTree);
  Init(fTree);

}


void MyAnalyzer::SetFileChain(TString fileName)
{
	TString HEPDPath = "/HEPD/EventTree";
  fTreeChain = new TChain(HEPDPath);

  std::ifstream File_Input(fileName);
 int n_files=0;
  while(!File_Input.eof()){
    TString singlefile = "noName";
    File_Input >> singlefile;
    if(singlefile!="noName"&&singlefile!=""){
    fTreeChain->Add(singlefile);
    printf("ajoute %s\n",singlefile.Data());
    if(n_files==0) GenerateOutputFileName(singlefile.Data());
    n_files++;
    }
  }
  File_Input.close();
  InitChain();
}


Int_t MyAnalyzer::GetEntry(Long64_t entry)
{

  if (!fTree) return 0;
  return fTree->GetEntry(entry);
}
Long64_t MyAnalyzer::LoadTree(Long64_t entry)
{

   if (!fTreeChain) return -5;
   Long64_t centry = fTreeChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fTreeChain->GetTreeNumber() != fCurrent) {
      fCurrent = fTreeChain->GetTreeNumber();
      Notify();
   }
   return centry;
}


void MyAnalyzer::Init(TTree *tree)
{
  Event = 0;
  if (!tree) return;
  fTree = tree;
  fCurrent = -1;

  fTree->SetBranchAddress("Event", &Event, &b_Event);
  Notify();
}

void MyAnalyzer::InitChain()
{
  Event = 0;
  fCurrent = -1;
  printf("fTreeChain %p\n",fTreeChain);

  fTreeChain->SetBranchAddress("Event", &Event, &b_Event);
  printf("fTreeChain %p\n",fTreeChain);
  Notify();
}

Bool_t MyAnalyzer::Notify()
{
  return kTRUE;
}

void MyAnalyzer::Show(Long64_t entry)
{
  if (!fTree) return;
  fTree->Show(entry);
}

void MyAnalyzer::GenerateOutputFileName(std::string filename){

        vector<string> strings;
	istringstream f(filename.c_str());
        string s;    
        while (std::getline(f, s, '/')) {
                strings.push_back(s);
        }
        cout <<strings[strings.size()-1]<<endl;
        outfilename = strings[strings.size()-1];
}


void MyAnalyzer::Edep_Plots(){

	TFile * Plotfile = TFile::Open((outfilename+"_Plot").c_str(), "RECREATE");

	Long64_t nentries = fTreeChain->GetEntries();

	TH2F * EdepTrig = new TH2F("EdepTrig","EdepTrig",6,0,6,300,0,50);
	TH2F * EdepCalo = new TH2F("EdepCalo","EdepCalo",16,0,16,300,0,50);
	TH2F * EdepLYSO = new TH2F("EdepLYSO","EdepLYSO",9 ,0,9 ,300,0,100);

	TH2F * PrimFractionTrig = new TH2F("PrimFractionTrig","PrimFractionTrig",6,0,6,100,0,1.1);
	TH2F * PrimFractionCalo = new TH2F("PrimFractionCalo","PrimFractionCalo",16,0,16,100,0,1.1);
	TH2F * PrimFractionLYSO = new TH2F("PrimFractionLYSO","PrimFractionLYSO",9 ,0,9 ,100,0,1.1);

	
	for (Long64_t jentry=0; jentry<nentries;jentry++) { 
	
		Long64_t ientry = LoadTree(jentry);
    		int nb = fTreeChain->GetEntry(jentry);
		MapEvents * Map = new MapEvents();
		Map->LoadEvent(Event);
		Map->Mappify();
	
		for(int triglayer=0;triglayer<6;triglayer++)  EdepTrig->Fill(triglayer,Map->GetCaloLayerEdep("Trigger",triglayer));
		for(int calolayer=0;calolayer<16;calolayer++) EdepCalo->Fill(calolayer,Map->GetCaloLayerEdep("Calo",calolayer));
		for(int lysolayer=0;lysolayer<9 ;lysolayer++) EdepLYSO->Fill(lysolayer,Map->GetCaloLayerEdep("LYSO",lysolayer));

		for(int triglayer=0;triglayer<6;triglayer++) 
			PrimFractionTrig->Fill(triglayer,Map->GetCaloLayerEdepPrim("Trigger",triglayer)/Map->GetCaloLayerEdep("Trigger",triglayer));
		for(int calolayer=0;calolayer<16;calolayer++) 
			PrimFractionCalo->Fill(calolayer,Map->GetCaloLayerEdepPrim("Calo",calolayer)/Map->GetCaloLayerEdep("Calo",calolayer));
		for(int lysolayer=0;lysolayer<9 ;lysolayer++) 
			PrimFractionLYSO->Fill(lysolayer,Map->GetCaloLayerEdepPrim("LYSO",lysolayer)/Map->GetCaloLayerEdep("LYSO",lysolayer));
	
	}

	EdepTrig->Write(); 
	EdepCalo->Write(); 
	EdepLYSO->Write(); 

	PrimFractionTrig->Write(); 
	PrimFractionCalo->Write(); 
	PrimFractionCalo->Write(); 


	Plotfile->Write();
	Plotfile->Close();
}


void MyAnalyzer::ImpactPoint_Plots(){

	TFile * Plotfile = TFile::Open((outfilename+"_Plot").c_str(), "RECREATE");


	TH3F * ImpactPointHisto = new TH3F("ImpactPointHisto","ImpactPointHisto",500,0,240,500,0,240,50,0,240);

	Long64_t nentries = fTreeChain->GetEntries();
		for (Long64_t jentry=0; jentry<nentries;jentry++) { 
	
		Long64_t ientry = LoadTree(jentry);
    		int nb = fTreeChain->GetEntry(jentry);
		MapEvents * Map = new MapEvents();
		Map->LoadEvent(Event);
		Map->Mappify();

		for(int triglayer=0;triglayer<6;triglayer++) 
				ImpactPointHisto->Fill(Map->GetCaloHitImpactPoint("Trigger",triglayer).x(),Map->GetCaloHitImpactPoint("Trigger",triglayer).y(),Map->GetCaloHitImpactPoint("Trigger",triglayer).z());		
		for(int calolayer=0;calolayer<16;calolayer++) 
				ImpactPointHisto->Fill(Map->GetCaloHitImpactPoint("Calo",calolayer).x(),Map->GetCaloHitImpactPoint("Calo",calolayer).y(),Map->GetCaloHitImpactPoint("Calo",calolayer).z());		
		for(int lysolayer=0;lysolayer<9 ;lysolayer++) 
				ImpactPointHisto->Fill(Map->GetCaloHitImpactPoint("LYSO",lysolayer).x(),Map->GetCaloHitImpactPoint("LYSO",lysolayer).y(),Map->GetCaloHitImpactPoint("LYSO",lysolayer).z());		

		}

	ImpactPointHisto->Write();

	Plotfile->Write();
	Plotfile->Close();
}

std::vector<blip> FromEdep2blip(RootEvent *Event){
	MapEvents * Map = new MapEvents();
	Map->LoadEvent(Event);
	Map->Mappify();
	std::vector<blip> PMTs =Map->FromEdep2blip();	
	return PMTs;
}


void MyAnalyzer::LVL0Converter(){

	TFile * LVL0file = TFile::Open((outfilename+"_Plot").c_str(), "RECREATE");


	Long64_t nentries = fTreeChain->GetEntries();
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		int nb = fTreeChain->GetEntry(jentry);
		std::vector<blip> PMTsLowGain =FromEdep2blip(Event);
	}
}
