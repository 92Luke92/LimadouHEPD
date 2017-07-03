#define Analyzer_cxx
#include "Analyzer.h"
#include <TH2.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TLegend.h>
#include <iostream>
#include <TRandom3.h>
#include <TMath.h>
#include <vector>


// change the BeamEnergy value according to the value put in the simulation (/hepd/gun/energy ... MeV in "../run/job/HEPD.mac")
const double BeamEnergy = 227.19;  // in MeV   other nominal energies: 36,50,(68),100,125,154,174,(202),228
/* if air is considered (from Francesco's results): 32.68, 47.49, 98.55, 123.78, 152.94, 173.03, 227.19 MeV */
const int maxCharge = 72; 

extern double const p2ch_ADCconv;   // layer1
extern double const p3ch_ADCconv;   // layer2
extern double const n2ch_ADCconv;   // layer1
extern double const n3ch_ADCconv;   // layer2

extern double const sigmaNoise;

extern double const mProton;


void Analyzer::Particle(const char* cpart, FILE* frout_charge)
{
  /*
  This function reads the particle name cpart and write in the file frout_charge the particle's properties,
  such as the charge Z, the atomic mass A and the mass. The particle list indentification must be implemented
  with new particles and isotopes.
  */

  const char *partstring[169] =
  {"e-","e+","mu-","mu+","anti_nu_e","nu_e","anti_nu_mu","nu_mu","gamma","neutron",
  "proton","deuteron","triton",
  "He3","alpha","He6",
  "Li3","Li4","Li5","Li6","Li7","Li8","Li9",
  "Be6","Be7","Be8","Be9","Be10","Be11","Be12",
  "B8","B9","B10","B11","B12","B13",
  "C9","C10","C11","C12","C13","C14","C15",
  "N11","N12","N13","N14","N15","N16","N17","N18",
  "O13","O14","O15","O16","O17","O18","O19",
  "F16","F17","F18","F19","F20","F21","F22","F23",
  "Ne20","Ne21","Ne22",
  "Na21","Na22","Na23","Na24","Na25",
  "Mg23","Mg24","Mg25","Mg26",
  "Al25","Al26","Al27","Al28",
  "Si27","Si28","Si29","Si30","Si31",
  "P28",
  "Cu63",
  "Se77",
  "Kr79",
  "Rb81","Rb82","Rb83","Rb84","Rb85","Rb86",
  "Sr82","Sr83","Sr84","Sr85","Sr86","Sr87","Sr88",
  "Y84","Y85","Y86","Y87","Y88","Y89","Y90",
  "Zr84","Zr85","Zr86","Zr87","Zr88",
  "Tm162","Tm163","Tm164","Tm165","Tm166","Tm167","Tm168","Tm169","Tm170","Tm171","Tm172","Tm173",
  "Yb163","Yb164","Yb165","Yb166","Yb167","Yb168","Yb169","Yb170","Yb171","Yb172","Yb173","Yb174",
  "Lu163","Lu164","Lu165","Lu166","Lu167","Lu168","Lu169","Lu170","Lu171","Lu172","Lu173","Lu174","Lu175","Lu176","Lu177",
  "Hf163","Hf164","Hf165","Hf166","Hf167","Hf168","Hf169","Hf170","Hf171","Hf172","Hf173","Hf174","Hf175","Hf176"};

  Double_t charge_value[169] = {-1,1,-1,1,0,0,0,0,0,0,
                                1,1,1,
                                2,2,2,
                                3,3,3,3,3,3,3,
                                4,4,4,4,4,4,4,
                                5,5,5,5,5,5,
                                6,6,6,6,6,6,6,
                                7,7,7,7,7,7,7,7,
                                8,8,8,8,8,8,8,
                                9,9,9,9,9,9,9,9,
                                10,10,10,
                                11,11,11,11,11,
                                12,12,12,12,
                                13,13,13,13,
                                14,14,14,14,14,
                                15,
                                29,
                                34,
                                36,
                                37,37,37,37,37,37,
                                38,38,38,38,38,38,38,
                                39,39,39,39,39,39,39,
                                40,40,40,40,40,
                                69,69,69,69,69,69,69,69,69,69,69,69,
                                70,70,70,70,70,70,70,70,70,70,70,70,
                                71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,
                                72,72,72,72,72,72,72,72,72,72,72,72,72,72};

  Double_t A_value[169] = {0,0,0,0,0,0,0,0,0,1,
                          1,2,3,
                          3,4,6,
                          3,4,5,6,7,8,9,
                          6,7,8,9,10,11,12,
                          8,9,10,11,12,13,
                          9,10,11,12,13,14,15,
                          11,12,13,14,15,16,17,18,
                          13,14,15,16,17,18,19,
                          16,17,18,19,20,21,22,23,
                          20,21,22,
                          21,22,23,24,25,
                          23,24,25,26,
                          25,26,27,28,
                          27,28,29,30,31,
                          28,
                          63,
                          77,
                          79,
                          81,82,83,84,85,86,
                          82,83,84,85,86,87,88,
                          84,85,86,87,88,89,90,
                          84,85,86,87,88,
                          162,163,164,165,166,167,168,169,170,171,172,173,
                          163,164,165,166,167,168,169,170,171,172,173,174,
                          163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,
                          163,164,165,166,167,168,169,170,171,172,173,174,175,176};

  // in MeV (values from "ElementMass.xlsx")
  Double_t Mass_value[169] = {0.5109989461,0.5109989461,105.6583745,105.6583745,0,0,0,0,0,939.565413,
                              938.2720813,1876.123914,2809.432094,
                              2809.413502,3728.401297,5606.55968,
                              2823.149017,3751.299568,4669.149364,5603.051366,6535.36681,7472.899608,8408.401124,
                              5607.33952,6536.228703,7456.894435,8394.794507,9327.547626,10266.60911,11203.00565,
                              7474.874255,8395.86254,9326.991686,10255.10298,11191.29804,12125.98541,
                              8412.357351,9330.639636,10257.08539,11177.92914,12112.54825,13043.93723,13982.28458,
                              10270.73862,11195.26723,12114.76872,13043.78075,13972.51287,14909.58918,15843.27099,16780.00818,
                              12132.53567,13048.92469,13975.26704,14899.16852,15834.59081,16766.11219,17701.72268,
                              14914.58578,15837.35132,16767.76742,17696.90043,18629.8645,19561.32845,20502.174,21427.69394,
                              18622.83998,19555.64423,20484.84538,
                              19559.19184,20487.68766,21414.83434,22347.44017,23277.99457,
                              21418.89043,22341.92472,23274.15956,24202.6319,
                              23278.43621,24206.63617,25133.14392,26064.98423,
                              25137.95627,26060.34187,26991.43372,27920.38993,28853.36795,
                              26074.67592,
                              58618.54847,
                              71650.44575,
                              73513.59079,
                              75375.5669,76306.32761,77234.93511,78165.75398,79094.83077,80025.74518,
                              76306.50743,77237.21447,78164.86017,80023.96862,90270.04683,79095.89553,81883.55865,
                              78171.34615,79099.15598,80029.20862,80956.96757,81887.18125,82815.27273,83747.98112,
                              78174.01184,79103.84898,80030.68785,80960.63814,81887.85729,
                              150840.5599,151770.8025,152703.1432,153633.5898,154566.1259,155496.9656,156429.6904,157361.2222,158294.1956,159226.2747,160159.6044,161092.2196,
                              151774.2333,152704.0089,153636.2385,154566.4313,155498.9199,156429.4334,157362.1318,158293.2273,159226.1782,160157.7241,161090.9222,162023.023,
                              151778.7461,152710.3893,153640.0835,154571.9989,155502.0128,156433.9439,157364.4248,158296.686,159227.6568,160160.2431,161091.5927,162024.3973,162956.296,163889.5734,164822.0658,
                              151784.2513,152713.2101,153644.8902,154574.1608,155506.0462,156435.6475,157367.7852,158297.7424,159230.059,160160.5809,161093.0667,162024.126,162956.9828,163888.3833};
 
  
  pcharge = 0;
  A = 0;
  Mass = 0;
  for (int i=0; i<169; i++) {
    if (strcmp(cpart,partstring[i])==0) {
      pcharge = charge_value[i];
      A = A_value[i];
      Mass = Mass_value[i];
      break;
    }
  }

  Int_t ecrire = 1;

  Int_t neutron = 0;
  if (strcmp(cpart,NEUTRON)==0) neutron = true;
  Int_t gamma = 0;
  if (strcmp(cpart,GAMMA)==0) gamma = true;
  Int_t positron = 0;
  if (strcmp(cpart,POSITRON)==0) positron = true;
  Int_t anti_muon = 0;
  if (strcmp(cpart,ANTI_MUON)==0) anti_muon = true;
  Int_t anti_nu_e = 0;
  if (strcmp(cpart,ANTI_NU_E)==0) anti_nu_e = true;
  Int_t nu_e = 0;
  if (strcmp(cpart,NU_E)==0) nu_e = true;
  Int_t anti_nu_mu = 0;
  if (strcmp(cpart,ANTI_NU_MU)==0) anti_nu_mu = true;
  Int_t nu_mu = 0;
  if (strcmp(cpart,NU_MU)==0) nu_mu = true;
  Int_t proton = 0;
  if (strcmp(cpart,PROTON)==0) proton = true;
 
  if (pcharge == 0 && A > 2) {
    printf("charge zero cpart %s\n",cpart);
    pcharge = 100;
    A = 0;
  }
 

  if (!gamma && !neutron && !anti_nu_e && !nu_e && !anti_nu_mu && !nu_mu && !positron && !anti_muon) {  
    // also positrons, anti_muons and neutrinos are not considered
    if (pcharge == 0) {
      fprintf(frout_charge,"---> cpart %s charge zero\n",cpart);
    }
    else if (pcharge > 0 && !proton) {  // protons are excluded
      fprintf(frout_charge,"cpart %s   Z %2.0lf A %2.0lf\n",cpart, pcharge, A);
    }
  }

  return;

}

void Analyzer::InitHisto()
{
  /*
  This function writes the histograms for the track multiplicity, for the charge and kinetic energy of secondary nuclei
  in a Root file that will be read in "aiglon_g4_hist.C" 
  */

  int int_BeamEnergy = (int)BeamEnergy;
  char ctext[80];

  hntrace = new TH1D("hntrace","",((int) (BeamEnergy/2))+2,-1.0,((int)(BeamEnergy/2))+1);
  hcsec = new TH1D("hcsec","",maxCharge+2,-1.0,maxCharge+1);
  for (int i=0; i<27; i++) {
    sprintf(ctext,"hcesec%d",i+1);
    hcesec[i] = new TH1D(ctext,"",(int_BeamEnergy+2)*4,-1.0,int_BeamEnergy+1); // it creates 26 histograms, one for each Z-particle
  }

}

void Analyzer::CloseHisto()
{
  /*
  This function writes the histograms for the track multiplicity, for the charge and kinetic energy of secondary nuclei
  in a Root file that will be read in "aiglon_g4_hist.C" 
  */

  char chist[180];
  sprintf(chist,"hepd_qmd_%dMeV_proton_3C0_tracks_hist.root",(int)BeamEnergy);	// change the name in relation to the hadronic model used and the geometry of the beam!
  TFile *fhist = new TFile(chist,"RECREATE");
  
  hntrace->Write();
  hcsec->Write();
  for (int i=0; i<27; i++) hcesec[i]->Write();

  fhist->Close();
  
}

