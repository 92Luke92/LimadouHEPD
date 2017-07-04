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
  
}

LScintillatorsL2 &LScintillatorsL2::GetInstance() {
  static LScintillatorsL2 instance;  // Guaranteed to be destroyed.
  return instance;
}



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
}

int LScintillatorsL2::DeviceStatus(double sn1, double sn2){

int status=0;double act_threshold=5.0;
if( sn1>act_threshold || sn2>act_threshold){status=1;}
return status;
}


//---------------------------------------------------------------------------

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
}

//---------------------------------------------------------------------------


int LScintillatorsL2::Test(int firstevent) {


double teq[12], equ[32],leq[9];
input_calib_pars(teq,equ,leq);
//for(int t=0;t< 32; t++){std::cout << equ[t]<<endl;}
return 0;

}

//---------------------------------------------------------------------------
int LScintillatorsL2::Calc(int hw_condition) {

//  calibration input-------------
double teq[12], equ[32],leq[9];input_calib_pars(teq,equ,leq);

double act_threshold= 5.00;// general threshold to declare a device hit

// energy calibration constants
const double Ek[2]={-51, 112.7};//MeV/adc_counts
const double TriggerP0[6]={728,593,1227,1066, 616, 801},
TriggerP1[6]={-44,+45,-517,-361,-15,-98};// S=(p0*E) + p1

// here varibles which contain the output
int tsum=0, tmult=0, hitbar[6]={0}; double tene=0; // trigger vars
int sumall=0,suma=0,sumb=0, mult=0, conn[16]={0}; double enUC=0, ChVsPlane[16];// calo vars

//---------------------------------------------------------------------------
// connecting data

LEvRec1 cev;
RunFile->SetTheEventPointer(cev);
// loop on data events
int nEvents = RunFile->GetEntries();
for (int iEv = 0; iEv < nEvents; iEv++)  // Event loop
  {
   std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2)
              << int(double(iEv) / double(nEvents - 1) * 100) << "%"
              << std::flush;
    RunFile->GetEntry(iEv); 
// loop on trigger bars ---------------------------------------------------------------

// reset vars
tsum=0, tmult=0, hitbar[6]={0}, tene=0;



   for (int bar=0;bar<6;bar++){ 
   int  tpmt1=(bar*2)+0;
   int  tpmt2=(bar*2)+1;// check if they are correct !!

// selection of signals above thr of 5 rms on both pmts

      if( cev.trig.sn_hg[bar][0]>act_threshold && cev.trig.sn_hg[bar][1]>act_threshold)
         {
         double c1=teq[tpmt1]*cev.trig.cont_hg[bar][0];
         double c2=teq[tpmt2]*cev.trig.cont_hg[bar][1];
         tsum=tsum+c1+c2;
         tmult++;
         hitbar[bar]=1;     
         tene = tene+((tsum-TriggerP1[bar])/TriggerP0[bar]); 
         }
         
   }// end loop on t bars

  


// loop on planes---------------------------------------------------------------

// reset of calo vars
sumall=0,suma=0,sumb=0, mult=0, conn[16]={0}, enUC=0, ChVsPlane[16]={0};


   for (int pln=0;pln<16;pln++){

    int  pmt1=(pln*2);
    int  pmt2=(pln*2)+1;

// selection of signals above thr of 5 rms on both pmts

      if( cev.scint.sn_hg[pln][0]>act_threshold && cev.scint.sn_hg[pln][1]>act_threshold)
         {
         equ[0]=100./122.528; equ[1]=100./105.288;
         double c1=equ[pmt1]*cev.scint.cont_hg[pln][0];
         double c2=equ[pmt2]*cev.scint.cont_hg[pln][1];
         
         //if(pln==0){std::cout<< equ[pmt1] << " " << equ[pmt2] << " " << cev.scint.cont_hg[pln][0] << " " << cev.scint.cont_hg[pln][1] << std::endl; }


         suma=suma+c1; 
         sumb=sumb+c2;
         sumall=sumall+c1+c2;
         ChVsPlane[pln]=c1+c2;
// range
     //   Range->Fill(pln,1); 
        mult++;
        conn[pln]=1;//
       
        }//end if for hit plhiane
       
      }// end pln loop------------------------------------------------------------

// check data posterior to ptm 5east failure
// then add pmt 5west to sum

      if(hw_condition==1){
      //std::cout<< "post 5East faliure processing"<< std::endl;
      sumall=sumall+(equ[8]*cev.scint.cont_hg[4][0])+(equ[9]*cev.scint.cont_hg[4][1]);
      } 



// max connected length        
        int clen=1;
        for (int i=0;i<16;i++){
        if(conn[i]==0  && i!=4) break; // second part to cope with broken pmt
        clen++;
        }

// energy calc 
         enUC=(sumall-Ek[0])/Ek[1];//MeV
   
 
      

// vetos and LYSO  flags -----------------------------------------------------------
  
int IsVetoBottomHit=0;
IsVetoBottomHit=DeviceStatus(cev.veto.cont_hg[4][0],cev.veto.cont_hg[4][1]);

int IsVetoLatHit=0;
for (int veto=0;veto<4;veto++){IsVetoLatHit=IsVetoLatHit+DeviceStatus(cev.veto.cont_hg[veto][0],cev.veto.cont_hg[veto][1]);}
 
int IsLYSOHit=0;
for (int lys=0;lys<8;lys++){IsLYSOHit=IsLYSOHit+DeviceStatus(cev.lyso.sn_hg[lys][0],cev.lyso.sn_hg[lys][0]);}
// both args are the same pmt
 
}// end event loop

return 0;
}//---------------------------------------------------------------------------




//int LScintillatorsL2::Filter(int name, double *argums){
//int filterval=-1.;
//return filterval;
//}

//---------------------------------------------------------------------------
LScintillatorsL2::~LScintillatorsL2() {
  // do not care about singleton destructor
}
