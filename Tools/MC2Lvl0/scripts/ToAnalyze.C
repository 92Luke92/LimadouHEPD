#define ToAnalyze_cxx
#include "ToAnalyze.h"
#include <TH2.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TLegend.h>
#include <iostream>
#include <TRandom.h>
#include <TMath.h>
#include <vector>

#define FLUXBINS 1000


void ToAnalyze::Loop()
{
  Int_t verbose = 1;
  cout << " ##### Particle setted to " << particle << " ##### "  << endl;

  TString particleType=particle;
  particleType.Prepend("Analyzed_");
  particleType.Append(".root");

  Int_t num_config=9;
  Bool_t s1Hit;
  Bool_t s1Hit3;
  Bool_t s1Hit4;
  Bool_t P1Hit;
  Bool_t P2Hit;
  Bool_t P3Hit;
  Bool_t P15Hit;
  Bool_t P16Hit;
  Bool_t P17Hit;
  Bool_t trigger[num_config];
  Bool_t noVetoHit;
  Bool_t isVetoLatHit;
  Bool_t isVetoBottHit;
  Bool_t hitOnTkLayer1;
  Bool_t hitOnTkLayer2;
  Bool_t goodHit;
  Bool_t isInAcc;
  Bool_t islower99;
  Bool_t islower95;
  Bool_t islower90;

  Int_t bufsize = 80000000;
  Int_t layerTrack=0;
  Int_t layer=0;
  Int_t checkID=0;
  Int_t eventID=0;

  Double_t Energy = 0;
  Double_t measEnergy = 0;

  Double_t TOTTRIGEdep;
  Double_t TRIGEdep[6];
  Double_t TOTSCINTEdep;
  Double_t SCINTEdep[16];
  Double_t TOTLYSOEdep;
  Double_t LYSOEdep[9];
  Double_t TOTVETOEdep;
  Double_t VNEdep;
  Double_t VSEdep;
  Double_t VEEdep;
  Double_t VWEdep;
  Double_t VBEdep;

  Double_t theta=0;
  Double_t zGenPos=0;
  Double_t fit99=0;
  Double_t fit95=0;
  Double_t fit90=0;

  TVector3 particlePos_mm;
  TVector3 particleDir ;

  TFile * histoFile = new TFile(particleType,"recreate");
  TTree *tree = new TTree("Tree","");
  tree->Branch("eventID",&eventID,"eventID/I");
  tree->Branch("trigger",&trigger[0],"trigger[9]/O");
  tree->Branch("particlePos_mm",&particlePos_mm[0],"particlePos_mm[3]/D");
  tree->Branch("genDir",&particleDir[0], "particleDir[3]/D");
  tree->Branch("theta",&theta,"theta/D",bufsize);
  tree->Branch("isInAcc",&isInAcc,"isInAcc/O",bufsize);
  tree->Branch("noVetoHit",&noVetoHit,"noVetoHit/O",bufsize);
  tree->Branch("isVetoLatHit",&isVetoLatHit,"isVetoLatHit/O",bufsize);
  tree->Branch("isVetoBottHit",&isVetoBottHit,"isVetoBottHit/O",bufsize);
  tree->Branch("hitOnTkLayer1",&hitOnTkLayer1,"hitOnTkLayer1/O",bufsize);
  tree->Branch("hitOnTkLayer2",&hitOnTkLayer2,"hitOnTkLayer2/O",bufsize);
  tree->Branch("s1Hit",&s1Hit,"s1Hit/O",bufsize);
  tree->Branch("s1Hit3",&s1Hit3,"s1Hit3/O",bufsize);
  tree->Branch("s1Hit4",&s1Hit4,"s1Hit4/O",bufsize);
  tree->Branch("P1Hit",&P1Hit,"P1Hit/O",bufsize);
  tree->Branch("P2Hit",&P2Hit,"P2Hit/O",bufsize);
  tree->Branch("P3Hit",&P3Hit,"P3Hit/O",bufsize);
  tree->Branch("P15Hit",&P15Hit,"P15Hit/O",bufsize);
  tree->Branch("P16Hit",&P16Hit,"P16Hit/O",bufsize);
  tree->Branch("P17Hit",&P17Hit,"P17Hit/O",bufsize);
  tree->Branch("genEnergy",&Energy,"genEnergy/D",bufsize);
  tree->Branch("SILEdep",&SILEdep,"SILEdep/D",bufsize);
  tree->Branch("TOTTRIGEdep",&TOTTRIGEdep,"TOTTRIGEdep/D",bufsize);
  tree->Branch("TRIGEdep",&TRIGEdep[0],"TRIGEdep[6]/D",bufsize);
  tree->Branch("TOTSCINTEdep",&TOTSCINTEdep,"TOTSCINTEdep/D",bufsize);
  tree->Branch("SCINTEdep",&SCINTEdep[0],"SCINTEdep[16]/D",bufsize);
  tree->Branch("TOTLYSOEdep",&TOTLYSOEdep,"TOTLYSOEdep/D",bufsize);
  tree->Branch("LYSOEdep",&LYSOEdep[0],"LYSOEdep[9]/D",bufsize);
  tree->Branch("VNEdep",&VNEdep,"VNEdep/D",bufsize);
  tree->Branch("VSEdep",&VSEdep,"VSEdep/D",bufsize);
  tree->Branch("VEEdep",&VEEdep,"VEEdep/D",bufsize);
  tree->Branch("VWEdep",&VWEdep,"VWEdep/D",bufsize);
  tree->Branch("VBEdep",&VBEdep,"VBEdep/D",bufsize);
  tree->Branch("TOTVETOEdep",&TOTVETOEdep,"TOTVETOEdep/D",bufsize);
  tree->Branch("TOTALEdep",&TOTALEdep,"TOTALEdep/D",bufsize);
  tree->Branch("islower99",&islower99,"islower99/O",bufsize);
  tree->Branch("islower95",&islower95,"islower95/O",bufsize);
  tree->Branch("islower90",&islower90,"islower90/O",bufsize);

  Long64_t nentries = fTree->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

/////////////////////////////////////////
// Histo Binning

  Int_t lastHit=0;
  Int_t nbin, binDep, nbinY, nbinX;
  Int_t stopBin=0;
  Double_t ybins[FLUXBINS];
  Double_t xfluxbins[FLUXBINS];
  xfluxbins[0] = 0.5;
  ybins[0]= 0.5;

  nbinY=1;
  while(nbinY!=FLUXBINS){
    ybins[nbinY]=ybins[nbinY-1] + .5;
    nbinY++;
  }
  nbinY--;

  nbin=1;
  while(nbin!=FLUXBINS){
    xfluxbins[nbin]=xfluxbins[nbin-1] + .5;
    nbin++;
  }
  nbin--;

  Double_t xbins[]={0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, -1};
  nbinX=0;
  while( (xbins[++nbinX]) != (-1) );
  nbinX--;
////////////////////////////////////

  TF1 *Fit99 = new TF1("Fit99","(7.32535e-001)*(1-exp(-x))",1,30);
  TF1 *Fit95 = new TF1("Fit95","(4.58305e-001)*(1-exp(-x))",1,30);
  TF1 *Fit90 = new TF1("Fit90","(3.60586e-001)*(1-exp(-x))",1,30);

  TH1F *genFlux   = new TH1F("genFlux","Number of generated particles",nbin, xfluxbins);
  TH2D *genSurface   = new TH2D("genSurface","Generation surface", 1000, -400, 400, 1000 , -400 , 400);
  TH1F *geomAccflux   = new TH1F("inGeoAccFlux","Number of particles in acceptance",nbin, xfluxbins);
  TH1F *trigflux   = new TH1F("triggFlux","Number of triggered particles",nbin, xfluxbins);
  TH1F *thetaTotal   = new TH1F("thetaTotal","Theta Total",180,0,90);
  TH1F *thetaVisible = new TH1F("thetaVisible","Theta Visible",50,0,90);
  TH2D *stoppingPlane   = new TH2D("stoppingPlane","Initial energy ", nbinY, ybins, nbinX, xbins);

  stoppingPlane->GetYaxis()->SetBinLabel(1,"T1");
  stoppingPlane->GetYaxis()->SetBinLabel(2,"P1");
  stoppingPlane->GetYaxis()->SetBinLabel(3,"P2");
  stoppingPlane->GetYaxis()->SetBinLabel(4,"P3");
  stoppingPlane->GetYaxis()->SetBinLabel(5,"P4");
  stoppingPlane->GetYaxis()->SetBinLabel(6,"P5");
  stoppingPlane->GetYaxis()->SetBinLabel(7,"P6");
  stoppingPlane->GetYaxis()->SetBinLabel(8,"P7");

  TProfile* layerDep = new TProfile("layerDep","Energy deposited in each detector layer",26,-0.5,25.5);
  layerDep->GetXaxis()->SetBinLabel(1,"");
  layerDep->GetXaxis()->SetBinLabel(2,"");
  layerDep->GetXaxis()->SetBinLabel(3,"Tk1");
  layerDep->GetXaxis()->SetBinLabel(4,"Tk2");
  layerDep->GetXaxis()->SetBinLabel(5,"T1");
  layerDep->GetXaxis()->SetBinLabel(6,"P1");
  layerDep->GetXaxis()->SetBinLabel(7,"P2");
  layerDep->GetXaxis()->SetBinLabel(8,"P3");
  layerDep->GetXaxis()->SetBinLabel(9,"P4");
  layerDep->GetXaxis()->SetBinLabel(10,"P5");
  layerDep->GetXaxis()->SetBinLabel(11,"P6");
  layerDep->GetXaxis()->SetBinLabel(12,"P7");
  layerDep->GetXaxis()->SetBinLabel(13,"P8");
  layerDep->GetXaxis()->SetBinLabel(14,"P9");
  layerDep->GetXaxis()->SetBinLabel(15,"P10");
  layerDep->GetXaxis()->SetBinLabel(16,"P11");
  layerDep->GetXaxis()->SetBinLabel(17,"P12");
  layerDep->GetXaxis()->SetBinLabel(18,"P13");
  layerDep->GetXaxis()->SetBinLabel(19,"P14");
  layerDep->GetXaxis()->SetBinLabel(20,"P15");
  layerDep->GetXaxis()->SetBinLabel(21,"P16");
  layerDep->GetXaxis()->SetBinLabel(22,"LYSO");
  if(verbose>0) cout << " nentries =  " << nentries << endl;

//  for (Long64_t jentry=0; jentry<nentries;jentry++)
  for (Long64_t jentry=0; jentry<100000;jentry++)
  {
//      if(jentry % 50000==0) cout << jentry << " and % " << (float)jentry/nentries*100. << endl;
    eventID= Event->eventID;

    TOTTRIGEdep = 0;
    TOTSCINTEdep = 0;
    TOTLYSOEdep = 0;
    for (int k=0; k<6; k++) TRIGEdep[k] = 0;
    for (int k=0; k<16; k++) SCINTEdep[k] = 0;
    for (int k=0; k<9; k++) LYSOEdep[k] = 0;
    VNEdep = 0;
    VSEdep = 0;
    VEEdep = 0;
    VWEdep = 0;
    VBEdep = 0;
    TOTVETOEdep = 0;
    TOTALEdep = 0;
    SILEdep = 0;

    s1Hit=false;
    s1Hit3=false;
    s1Hit4=false;
    P1Hit=false;
    P2Hit=false;
    P3Hit=false;
    P15Hit=false;
    P16Hit=false;
    P17Hit=false;
    noVetoHit=false;
    isVetoLatHit=false;
    isVetoBottHit=false;
    hitOnTkLayer1=false;
    hitOnTkLayer2=false;
    goodHit=false;
    islower99=false;
    islower95=false;
    islower90=false;

    for (int ii=0; ii<num_config; ii++) trigger[ii]=false;

      Long64_t ientry = LoadTree(jentry);

      if (ientry < 0) break;
      nb = fTree->GetEntry(jentry);   nbytes += nb;

      vector<RootTrack> myTracks = Event->GetTracks();
      std::vector<RootVertex> myVertex = Event->GetVertex();
      std::vector<RootCaloHit> myCaloHit = Event->GetCaloHit();
      std::vector<RootCaloHit> myVetoHit = Event->GetVetoHit();
      std::vector<RootTrackerHit> myTrackerHit = Event->GetTrackerHit();

      Energy = myTracks[0].GetKinEnergy();
      genFlux->Fill(Energy);

      particlePos_mm = myTracks[0].GetPosition();
      particleDir = myTracks[0].GetDirection();
      genSurface->Fill(particlePos_mm[0], particlePos_mm[1]);

      if(jentry == 10)
      zGenPos = particlePos_mm[2];

      isInAcc = IsInAcceptance(particlePos_mm, particleDir);
      if(isInAcc)
      geomAccflux->Fill(Energy);

      theta = particleDir.Theta()*180/TMath::Pi();
      if(theta>90)
      theta=180-theta;
      thetaTotal->Fill(theta);

///////////////////////////////////
// Definition of trigger configurations

// Check hit on veto
    if(myVetoHit.size()==0)
    {
      noVetoHit=true;
    }
    else
    {
      for(size_t vh=0;vh<myVetoHit.size();vh++)
      {
	layer = myVetoHit[vh].GetVolume();
	if(layer == 1430)
	  isVetoBottHit=true;
	else
	  isVetoLatHit=true;
	VETOEdep+=myVetoHit[vh].GetTotalEdep();
       }
  	if (verbose>4) cout << " VETO eDep = " << VETOEdep << endl;
    }

//Check hit on Tracker
    if(myTrackerHit.size()>0)
    {
      for(size_t th=0;th<myTrackerHit.size();th++)
      {
	layerTrack = myTrackerHit[th].GetDetectorId();
	checkID = layerTrack / 100;
	if(checkID==22)
	  hitOnTkLayer2=true;
	if(checkID==21)
	  hitOnTkLayer1=true;
      }
      if(hitOnTkLayer2&&hitOnTkLayer1)	goodHit=true; // SERVE???
    }

//Check hit on Calo
   if(myCaloHit.size()>0) // if(myCaloHit.size()>0 && goodHit)//
    {
//cout << " size " << myCaloHit.size() << endl;
      for(size_t ch=0;ch<myCaloHit.size();ch++)
      {
if(myCaloHit.size()>1) {
/*cout << " size " << myCaloHit.size() << endl;
cout << myCaloHit[ch].GetEdep(1) << endl;
cout << " 2 " << myCaloHit[ch].GetEdep(3) << endl;
cout << " total e dep " << myCaloHit[ch].GetTotalEdep() << endl;
*/
}
	layer = myCaloHit[ch].GetVolume();
	checkID = layer / 100;
cout << " event " << eventID << " scatta " << layer << endl;
	if(checkID == 13)
	  s1Hit = true;
	if(layer == 1313)
	  s1Hit3 = true;
	if(layer == 1321)
	  s1Hit4 = true;
	if(layer == 1216)
	  P1Hit = true;
	if(layer == 1215)
	  P2Hit = true;
	if(layer == 1214)
	  P3Hit = true;
	if(layer == 1202)
	  P15Hit = true;
	if(layer == 1201)
	  P16Hit = true;
	if(checkID == 11)
	  P17Hit = true;
       }

// Trigger configurations according the trigger masks
      if(s1Hit && P1Hit)
	trigger[0] = true;
      if(s1Hit && P1Hit && P2Hit )
	trigger[1] = true;
      if(s1Hit && (P1Hit || P2Hit) && (P15Hit || P16Hit) )
	trigger[2] = true;
      if(s1Hit && (P1Hit || P2Hit))
	trigger[3] = true;
      if( (s1Hit3 || s1Hit4) && (P1Hit || P2Hit) )
	trigger[4] = true;
      if( (s1Hit3 || s1Hit4) && P1Hit )
	trigger[5] = true;
      if( (s1Hit3 || s1Hit4) && P2Hit )
	trigger[6] = true;
      if(s1Hit && (P1Hit || P2Hit) &&  P17Hit)
	trigger[7] = true;
      if(s1Hit && P1Hit && P2Hit && P3Hit )
	trigger[8] = true;

    }
/////////////////////

      if(verbose > 1)
	cout << " MC Energy = " << Energy << endl;

      for(size_t th=0;th<myTrackerHit.size();th++)
      {
	TOTALEdep+=myTrackerHit[th].GetELoss();
	SILEdep+=myTrackerHit[th].GetELoss();
//	layerDep->Fill(2,myTrackerHit[th].GetELoss());
//	layerDep->Fill(3,myTrackerHit[th].GetELoss());
      }
      if (verbose > 4)
	cout << " Silicon eDep = " << SILEdep << endl;
      //      siliconEdepHisto->Fill(SILEdep);

      cout << "myCaloHit.size(): " << myCaloHit.size() << endl;
      for(size_t ch=0;ch<myCaloHit.size();ch++)
      {
	layer = myCaloHit[ch].GetVolume();
	if(layer==1311) TRIGEdep[0]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1312) TRIGEdep[1]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1313) TRIGEdep[2]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1321) TRIGEdep[3]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1322) TRIGEdep[4]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1323) TRIGEdep[5]+= myCaloHit[ch].GetTotalEdep();
	if((layer>1310 && layer<1314)||(layer>1320 && layer<1324)) TOTTRIGEdep+= myCaloHit[ch].GetTotalEdep();

	if(layer==1201) SCINTEdep[0]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1202) SCINTEdep[1]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1203) SCINTEdep[2]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1204) SCINTEdep[3]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1205) SCINTEdep[4]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1206) SCINTEdep[5]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1207) SCINTEdep[6]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1208) SCINTEdep[7]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1209) SCINTEdep[8]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1210) SCINTEdep[9]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1211) SCINTEdep[10]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1212) SCINTEdep[11]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1213) SCINTEdep[12]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1214) SCINTEdep[13]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1215) SCINTEdep[14]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1216) SCINTEdep[15]+= myCaloHit[ch].GetTotalEdep();
	if(layer>1200 && layer<1217) TOTSCINTEdep+= myCaloHit[ch].GetTotalEdep();

	if(layer==1111) LYSOEdep[0]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1112) LYSOEdep[1]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1113) LYSOEdep[2]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1121) LYSOEdep[3]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1122) LYSOEdep[4]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1123) LYSOEdep[5]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1131) LYSOEdep[6]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1132) LYSOEdep[7]+= myCaloHit[ch].GetTotalEdep();
	if(layer==1133) LYSOEdep[8]+= myCaloHit[ch].GetTotalEdep();
	if((layer>1110 && layer<1114)||(layer>1120 && layer<1124)||(layer>1130 && layer<1134)) TOTLYSOEdep+= myCaloHit[ch].GetTotalEdep();

	if(layer==1411) VNEdep+= myCaloHit[ch].GetTotalEdep();
	if(layer==1412) VSEdep+= myCaloHit[ch].GetTotalEdep();
	if(layer==1421) VWEdep+= myCaloHit[ch].GetTotalEdep();
	if(layer==1422) VEEdep+= myCaloHit[ch].GetTotalEdep();
	if(layer==1430) VBEdep+= myCaloHit[ch].GetTotalEdep();
	if((layer>1410 && layer<1413)||(layer>1420 && layer<1423)||(layer==1430)) TOTVETOEdep+= myCaloHit[ch].GetTotalEdep();


	TOTALEdep+= myCaloHit[ch].GetTotalEdep();