void Analyzer::Loop()
{
  /*
  This function allows to record the track multiplicity, the charge and the kinetic energy of secondary nuclei
  anlyzing each track at a time. The particle informations present in each event are recorded in the frout_tracks file
  and in the frout_charge file
  */

  char cfich[180];
  sprintf(cfich,"frout_charge_qmd_%dMeV_proton_3C0_tracks.txt",(int)BeamEnergy);
  FILE *frout_charge = fopen(cfich,"w");

  sprintf(cfich,"frout_tracks_qmd_%dMeV_proton_3C0_tracks.txt",(int)BeamEnergy);
  FILE *frout_tracks = fopen(cfich,"w");

  int bufsize = 80000000;
    
  Long64_t nentries = fTreeChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  Double_t maxLayer = 0;

  printf("Beam Energy = %3.2f MeV\n", BeamEnergy);
  fprintf(frout_tracks,"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
  fprintf(frout_tracks,"Beam Energy = %3.2f MeV\n", BeamEnergy);
  fprintf(frout_tracks,"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");

  printf("nentries %lld\n",nentries);
  for (Long64_t jentry=0; jentry<nentries;jentry++) { // Loop on the genrated Events
 
    bool s1Hit=false;
    bool s1Hit_centre=false;
    bool s2Hit=false;
    bool s3Hit=false;
    bool s4Hit=false;
    bool s5Hit=false;
    bool s6Hit=false;
    bool s7Hit=false;
    bool s8Hit=false;
    bool s9Hit=false;
    bool s10Hit=false;
    bool s11Hit=false;
    bool s12Hit=false;
    bool s13Hit=false;
    bool s14Hit=false;
    bool s15Hit=false;
    bool s16Hit=false;
    bool s17Hit=false;
    bool LYSOHit=false;
    bool noVetoHit=false;
    bool noVeto_lateral=true;
    bool noVeto_bottom=true;
    bool hitOnTkLayer1=false;
    bool hitOnTkLayer2=false;
    bool goodHit=false;
    maxLayer=0;
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fTreeChain->GetEntry(jentry);
    nbytes += nb;
    int eventid = Event->EventID();
    std::vector<RootTrack> myTracks = Event->GetTracks();
    std::vector<RootCaloHit> myCaloHit = Event->GetCaloHit();
    std::vector<RootCaloHit> myVetoHit = Event->GetVetoHit();
    std::vector<RootTrackerHit> myTrackerHit = Event->GetTrackerHit();

    int ntrace = (int) myTracks.size();
    int nCaloHit = (int) myCaloHit.size();
    int nTrackerHit = (int) myTrackerHit.size();
    int nVetoHit = (int) myVetoHit.size();

    printf("===> EventId %d ntrace %d nCaloHit %d nTrackerHit %d nVetoHit %d\n",eventid, ntrace, nCaloHit, nTrackerHit, nVetoHit);
    hntrace->Fill((Double_t) ntrace); // ntrace hist (1)
    fprintf(frout_tracks,"===> EventId %d ntrace %d\n", eventid, ntrace);

    for (int tr = 0; tr < ntrace; tr++){ // Loop on the tracks for each Event
      int trId = myTracks[tr].GetTrackID();
      int pdg = myTracks[tr].GetPDG();
      TString pname = myTracks[tr].GetParticleName();
      TString vvname = myTracks[tr].GetVertexVolumeName();
      TString cpname = myTracks[tr].GetCreatorProcessName();
      int parentId = myTracks[tr].GetParentTrackID();
      float ke = myTracks[tr].GetKinEnergy();
      const char* cpart = pname;
      printf("cpart %s\n",cpart);
      Particle(cpart, frout_charge);
      if (trId > 1 && pcharge > 0 && A > 0) {
        /* 
        trId > 1 excludes primary protons; 
        pcharge > 0 excludes gammas, neutrons, electrons, muons, neutrinos and everything is not in the particle list;
        A > 0 excludes positrons and anti_muons, which would behave as protons in hcesec[ih] without this condition 
        (to avoid protons, put A > 1)
        */
        hcsec->Fill(pcharge);
        Int_t ih = ((Int_t) pcharge) - 1;
        if (ih == 28) // put Cu as the following element of P (way out in order to avoid the gap in the particle list)
          ih = 15;
        else if (ih == 33)  // Se after Cu
          ih = 16;
        else if (ih == 35)  // Kr
          ih = 17;
        else if (ih == 36)  // Rb
          ih = 18;
        else if (ih == 37)  // Sr
          ih = 19;
        else if (ih == 38)  // Y
          ih = 20;
        else if (ih == 39)  // Zr
          ih = 21;
        else if (ih == 68)  // Tm
          ih = 22;
        else if (ih == 69)  // Yb
          ih = 23;
        else if (ih == 70)  // Lu
          ih = 24;
        else if (ih == 71)  // Hf
          ih = 25;
        hcesec[ih]->Fill((Double_t) ke);
      }
      if (trId > 1 && pcharge == 0 && A == 1)   // neutrons are included now
        hcesec[26]->Fill((Double_t) ke);

      if (trId == 1) {   // trId == 1 corresponds to primary protons
        fprintf(frout_tracks,"trId %d :  PDG %d   pname %s\n", trId, pdg, pname.Data());
        fprintf(frout_tracks,"          A %2.0lf   Z %2.0lf\n", A, pcharge);
        fprintf(frout_tracks,"          Mass %lf MeV\n", Mass);
        fprintf(frout_tracks,"          ke %f MeV\n", ke);
      }
      else {
        fprintf(frout_tracks,"trId %d :  PDG %d   pname %s\n", trId, pdg, pname.Data());
        fprintf(frout_tracks,"          A %2.0lf   Z %2.0lf\n", A, pcharge);
        fprintf(frout_tracks,"          Mass %lf MeV\n", Mass);
        fprintf(frout_tracks,"          ke %f MeV\n", ke); 
        fprintf(frout_tracks,"          vvol %s   cpname %s\n", vvname.Data(), cpname.Data());
        fprintf(frout_tracks,"          parentId %d\n", parentId);
      }
      fprintf(frout_tracks,"          ------------------------\n");
    }
    fprintf(frout_tracks,"\n");
  }

  fclose(frout_charge);
  fclose(frout_tracks);
}

//void Analyzer::Loop()
//{
//
//  char cfich[80];
//  sprintf(cfich,"frout.txt");
//  FILE *frout = fopen(cfich,"w");
//  sprintf(cfich,"frout_charge.txt");
//  FILE *frout_charge = fopen(cfich,"w");
//
//  //  double Energy = E;
//  int bufsize = 80000000;
//    
//  Long64_t nentries = fTreeChain->GetEntries();
//  Long64_t nbytes = 0, nb = 0;
//  Double_t maxLayer = 0;
//
//  printf("nentries %d\n",nentries);
//  //  for (Long64_t jentry=0; jentry<50000;jentry++) {
//  for (Long64_t jentry=0; jentry<nentries;jentry++) {
// 
//    bool s1Hit=false;
//    bool s1Hit_centre=false;
//    bool s2Hit=false;
//    bool s3Hit=false;
//    bool s4Hit=false;
//    bool s17Hit=false;
//    bool LYSOHit=false;
//    bool noVetoHit=false;
//    bool noVeto_lateral=true;
//    bool noVeto_bottom=true;
//    bool hitOnTkLayer1=false;
//    bool hitOnTkLayer2=false;
//    bool Trig=false;
//    bool Trig_sv=false;
//
//    maxLayer=0;
//    Long64_t ientry = LoadTree(jentry);
//    if (ientry < 0) break;
//    nb = fTreeChain->GetEntry(jentry);   nbytes += nb;
//    int eventid = Event->EventID();
//    std::vector<RootTrack> myTracks = Event->GetTracks();
//    std::vector<RootCaloHit> myCaloHit = Event->GetCaloHit();
//    std::vector<RootCaloHit> myVetoHit = Event->GetVetoHit();
//    std::vector<RootTrackerHit> myTrackerHit = Event->GetTrackerHit();
//
//    int ntrace = (int) myTracks.size();
//    //    hntrk_evt->Fill(((Double_t) ntrace));
//    printf("===> EventId %d ntrace %d\n",eventid,ntrace);
//    hntrace->Fill((Double_t) ntrace);
//    fprintf(frout,"===> EventId %d ntrace %d\n",eventid,ntrace);
//    for(size_t tr=0;tr<myTracks.size();tr++){
//      int pdg = myTracks[tr].GetPDG();
//      TString pname = myTracks[tr].GetParticleName();
//      TString vvname = myTracks[tr].GetVertexVolumeName();
//      TString cpname = myTracks[tr].GetCreatorProcessName();
//      float ke = myTracks[tr].GetKinEnergy();
//      const char* cpart = pname;
//      printf("cpart %s\n",cpart);
//      Particle(cpart,frout_charge);
//      if (tr == 0)
//  fprintf(frout," tr %d pdg %d nom %s ke %f MeV A %3.0lf Z %3.0lf\n",((int) tr),pdg,pname.Data(),ke,A,pcharge);
//      else  
//  fprintf(frout," tr %d pdg %d nom %s ke %f MeV A %3.0lf Z %3.0lf vvol %s pnom %s\n",((int) tr),pdg,pname.Data(),ke,A,pcharge,vvname.Data(),cpname.Data());
//      if (tr > 0 && pcharge > 0) {
//  hcsec->Fill(pcharge);
//        Int_t ih = ((Int_t) pcharge) - 1;
//  hcesec[ih]->Fill((Double_t) ke);
//      }
//    } //trace  
//
//    if(myCaloHit.size()>0){
//      for(size_t sh=0;sh<myCaloHit.size();sh++){
//  int scintLayer = myCaloHit[sh].GetVolume();
//  //        printf("scintlayer %d\n",scintLayer);
//  std::vector<int> plist = myCaloHit[sh].GetParticleList();       
//  for(size_t p=0;p<plist.size();p++)
//    printf("pid %d PDG %d\n",plist[p],myTracks[plist[p]].GetPDG());
//  //    if(scintLayer.Contains("S1"))
//  //    if (myCaloHit.size()>=2 && ce <= 15)
//        //  printf("scintLayer %d\n",scintLayer);
//        if (plist[0]==1) {
//  if (scintLayer >= 1300) s1Hit=true;
//        if (scintLayer == 1313 || scintLayer == 1321) s1Hit_centre=true;
//        if (scintLayer == 1216) s2Hit=true;
//        if (scintLayer == 1215) s3Hit=true;
//        if (scintLayer == 1214) s4Hit=true;
//        if (scintLayer == 1201) s17Hit=true;
//        if (scintLayer >= 1111 && scintLayer <= 1133) LYSOHit=true;
//        }
//      }
//      //      printf("\n");
//      // t3
//      //            if(s1Hit && (s2Hit || s3Hit) && s17Hit) {
//      // t4
//      //      if(s1Hit && (s2Hit || s3Hit)) {
//      // t5
//      //        if(s1Hit_centre && s2Hit) {
//      // t6
//      if(s1Hit && (s2Hit || s3Hit) && LYSOHit) {
//  if(myVetoHit.size()==0) noVetoHit=true;
//        if(myVetoHit.size()>0) {
//    for(size_t vh=0;vh<myVetoHit.size();vh++){
//      int scintLayer = myVetoHit[vh].GetVolume();
//            if (scintLayer >= 1411 && scintLayer <= 1422) noVeto_lateral=false;
//            if (scintLayer == 1430) noVeto_bottom=false;
//          }
//        }
//  if(myTrackerHit.size()>0){
//    for(size_t th=0;th<myTrackerHit.size();th++){
//      int detId = myTrackerHit[th].GetDetectorId();
//            int tkid = myTrackerHit[th].GetTrackId();
//      if(detId>2200 && tkid==1) {
//        hitOnTkLayer1=true;
//        //              printf("hit layer 1\n",th);
//            }
//      else if(detId>2100 && tkid==1) {
//        hitOnTkLayer2=true;
//        //              printf("hit layer 2\n",th);
//            }
//    }
//    if(hitOnTkLayer2&&hitOnTkLayer1) {
//      if (noVetoHit) Trig_sv=true;
//            else Trig=true;
//          }
//  }
//      }
//    }
//  
//  } //eve
//  fclose(frout);
//  fclose(frout_charge);
//}


void Analyzer::InitHisto_ELoss()
{
  /*
  This function creates histograms for the energy lost by primary or/and by secondary particles for each event in the whole detector,
  and the energy dissipated in the tracker or in the calo considered separately by primaries and/or by secondaries.
  */

  int int_BeamEnergy = (int)BeamEnergy;

  hELossPrimaryEvent = new TH1D("hELossPrimaryEvent","",int_BeamEnergy+2,-1.0,int_BeamEnergy+1);
  hELossSecondaryEvent = new TH1D("hELossSecondaryEvent","",int_BeamEnergy+2,-1.0,int_BeamEnergy+1);

  hELossEvent = new TH1D("hELossEvent","",int_BeamEnergy+2,-1.0,int_BeamEnergy+1);

  hELossPrimaryTracker = new TH1D("hELossPrimaryTracker","",64,-0.5,15.5);
  hELossSecondaryTracker = new TH1D("hELossSecondaryTracker","",64,-0.5,15.5);
  hELossPrimaryCalo = new TH1D("hELossPrimaryCalo","",int_BeamEnergy+2,-1.0,int_BeamEnergy+1);
  hELossSecondaryCalo = new TH1D("hELossSecondaryCalo","",int_BeamEnergy+2,-1.0,int_BeamEnergy+1);

  hELossTracker = new TH1D("hELossTracker","",64,-0.5,15.5);
  hELossCalo = new TH1D("hELossCalo","",int_BeamEnergy+2,-1.0,int_BeamEnergy+1);

}

void Analyzer::CloseHisto_ELoss()
{
  /*
  This function writes the histograms for the energy lost by primary or/and by secondary particles for each event in the whole detector,
  and the energy dissipated in the tracker or in the calo considered separately by primaries and/or by secondaries
  in a Root file that will be read in "aiglon_g4_hist.C" 
  */

  char chist[180];
  sprintf(chist,"hepd_qmd_%dMeV_proton_3C0_ELoss_hist.root",(int)BeamEnergy);	// change the name in relation to the hadronic model used and the geometry of the beam!
  TFile *fhist = new TFile(chist,"RECREATE");
  
  hELossPrimaryEvent->Write();
  hELossSecondaryEvent->Write();

  hELossEvent->Write();

  hELossPrimaryTracker->Write();
  hELossSecondaryTracker->Write();
  hELossPrimaryCalo->Write();
  hELossSecondaryCalo->Write();

  hELossTracker->Write();
  hELossCalo->Write();

  fhist->Close(); 

}

void Analyzer::Loop_ELoss()
{
  /*
  This function allows to record the energy lost by primary or/and by secondary particles for each event in the whole detector,
  and the energy dissipated in the tracker or in the calo considered separately by primaries and/or by secondaries
  anlyzing each track at a time. The particle informations, including the energy lost at each step, are recorded in the frout_tracks file
  and in the frout_charge file
  */

  char cfich[180];
  sprintf(cfich,"frout_charge_qmd_%dMeV_proton_3C0_ELoss.txt",(int)BeamEnergy);
  FILE *frout_charge = fopen(cfich,"w");

  sprintf(cfich,"frout_tracks_qmd_%dMeV_proton_3C0_ELoss.txt",(int)BeamEnergy);
  FILE *frout_tracks = fopen(cfich,"w");

  int bufsize = 80000000;
    
  Long64_t nentries = fTreeChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  Double_t maxLayer = 0;

  printf("Beam Energy = %3.2f MeV\n", BeamEnergy);
  fprintf(frout_tracks,"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
  fprintf(frout_tracks,"Beam Energy = %3.2f MeV\n", BeamEnergy);
  fprintf(frout_tracks,"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");

  printf("nentries %lld\n",nentries);
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
 
    bool s1Hit=false;
    bool s1Hit_centre=false;
    bool s2Hit=false;
    bool s3Hit=false;
    bool s4Hit=false;
    bool s5Hit=false;
    bool s6Hit=false;
    bool s7Hit=false;
    bool s8Hit=false;
    bool s9Hit=false;
    bool s10Hit=false;
    bool s11Hit=false;
    bool s12Hit=false;
    bool s13Hit=false;
    bool s14Hit=false;
    bool s15Hit=false;
    bool s16Hit=false;
    bool s17Hit=false;
    bool LYSOHit=false;
    bool noVetoHit=false;
    bool noVeto_lateral=true;
    bool noVeto_bottom=true;
    bool hitOnTkLayer1=false;
    bool hitOnTkLayer2=false;
    bool goodHit=false;
    maxLayer=0;
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fTreeChain->GetEntry(jentry);
    nbytes += nb;
    int eventid = Event->EventID();
    std::vector<RootTrack> myTracks = Event->GetTracks();
    std::vector<RootCaloHit> myCaloHit = Event->GetCaloHit();
    std::vector<RootCaloHit> myVetoHit = Event->GetVetoHit();
    std::vector<RootTrackerHit> myTrackerHit = Event->GetTrackerHit();

    int ntrace = (int) myTracks.size();
    int nCaloHit = (int) myCaloHit.size();
    int nTrackerHit = (int) myTrackerHit.size();
    int nVetoHit = (int) myVetoHit.size();

    printf("===> EventId %d ntrace %d nCaloHit %d nTrackerHit %d nVetoHit %d\n",eventid, ntrace, nCaloHit, nTrackerHit, nVetoHit);
    fprintf(frout_tracks,"===> EventId %d ntrace %d\n", eventid, ntrace);

    float ELossPrimaryEvent = 0;  // to each event, it starts from 0
    float ELossSecondaryEvent = 0;  // to each event, it starts from 0

    float ELossEvent = 0;

    float ELossPrimaryTracker = 0;
    float ELossSecondaryTracker = 0;
    float ELossPrimaryCalo = 0;
    float ELossSecondaryCalo = 0;

    float ELossTracker = 0;
    float ELossCalo = 0;

    for(int tr = 0; tr < ntrace; tr++){
      int trId = myTracks[tr].GetTrackID();
      int pdg = myTracks[tr].GetPDG();
      TString pname = myTracks[tr].GetParticleName();
      TString vvname = myTracks[tr].GetVertexVolumeName();
      TString cpname = myTracks[tr].GetCreatorProcessName();
      int parentId = myTracks[tr].GetParentTrackID();
      float ke = myTracks[tr].GetKinEnergy();
      const char* cpart = pname;
      printf("cpart %s\n",cpart);
      Particle(cpart, frout_charge);

      float ELossPrimaryTrack = 0;    // to each track, it starts from 0
      float ELossSecondaryTrack = 0;  // to each track, it starts from 0    

      if (trId == 1) {  // trId == 1 corresponds to primary protons
        fprintf(frout_tracks,"trId %d :  PDG %d   pname %s\n", trId, pdg, pname.Data());
        fprintf(frout_tracks,"          A %2.0lf   Z %2.0lf\n", A, pcharge);
        fprintf(frout_tracks,"          Mass %lf MeV\n", Mass);
        fprintf(frout_tracks,"          ke %f MeV\n", ke);
      }
      else {
        fprintf(frout_tracks,"trId %d :  PDG %d   pname %s\n", trId, pdg, pname.Data());
        fprintf(frout_tracks,"          A %2.0lf   Z %2.0lf\n", A, pcharge);
        fprintf(frout_tracks,"          Mass %lf MeV\n", Mass);
        fprintf(frout_tracks,"          ke %f MeV\n", ke); 
        fprintf(frout_tracks,"          vvol %s   cpname %s\n", vvname.Data(), cpname.Data());
        fprintf(frout_tracks,"          parentId %d\n", parentId);
      }
      fprintf(frout_tracks,"          ------------------------\n");
      // For each track, its dynamics in the tracker is considered
      if (nTrackerHit > 0) 
        for (int th = 0; th < nTrackerHit; th++) {
          int detId = myTrackerHit[th].GetDetectorId();
          int tkId = myTrackerHit[th].GetTrackId();
          if (trId == tkId) { // we force the TrackerId to be the same as the TrackId in order to get the energy lost by each particle corresponding to each the track at a time
            fprintf(frout_tracks,"          tracker : ELoss %f MeV\n", myTrackerHit[th].GetELoss());
            fprintf(frout_tracks,"                    detId %d", detId);
            if (trId == 1) {
              ELossPrimaryTrack += myTrackerHit[th].GetELoss();
              ELossPrimaryEvent += myTrackerHit[th].GetELoss();
              ELossPrimaryTracker += myTrackerHit[th].GetELoss();
            }
            else {
              ELossSecondaryTrack += myTrackerHit[th].GetELoss();
              ELossSecondaryEvent += myTrackerHit[th].GetELoss();
              ELossSecondaryTracker += myTrackerHit[th].GetELoss();
            }
            if (detId > 2200) {
              hitOnTkLayer1 = true;
              fprintf(frout_tracks," ---> hit layer 1\n");
            }
            else if (detId > 2100 && detId < 2200) {  // instead of only detId>2100
              hitOnTkLayer2 = true;
              fprintf(frout_tracks," ---> hit layer 2\n");
            }
            hitOnTkLayer1 = false;
            hitOnTkLayer2 = false;
          }
        }
        else
          fprintf(frout_tracks,"          tracker : NULL\n");
        // The physics of the same track is studied in the Calo
        if (nCaloHit > 0)
          for(int sh = 0; sh < nCaloHit; sh++) {
            int scintLayer = myCaloHit[sh].GetVolume();
            std::vector<int> plist = myCaloHit[sh].GetParticleList();
            int nplist = (int) plist.size();
            for(int p = 0; p < nplist; p++) 
              if (trId == plist[p]) { // we consider the same TrackId as before since we are focussing on the same particle 
                fprintf(frout_tracks,"          scintillator : ELoss %f MeV\n", myCaloHit[sh].GetEdep(plist[p]));
                fprintf(frout_tracks,"                         scintlayer %d", scintLayer);
                if (trId == 1) {
                  ELossPrimaryTrack += myCaloHit[sh].GetEdep(plist[p]);
                  ELossPrimaryEvent += myCaloHit[sh].GetEdep(plist[p]);
                  ELossPrimaryCalo += myCaloHit[sh].GetEdep(plist[p]);
                }
                else {
                  ELossSecondaryTrack += myCaloHit[sh].GetEdep(plist[p]);
                  ELossSecondaryEvent += myCaloHit[sh].GetEdep(plist[p]);
                  ELossSecondaryCalo += myCaloHit[sh].GetEdep(plist[p]);
                }
                if (scintLayer >= 1300 && scintLayer < 1400) {
                  s1Hit=true;
                  fprintf(frout_tracks," ---> hit scintillator s1\n");
                  if (scintLayer == 1313 || scintLayer == 1321) s1Hit_centre = true;
                }
                switch (scintLayer) {
                  case 1216 : {
			                        s2Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s2\n"); 
                              break;
			                        }
                  case 1215 : {
			                        s3Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s3\n"); 
                              break;
			                        }
                  case 1214 : {
			                        s4Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s4\n"); 
                              break;
			                        }
                  case 1213 : {
			                        s5Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s5\n"); 
                              break;
			                        }
                  case 1212 : {
			                        s6Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s6\n"); 
                              break;
			                        }
                  case 1211 : {
			                        s7Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s7\n"); 
                              break;
			                        }
                  case 1210 : {
			                        s8Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s8\n"); 
                              break;
			                        }	
                  case 1209 : {
			                        s9Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s9\n"); 
                              break;
			                        }
                  case 1208 : {
			                        s10Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s10\n"); 
                              break;
			                        }
                  case 1207 : {
			                        s11Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s11\n"); 
                              break;
			                        }
                  case 1206 : {
			                        s12Hit = true; 
                              fprintf(frout_tracks," ---> hit scintillator s12\n");
                              break;
			                        }
                  case 1205 : {
			                        s13Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s13\n"); 
                              break;
			                        }
                  case 1204 : {
			                        s14Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s14\n"); 
                              break;
			                        }
                  case 1203 : {
                              s15Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s15\n"); 
                              break;
			                        }
                  case 1202 : {
			                        s16Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s16\n"); 
                              break;
			                        }
                  case 1201 : {
    			                    s17Hit = true;
                              fprintf(frout_tracks," ---> hit scintillator s17\n"); 
                              break;
			                        }
                  default : break;
                }
                if (scintLayer >= 1111 && scintLayer <= 1133) {
                  LYSOHit = true;
                  fprintf(frout_tracks," ---> hit LYSO\n");
                }
              }
          }
          else
            fprintf(frout_tracks,"          scintillator : NULL\n");
          // Test if the considered particle hits the veto
          if(nVetoHit > 0) {
            for(int vh = 0; vh < nVetoHit; vh++) {
              int scintLayerV = myVetoHit[vh].GetVolume();
              std::vector<int> plistV = myVetoHit[vh].GetParticleList();
              int nplistV = (int) plistV.size();
              for(int p = 0; p < nplistV; p++) 
                if (trId == plistV[p]) {  // consider the same particle as before
                  fprintf(frout_tracks,"          veto : ELoss %f MeV\n", myVetoHit[vh].GetEdep(plistV[p]));
                  fprintf(frout_tracks,"                 scintlayer %d", scintLayerV);
                  if (trId == 1) {
                    ELossPrimaryTrack += myVetoHit[vh].GetEdep(plistV[p]);
                    ELossPrimaryEvent += myVetoHit[vh].GetEdep(plistV[p]);
                    ELossPrimaryCalo += myVetoHit[vh].GetEdep(plistV[p]);
                  }
                  else {
                    ELossSecondaryTrack += myVetoHit[vh].GetEdep(plistV[p]);
                    ELossSecondaryEvent += myVetoHit[vh].GetEdep(plistV[p]);
                    ELossSecondaryCalo += myVetoHit[vh].GetEdep(plistV[p]);
                  }
                  if (scintLayerV >= 1411 && scintLayerV <= 1422) {
                    noVeto_lateral = false;
                    fprintf(frout_tracks," ---> hit lateral veto\n");
                  }
                  if (scintLayerV == 1430) {
                    noVeto_bottom = false;
                    fprintf(frout_tracks," ---> hit bottom veto\n");
                  }
                }
            }
          }
          else {
            noVetoHit = true;
            fprintf(frout_tracks,"          veto : NULL\n");
          }
          fprintf(frout_tracks,"          ------------------------\n");
          if (trId == 1)
            fprintf(frout_tracks,"          ELossPrimaryTrack %f MeV\n", ELossPrimaryTrack);
          else
            fprintf(frout_tracks,"          ELossSecondaryTrack %f MeV\n", ELossSecondaryTrack);
          fprintf(frout_tracks,"          ------------------------\n");
    }
    ELossTracker = ELossPrimaryTracker + ELossSecondaryTracker;
    ELossCalo = ELossPrimaryCalo + ELossSecondaryCalo;
    ELossEvent = ELossTracker + ELossCalo;

    fprintf(frout_tracks,"\n");

    fprintf(frout_tracks,"          ------------------------\n");
    fprintf(frout_tracks,"          ELossPrimaryTracker %f MeV\n", ELossPrimaryTracker);
    fprintf(frout_tracks,"          ELossPrimaryCalo %f MeV\n", ELossPrimaryCalo);
    fprintf(frout_tracks,"\n");
    fprintf(frout_tracks,"          ELossSecondaryTracker %f MeV\n", ELossSecondaryTracker);
    fprintf(frout_tracks,"          ELossSecondaryCalo %f MeV\n", ELossSecondaryCalo);
    fprintf(frout_tracks,"\n");
    fprintf(frout_tracks,"          ELossTracker %f MeV\n", ELossTracker);
    fprintf(frout_tracks,"          ELossCalo %f MeV\n", ELossCalo);
    fprintf(frout_tracks,"          ------------------------\n");

    hELossPrimaryTracker->Fill((Double_t) ELossPrimaryTracker);
    hELossSecondaryTracker->Fill((Double_t) ELossSecondaryTracker);
    hELossPrimaryCalo->Fill((Double_t) ELossPrimaryCalo);
    hELossSecondaryCalo->Fill((Double_t) ELossSecondaryCalo);
    hELossTracker->Fill((Double_t) ELossTracker);
    hELossCalo->Fill((Double_t) ELossCalo);

    fprintf(frout_tracks,"\n");

    fprintf(frout_tracks,"----------------------------------\n");
    fprintf(frout_tracks,"          ELossPrimaryEvent %f MeV\n", ELossPrimaryEvent);
    fprintf(frout_tracks,"          ELossSecondaryEvent %f MeV\n", ELossSecondaryEvent);
    fprintf(frout_tracks,"\n");
    fprintf(frout_tracks,"          ELossEvent %f MeV\n", ELossEvent);
    fprintf(frout_tracks,"----------------------------------\n");
    fprintf(frout_tracks,"\n");
    hELossPrimaryEvent->Fill((Double_t) ELossPrimaryEvent);
    hELossSecondaryEvent->Fill((Double_t) ELossSecondaryEvent);
    hELossEvent->Fill((Double_t) ELossEvent);
  }
  fclose(frout_charge);
  fclose(frout_tracks);
}


void Analyzer::InitHisto_P_ELoss_Trk()
{
  /*
  This function creates histograms for the energy lost only in the tracker, both in layer 1 and layer 2.
  The MeV data are then converted in ADC counts for each available channel (N.B. the conversion is based on the analysis on real data done by Francesco)
  and a rough estimation of the noise due to electronics is added. 
  The values for BetaGamma of the incoming particles, based on the kinetic energy at the entrance in each layer, are recorded
  */

  hELossTrackerLayer1 = new TH1D("hELossTrackerLayer1","",250,-0.5,2.0);
  hELossTrackerLayer2 = new TH1D("hELossTrackerLayer2","",250,-0.5,2.0);
  hBetaGammaLayer1 = new TH1D("hBetaGammaLayer1","",1200,-0.1,1.1);   // for protons; for muons put (-0.5,4.5)
  hBetaGammaLayer2 = new TH1D("hBetaGammaLayer2","",1200,-0.1,1.1);

  hADCLayer1_pch = new TH1D("hADCLayer1_pch","",800,-5.0,795);
  hADCLayer2_pch = new TH1D("hADCLayer2_pch","",800,-5.0,795);
  hADCLayer1_nch = new TH1D("hADCLayer1_nch","",800,-5.0,795);
  hADCLayer2_nch = new TH1D("hADCLayer2_nch","",800,-5.0,795);

  hADCLayer1_pch_noise = new TH1D("hADCLayer1_pch_noise","",800,-5.0,795);
  hADCLayer2_pch_noise = new TH1D("hADCLayer2_pch_noise","",800,-5.0,795);
  hADCLayer1_nch_noise = new TH1D("hADCLayer1_nch_noise","",800,-5.0,795);
  hADCLayer2_nch_noise = new TH1D("hADCLayer2_nch_noise","",800,-5.0,795);

}

void Analyzer::CloseHisto_P_ELoss_Trk()
{
  /*
  This function writes the histograms for the energy lost only in the tracker, both in layer 1 and 2, and for the BetaGamma of the incoming particles
  in a Root file that will be read in "aiglon_g4_hist.C"
  */

  char chist[180];
  sprintf(chist,"hepd_qmd_%dMeV_proton_3C0_ELoss_Trk_hist.root",(int)BeamEnergy);	// change the name in relation to the hadronic model used and the geometry of the beam!
  TFile *fhist = new TFile(chist,"RECREATE");
  
  hELossTrackerLayer1->Write();
  hELossTrackerLayer2->Write();
  hBetaGammaLayer1->Write();
  hBetaGammaLayer2->Write();

  hADCLayer1_pch->Write();
  hADCLayer2_pch->Write();
  hADCLayer1_nch->Write();
  hADCLayer2_nch->Write();

  hADCLayer1_pch_noise->Write();
  hADCLayer2_pch_noise->Write();
  hADCLayer1_nch_noise->Write();
  hADCLayer2_nch_noise->Write();

  fhist->Close(); 

}

void Analyzer::Loop_P_ELoss_Trk()
{
  /*
  This function allows to record the energy lost by the incoming particles only in the tracker (both layer 1 and 2).
  The MeV data are then converted in ADC counts for each available channel (N.B. the conversion is based on the analysis on real data done by Francesco)
  and a rough estimation of the noise due to electronics is added.  
  The particle informations, including the energy lost at each layer, are recorded in the frout_tracks file
  and in the frout_charge file.
  The outcomes of this function are necessary to build the Bethe-Bloch fit function for different BeamEnergy,
  defined in "aiglon_g4_Bethe.C"
  */

  char cfich[180];
  sprintf(cfich,"frout_charge_qmd_%dMeV_proton_3C0_ELoss_Trk.txt",(int)BeamEnergy);
  FILE *frout_charge = fopen(cfich,"w");

  sprintf(cfich,"frout_tracks_qmd_%dMeV_proton_3C0_ELoss_Trk.txt",(int)BeamEnergy);
  FILE *frout_tracks = fopen(cfich,"w");

  int bufsize = 80000000;
    
  Long64_t nentries = fTreeChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  Double_t maxLayer = 0;

  gRandom = new TRandom3(); // required to add a rough estimation of the electronic noise to the energy loss data

  printf("Beam Energy = %3.2f MeV\n", BeamEnergy);
  fprintf(frout_tracks,"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
  fprintf(frout_tracks,"Beam Energy = %3.2f MeV\n", BeamEnergy);
  fprintf(frout_tracks,"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");

  // The relation between the BetaGamma and the BeamEnergy is contained in: 
  // https://agenda.infn.it/getFile.py/access?contribId=3&resId=0&materialId=slides&confId=12884 
  double BetaGamma_nominal = TMath::Sqrt(BeamEnergy/mProton * (BeamEnergy/mProton + 2));   // nominal kinetic energy == BeamEnergy
  fprintf(frout_tracks,"BetaGamma nominal = %f\n", BetaGamma_nominal);
  fprintf(frout_tracks,"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
  fprintf(frout_tracks,"\n");

  printf("nentries %lld\n",nentries);
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
 
    bool s1Hit=false;
    bool s1Hit_centre=false;
    bool s2Hit=false;
    bool s3Hit=false;
    bool s4Hit=false;
    bool s5Hit=false;
    bool s6Hit=false;
    bool s7Hit=false;
    bool s8Hit=false;
    bool s9Hit=false;
    bool s10Hit=false;
    bool s11Hit=false;
    bool s12Hit=false;
    bool s13Hit=false;
    bool s14Hit=false;
    bool s15Hit=false;
    bool s16Hit=false;
    bool s17Hit=false;
    bool LYSOHit=false;
    bool noVetoHit=false;
    bool noVeto_lateral=true;
    bool noVeto_bottom=true;
    bool hitOnTkLayer1=false;
    bool hitOnTkLayer2=false;
    bool goodHit=false;
    maxLayer=0;
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fTreeChain->GetEntry(jentry);
    nbytes += nb;
    int eventid = Event->EventID();
    std::vector<RootTrack> myTracks = Event->GetTracks();
    std::vector<RootCaloHit> myCaloHit = Event->GetCaloHit();
    std::vector<RootCaloHit> myVetoHit = Event->GetVetoHit();
    std::vector<RootTrackerHit> myTrackerHit = Event->GetTrackerHit();

    int ntrace = (int) myTracks.size();
    int nCaloHit = (int) myCaloHit.size();
    int nTrackerHit = (int) myTrackerHit.size();
    int nVetoHit = (int) myVetoHit.size();

    printf("===> EventId %d ntrace %d nCaloHit %d nTrackerHit %d nVetoHit %d\n",eventid, ntrace, nCaloHit, nTrackerHit, nVetoHit);
    fprintf(frout_tracks,"===> EventId %d ntrace %d\n", eventid, ntrace);

    float ELossTrackerLayer1 = 0;
    float BetaGammaLayer1 = 0;
    float ELossTrackerLayer2 = 0;
    float BetaGammaLayer2 = 0;

    double ADCLayer1_pch = 0;
    double ADCLayer2_pch = 0;
    double ADCLayer1_nch = 0;
    double ADCLayer2_nch = 0;

    double ADCLayer1_pch_noise = 0;
    double ADCLayer2_pch_noise = 0;
    double ADCLayer1_nch_noise = 0;
    double ADCLayer2_nch_noise = 0;

    for(int tr = 0; tr < ntrace; tr++){
      int trId = myTracks[tr].GetTrackID();
      int pdg = myTracks[tr].GetPDG();
      TString pname = myTracks[tr].GetParticleName();
      TString vvname = myTracks[tr].GetVertexVolumeName();
      TString cpname = myTracks[tr].GetCreatorProcessName();
      int parentId = myTracks[tr].GetParentTrackID();
      float ke = myTracks[tr].GetKinEnergy();
      const char* cpart = pname;
      printf("cpart %s\n",cpart);
      Particle(cpart, frout_charge);  

      if (trId == 1) {
        fprintf(frout_tracks,"trId %d :  PDG %d   pname %s\n", trId, pdg, pname.Data());
        fprintf(frout_tracks,"          A %2.0lf   Z %2.0lf\n", A, pcharge);
        fprintf(frout_tracks,"          Mass %lf MeV\n", Mass);
        fprintf(frout_tracks,"          ke %f MeV\n", ke);
      }
      else {
        fprintf(frout_tracks,"trId %d :  PDG %d   pname %s\n", trId, pdg, pname.Data());
        fprintf(frout_tracks,"          A %2.0lf   Z %2.0lf\n", A, pcharge);
        fprintf(frout_tracks,"          Mass %lf MeV\n", Mass);
        fprintf(frout_tracks,"          ke %f MeV\n", ke); 
        fprintf(frout_tracks,"          vvol %s   cpname %s\n", vvname.Data(), cpname.Data());
        fprintf(frout_tracks,"          parentId %d\n", parentId);
      }
      fprintf(frout_tracks,"          ------------------------\n");
      // Tracker
      if (nTrackerHit > 0) 
        for (int th = 0; th < nTrackerHit; th++) {
          int detId = myTrackerHit[th].GetDetectorId();
          int tkId = myTrackerHit[th].GetTrackId();
          TVector3 EntryPoint = myTrackerHit[th].GetEntryPoint();
          TVector3 ExitPoint = myTrackerHit[th].GetExitPoint();
          float layerWidth = EntryPoint.Z() - ExitPoint.Z();  // in order to get the width of each layer used in the simulation
          float Ekin = myTrackerHit[th].GetKinEnergy();
          float BetaGamma = TMath::Sqrt(Ekin/Mass * (Ekin/Mass + 2));
          if (trId == tkId) {
            fprintf(frout_tracks,"          tracker : Ekin %f MeV   BetaGamma %f\n", Ekin, BetaGamma);
            fprintf(frout_tracks,"                    EntryPoint (%.2f, %.2f, %.2f) mm   ExitPoint (%.2f, %.2f, %.2f) mm   layerWidth %.2f mm\n", EntryPoint.X(), EntryPoint.Y(), EntryPoint.Z(), ExitPoint.X(), ExitPoint.Y(), ExitPoint.Z(), layerWidth);
            fprintf(frout_tracks,"                    ELoss %f MeV\n", myTrackerHit[th].GetELoss());
            fprintf(frout_tracks,"                    detId %d", detId);
            if (detId > 2200) {
              hitOnTkLayer1 = true;
              if (trId == 1) {  // consider only primary protons since the particles detected by the tracker are only them
                ELossTrackerLayer1 += myTrackerHit[th].GetELoss();
                BetaGammaLayer1 += BetaGamma;
              }
              fprintf(frout_tracks," ---> hit layer 1\n");
            }
            else if (detId > 2100 && detId < 2200) {  // instead of only detId>2100
              hitOnTkLayer2 = true;
              if (trId == 1) {
                ELossTrackerLayer2 += myTrackerHit[th].GetELoss();
                BetaGammaLayer2 += BetaGamma;
              }
              fprintf(frout_tracks," ---> hit layer 2\n");
            }
            hitOnTkLayer1 = false;
            hitOnTkLayer2 = false;
          }
        }
        else
          fprintf(frout_tracks,"          tracker : NULL\n");
    }
    // MeV to ADC conversion
    ADCLayer1_pch = p2ch_ADCconv * ELossTrackerLayer1;
    ADCLayer2_pch = p3ch_ADCconv * ELossTrackerLayer2;
    ADCLayer1_nch = n2ch_ADCconv * ELossTrackerLayer1;
    ADCLayer2_nch = n3ch_ADCconv * ELossTrackerLayer2;

    // Addition of the electronic noise (approximation): use of a normalized gaussian with zero mean and std as the average sigmaNoise
    ADCLayer1_pch_noise = ADCLayer1_pch + gRandom->Gaus(0,sigmaNoise);
    ADCLayer2_pch_noise = ADCLayer2_pch + gRandom->Gaus(0,sigmaNoise);
    ADCLayer1_nch_noise = ADCLayer1_nch + gRandom->Gaus(0,sigmaNoise);
    ADCLayer2_nch_noise = ADCLayer2_nch + gRandom->Gaus(0,sigmaNoise);

    fprintf(frout_tracks,"\n");
    fprintf(frout_tracks,"----------------------------------\n");
    fprintf(frout_tracks,"          ELossTrackerLayer1 (primary proton) %f MeV\n", ELossTrackerLayer1);
    fprintf(frout_tracks,"          BetaGammaLayer1 (primary proton) %f\n", BetaGammaLayer1);
    fprintf(frout_tracks,"          ELossTrackerLayer2 (primary proton) %f MeV\n", ELossTrackerLayer2);
    fprintf(frout_tracks,"          BetaGammaLayer2 (primary proton) %f\n", BetaGammaLayer2);
    fprintf(frout_tracks,"\n");
    fprintf(frout_tracks,"          ADCLayer1_pch (primary proton) %f ADC\n", ADCLayer1_pch);
    fprintf(frout_tracks,"          ADCLayer2_pch (primary proton) %f ADC\n", ADCLayer2_pch);
    fprintf(frout_tracks,"          ADCLayer1_nch (primary proton) %f ADC\n", ADCLayer1_nch);
    fprintf(frout_tracks,"          ADCLayer2_nch (primary proton) %f ADC\n", ADCLayer2_nch);
    fprintf(frout_tracks,"\n");
    fprintf(frout_tracks,"          ADCLayer1_pch_noise (primary proton) %f ADC\n", ADCLayer1_pch_noise);
    fprintf(frout_tracks,"          ADCLayer2_pch_noise (primary proton) %f ADC\n", ADCLayer2_pch_noise);
    fprintf(frout_tracks,"          ADCLayer1_nch_noise (primary proton) %f ADC\n", ADCLayer1_nch_noise);
    fprintf(frout_tracks,"          ADCLayer2_nch_noise (primary proton) %f ADC\n", ADCLayer2_nch_noise);
    fprintf(frout_tracks,"----------------------------------\n");
    fprintf(frout_tracks,"\n");

    hELossTrackerLayer1->Fill((Double_t) ELossTrackerLayer1);
    hELossTrackerLayer2->Fill((Double_t) ELossTrackerLayer2);
    hBetaGammaLayer1->Fill((Double_t) BetaGammaLayer1);
    hBetaGammaLayer2->Fill((Double_t) BetaGammaLayer2);

    hADCLayer1_pch->Fill((Double_t) ADCLayer1_pch);
    hADCLayer2_pch->Fill((Double_t) ADCLayer2_pch);
    hADCLayer1_nch->Fill((Double_t) ADCLayer1_nch);
    hADCLayer2_nch->Fill((Double_t) ADCLayer2_nch);

    hADCLayer1_pch_noise->Fill((Double_t) ADCLayer1_pch_noise);
    hADCLayer2_pch_noise->Fill((Double_t) ADCLayer2_pch_noise);
    hADCLayer1_nch_noise->Fill((Double_t) ADCLayer1_nch_noise);
    hADCLayer2_nch_noise->Fill((Double_t) ADCLayer2_nch_noise);
  }

  fclose(frout_charge);
  fclose(frout_tracks);

  printf("Beam Energy = %3.2f MeV\n", BeamEnergy);
  printf("BetaGamma nominal = %f\n", BetaGamma_nominal);
}


// ----------------------------------------
// The rest of the code is done by William!
 
void Analyzer::InitHisto_Muon_acc()
{

  TString cfichhist = Form("HistMuon_acct.root");
  fichhist = new TFile(cfichhist,"recreate");

  char ctext[50];

  int nebin = 16;
  Double_t ebin[16] = {2, 3, 5, 10, 15, 25, 35, 45, 55, 75, 100, 125, 150, 200, 6.7, 8.4};
  Double_t epsilon = 0.001;

  float rmax = (Xgen/2)*(Xgen/2) + (Ygen/2)*(Ygen/2);
  rmax = TMath::Sqrt(rmax);
  sprintf(ctext,"xy_gen");
  xy_gen = new TH2D(ctext,"xy gen",100,-30,30,100,-30,30);
  sprintf(ctext,"ke_gen");
  ke_gen = new TH1D(ctext,"ke gen",10000,0.05,100);
  sprintf(ctext,"costhe_gen");
  costhe_gen = new TH1D(ctext,"costhe gen",100,-1,1);
  sprintf(ctext,"hrt_gen");
  rtheta_gen = new TH2D(ctext,"rtheta gen",50,0,rmax,50,0,90);
  sprintf(ctext,"hrt_acc");
  rtheta_acc = new TH2D(ctext,"rtheta acc",50,0,rmax,50,0,90);
  sprintf(ctext,"hrt_acc_sv");
  rtheta_acc_sv = new TH2D(ctext,"rtheta acc sans veto",50,0,rmax,50,0,90);
  sprintf(ctext,"hrt_acc_svl");
  rtheta_acc_svl = new TH2D(ctext,"rtheta acc sans veto lat",50,0,rmax,50,0,90);

}

void Analyzer::CloseHisto_Muon_acc() 
{

  fichhist->ls();
  fichhist->cd();
   //  hf->Write();
  int nx = rtheta_gen->GetNbinsX();
  int ny = rtheta_gen->GetNbinsY();
  float ngen = rtheta_gen->GetEntries();
  printf("ngen %f\n",ngen);
  int bin;
 
  float val_acc;
  for (int ix=1; ix<=nx; ix++)
    for (int iy=1; iy<=ny; iy++) {
      bin = rtheta_gen->GetBin(ix,iy);
      val_acc = rtheta_acc->GetBinContent(bin);
      val_acc*=(Xgen*Ygen*sigma/ngen);
      //      printf("bin %d, val_acc %f\n",bin,val_acc);
      rtheta_acc->SetBinContent(bin,val_acc);
      val_acc = rtheta_acc_sv->GetBinContent(bin);
      val_acc*=(Xgen*Ygen*sigma/ngen);
      //      printf("bin %d, val_acc_sv %f\n",bin,val_acc);
      rtheta_acc_sv->SetBinContent(bin,val_acc);
      val_acc = rtheta_acc_svl->GetBinContent(bin);
      val_acc*=(Xgen*Ygen*sigma/ngen);
      //      printf("bin %d, val_acc_sv %f\n",bin,val_acc);
      rtheta_acc_svl->SetBinContent(bin,val_acc);
    };

  rtheta_gen->Write();
  rtheta_acc->Write();
  rtheta_acc_sv->Write();
  rtheta_acc_svl->Write();
  xy_gen->Write();
  ke_gen->Write();
  costhe_gen->Write();
        
  return;

}

void Analyzer::Loop_muon()
{

  //  double Energy = E;
  int bufsize = 80000000;
    
  printf("get entries\n");
  Long64_t nentries = fTreeChain->GetEntries();
  std::cout<<" nentries = " << nentries << std::endl;
  Long64_t nbytes = 0, nb = 0;
  Double_t maxLayer = 0;

  //  for (Long64_t jentry=0; jentry<10000;jentry++) {
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
 
    bool s1Hit=false;
    bool s1Hit_centre=false;
    bool s2Hit=false;
    bool s3Hit=false;
    bool s4Hit=false;
    bool s17Hit=false;
    bool LYSOHit=false;
    bool noVetoHit=false;
    bool noVeto_lateral=true;
    bool noVeto_bottom=true;
    bool hitOnTkLayer1=false;
    bool hitOnTkLayer2=false;
    bool Trig=false;
    bool Trig_sv=false;
    maxLayer=0;
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fTreeChain->GetEntry(jentry);   nbytes += nb;
    //    int eventid = Event->EventID();
    std::cout<<"EventId " << Event->EventID() << std::endl;
    std::vector<RootTrack> myTracks = Event->GetTracks();
    std::vector<RootCaloHit> myCaloHit = Event->GetCaloHit();
    std::vector<RootCaloHit> myVetoHit = Event->GetVetoHit();
    std::vector<RootTrackerHit> myTrackerHit = Event->GetTrackerHit();

    TVector3 muonDir = myTracks[0].GetDirection();
    TVector3 position = myTracks[0].GetPosition();
    Double_t costhe = muonDir.CosTheta();
    costhe_gen->Fill(costhe);
    Double_t xgen = position.X()/10; 
    Double_t ygen = position.Y()/10;
    xy_gen->Fill(xgen,ygen);
    Double_t kegen = myTracks[0].GetKinEnergy();
    kegen /= 1000;
    ke_gen->Fill(kegen); 
    float theta = muonDir.Theta()*180/TMath::Pi();
    float rgen = position.X()*position.X() + position.Y()*position.Y();
    if (rgen > 0) rgen=TMath::Sqrt(rgen);
    rgen/=10;
    //    printf("rgen %f\n",rgen);
    if(theta>90)
      theta=180-theta;
    rtheta_gen->Fill(rgen,theta);

    if(myCaloHit.size()>0){
      for(size_t sh=0;sh<myCaloHit.size();sh++){
  		int scintLayer = myCaloHit[sh].GetVolume();
  		//        printf("scintlayer %d\n",scintLayer);
  		std::vector<int> plist = myCaloHit[sh].GetParticleList();       
  		//f or(size_t p=0;p<plist.size();p++)
  		//    printf("pid %d PDG %d\n",plist[p],myTracks[plist[p]].GetPDG());
  		//  if(scintLayer.Contains("S1"))
  		//    if (myCaloHit.size()>=2 && ce <= 15)
        //  printf("scintLayer %d\n",scintLayer);
        if (plist[0]==1) {
        	if (scintLayer >= 1300) s1Hit=true;
        	if (scintLayer == 1313 || scintLayer == 1321) s1Hit_centre=true;
        	if (scintLayer == 1216) s2Hit=true;
        	if (scintLayer == 1215) s3Hit=true;
        	if (scintLayer == 1214) s4Hit=true;
        	if (scintLayer == 1201) s17Hit=true;
        	if (scintLayer >= 1111 && scintLayer <= 1133) LYSOHit=true;
        }
      }
      //      printf("\n");
      // t3
      //            if(s1Hit && (s2Hit || s3Hit) && s17Hit) {
      // t4
      //      if(s1Hit && (s2Hit || s3Hit)) {
      // t5
      //        if(s1Hit_centre && s2Hit) {
      // t6
      if(s1Hit && (s2Hit || s3Hit) && LYSOHit) {
  		if(myVetoHit.size()==0) noVetoHit=true;
        if(myVetoHit.size()>0) {
    		for(size_t vh=0;vh<myVetoHit.size();vh++){
      			int scintLayer = myVetoHit[vh].GetVolume();
            	if (scintLayer >= 1411 && scintLayer <= 1422) noVeto_lateral=false;
            	if (scintLayer == 1430) noVeto_bottom=false;
          	}
        }
  		if(myTrackerHit.size()>0){
    		for(size_t th=0;th<myTrackerHit.size();th++){
      			int detId = myTrackerHit[th].GetDetectorId();
            	int tkid = myTrackerHit[th].GetTrackId();
      			if(detId>2200 && tkid==1) {
        			hitOnTkLayer1=true;
        			//              printf("hit layer 1\n",th);
            	}
      			else if(detId>2100 && tkid==1) {
        			hitOnTkLayer2=true;
        			//              printf("hit layer 2\n",th);
            	}
    		}
    		if(hitOnTkLayer2&&hitOnTkLayer1) {
      			if (noVetoHit) Trig_sv=true;
            	else Trig=true;
          	}
  		}
      }
    }
    if (Trig) rtheta_acc->Fill(rgen,theta);       
    if (Trig_sv) rtheta_acc_sv->Fill(rgen,theta);
    if (Trig && noVeto_lateral && !(noVeto_bottom)) rtheta_acc_svl->Fill(rgen,theta);
  }
}


void Analyzer::InitHisto_e_acc(float energie){

  TString cfichhist = Form("HistElectron_acct6_%3.1f_MeV.root",energie);
  fichhist = new TFile(cfichhist,"recreate");

  Double_t theERange = energie;
  printf("energie %3.1f \n",energie);
  char chistg[10];
  char chista[10];
  char chistasv[10];
  char chistasvl[10];

  int nebin = 16;
  Double_t ebin[16] = { 2, 3, 5, 10, 15, 25, 35, 45, 55, 75, 100, 125, 150, 200, 6.7, 8.4};
  Double_t epsilon = 0.001;

  for (int i=0; i<nebin; i++) {
    if(TMath::AreEqualRel(theERange,ebin[i],epsilon)) {
      sprintf(chistg,"h%d",1000+i);
      sprintf(chista,"h%d",1100+i);
      sprintf(chistasv,"h%d",1200+i);
      sprintf(chistasvl,"h%d",1300+i);
    }
  }
  /*
  if(theERange == 2.0){
    sprintf(chistg,"h1000");
    sprintf(chista,"h1100");
    sprintf(chistasv,"h1200");
  }else if(theERange == 3){
    sprintf(chistg,"h1001");
    sprintf(chista,"h1101");
    sprintf(chistasv,"h1201");
  }else if(theERange == 5){
    sprintf(chistg,"h1002");
    sprintf(chista,"h1102");
    sprintf(chistasv,"h1202");
  }else if(theERange == 10){
    sprintf(chistg,"h1003");
    sprintf(chista,"h1103");
    sprintf(chistasv,"h1203");
  }else if(theERange == 15){
    sprintf(chistg,"h1004");
    sprintf(chista,"h1104");
    sprintf(chistasv,"h1204");
  }else if(theERange == 25){
    sprintf(chistg,"h1005");
    sprintf(chista,"h1105");
    sprintf(chistasv,"h1205");
  }else if(theERange == 35){
    sprintf(chistg,"h1006");
    sprintf(chista,"h1106");
    sprintf(chistasv,"h1206");
  }else if(theERange == 45){
    sprintf(chistg,"h1007");
    sprintf(chista,"h1107");
    sprintf(chistasv,"h1207");
  }else if(theERange == 55){
    sprintf(chistg,"h1008");
    sprintf(chista,"h1108");
    sprintf(chistasv,"h1208");
  }else if(theERange == 75){
    sprintf(chistg,"h1009");
    sprintf(chista,"h1109");
    sprintf(chistasv,"h1209");
  }else if(theERange == 100){
    sprintf(chistg,"h1010");
    sprintf(chista,"h1110");
    sprintf(chistasv,"h1210");
  }else if(theERange == 125){
    sprintf(chistg,"h1011");
    sprintf(chista,"h1111");
    sprintf(chistasv,"h1211");
  }else if(theERange == 150){
    sprintf(chistg,"h1012");
    sprintf(chista,"h1112");
    sprintf(chistasv,"h1212");
  }else if(theERange == 200){
    sprintf(chistg,"h1013");
    sprintf(chista,"h1113");
    sprintf(chistasv,"h1213");
  }else if(AreEqualAbs(theERange,6.7,0.1)){
    sprintf(chistg,"h1014");
    sprintf(chista,"h1114");
    sprintf(chistasv,"h1214");
    } */
  float rmax = (Xgen/2)*(Xgen/2) + (Ygen/2)*(Ygen/2);
  rmax = TMath::Sqrt(rmax);
  printf("chistg %s\n",chistg);
  printf("chista %s\n",chista);
  printf("chistasv %s\n",chistasv);
  printf("chistasvl %s\n",chistasvl);
  rtheta_gen = new TH2D(chistg,"rtheta gen",50,0,rmax,50,0,90);
  rtheta_acc = new TH2D(chista,"rtheta acc",50,0,rmax,50,0,90);
  rtheta_acc_sv = new TH2D(chistasv,"rtheta acc sans veto",50,0,rmax,50,0,90);
  rtheta_acc_svl = new TH2D(chistasvl,"rtheta acc sans veto lat",50,0,rmax,50,0,90);

}

 
void Analyzer::InitHisto_p_acc(float energie)
{

  TString cfichhist = Form("HistProton_acct6_%3.1f_MeV.root",energie);
  fichhist = new TFile(cfichhist,"recreate");

  float theERange = energie;
  printf("energie %3.1f \n",energie);
  char chistg[10];
  char chista[10];
  char chistasv[10];
  char chistasvl[10];
  if(theERange == 30){
    sprintf(chistg,"h1000");
    sprintf(chista,"h1100");
    sprintf(chistasv,"h1200");
    sprintf(chistasvl,"h1300");
  }else if(theERange == 35){
    sprintf(chistg,"h1001");
    sprintf(chista,"h1101");
    sprintf(chistasv,"h1201");
    sprintf(chistasvl,"h1301");
  }else if(theERange == 55){
    sprintf(chistg,"h1002");
    sprintf(chista,"h1102");
    sprintf(chistasv,"h1202");
    sprintf(chistasvl,"h1302");
  }else if(theERange == 75){
    sprintf(chistg,"h1003");
    sprintf(chista,"h1103");
    sprintf(chistasv,"h1203");
    sprintf(chistasvl,"h1303");
  }else if(theERange == 100){
    sprintf(chistg,"h1004");
    sprintf(chista,"h1104");
    sprintf(chistasv,"h1204");
    sprintf(chistasvl,"h1304");
  }else if(theERange == 125){
    sprintf(chistg,"h1005");
    sprintf(chista,"h1105");
    sprintf(chistasv,"h1205");
    sprintf(chistasvl,"h1305");
  }else if(theERange == 150){
    sprintf(chistg,"h1006");
    sprintf(chista,"h1106");
    sprintf(chistasv,"h1206");
    sprintf(chistasvl,"h1306");
  }else if(theERange == 200){
    sprintf(chistg,"h1007");
    sprintf(chista,"h1107");
    sprintf(chistasv,"h1207");
    sprintf(chistasvl,"h1307");
  }else if(theERange == 225){
    sprintf(chistg,"h1008");
    sprintf(chista,"h1108");
    sprintf(chistasv,"h1208");
    sprintf(chistasvl,"h1308");
  }else if(theERange == 250){
    sprintf(chistg,"h1009");
    sprintf(chista,"h1109");
    sprintf(chistasv,"h1209");
    sprintf(chistasvl,"h1309");
  }else if(theERange == 300){
    sprintf(chistg,"h1010");
    sprintf(chista,"h1110");
    sprintf(chistasv,"h1210");
    sprintf(chistasvl,"h1310");
  }else if(theERange == 45){
    sprintf(chistg,"h1011");
    sprintf(chista,"h1111");
    sprintf(chistasv,"h1211");
    sprintf(chistasvl,"h1311");
  }else if(theERange == 65){
    sprintf(chistg,"h1012");
    sprintf(chista,"h1112");
    sprintf(chistasv,"h1212");
    sprintf(chistasvl,"h1312");
  }else if(theERange == 450){
    sprintf(chistg,"h1013");
    sprintf(chista,"h1113");
    sprintf(chistasv,"h1213");
    sprintf(chistasvl,"h1313");
  }else if(theERange == 500){
    sprintf(chistg,"h1014");
    sprintf(chista,"h1114");
    sprintf(chistasv,"h1214");
    sprintf(chistasvl,"h1314");
  }
  float rmax = (Xgen/2)*(Xgen/2) + (Ygen/2)*(Ygen/2);
  rmax = TMath::Sqrt(rmax);
  printf("chistg %s\n",chistg);
  printf("chista %s\n",chista);
  printf("chistasv %s\n",chistasv);
  printf("chistasvl %s\n",chistasvl);
  rtheta_gen = new TH2D(chistg,"rtheta gen",50,0,rmax,50,0,90);
  rtheta_acc = new TH2D(chista,"rtheta acc",50,0,rmax,50,0,90);
  rtheta_acc_sv = new TH2D(chistasv,"rtheta acc sans veto",50,0,rmax,50,0,90);
  rtheta_acc_svl = new TH2D(chistasvl,"rtheta acc sans veto lat",50,0,rmax,50,0,90);

}

void Analyzer::CloseHisto_acc() 
{

  fichhist->ls();
  fichhist->cd();
   //  hf->Write();
  int nx = rtheta_gen->GetNbinsX();
  int ny = rtheta_gen->GetNbinsY();
  float ngen = rtheta_gen->GetEntries();
  printf("ngen %f\n",ngen);
  int bin;
 
  float val_acc;
  for (int ix=1; ix<=nx; ix++)
    for (int iy=1; iy<=ny; iy++) {
      bin = rtheta_gen->GetBin(ix,iy);
      val_acc = rtheta_acc->GetBinContent(bin);
      val_acc*=(Xgen*Ygen*sigma/ngen);
      //      printf("bin %d, val_acc %f\n",bin,val_acc);
      rtheta_acc->SetBinContent(bin,val_acc);
      val_acc = rtheta_acc_sv->GetBinContent(bin);
      val_acc*=(Xgen*Ygen*sigma/ngen);
      //      printf("bin %d, val_acc_sv %f\n",bin,val_acc);
      rtheta_acc_sv->SetBinContent(bin,val_acc);
      val_acc = rtheta_acc_svl->GetBinContent(bin);
      val_acc*=(Xgen*Ygen*sigma/ngen);
      //      printf("bin %d, val_acc_sv %f\n",bin,val_acc);
      rtheta_acc_svl->SetBinContent(bin,val_acc);
    };

  rtheta_gen->Write();
  rtheta_acc->Write();
  rtheta_acc_sv->Write();
  rtheta_acc_svl->Write();
        
  return;

}


void Analyzer::Loop_acc()
{

  //  double Energy = E;
  int bufsize = 80000000;
    
  printf("get entries\n");
  Long64_t nentries = fTreeChain->GetEntriesFast();
  printf("get entries %lld\n",nentries);
  Long64_t nbytes = 0, nb = 0;
  Double_t maxLayer = 0;

  //  for (Long64_t jentry=0; jentry<10000;jentry++) {
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
 
    bool s1Hit=false;
    bool s1Hit_centre=false;
    bool s2Hit=false;
    bool s3Hit=false;
    bool s4Hit=false;
    bool s17Hit=false;
    bool LYSOHit=false;
    bool noVetoHit=false;
    bool noVeto_lateral=true;
    bool noVeto_bottom=true;
    bool hitOnTkLayer1=false;
    bool hitOnTkLayer2=false;
    bool Trig=false;
    bool Trig_sv=false;
    maxLayer=0;
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fTreeChain->GetEntry(jentry);   nbytes += nb;
    std::vector<RootTrack> myTracks = Event->GetTracks();
    std::vector<RootCaloHit> myCaloHit = Event->GetCaloHit();
    std::vector<RootCaloHit> myVetoHit = Event->GetVetoHit();
    std::vector<RootTrackerHit> myTrackerHit = Event->GetTrackerHit();

    TVector3 electronDir = myTracks[0].GetDirection();
    TVector3 position = myTracks[0].GetPosition();
    float theta = electronDir.Theta()*180/TMath::Pi();
    float rgen = position.X()*position.X() + position.Y()*position.Y();
    if (rgen > 0) rgen=TMath::Sqrt(rgen);
    rgen/=10;
    //    printf("rgen %f\n",rgen);
    if(theta>90)
      theta=180-theta;
    rtheta_gen->Fill(rgen,theta);

    if(myCaloHit.size()>0){
      for(size_t sh=0;sh<myCaloHit.size();sh++){
  		int scintLayer = myCaloHit[sh].GetVolume();
  		//        printf("scintlayer %d\n",scintLayer);
  		std::vector<int> plist = myCaloHit[sh].GetParticleList();       
  		//f or(size_t p=0;p<plist.size();p++)
  		//    printf("pid %d PDG %d\n",plist[p],myTracks[plist[p]].GetPDG());
  		//  if(scintLayer.Contains("S1"))
  		//    if (myCaloHit.size()>=2 && ce <= 15)
        //  printf("scintLayer %d\n",scintLayer);
        if (plist[0]==1) {
  			if (scintLayer >= 1300) s1Hit=true;
        	if (scintLayer == 1313 || scintLayer == 1321) s1Hit_centre=true;
        	if (scintLayer == 1216) s2Hit=true;
        	if (scintLayer == 1215) s3Hit=true;
        	if (scintLayer == 1214) s4Hit=true;
        	if (scintLayer == 1201) s17Hit=true;
        	if (scintLayer >= 1111 && scintLayer <= 1133) LYSOHit=true;
        }
      }
      //      printf("\n");
      // t3
      //            if(s1Hit && (s2Hit || s3Hit) && s17Hit) {
      // t4
      //      if(s1Hit && (s2Hit || s3Hit)) {
      // t5
      //        if(s1Hit_centre && s2Hit) {
      // t6
      if(s1Hit && (s2Hit || s3Hit) && LYSOHit) {
  		if(myVetoHit.size()==0) noVetoHit=true;
        if(myVetoHit.size()>0) {
    		for(size_t vh=0;vh<myVetoHit.size();vh++){
      			int scintLayer = myVetoHit[vh].GetVolume();
            	if (scintLayer >= 1411 && scintLayer <= 1422) noVeto_lateral=false;
            	if (scintLayer == 1430) noVeto_bottom=false;
          	}
        }
  		if(myTrackerHit.size()>0){
    		for(size_t th=0;th<myTrackerHit.size();th++){
      			int detId = myTrackerHit[th].GetDetectorId();
            	int tkid = myTrackerHit[th].GetTrackId();
      			if(detId>2200 && tkid==1) {
        			hitOnTkLayer1=true;
        			//              printf("hit layer 1\n",th);
            	}
      			else if(detId>2100 && tkid==1) {
        			hitOnTkLayer2=true;
        			//              printf("hit layer 2\n",th);
            	}
    		}
    		if(hitOnTkLayer2&&hitOnTkLayer1) {
      			if (noVetoHit) Trig_sv=true;
            	else Trig=true;
          	}
  		}
      }
    }
    if (Trig) rtheta_acc->Fill(rgen,theta);       
    if (Trig_sv) rtheta_acc_sv->Fill(rgen,theta);
    if (Trig && noVeto_lateral && !(noVeto_bottom)) rtheta_acc_svl->Fill(rgen,theta);
  }
}



