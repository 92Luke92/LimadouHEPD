// here to start documentation
// author: vincenzo vitale, vvitale@roma2.infn.it
//
// class purpose: collection of algorithm for level2 observables
// obtained with the calorimeter and vetos measuremets

#include "TH1.h"
#include "TH2.h"
#include "LEvRec1File.hh"
#include "LScintillatorsL2.hh"
#include <iostream>
#include <fstream>
#include "LEvTemp.hh"
#include "TFile.h"
#include "TTree.h"
#include <string>
//#include "iomanip.h"

using namespace std;


LScintillatorsL2::LScintillatorsL2() {

}//--------------------------------------------------------------------------------------



LScintillatorsL2 &LScintillatorsL2::GetInstance() {

  static LScintillatorsL2 instance;  // Guaranteed to be destroyed.

  return instance;

}//--------------------------------------------------------------------------------------



void LScintillatorsL2::input_calib_pars(double *teq, double *equ, double *leq){

 //  calibration input-------------
 double dummy;

 // high gain
 ifstream hg; hg.open("eqHG.txt.off");

 // trigger bars
 for(int t=0;t<12;t++){hg >> teq[t] >> dummy;}

 // UC planes
 for(int t=0;t<32;t++){hg >> equ[t] >> dummy;}

 // log gain 
 ifstream lg;

 lg.open("eqLG.txt.off");

 // skipping
 for(int t=0;t<54;t++){lg >> dummy >> dummy;}

 // LYSO crystals

 for(int t=0;t<9;t++){lg >> leq[t] >> dummy;}
 // equalization all to ref

 double ref = 200.000; 

 for(int t=0;t<32;t++){equ[t]=ref/equ[t];} 

  for(int t=0;t<9;t++){leq[t]=ref/leq[t];} 

 for(int t=0;t<12;t++){teq[t]=ref/teq[t];}
}//--------------------------------------------------------------------------------------



void LScintillatorsL2::input_energyrecon_pars(double *t_p0, double *t_p1, double *c_p0, double *c_p1){
 
 //  calibration input-------------
 //  t_p0 and t_p1 are trigger p0 and p1 X 6
 //  c_p0 and c_p1  are upper calo and lyso p0 and p1
 //  input file format is :
 //  row = p0 p1
 //  6 row trigger
 //  1 row uc
 //  1 row lyso


 double dummy;


 // high gain
 ifstream er; er.open("energyrecon.off");


 // trigger bars
 for(int t=0;t<6;t++){er >> t_p0[t] >> t_p1[t];}


 // UC 
 {er >> c_p0[0] >> c_p1[0];}


 // lyso
 {er >> c_p0[1] >> c_p1[1];} 
}//--------------------------------------------------------------------------------------





int LScintillatorsL2::DeviceStatus(double sn1, double sn2, double th){

int status=0;double act_threshold=th;

if( sn1>act_threshold  && sn2>act_threshold){status=1;}

return status;

}//--------------------------------------------------------------------------------------




int LScintillatorsL2::LoadRun(const std::string inputFileROOT) {
  
// Reset previous loaded runs
  if (RunFile) {

    if (RunFile->IsOpen()) RunFile->Close();

    RunFile = 0;

  }

  RunFile = new LEvRec1File(inputFileROOT.c_str(), "READ");

  if (!RunFile || !(RunFile->IsOpen())) {

    std::cerr << __LScintillatorsL2__

              << "Error! Attempt to load a data run, but the file "

                 "cannot be opened."

              << std::endl;

    RunFile = 0;

    return EXIT_FAILURE;

  }

  return EXIT_SUCCESS;

}//--------------------------------------------------------------------------------------