//	layerDep->Fill(4,myCaloHit[ch].GetTotalEdep());
	binDep = 1216 - layer + 5;
//	layerDep->Fill(binDep,myCaloHit[ch].GetTotalEdep());

	if(checkID == 11)
//	  layerDep->Fill(21, myCaloHit[ch].GetTotalEdep());

	if (verbose > 1)
	  cout << " Layer = " << layer << endl;

	if(ch == myCaloHit.size() -1)
	  lastHit = layer;
      }

/*
      fit99 = Fit99->Eval(TOTALEdep);
      fit95 = Fit95->Eval(TOTALEdep);
      fit90 = Fit90->Eval(TOTALEdep);
*/
      if (SILEdep<fit99)
	islower99=true;
      if (SILEdep<fit95)
	islower95=true;
      if (SILEdep<fit90)
	islower90=true;

      stopBin=1216-lastHit+2;
      if(lastHit /100 == 11)
	stopBin=17;

      if (verbose > 1)
	cout << " Last Hit on : " << lastHit << " Bin Filled = "  << stopBin  << " with energy = " << Energy << endl;

      //stoppingPlane->Fill(Energy, stopBin);
      stoppingPlane->Fill(Energy, stopBin);
      measEnergy = TOTALEdep;

      if (verbose > 1)
	cout << " measured energy = " << TOTALEdep << endl;
      //if (verbose > 4){

	cout << " TOTTRIG eDep = " << TOTTRIGEdep << endl;
        cout << " TOTSCINT eDep = " << TOTSCINTEdep << endl;
        cout << " TOTLYSO eDep = " << TOTLYSOEdep << endl;
        cout << " TOTVETO eDep = " << TOTVETOEdep << endl;
	//}