void Analyzer::InitHisto_P()
{

  Pcinetique_gen = new TH1F("hecgen","Distribution Ek generated",5000,5,500);
  Pcinetique_gen->GetXaxis()->SetTitle("Kineteic Energy (MeV)");
  Pcinetique_gen->GetXaxis()->CenterTitle(true);
  Pcinetique_gen->GetXaxis()->SetTitleSize(0.06);
  Pcinetique_gen->GetXaxis()->SetTitleOffset(1.10);
  Pcinetique_gen->GetXaxis()->SetLabelOffset(0.001);
  Pcinetique_gen->GetXaxis()->SetLabelSize(0.055);
  Pcinetique_gen->GetYaxis()->SetTitle("Entries");  
  Pcinetique_gen->GetYaxis()->CenterTitle(true);
  Pcinetique_gen->GetYaxis()->SetTitleSize(0.06);
  Pcinetique_gen->GetYaxis()->SetTitleOffset(1.10);
  Pcinetique_gen->GetYaxis()->SetLabelSize(0.055);
  Pcinetique_gen->SetLineColor(kRed);
  //  Pcinetique_gen->GetXaxis()->SetRangeUser(0.5,500.);

  Vertex_xy_gen = new TH2D("hvxgen","Distribution XY generated",250,-40,40,300,-50,50);
  Vertex_xy_gen->GetXaxis()->SetTitle("Vertex X Position (cm)");
  Vertex_xy_gen->GetXaxis()->CenterTitle(true);
  Vertex_xy_gen->GetXaxis()->SetTitleSize(0.06);
  Vertex_xy_gen->GetXaxis()->SetTitleOffset(1.10);
  Vertex_xy_gen->GetXaxis()->SetLabelOffset(0.001);
  Vertex_xy_gen->GetXaxis()->SetLabelSize(0.055);
  Vertex_xy_gen->GetYaxis()->SetTitle("Verteix Y Position (cm)");  
  Vertex_xy_gen->GetYaxis()->CenterTitle(true);
  Vertex_xy_gen->GetYaxis()->SetTitleSize(0.06);
  Vertex_xy_gen->GetYaxis()->SetTitleOffset(1.10);
  Vertex_xy_gen->GetYaxis()->SetLabelSize(0.055);
  Vertex_xy_gen->SetLineColor(kBlue);

  Vertex_xy_gen_ce = new TH2D("hvxgence","Distribution XY generated ce",250,-40,40,300,-50,50);
  Vertex_xy_gen_ce->GetXaxis()->SetTitle("Vertex X Position (cm)");
  Vertex_xy_gen_ce->GetXaxis()->CenterTitle(true);
  Vertex_xy_gen_ce->GetXaxis()->SetTitleSize(0.06);
  Vertex_xy_gen_ce->GetXaxis()->SetTitleOffset(1.10);
  Vertex_xy_gen_ce->GetXaxis()->SetLabelOffset(0.001);
  Vertex_xy_gen_ce->GetXaxis()->SetLabelSize(0.055);
  Vertex_xy_gen_ce->GetYaxis()->SetTitle("Verteix Y Position (cm)");  
  Vertex_xy_gen_ce->GetYaxis()->CenterTitle(true);
  Vertex_xy_gen_ce->GetYaxis()->SetTitleSize(0.06);
  Vertex_xy_gen_ce->GetYaxis()->SetTitleOffset(1.10);
  Vertex_xy_gen_ce->GetYaxis()->SetLabelSize(0.055);
  Vertex_xy_gen_ce->SetLineColor(kBlue);
 
  Pcinetique_acc_trk = new TH1F("hecacctrk","Distribution Ek accepteed trk",5000,5,500);
  Pcinetique_acc_trk->GetXaxis()->SetTitle("Kineteic Energy (MeV)");
  Pcinetique_acc_trk->GetXaxis()->CenterTitle(true);
  Pcinetique_acc_trk->GetXaxis()->SetTitleSize(0.06);
  Pcinetique_acc_trk->GetXaxis()->SetTitleOffset(1.10);
  Pcinetique_acc_trk->GetXaxis()->SetLabelSize(0.05);
  Pcinetique_acc_trk->GetXaxis()->SetLabelOffset(0.001);
  Pcinetique_acc_trk->GetYaxis()->SetTitle("Entries");  
  Pcinetique_acc_trk->GetYaxis()->CenterTitle(true);
  Pcinetique_acc_trk->GetYaxis()->SetTitleSize(0.06);
  Pcinetique_acc_trk->GetYaxis()->SetTitleOffset(1.10);
  Pcinetique_acc_trk->GetYaxis()->SetLabelSize(0.055);
  Pcinetique_acc_trk->SetLineColor(kRed);
  //  Pcinetique_acc_trk->GetXaxis()->SetRangeUser(0.5,500.);

  Vertex_xy_acc_trk = new TH2D("hvxacc_trk","Distribution XY trk acc",250,-40,40,300,-50,50);
  Vertex_xy_acc_trk->GetXaxis()->SetTitle("Vertex X Position (cm)");
  Vertex_xy_acc_trk->GetXaxis()->CenterTitle(true);
  Vertex_xy_acc_trk->GetXaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_trk->GetXaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_trk->GetXaxis()->SetLabelOffset(0.001);
  Vertex_xy_acc_trk->GetXaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_trk->GetYaxis()->SetTitle("Verteix Y Position (cm)");  
  Vertex_xy_acc_trk->GetYaxis()->CenterTitle(true);
  Vertex_xy_acc_trk->GetYaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_trk->GetYaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_trk->GetYaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_trk->SetLineColor(kBlue);

  Vertex_xy_acc_trk_ce = new TH2D("hvxacc_trkce","Distribution XY trk acc ce",250,-40,40,300,-50,50);
  Vertex_xy_acc_trk_ce->GetXaxis()->SetTitle("Vertex X Position (cm)");
  Vertex_xy_acc_trk_ce->GetXaxis()->CenterTitle(true);
  Vertex_xy_acc_trk_ce->GetXaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_trk_ce->GetXaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_trk_ce->GetXaxis()->SetLabelOffset(0.001);
  Vertex_xy_acc_trk_ce->GetXaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_trk_ce->GetYaxis()->SetTitle("Verteix Y Position (cm)");  
  Vertex_xy_acc_trk_ce->GetYaxis()->CenterTitle(true);
  Vertex_xy_acc_trk_ce->GetYaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_trk_ce->GetYaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_trk_ce->GetYaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_trk_ce->SetLineColor(kBlue);

  Pcinetique_acc_sci = new TH1F("hecaccaco","Distribution Ek accepteed sci",5000,5,500);
  Pcinetique_acc_sci->GetXaxis()->SetTitle("Kineteic Energy (MeV)");
  Pcinetique_acc_sci->GetXaxis()->CenterTitle(true);
  Pcinetique_acc_sci->GetXaxis()->SetTitleSize(0.06);
  Pcinetique_acc_sci->GetXaxis()->SetTitleOffset(1.10);
  Pcinetique_acc_sci->GetXaxis()->SetLabelSize(0.055);
  Pcinetique_acc_sci->GetXaxis()->SetLabelOffset(0.001);
  Pcinetique_acc_sci->GetYaxis()->SetTitle("Entries");  
  Pcinetique_acc_sci->GetYaxis()->CenterTitle(true);
  Pcinetique_acc_sci->GetYaxis()->SetTitleSize(0.06);
  Pcinetique_acc_sci->GetYaxis()->SetTitleOffset(1.10);
  Pcinetique_acc_sci->GetYaxis()->SetLabelSize(0.055);
  Pcinetique_acc_sci->SetLineColor(kRed);
  //  Pcinetique_acc_sci->GetXaxis()->SetRangeUser(0.5,500.);

  Vertex_xy_acc_sci = new TH2D("hvxacc_sci","Distribution XY sci acc",250,-40,40,300,-50,50);
  Vertex_xy_acc_sci->GetXaxis()->SetTitle("Vertex X Position (cm)");
  Vertex_xy_acc_sci->GetXaxis()->CenterTitle(true);
  Vertex_xy_acc_sci->GetXaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_sci->GetXaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_sci->GetXaxis()->SetLabelOffset(0.001);
  Vertex_xy_acc_sci->GetXaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_sci->GetYaxis()->SetTitle("Verteix Y Position (cm)");  
  Vertex_xy_acc_sci->GetYaxis()->CenterTitle(true);
  Vertex_xy_acc_sci->GetYaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_sci->GetYaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_sci->GetYaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_sci->SetLineColor(kBlue);

  Vertex_xy_acc_sci_ce = new TH2D("hvxacc_sci_ce","Distribution XY sci acc ce",250,-40,40,300,-50,50);
  Vertex_xy_acc_sci_ce->GetXaxis()->SetTitle("Vertex X Position (cm)");
  Vertex_xy_acc_sci_ce->GetXaxis()->CenterTitle(true);
  Vertex_xy_acc_sci_ce->GetXaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_sci_ce->GetXaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_sci_ce->GetXaxis()->SetLabelOffset(0.001);
  Vertex_xy_acc_sci_ce->GetXaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_sci_ce->GetYaxis()->SetTitle("Verteix Y Position (cm)");  
  Vertex_xy_acc_sci_ce->GetYaxis()->CenterTitle(true);
  Vertex_xy_acc_sci_ce->GetYaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_sci_ce->GetYaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_sci_ce->GetYaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_sci_ce->SetLineColor(kBlue);

  Pcinetique_acc_veto = new TH1F("hecaccvetp","Distribution Ek accepteed veto",5000,5,500);
  Pcinetique_acc_veto->GetXaxis()->SetTitle("Kinetic Energy (MeV)");
  Pcinetique_acc_veto->GetXaxis()->CenterTitle(true);
  Pcinetique_acc_veto->GetXaxis()->SetTitleSize(0.06);
  Pcinetique_acc_veto->GetXaxis()->SetTitleOffset(1.10);
  Pcinetique_acc_veto->GetXaxis()->SetLabelSize(0.055);
  Pcinetique_acc_veto->GetXaxis()->SetLabelOffset(0.001);
  Pcinetique_acc_veto->GetYaxis()->SetTitle("Entries");  
  Pcinetique_acc_veto->GetYaxis()->CenterTitle(true);
  Pcinetique_acc_veto->GetYaxis()->SetTitleSize(0.06);
  Pcinetique_acc_veto->GetYaxis()->SetTitleOffset(1.10);
  Pcinetique_acc_veto->GetYaxis()->SetLabelSize(0.055);
  Pcinetique_acc_veto->SetLineColor(kRed);
  //  Pcinetique_acc_veto->GetXaxis()->SetRangeUser(0.5,500.);

  Vertex_xy_acc_veto = new TH2D("hvxacc_veto","Distribution XY veto acc",250,-40,40,300,-50,50);
  Vertex_xy_acc_veto->GetXaxis()->SetTitle("Vertex X Position (cm)");
  Vertex_xy_acc_veto->GetXaxis()->CenterTitle(true);
  Vertex_xy_acc_veto->GetXaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_veto->GetXaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_veto->GetXaxis()->SetLabelOffset(0.001);
  Vertex_xy_acc_veto->GetXaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_veto->GetYaxis()->SetTitle("Verteix Y Position (cm)");  
  Vertex_xy_acc_veto->GetYaxis()->CenterTitle(true);
  Vertex_xy_acc_veto->GetYaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_veto->GetYaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_veto->GetYaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_veto->SetLineColor(kBlue);

  Vertex_xy_acc_veto_ce = new TH2D("hvxacc_veto_ce","Distribution XY sci acc ce",250,-40,40,300,-50,50);
  Vertex_xy_acc_veto_ce->GetXaxis()->SetTitle("Vertex X Position (cm)");
  Vertex_xy_acc_veto_ce->GetXaxis()->CenterTitle(true);
  Vertex_xy_acc_veto_ce->GetXaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_veto_ce->GetXaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_veto_ce->GetXaxis()->SetLabelOffset(0.001);
  Vertex_xy_acc_veto_ce->GetXaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_veto_ce->GetYaxis()->SetTitle("Verteix Y Position (cm)");  
  Vertex_xy_acc_veto_ce->GetYaxis()->CenterTitle(true);
  Vertex_xy_acc_veto_ce->GetYaxis()->SetTitleSize(0.06);
  Vertex_xy_acc_veto_ce->GetYaxis()->SetTitleOffset(1.10);
  Vertex_xy_acc_veto_ce->GetYaxis()->SetLabelSize(0.055);
  Vertex_xy_acc_veto_ce->SetLineColor(kBlue);

}