int LScintillatorsL2::Calc(int hw_condition, double sign_th) {

// calibration constants and analysis pars

double teq[12], equ[32],leq[9];

input_calib_pars(teq,equ,leq);

double t_p0[6], t_p1[6], c_p0[2], c_p1[2];

input_energyrecon_pars(t_p0, t_p1, c_p0, c_p1);

double const act_threshold= 5.00;



double tene=0., tsum =0, tmult=0, hitbar[6];
int IsLYSOHit=0; // lyso.;
int IsVetoBottomHit=0, IsVetoLatHit=0;// veto



// data output  ---------------------------------------------------------------------------

//ofstream os; os.open("out.calo.sign");

 LEvRec1 cev;
 
 RunFile->SetTheEventPointer(cev);
 
 TFile *fFile = new TFile("outFile.root", "new");

 TTree *fTree = new TTree("Temp","Temporary L2");
  
 LEvTemp lev2;
 
 fTree->Branch("temp_data", &lev2,"trig_mult/I:bars_status[6]/I:calo_mult/I:lyso_mult/I:IsVetoLatHit/I:IsLYSOHit/I:IsVetoBottomHit/I:trig_signal_sum/F:calo_signal_sum/F:lyso_signal_sum/F:plane_signal[16]/F:trig_ener_dep/F:calo_ener_dep/F:lyso_ener_dep/F:ener_dep/F");
// ---------------------------------------------------------------------------


// loop on data events
int nEvents = RunFile->GetEntries();

for (int iEv = 0; iEv < nEvents; iEv++)  // Event loop

  { //std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2) << int(double(iEv) / double(nEvents - 1) * 100) << "%" << std::flush;

     RunFile->GetEntry(iEv); 


// trigger bars ------------------------------------------------------------------------------------------------------------
// reset vars
tsum=0., tmult=0, tene=0.; for(int a=0;a<6;a++){hitbar[a]=0.0;}; 


for (int bar=0;bar<6;bar++){ 
    int  tpmt1=(bar*2)+0;
    int  tpmt2=(bar*2)+1;
    if( DeviceStatus(cev.trig.sn_hg[bar][0],cev.trig.sn_hg[bar][1], sign_th)==1 , sign_th)
         {
         double c1=teq[tpmt1]*cev.trig.cont_hg[bar][0];
         double c2=teq[tpmt2]*cev.trig.cont_hg[bar][1];
         tsum=tsum+c1+c2;
         tmult++;
         hitbar[bar]=1;   
         tene = tene+((tsum-t_p1[bar])/t_p0[bar]);
           
         }
         
   }// end loop on t bars


// Upp C vars ---------------------------------------------------------------------------------------------------------------------------

// serv array
double cal1[32],cal2[9],cres[3],cresarray[16], ck[2]; 

// data swap into service arrays
for(int cr=0; cr<32;cr++){

int plane = (int) cr / 2;// starts from 0

int dev   = cr - (plane * 2);

cal1[cr]=equ[cr]*cev.scint.cont_hg[plane][dev];// check equ mapping

cal2[cr]=cev.scint.sn_hg[plane][dev];


}


// energy reconstruction constants
ck[0]=c_p0[0]; ck[1]=c_p1[0];

// calculation
CalcUpperC(cal1,cal2,cres,cresarray,ck, sign_th);


// LYSO vars ---------------------------------------------------------------------------------------------------------------------------
 
// serv array
double l1[9],l2[9],lres[4],lk[2];  

// data swap into service array - LOW GAIN
for(int cr=0; cr<9;cr++){

l1[cr]=leq[cr]*cev.lyso.cont_lg[cr][0];

l2[cr]=cev.lyso.sn_lg[cr][0];

}

// energy reconstruction constants
lk[0]=c_p0[1]; lk[1]=c_p1[1];

// calculation
CalcLYSO(l1,l2,lres,lk, sign_th);

// to be changed with lev2 elements
//IsLYSOHit=lres[2], lmult=lres[1], lsum=lres[0], enLYSO=lres[3];


// reset vars
IsVetoBottomHit=0, IsVetoLatHit=0;

IsVetoBottomHit=DeviceStatus(cev.veto.sn_hg[4][0],cev.veto.sn_hg[4][1], sign_th);


for (int veto=0;veto<4;veto++){IsVetoLatHit=IsVetoLatHit+DeviceStatus(cev.veto.sn_hg[veto][0],cev.veto.sn_hg[veto][1], sign_th);}
 

//int IsLYSOHit=0;
//for (int lys=0;lys<8;lys++){IsLYSOHit=IsLYSOHit+DeviceStatus(cev.lyso.sn_hg[lys][0],cev.lyso.sn_hg[lys][0]);}

// root output ---------------------------------------------

  
  lev2.trig_mult=tmult;  

  for(int i=0;i<6;i++){
  lev2.bars_status[i]=hitbar[i];
  }

  lev2.calo_mult=cres[1];

  lev2.lyso_mult=lres[1];

  lev2.trig_signal_sum= tsum; 


 // here ptm 5east correction, A.S. correction under test, just double the pmt 5west 
 // directly added to upper calo signal output
           
 //           std::cout << " pmt 5 east compensation !" << std::endl; 
 //            cres[0]=cres[0] + 1.*(equ[9]*cev.scint.cont_hg[4][1]);

  lev2.calo_signal_sum=cres[0];
  
  lev2.lyso_signal_sum=lres[0];
  
  for(int i=0;i<16;i++){
  lev2.plane_signal[i]=cresarray[i];
  }

  lev2.trig_ener_dep=tene;
  
  lev2.calo_ener_dep=cres[2];
  
  lev2.lyso_ener_dep=lres[3];

  lev2.IsVetoLatHit=IsVetoLatHit; 
  
  lev2.IsLYSOHit=lres[2]; 
  
  lev2.IsVetoBottomHit=IsVetoBottomHit;

  // Energy cals ---------------------------------------------------------------------------------------------------------------------------

  double ek1[2]={-28.4,249.6};
  double ek2[2]={-17.2,4.73};
  lev2.ener_dep= CalcEnergy(lev2.calo_signal_sum + (0.5 * lev2.trig_signal_sum),lev2.lyso_signal_sum ,ek1,ek2);

// vetos flags ----------------------------------------------------------------------------------------------------------------------



  fTree->Fill();






  
}// end event loop

  fTree->Write();

  fFile->Close();

  return 0;


}//--------------------------------------------------------------------------------------