/////////////////

      tree->Fill();
  }

/////////////////
// Save plot
  TString  meanText;
  TString  rmsText;
  TString fileName;

  TCanvas* c0c = new TCanvas("stopPlane","",200,10,700,500);
  stoppingPlane->SetTitle(particle+" StoppingPlane ");
  stoppingPlane->GetXaxis()->SetTitle("Energy [MeV]");
  stoppingPlane->GetYaxis()->SetTitle("Plane");
  stoppingPlane->Draw("colz");
  fileName = "StoppingPlane_"+particle+".png";
  c0c->SaveAs(fileName);

  TCanvas* c1 = new TCanvas("TrigFlux","",200,10,700,500);
  c1->SetLogx();
  c1->SetLogy();
  trigflux->SetTitle(particle+" Triggered Flux ");
  trigflux->GetXaxis()->SetTitle("Energy [MeV]");
  trigflux->GetYaxis()->SetTitle("# of particles");
  trigflux->Draw();
  fileName = "Trigflux_"+particle+".png";
  c1->SaveAs(fileName);


  TCanvas* c1a = new TCanvas("GenFlux","",200,10,700,500);
  c1a->SetLogx();
  c1a->SetLogy();
  genFlux->SetTitle(particle + " Generated flux ");
  genFlux->GetXaxis()->SetTitle("Energy [MeV]");
  genFlux->GetYaxis()->SetTitle("# particles");
  genFlux->Draw();
  fileName = "GenFlux_"+particle+".png";
  c1a->Update();
  c1a->SaveAs(fileName);

  TCanvas* c2 = new TCanvas("geomAccFlux","",200,10,700,500);
  c2->SetLogx();
  c2->SetLogy();
  geomAccflux->SetTitle(particle+" Geo Acceptance Flux ");
  geomAccflux->GetXaxis()->SetTitle("Energy [MeV]");
  geomAccflux->GetYaxis()->SetTitle("# of particles");
  geomAccflux->Draw();
  fileName = "GeomAccflux_"+particle+".png";
  c2->SaveAs(fileName);

  TCanvas* c5 = new TCanvas("GenSurface","",200,10,700,500);
  genSurface->SetTitle(particle+" Generating surface");
  genSurface->GetXaxis()->SetTitle("X [mm]");
  genSurface->GetYaxis()->SetTitle("Y [mm]");
  genSurface->Draw("colz");
  fileName = "GenSurface_"+particle+".png";

  TString gensurfpave = Form("Z = %.1f", zGenPos);
  TPaveText* pavegenSurface = new TPaveText(0.12,0.8,0.22,0.85, "NDC"); // NDC sets coords
  pavegenSurface->SetFillColor(0); // text is black on white
  pavegenSurface->SetTextSize(0.03);
  pavegenSurface->SetTextAlign(12);
  pavegenSurface->AddText(gensurfpave);
  pavegenSurface->Draw("");
  c5->SaveAs(fileName);
  /*
  c0c->Write("");
  stoppingPlane->Write("");
  c1->Write("");
  trigflux->Write("");


  c1a->Write("");
  genFlux->Write("");
  c2->Write("");
  geomAccflux->Write("");
  c5->Write("");
  genSurface->Write("");
  */


  tree->Write(0,TObject::kOverwrite);
  histoFile->Close();
}