void Analyzer::CloseHisto_P()
{
  
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  gStyle->SetPalette(1,0);
  gStyle->SetFrameFillColor(kWhite);

  char cpad[80];
  TPad* ptpad;
  
  TCanvas* c1 = new TCanvas("c1","ec gen electron",0,0,500,500);
  c1->SetFillColor(0);
  c1->Divide(2,2,0.001,0.001);
  for (int i=0; i<4; i++) {
    sprintf(cpad,"c1_%d",i+1);
    ptpad = (TPad*) c1->FindObject(cpad);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(1);
    ptpad->SetLogx(1);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.125);
    ptpad->SetTopMargin(0.125);
    c1->cd(i+1);
    switch(i) {
    case 0: Pcinetique_gen->Draw(); break;
    case 1: Pcinetique_acc_trk->Draw(); break;
    case 2: Pcinetique_acc_sci->Draw(); break;
    case 3: Pcinetique_acc_veto->Draw(); break;
    default: break;
    }
  }
  
  TCanvas* c2 = new TCanvas("c2","vxy gen electron",0,0,500,500);
  c2->SetFillColor(0);
  c2->Divide(2,2,0.001,0.001);
  for (int i=0; i<4; i++) {
    sprintf(cpad,"c2_%d",i+1);
    ptpad = (TPad*) c2->FindObject(cpad);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogx(0);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.125);
    ptpad->SetTopMargin(0.125);
    c2->cd(i+1);
    switch(i) {
    case 0: Vertex_xy_gen->Draw("colz"); break;
    case 1: Vertex_xy_acc_trk->Draw("colz"); break;
    case 2: Vertex_xy_acc_sci->Draw("colz"); break;
    case 3: Vertex_xy_acc_veto->Draw("colz"); break;
    default: break;
    }
  }
  
  TCanvas* c3 = new TCanvas("c3","vxy gen electron ce",0,0,500,500);
  c3->SetFillColor(0);
  c3->Divide(2,2,0.001,0.001);
  for (int i=0; i<4; i++) {
    sprintf(cpad,"c3_%d",i+1);
    ptpad = (TPad*) c3->FindObject(cpad);
    ptpad->SetFillColor(10);
    ptpad->SetLogy(0);
    ptpad->SetLogx(0);
    ptpad->SetLeftMargin(0.15);
    ptpad->SetBottomMargin(0.15);
    ptpad->SetRightMargin(0.125);
    ptpad->SetTopMargin(0.125);
    c3->cd(i+1);
    switch(i) {
    case 0: Vertex_xy_gen_ce->Draw("colz"); break;
    case 1: Vertex_xy_acc_trk_ce->Draw("colz"); break;
    case 2: Vertex_xy_acc_sci_ce->Draw("colz"); break;
    case 3: Vertex_xy_acc_veto_ce->Draw("colz"); break;
    default: break;
    }
  }

}