double LScintillatorsL2::CalcEnergy(double supp, double slyso, double k1[2], double k2[2]){
// all signals in adc_cts, all energies in MeV
double sth = 00.;
double eupp  = (supp/k1[1])-(k1[0]/k1[1]);
double elyso = (slyso/k2[1])-(k2[0]/k2[1]);

double energy = eupp;
if (slyso > sth ) { energy=eupp+elyso;} 
 
return  energy;

}//--------------------------------------------------------------------------------------




void LScintillatorsL2::CalcLYSO(double signal[9], double sn[9], double out[4], double k[2], double sign_th ){

for (int i=0;i<4;i++){out[i]=0;}//  reset out[4], it contains signal , multip flags, energy

for (int lys=0;lys<9;lys++){

      if(DeviceStatus(sn[lys],sn[lys], sign_th)==1){

      out[1]++;

      out[2]++;

      out[0]=out[0]+signal[lys]; }

      }// end lyso loop


// LYSO energy calc 
      k[0]=0.39 ,k[1]=0. ;// fixing conversion
      //out[3]=(double)(out[0]-k[1])/k[0];//MeV
       out[3]=(double)(out[0]*0.42)+5.5;//MeV
 
return ;

}//--------------------------------------------------------------------------------------











void LScintillatorsL2::CalcUpperC(double pmt_signals[32], double pmt_sns[32], double out[3], double outarray[16], double  k[2], double sign_th){

double PlaneEqF[16]={445.,440.,441.,424.,400.,439.,423.,426.,434.,430.,427.,433.,425.,422.,431.,425.};// temporary hardcoded plane equalization
for (int p = 0; p<16; p++){PlaneEqF[p]=400./PlaneEqF[p]; }

for (int i=0;i<3;i++){out[i]=0;}//  reset out[3], it contains signal , multip flag, energy

for (int i=0;i<16;i++){outarray[i]=0;}//  reset outarray[3], it contains signals vs plane

for (int pln=0;pln<16;pln++){

    int  pmt1=(pln*2);

    int  pmt2=(pln*2)+1; // firts plane shoudl have pmt 0 and 1

    if( DeviceStatus(pmt_sns[pmt1],pmt_sns[pmt2], sign_th)==1)
         {

         double c1=pmt_signals[pmt1]; // already de-pedestaleld and equalized

         double c2=pmt_signals[pmt2];


         if(pln == 4){ c1 = 0.00;}// ignoring pmt 5east

         
         out[0]=out[0]+c1+c2;

         outarray[pln]=PlaneEqF[pln]*(c1+c2);// this is equalized to have each plane to 400 with a MIP

         out[1]++;

         //conn[pln]=1;//
      
         }//end if for hit plane
      }// end pln loop


         

/*
// max connected length        
trlen=1;
for (int i=0;i<16;i++){
    if(conn[i]==0  && i!=4) break; // second part to cope with broken pmt
    trlen++;
    }

*/



// energy calc old

// 

out[2]=(out[0]-k[1])/k[0];//MeV


   
 
return ;

}//--------------------------------------------------------------------------------------