Bool_t ToAnalyze::IsInAcceptance(TVector3 generationPosition, TVector3 direction)
{
  Int_t verbose = 0;

  if( verbose > 5 ){
    cout << " x= " << generationPosition[0] << " mm   y= " << generationPosition[1] << " mm   z= " << generationPosition[2] << " mm" << endl;
    cout << " vx = " << direction[0] << " vy = " << direction[1] << " vz = " << direction[2] << endl;
  }

  // Define quotes of acceptance definition planes
  static const float zS1 = 333.3 ; // mm. generationPosition and direction are expressed in millimeters
  static const float zP1 = 316.5 ;
  static const float zTracker0 = 359.1 ; //

  // Define acceptance regions on each layer as distances from Z axis (half-layer width)
  static const float xWidthS1 = 242/2. ;
  static const float yWidthS1 = 186/2. ;

  static const float xWidthP1 = 150/2. ;
  static const float yWidthP1 = 150/2. ;

  static const float xWidthTrack = 237.7/2. ;
  static const float yWidthTrack = 213/2. ;

  // First of all, check that primary is downgoing
  if (direction[2] >= 0.)
    return false;

  float tanThetaX = direction[0] / direction[2];
  float tanThetaY = direction[1] / direction[2];


  // Check entrance in Tracker[0]
  float zFromTracker0 = zTracker0 - generationPosition[2];
  if (fabs(zFromTracker0 * tanThetaX + generationPosition[0]) > xWidthTrack)
    return false;
  if (fabs(zFromTracker0 * tanThetaY + generationPosition[1]) > yWidthTrack)
    return false;


  // Check entrance in S1
  float zFromS1 = zS1 - generationPosition[2];
  if (fabs(zFromS1 * tanThetaX + generationPosition[0]) > xWidthS1)
    return false;
  if (fabs(zFromS1 * tanThetaY + generationPosition[1]) > yWidthS1)
    return false;

  // Check entrance in P1
  float zFromP1 = zP1 - generationPosition[2];
  if (fabs(zFromP1 * tanThetaX + generationPosition[0]) > xWidthP1)
    return false;
  if (fabs(zFromP1 * tanThetaY + generationPosition[1]) > yWidthP1)
    return false;

  //cout << " Return true " << endl;
  return true;
}


