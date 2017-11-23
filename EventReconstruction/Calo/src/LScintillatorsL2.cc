// here to start documentation
// author: vincenzo vitale, vvitale@roma2.infn.it
//
// class purpose: collection of algorithm for level2 observables
// obtained with the calorimeter and vetos measuremets

#include "TH1.h"
#include "LEvRec1File.hh"
#include "LScintillatorsL2.hh"
#include <iostream>
#include <fstream>
#include "LEvTemp.hh"
#include "TFile.h"
#include "TTree.h"

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

 double ref = 100.000; 

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





int LScintillatorsL2::DeviceStatus(double sn1, double sn2){

int status=0;double act_threshold=5.0;

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




int LScintillatorsL2::Calc(int hw_condition, int ascii_dump) {

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
 
 fTree->Branch("temp_data", &lev2,"trig_mult/I:bars_status[6]/I:calo_mult/I:lyso_mult/I:IsVetoLatHit/I:IsLYSOHit/I:IsVetoBottomHit/I:trig_signal_sum/F:calo_signal_sum/F:lyso_signal_sum/F:plane_signal[16]/F:trig_ener_dep/F:calo_ener_dep/F:lyso_ener_dep/F");
// ---------------------------------------------------------------------------


// loop on data events
int nEvents = RunFile->GetEntries();

for (int iEv = 0; iEv < nEvents; iEv++)  // Event loop

     { std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2) << int(double(iEv) / double(nEvents - 1) * 100) << "%" << std::flush;

     RunFile->GetEntry(iEv); 


// trigger bars ------------------------------------------------------------------------------------------------------------
// reset vars
tsum=0., tmult=0, tene=0.; for(int a=0;a<6;a++){hitbar[a]=0.0;}; 


for (int bar=0;bar<6;bar++){ 
    int  tpmt1=(bar*2)+0;
    int  tpmt2=(bar*2)+1;
    if( DeviceStatus(cev.trig.sn_hg[bar][0],cev.trig.sn_hg[bar][1])==1 )
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
CalcUpperC(cal1,cal2,cres,cresarray,ck);


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
CalcLYSO(l1,l2,lres,lk);

// to be changed with lev2 elements
//IsLYSOHit=lres[2], lmult=lres[1], lsum=lres[0], enLYSO=lres[3];




// vetos flags ----------------------------------------------------------------------------------------------------------------------
// reset vars
IsVetoBottomHit=0, IsVetoLatHit=0;

IsVetoBottomHit=DeviceStatus(cev.veto.cont_hg[4][0],cev.veto.cont_hg[4][1]);


for (int veto=0;veto<4;veto++){IsVetoLatHit=IsVetoLatHit+DeviceStatus(cev.veto.sn_hg[veto][0],cev.veto.sn_hg[veto][1]);}
 

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

  fTree->Fill();






// optional ascii output ----------------------------------------------------------------------------------------------------------------
/*if(ascii_dump==1){
os << iEv << " " 
<< tsum << " " <<  tmult << " "  << tene << " " 
<< sumall  << " "  << mult << " " << trlen << " " << enUC <<  " "
<< IsLYSOHit << " " << lmult << " " << lsum << " " << enLYSO  << " " 
<< IsVetoBottomHit  << " " << IsVetoLatHit << " " 
<< hitbar[0] << " " << hitbar[1] << " " << hitbar[2] << " " << hitbar[3] << " " << hitbar[4] << " " << hitbar[5] << " " 
<< 987654321 << " " 
<< ChVsPlane[0] << " " <<  ChVsPlane[1] << " " <<  ChVsPlane[2] << " " << ChVsPlane[3] << " " <<  ChVsPlane[4] << " " <<  ChVsPlane[5] << " "
<< ChVsPlane[6] << " " <<  ChVsPlane[7] << " " <<  ChVsPlane[8] << " " << ChVsPlane[9] << " " <<  ChVsPlane[10] << " " <<  ChVsPlane[11] << " "
<< ChVsPlane[12] << " " <<  ChVsPlane[13] << " " <<  ChVsPlane[14] << " " << ChVsPlane[15]
<< endl;}*/

// all available vars
//int tsum=0, tmult=0, hitbar[6]={0}; double tene=0; // trigger vars
//int sumall=0,suma=0,sumb=0, mult=0, conn[16]={0}; double enUC=0, ChVsPlane[16];// calo vars
//int IsLYSOHit=0, lmult=0; double lsum=0, enLYSO=0; // lyso
//int IsVetoBottomHit=0, IsVetoLatHit=0;// veto


  
}// end event loop

  fTree->Write();

  fFile->Close();

  return 0;


}//--------------------------------------------------------------------------------------

void LScintillatorsL2::CalcLYSO(double signal[9], double sn[9], double out[4], double k[2] ){

for (int i=0;i<4;i++){out[i]=0;}//  reset out[4], it contains signal , multip flags, energy

for (int lys=0;lys<9;lys++){

      if(DeviceStatus(sn[lys],sn[lys])==1){

      out[1]++;

      out[2]++;

      out[0]=out[0]+signal[lys]; }

      }// end lyso loop


// LYSO energy calc 
//      double k[0]=0.0 ,k[1]=1.32 ;
      out[3]=(double)(out[0]-k[1])/k[0];//MeV

 
return ;

}//--------------------------------------------------------------------------------------











void LScintillatorsL2::CalcUpperC(double pmt_signals[32], double pmt_sns[32], double out[3], double outarray[16], double  k[2]){

for (int i=0;i<3;i++){out[i]=0;}//  reset out[3], it contains signal , multip flag, energy

for (int i=0;i<16;i++){outarray[i]=0;}//  reset outarray[3], it contains signals vs plane

for (int pln=0;pln<16;pln++){

    int  pmt1=(pln*2);

    int  pmt2=(pln*2)+1; // firts plane shoudl have pmt 0 and 1

    if( DeviceStatus(pmt_sns[pmt1],pmt_sns[pmt2])==1)
         {

         double c1=pmt_signals[pmt1]; // already de-pedestaleld and equalized

         double c2=pmt_signals[pmt2];
         
         out[0]=out[0]+c1+c2;

         outarray[pln]=c1+c2;

         out[1]++;

         //conn[pln]=1;//
      
         }//end if for hit plane
      }// end pln loop

// check data posterior to ptm 5east failure then correct sumall
/*if(hw_condition==1){
      double comp=(
      (equ[6]*cev.scint.cont_hg[3][0])+
      (equ[7]*cev.scint.cont_hg[3][1])+
      (equ[9]*cev.scint.cont_hg[4][1])+
      (equ[10]*cev.scint.cont_hg[5][0])+
      (equ[11]*cev.scint.cont_hg[5][1])  )/5.;
      sumall=sumall+comp;}

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





double LScintillatorsL2::Sign2En(double signal, double thr){

double k=120.;

if(signal>thr){k = 120. +(3.*0.004166*signal); }


return k;

}//--------------------------------------------------------------------------------------

LScintillatorsL2::~LScintillatorsL2() {
  // do not care about singleton destructor
}