void Analyzer::Loop_P()
{

  //  double Energy = E;
  int bufsize = 80000000;
    
  printf("get entries\n");
  Long64_t nentries = fTreeChain->GetEntries();
  std::cout<<" nentries = " << nentries << std::endl;
  Long64_t nbytes = 0, nb = 0;
  Double_t maxLayer = 0;

  //  printf("nentries %d\n",((int) nentries));
  //  for (Long64_t jentry=0; jentry<50000;jentry++) {
      for (Long64_t jentry=0; jentry<nentries;jentry++) {
 
    bool s1Hit=false;
    bool s2Hit=false;
    bool noVetoHit=false;
    bool hitOnTkLayer1=false;
    bool hitOnTkLayer2=false;
    bool goodHit=false;
    maxLayer=0;
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fTreeChain->GetEntry(jentry);   nbytes += nb;
    std::vector<RootTrack> myTracks = Event->GetTracks();
    std::vector<RootCaloHit> myCaloHit = Event->GetCaloHit();
    std::vector<RootCaloHit> myVetoHit = Event->GetVetoHit();
    std::vector<RootTrackerHit> myTrackerHit = Event->GetTrackerHit();

    TVector3 electronDir = myTracks[0].GetDirection();
    TVector3 position = myTracks[0].GetPosition();
    float theta = electronDir.Theta()*180/TMath::Pi();
    if(theta>90)
      theta=180-theta;
    float ce = myTracks[0].GetKinEnergy();
    Pcinetique_gen->Fill(ce);
    Vertex_xy_gen->Fill(position.X()/10,position.Y()/10);
    if (ce <=40) Vertex_xy_gen_ce->Fill(position.X()/10,position.Y()/10);
	if(myTrackerHit.size()>0){
	  //          printf("trk no hits %d\n",myTrackerHit.size());
	  for(size_t th=0;th<myTrackerHit.size();th++){
	    int detId = myTrackerHit[th].GetDetectorId();
            int tkid = myTrackerHit[th].GetTrackId();
	    //            printf("tk hit %d det id %d\n",th,detId); 
	    if(detId>2200 && tkid==1) {
	      hitOnTkLayer1=true;
	      //              printf("hit layer 1\n",th);
            }
	    else if(detId>2100 && tkid==1) {
	      hitOnTkLayer2=true;
	      //              printf("hit layer 2\n",th);
            }
	  }
	  if(hitOnTkLayer2&&hitOnTkLayer1) {
	           Pcinetique_acc_trk->Fill(ce);
                   Vertex_xy_acc_trk->Fill(position.X()/10,position.Y()/10);
            if (ce <=40) Vertex_xy_acc_trk_ce->Fill(position.X()/10,position.Y()/10);
	    //		   printf("no tracker hits 2\n",ce);
	  //          printf("\n");
          }
	}
    hitOnTkLayer1=false;
    hitOnTkLayer2=false;
    if(myCaloHit.size()>0){
      if (ce > 10) {
	//         printf("pdg %d x %f y %f z %f\n",myTracks[0].GetPDG(),position.X(),position.Y(),position.Z());
	//         printf("ientry %d energie %10.3e\n",ientry,ce);
      }
      //    printf("Calo hit size %d\n",myCaloHit.size());
      for(size_t sh=0;sh<myCaloHit.size();sh++){
	int scintLayer = myCaloHit[sh].GetVolume();
	//        printf("scintlayer %d\n",scintLayer);
	std::vector<int> plist = myCaloHit[sh].GetParticleList();       
	//f or(size_t p=0;p<plist.size();p++)
	//	  printf("pid %d PDG %d\n",plist[p],myTracks[plist[p]].GetPDG());
	//	if(scintLayer.Contains("S1"))
	//    if (myCaloHit.size()>=2 && ce <= 15)
	  	  //	printf("scintLayer %d\n",scintLayer);
	if (scintLayer >= 1300 && plist[0]==1) s1Hit=true;
        if (scintLayer == 1216 && plist[0]==1) s2Hit=true;
      }
      //      printf("\n");
      if(s1Hit&&s2Hit){
        Pcinetique_acc_sci->Fill(ce);
        Vertex_xy_acc_sci->Fill(position.X()/10,position.Y()/10);
        if (ce <=40) Vertex_xy_acc_sci_ce->Fill(position.X()/10,position.Y()/10);
	if(myVetoHit.size()==0){
	  noVetoHit=true;
          Pcinetique_acc_veto->Fill(ce);
          Vertex_xy_acc_veto->Fill(position.X()/10,position.Y()/10);
          if (ce <=40) Vertex_xy_acc_veto_ce->Fill(position.X()/10,position.Y()/10);
	}
	if(myTrackerHit.size()>0&&noVetoHit){
	  for(size_t th=0;th<myTrackerHit.size();th++){
	    int detId = myTrackerHit[th].GetDetectorId();
            int tkid = myTrackerHit[th].GetTrackId();
	    if(detId>2200 && tkid==1) {
	      hitOnTkLayer1=true;
	      //              printf("hit layer 1\n",th);
            }
	    else if(detId>2100 && tkid==1) {
	      hitOnTkLayer2=true;
	      //              printf("hit layer 2\n",th);
            }
	  }
	  if(hitOnTkLayer2&&hitOnTkLayer1)
	    goodHit=true;
	}
      }
    }
    
    /*    if(checkPos&&goodHit){
      for(size_t j=0;j<myTracks.size();j++){
	if(myTracks[j].GetTrackID()==1){
	  Double_t Xpos =std::fabs(myTracks[j].GetPosition().X());
	  Double_t Ypos =std::fabs(myTracks[j].GetPosition().Y());
	  if(Xpos>Xlimit&&Ypos>Ylimit)
	    goodHit=false;
	}
      }
    } 
    if(checkTheta&&goodHit){
      Double_t myThetaWithSmearing = ComputeAngleWithSmearing(myTrackerHit,0.05)*180/TMath::Pi();
      if(myThetaWithSmearing>Thetalimit)
	goodHit=false;
	} */

    /*    if(goodHit){
      Double_t myTheta = ComputeAngle(myTrackerHit)*180/TMath::Pi();
      Double_t myThetaWithSmearing = ComputeAngleWithSmearing(myTrackerHit,0.05)*180/TMath::Pi();
      
      Double_t trackTheta=0;
      for(size_t j=0;j<myTracks.size();j++){
	if(myTracks[j].GetTrackID()==1)
	  trackTheta=myTracks[j].GetDirection().Theta()*180/TMath::Pi();
      }
      if(trackTheta>90)
	trackTheta=180-trackTheta;
      
      float totalEdep   = 0;
      float totalEnoS1  = 0;
      float scintS1Edep = 0;
      float scintS2Edep = 0;
      float scintEdep   = 0;
      float caloEdep    = 0;
      float siliconEdep = 0;

      for(size_t i=0;i<myTrackerHit.size();i++){
	siliconEdep+=myTrackerHit[i].GetELoss();
	totalEdep+=myTrackerHit[i].GetELoss();
	totalEnoS1+=myTrackerHit[i].GetELoss();
	int detId = myTrackerHit[i].GetDetectorId();
	if(detId>200)
	  layerDepElectron->Fill(1,myTrackerHit[i].GetELoss());
	if(detId<200)
	layerDepElectron->Fill(2,myTrackerHit[i].GetELoss()); 
      //      siliconEdepHisto->Fill(siliconEdep);
        for(size_t i=0;i<myCaloHit.size();i++){
	scintEdep+=myCaloHit[i].GetTotalEdep();
	totalEdep+=myCaloHit[i].GetTotalEdep();
	TString volume = myCaloHit[i].GetVolume();
	if(volume.Contains("S1")){
	  scintS1Edep+=myCaloHit[i].GetTotalEdep();
	  //	  layerDepElectron->Fill(3,myCaloHit[i].GetTotalEdep());
	}
	else if(volume.Contains("S2")){
	  scintS2Edep+=myCaloHit[i].GetTotalEdep();
	  totalEnoS1+=myCaloHit[i].GetTotalEdep();
	  //	  layerDepElectron->Fill(4,myCaloHit[i].GetTotalEdep());
	}
      }
      //      scintS1EdepHisto->Fill(scintS1Edep);
      //      scintS2EdepHisto->Fill(scintS2Edep);
      //      scintEdepHisto->Fill(scintEdep);
      if(myCaloHit.size()){
	float E_Rec_Scint=0;
	float E_Rec_LYSO=0;
	int hittedBricks=0;
	float eDep = 0;
	float bricksEDep[9];
	for(int i =0;i<9;i++)
	  bricksEDep[i] = 0;
	for(size_t i=0;i<myCaloHit.size();i++){
	  eDep = myCaloHit[i].GetTotalEdep();;
	  caloEdep+=eDep;
	  totalEdep+=eDep;
	  totalEnoS1+=eDep;
	  TString layer = myCaloHit[i].GetVolume();
	  int layernumb;
	  if(numbLayerCrystal==-1&&layer.Contains("ActiveBlockCrystal")){
	    layernumb = numbLayerScint+1;
	    hittedBricks++;
	    layer.Remove(0,18);
	    bricksEDep[layer.Atoi()]+=eDep;
	  }else if(layer.Contains("ActiveLayerCrystal")){
	    layer.Remove(0,18);
	    layernumb = numbLayerScint+numbLayerCrystal-layer.Atoi();
	  }else if(layer.Contains("ActiveLayerScint")){
	    layer.Remove(0,16);
	    layernumb = numbLayerScint-layer.Atoi();
	  }else{ 
	    layer.Remove(0,11);
	    layernumb = numbCaloLayer-layer.Atoi();
	  }
	  if(maxLayer<layernumb)
	    maxLayer=layernumb;
	  //	  layerDepElectron->Fill(layernumb+4,eDep);
	  if(layernumb== numbLayerScint+1)
	    E_Rec_LYSO+=eDep;
	  else
	    E_Rec_Scint+=eDep;
	}
	float lysoDep=0;
	for(int i = 0;i<9;i++){
	  if(bricksEDep[i]!=0){
	    lysoDep+=bricksEDep[i];
	    //	    singlebrickDep->Fill(bricksEDep[i]);	
	  }
	  //	  if(lysoDep!=0&&i==8)
	  //  allbricksDep->Fill(lysoDep);
	}
	//	bricksHitted->Fill(hittedBricks);
	//      	caloEdepHisto->Fill(caloEdep);
	//	layerDepthElectron->Fill(theERange,maxLayer);
	//	energyAVGRangeElectron->Fill(theERange,E_Rec_LYSO/(E_Rec_Scint/numbLayerScint));
	//	energySUMRangeElectron->Fill(theERange,E_Rec_LYSO/E_Rec_Scint);
      }

      //      totalEdepHistoNoS1NoCut->Fill(totalEnoS1);
      //      totalEdepHistoNoCut->Fill(totalEdep);
      
      if(totalEnoS1>2.5)
	totalEdepHistoNoS1->Fill(totalEnoS1);
      if(totalEnoS1>1.5)
	totalEdepHistoNoS1Cut2->Fill(totalEnoS1);
      if(totalEdep>=2.5){
	totalEdepHisto->Fill(totalEdep);
	energyVSangle->Fill(myThetaWithSmearing,totalEdep/theERange);
	} 
    } */
    
  }
}