int LScintillatorsL2::MakeLYSOStudy(const std::string inputFileROOT, double sign_th){

 
  double const LConv = 0.42, Offset=5.5;// MeV/adc , MeV

  //histos
  TH1D *s = new TH1D("","Signal Sum",500,0,1500);// lyso sum
  TH1D *sall = new TH1D("","Signal Sum",500,0,1500);// lyso sum
  TH1D *hmult = new TH1D("","Multiplicity",10,0,10);// lyso sum
  TH1D *h[6];  for (int i = 0; i < 6; i++) {h[i] = new TH1D("","",500,0,1500);}
  TH2D *cor[2]; for (int i = 0; i < 2; i++) 
  cor[0] = new TH2D("","",3,0,4,3,0,4);
  cor[1] = new TH2D("","",300,0,400,300,0,400);
  TH1D *lh[9];  for (int i = 0; i < 9; i++) {lh[i] = new TH1D("","",500,10,1000);}// lyso indiv
  TH1D *srecon = new TH1D("","Energy Recon",500,0,250);// lyso sum

  // counters 

  int CounterVetoB=0, CounterP1=0, CounterP16=0;

  // data and equalization
  double P1l1=0.,P1l2=0.,P16l1=0., P16l2=0., Vbl1=0.,Vbl2=0.;// high gain pmts
  double Lyl[9]={0},Lyh[9]={0}, lowLsum=0. ;// low-high gain lyso
  int multL=0;// multiplicity
  double equP1[2]={100./107.5,100./100.},equP16[2]={100./157.5,100./157.2},equVB[2]={100./77.4,100./55.0};// equal
  double equLlow[9]={100./303,100./544,100./296,100./410,100./270,100./275,100./225,100./268,100./342};

  LEvRec1File inputFile(inputFileROOT.c_str(),"READ");
  LEvRec1 cev;
  inputFile.SetTheEventPointer(cev);

  int nentries=inputFile.GetEntries();
  for(int ie=0; ie<nentries; ++ie) {
    inputFile.GetEntry(ie);
    
   for(int i = 0 ; i < 9; i++){
       Lyl[i]=cev.lyso.cont_lg[i][0]*equLlow[i]; 
       Lyh[i]=cev.lyso.cont_hg[i][0];
   }

   P1l1=cev.scint.cont_hg[0][0]*equP1[0];
   P1l2=cev.scint.cont_hg[0][1]*equP1[1]; 
   P16l1=cev.scint.cont_hg[15][0]*equP16[0];
   P16l2=cev.scint.cont_hg[15][1]*equP16[1];
   Vbl1=cev.veto.cont_hg[4][0]*equVB[0];
   Vbl2=cev.veto.cont_hg[4][1]*equVB[1];
   


   lowLsum=0., multL=0;// reset lyso sum and mult
   for(int i = 0 ; i < 9; i++){
       
       lowLsum=lowLsum+Lyl[i];
       lh[i]->Fill(Lyl[i]);

       if(DeviceStatus(cev.lyso.sn_lg[i][0], cev.lyso.sn_lg[i][0], sign_th)==1){multL++;}
      
   }

   int st=DeviceStatus(cev.veto.sn_hg[4][0], cev.veto.sn_hg[4][1], sign_th);// signal in bottom veto
  
      if(1){
      
         h[0]->Fill(P1l1);
         h[1]->Fill(P1l2);
         h[2]->Fill(P16l1);
         h[3]->Fill(P16l2);
         h[4]->Fill(Vbl1);
         h[5]->Fill(Vbl2);
   
         cor[0]->Fill(lowLsum,P16l1+P16l2);
       
        
   
         s->Fill(lowLsum);
         hmult->Fill(multL);
         srecon->Fill((lowLsum*LConv)+Offset);
   
          if(DeviceStatus(cev.veto.sn_hg[4][0],cev.veto.sn_hg[4][1], sign_th)==1){CounterVetoB++;}
          if(DeviceStatus(cev.scint.sn_hg[0][0],cev.scint.sn_hg[0][1], sign_th)==1){CounterP1++;}
          if(DeviceStatus(cev.scint.sn_hg[15][0],cev.scint.sn_hg[15][1], sign_th)==1){CounterP16++;}

 


       }// end selection

  }

  inputFile.Close();

   std::cout << " counter P1 " <<  CounterP1 << std::endl;
   std::cout << " counter P16 " <<  CounterP16 << std::endl;
   std::cout << " counter VetoB " <<  CounterVetoB << std::endl;


   TFile f("histos.root", "recreate");

   h[0]->SetTitle("P1l1");
   h[1]->SetTitle("P1l2");
   h[2]->SetTitle("P16l1");
   h[3]->SetTitle("P16l2");
   h[4]->SetTitle("Vbl1");
   h[5]->SetTitle("Vbl2");

   for (int i = 0; i < 6; i++) {h[i]->Write();}
   cor[0]->Write();
   cor[1]->Write(); 
   
  
   for (int i = 0; i < 9; i++) {lh[i]->Write();}
   s->Fit("landau","","",100,300);
   s->Write();
   srecon->Fit("landau","","",35,100);
   srecon->Write();
   hmult->Write();

  return 0;

}//--------------------------------------------------------------------------------------





LScintillatorsL2::~LScintillatorsL2() {
  // do not care about singleton destructor
}
