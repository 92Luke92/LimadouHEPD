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
 double ref = 100.000; for(int t=0;t<32;t++){equ[t]=ref/equ[t];} for(int t=0;t<9;t++){leq[t]=ref/leq[t];} for(int t=0;t<12;t++){teq[t]=ref/teq[t];}
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
}

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

int tsum=0, tmult=0, hitbar[6]={0}; double tene=0; // trigger vars
int sumall=0,suma=0,sumb=0, mult=0, conn[16]={0}; double enUC=0, ChVsPlane[16]; int trlen=0;// calo vars
int IsLYSOHit=0, lmult=0; double lsum=0, enLYSO=0; // lyso
int IsVetoBottomHit=0, IsVetoLatHit=0;// veto

// connecting data and output ascii - temporary ---------------------------------------------------------------------------

ofstream os; os.open("out.calo.sign");

LEvRec1 cev;
RunFile->SetTheEventPointer(cev);

// loop on data events
int nEvents = RunFile->GetEntries();
for (int iEv = 0; iEv < nEvents; iEv++)  // Event loop
    { std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2) << int(double(iEv) / double(nEvents - 1) * 100) << "%" << std::flush;
    RunFile->GetEntry(iEv); 


// trigger bars ------------------------------------------------------------------------------------------------------------
// reset vars
tsum=0, tmult=0, tene=0; for(int a=0;a<6;a++){hitbar[a]=0.0;};

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

// upper calorimeter ------------------------------------------------------------------------------------------------------------

sumall=0,suma=0,sumb=0, mult=0; for(int a=0;a<16;a++){conn[a]=0;}; enUC=0;for(int a=0;a<16;a++){ChVsPlane[a]=0;};trlen=0;

for (int pln=0;pln<16;pln++){
    int  pmt1=(pln*2);
    int  pmt2=(pln*2)+1;
    if( DeviceStatus(cev.scint.sn_hg[pln][0],cev.scint.sn_hg[pln][1])==1 )
         {
         double c1=equ[pmt1]*cev.scint.cont_hg[pln][0];
         double c2=equ[pmt2]*cev.scint.cont_hg[pln][1];
         suma=suma+c1; 
         sumb=sumb+c2;
         sumall=sumall+c1+c2;
         ChVsPlane[pln]=c1+c2;
         mult++;
         conn[pln]=1;//
         }//end if for hit plane
      }// end pln loop

// check data posterior to ptm 5east failure then correct sumall
if(hw_condition==1){
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





// energy calc 
enUC=(sumall-c_p1[0])/c_p0[0];//MeV
   
// LYSO vars ---------------------------------------------------------------------------------------------------------------------------

// vars reset 
IsLYSOHit=0, lmult=0, lsum=0, enLYSO=0;

for (int lys=0;lys<8;lys++){
      if(DeviceStatus(cev.lyso.sn_hg[lys][0],cev.lyso.sn_hg[lys][0])==1){
      lmult++;
      IsLYSOHit++;
      lsum=lsum+(leq[lys]*cev.scint.cont_hg[lys][0]); }
      }// end lyso loop

// LYSO energy calc 
        enLYSO=(lsum-c_p1[1])/c_p0[1];//MeV


// vetos flags ----------------------------------------------------------------------------------------------------------------------
// reset vars
IsVetoBottomHit=0, IsVetoLatHit=0;

IsVetoBottomHit=DeviceStatus(cev.veto.cont_hg[4][0],cev.veto.cont_hg[4][1]);


for (int veto=0;veto<4;veto++){IsVetoLatHit=IsVetoLatHit+DeviceStatus(cev.veto.sn_hg[veto][0],cev.veto.sn_hg[veto][1]);}
 

//int IsLYSOHit=0;
//for (int lys=0;lys<8;lys++){IsLYSOHit=IsLYSOHit+DeviceStatus(cev.lyso.sn_hg[lys][0],cev.lyso.sn_hg[lys][0]);}


// optional ascii output ----------------------------------------------------------------------------------------------------------------
if(ascii_dump==1){
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
<< endl;}

// all available vars
//int tsum=0, tmult=0, hitbar[6]={0}; double tene=0; // trigger vars
//int sumall=0,suma=0,sumb=0, mult=0, conn[16]={0}; double enUC=0, ChVsPlane[16];// calo vars
//int IsLYSOHit=0, lmult=0; double lsum=0, enLYSO=0; // lyso
//int IsVetoBottomHit=0, IsVetoLatHit=0;// veto

}// end event loop

 

return 0;
}//--------------------------------------------------------------------------------------


//int LScintillatorsL2::Filter(int name, double *argums){
//int filterval=-1.;
//return filterval;
//}//--------------------------------------------------------------------------------------



LScintillatorsL2::~LScintillatorsL2() {
  // do not care about singleton destructor
}
