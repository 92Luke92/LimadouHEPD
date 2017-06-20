// here to start documentation
// author: vincenzo vitale, vvitale@roma2.infn.it
//
// class purpose: collection of algorithm for level2 observables
// obtained with the calorimeter and vetos measuremets




#include "TH1.h"
#include "LEvRec1File.hh"
#include "LScintillatorsL2.hh"
#include <iostream>



LScintillatorsL2::LScintillatorsL2() {
  
}


LScintillatorsL2 &LScintillatorsL2::GetInstance() {
  static LScintillatorsL2 instance;  // Guaranteed to be destroyed.
  return instance;
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


int LScintillatorsL2::Test(int bar) {


LEvRec1 cev;
RunFile->SetTheEventPointer(cev);
int nEvents = RunFile->GetEntries();



for (int iEv = 0; iEv < nEvents; iEv++)  // Event loop
  {
   
    RunFile->GetEntry(iEv);
int bar= 3;
std::cout << iEv <<  " " << cev.trig.cont_hg[bar][0] << std::endl;

}

//TFile f("histos.root","new");
//h1->Write();
return 0;
}

//---------------------------------------------------------------------------
int LScintillatorsL2::Calc(int hw_condition) {

// histo for debug-----------------------

TH1D *h1 = new TH1D("","",330,0,200);
//---------------------------------------------------------------------------




// as this is a development version, several needed constants  are temp. hardcoded
// these will be in futere read from energy calibration input cards
//---




double act_threshold= 5.00;// general threshold to declare a device hit

// here varibles which contain the future output
int tsum=0, tmult=0, hitbar[6]={0}; double tene=0; // trigger vars
int sumall=0,suma=0,sumb=0, mult=0, conn[16]={0}, enUC=0;// calo vars


// here temp hardcoded calibration constants

// Trigger equalization array, it follows HEPD L1 mapping

double teq[12]={40.5,28.0, 52.8,58.5,87.7,70.5,57.3,95.3,50.5,56.3,30.1,80};

for(int t=0;t<12;t++){teq[t]=100./teq[t];}// setting equ peaks at 100!

// Upper Calo equalization array it follows HEPD L1 mapping
double eq[32]={
122.528, 105.288, 
122.851, 85.309, 
157.372, 105.657, 
192.692, 169.451, 
106.393, 95.841, 
95.1923, 92.7526, 
182.96, 174.202, 
168.76, 157.614, 
82.4866, 153.741, 
133.773, 100.741, 
119.955, 145.598, 
133.313, 114.355, 
161.308, 149.244, 
170.022, 204.684, 
146.195, 172.694, 
166.352, 152.972};
for(int t=0;t<32;t++){eq[t]=100./eq[t];}

// energy calibration constants

double Ek[2]={164, 1./104.5};//MeV/adc_counts
double TriggerP0[6]={728,593,1227,1066, 616, 801}, TriggerP1[6]={-44,+45,-517,-361,-15,-98};// S=(p0*E) + p1



//---------------------------------------------------------------------------

// connecting data
LEvRec1 cev;
RunFile->SetTheEventPointer(cev);
//---------------------------------------------------------------------------

// loop on data events
 
int nEvents = RunFile->GetEntries();
for (int iEv = 0; iEv < nEvents; iEv++)  // Event loop
  {
    std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2)
              << int(double(iEv) / double(nEvents - 1) * 100) << "%"
              << std::flush;
    RunFile->GetEntry(iEv); 


// compensation for pmt 5a likely to be put here


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
sumall=0,suma=0,sumb=0, mult=0, conn[16]={0}, enUC=0;

   for (int pln=0;pln<16;pln++){

   
    int  pmt1=(pln*2);
    int  pmt2=(pln*2)+1;

// selection of signals above thr of 5 rms on both pmts

      if( cev.scint.sn_hg[pln][0]>act_threshold && cev.scint.sn_hg[pln][1]>act_threshold)
         {
         double c1=eq[pmt1]*cev.scint.cont_hg[pln][0];
         double c2=eq[pmt2]*cev.scint.cont_hg[pln][1];
         suma=suma+c1; 
         sumb=sumb+c2;
         sumall=sumall+c1+c2;
// range
     //   Range->Fill(pln,1); 
        mult++;
        conn[pln-1]=1;// index= plane -1
       
        }//end if for hit plhiane
       
      }// end pln loop------------------------------------------------------------

// check data posterior to ptm 5east failure
// then add pmt 5west to sum

      if(hw_condition==1){
      //std::cout<< "post 5East faliure processing"<< std::endl;
      sumall=sumall+(eq[8]*cev.scint.cont_hg[4][0])+(eq[9]*cev.scint.cont_hg[4][1]);
      } 



// max connected length        
        int clen=0;
        for (int i=0;i<16;i++){
        if(conn[i]==0) break;
        clen++;
        }

// energy calc 
        enUC=(sumall-Ek[0])*Ek[1];//MeV
   
        h1->Fill(enUC);

// loop on lat veto ---------------------------------------------------------------
// flag for lat veto hitting 

int IsVetoLatHit=0;


   for (int veto=0;veto<5;veto++){
    
   int veto2=veto+5;

// check for signal within the vetos signals

      if( cev.veto.cont_hg[veto][0]>act_threshold && cev.veto.cont_hg[veto][0]>act_threshold ){IsVetoLatHit=1;}
  
      }// end veto loop------------------------------------------------------------


// loop on lat LYSO ---------------------------------------------------------------
// flag for LYSO hitting

int IsLYSOHit=0;


   for (int lys=0;lys<8;lys++){
   

// check for signal within the vetos signals

      if( cev.lyso.sn_hg[lys][0]>act_threshold){IsLYSOHit=1;}
  
      }// end veto loop------------------------------------------------------------




}// end event loop


TFile f("histos.root","new");
h1->Write();

return 0;
}
//---------------------------------------------------------------------------

int LScintillatorsL2::Filter(int type, double *argums){
int filterval=-1.;
if(1){}
return filterval;

}

//---------------------------------------------------------------------------
LScintillatorsL2::~LScintillatorsL2() {
  // do not care about singleton destructor
}