void Analyzer::SetAcceptanceWindows(Double_t X,Double_t Y)
{
  checkPos=true;
  Xlimit=X/2.;
  Ylimit=Y/2.;
}

void Analyzer::SetThetaAcceptance(Double_t aTheta)
{
  checkTheta=true;
  Thetalimit=aTheta;
}

Double_t Analyzer::ComputeAngle(std::vector<RootTrackerHit>& myTrackerHit)
{

  std::vector<RootTrackerHit> layer2Hit;
  std::vector<RootTrackerHit> layer1Hit;
  
  for(size_t th=0;th<myTrackerHit.size();th++){
    int detId = myTrackerHit[th].GetDetectorId();
    if(detId>200)
      layer2Hit.push_back(myTrackerHit[th]);
    if(detId<200)
      layer1Hit.push_back(myTrackerHit[th]);
  }
  
  TVector3 posL2;
  TVector3 posL1;

  if(layer2Hit.size()==1){
    posL2 = (layer2Hit[0].GetEntryPoint()+layer2Hit[0].GetExitPoint())*0.5;
  }else{
    for(size_t j= 0;j<layer2Hit.size();j++){
      if(layer2Hit[j].GetTrackId()==1)
	posL2 = (layer2Hit[j].GetEntryPoint()+layer2Hit[j].GetExitPoint())*0.5;
    }
  }
  if(layer1Hit.size()==1){
    posL1 = (layer1Hit[0].GetEntryPoint()+layer1Hit[0].GetExitPoint())*0.5;
  }else{
    for(size_t j= 0;j<layer2Hit.size();j++){
      if(layer1Hit[j].GetTrackId()==1)
	posL1 = (layer1Hit[j].GetEntryPoint()+layer1Hit[j].GetExitPoint())*0.5;
    }
  }

  TVector3 segment = posL2-posL1;

  return segment.Theta();
}

Double_t Analyzer::ComputeAngleWithSmearing(std::vector<RootTrackerHit>& myTrackerHit,Double_t delta)
{

  std::vector<RootTrackerHit> layer2Hit;
  std::vector<RootTrackerHit> layer1Hit;
  
  for(size_t th=0;th<myTrackerHit.size();th++){
    int detId = myTrackerHit[th].GetDetectorId();
    if(detId>200)
      layer2Hit.push_back(myTrackerHit[th]);
    if(detId<200)
      layer1Hit.push_back(myTrackerHit[th]);
  }
  
  TVector3 posL2;
  TVector3 posL1;

  if(layer2Hit.size()==1){
    posL2 = (layer2Hit[0].GetEntryPoint()+layer2Hit[0].GetExitPoint())*0.5;
  }else{
    for(size_t j= 0;j<layer2Hit.size();j++){
      if(layer2Hit[j].GetTrackId()==1)
	posL2 = (layer2Hit[j].GetEntryPoint()+layer2Hit[j].GetExitPoint())*0.5;
    }
  }
  if(layer1Hit.size()==1){
    posL1 = (layer1Hit[0].GetEntryPoint()+layer1Hit[0].GetExitPoint())*0.5;
  }else{    
    for(size_t j= 0;j<layer2Hit.size();j++){
      if(layer1Hit[j].GetTrackId()==1)
	posL1 = (layer1Hit[j].GetEntryPoint()+layer1Hit[j].GetExitPoint())*0.5;
    }
  }

  TRandom myRand;

  Double_t xL1 = posL1.X()+myRand.Uniform(-delta,delta);
  Double_t yL1 = posL1.Y()+myRand.Uniform(-delta,delta);

  posL1.SetX(xL1);
  posL1.SetY(yL1);

  Double_t xL2 = posL2.X()+myRand.Uniform(-delta,delta);
  Double_t yL2 = posL2.Y()+myRand.Uniform(-delta,delta);

  posL2.SetX(xL2);
  posL2.SetY(yL2);

  TVector3 segment = posL2-posL1;

  return segment.Theta();
}
