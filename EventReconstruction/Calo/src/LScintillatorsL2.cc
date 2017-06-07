#include "TH1I.h"
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
int LScintillatorsL2::Calc() {

// histo for debug
TH1D *h1 = new TH1D("","",100,0,4000);

// constants!!

// Trigger equalization array
// it follows HEPD L1 mapping
double teq[12]={172, 100, 178, 207, 471, 324, 244, 462, 174, 191, 100, 380};
for(int t=0;t<12;t++){teq[t]=100./teq[t];}




LEvRec1 cev;
RunFile->SetTheEventPointer(cev);
int nEvents = RunFile->GetEntries();
for (int iEv = 0; iEv < nEvents; iEv++)  // Event loop
  {
    std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2)
              << int(double(iEv) / double(nEvents - 1) * 100) << "%"
              << std::flush;
    RunFile->GetEntry(iEv); 


// compensating pmt 5a
// likely to be put here


// loop on trigger bars ---------------------------------------------------------------
// calc of tsum, tmult


int tsum=0, tmult=0, hitbar[6]={0};
double tene=0;


   for (int bar=0;bar<6;bar++){
  
   int  tpmt1=(bar*2)+0;
   int  tpmt2=(bar*2)+1;

// selection of signals above thr of 5 rms on both pmts
// ip = 01, 

      if( cev.trig.sn_hg[bar][0]>5. && cev.trig.sn_hg[bar][1]>5.
        )
         {
         double c1=teq[tpmt1]*cev.trig.cont_hg[bar][0];
         double c2=teq[tpmt2]*cev.trig.cont_hg[bar][1];
         tsum=tsum+c1+c2;
         tmult++;
         hitbar[bar]=1;     
   
         }
         
   }// end loop on t bars

   h1->Fill(tsum);

/*
// loop on planes---------------------------------------------------------------
// calc of sum, mult, clen

int sumall=0,suma=0,sumb=0, mult=0, conn[16]={0}, enUC=0;;

   for (int pln=1;pln<=16;pln++){

    
    int  pmt1=(pln*2)-2;
    int  pmt2=(pln*2)-1;

// selection of signals above thr of 5 rms on both pmts

      if( scint_snHG[pmt1]>5. && scint_snHG[pmt2]>5. 
        )
         {
         double c1=eq[pmt1]*scint_countHG[pmt1];
         double c2=eq[pmt2]*scint_countHG[pmt2];
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

      if(atoi(argv[2])==2){
      //std::cout<< "post 5East faliure processing"<< std::endl;
      sumall=sumall+(eq[8]*scint_countHG[8])+(eq[9]*scint_countHG[9]);
      } 



// max connected length        
        int clen=0;
        for (int i=0;i<16;i++){
        if(conn[i]==0) break;
        clen++;
        }

// energy calc 
        enUC=(sumall-Ek[0])*Ek[1];//MeV
   


// loop on lat veto ---------------------------------------------------------------
// flag for lat veto hitting 

int IsVetoLatHit=0;


   for (int veto=0;veto<5;veto++){
    
   int veto2=veto+5;

// check for signal within the vetos signals

      if( veto_snHG[veto]>5. && veto_snHG[veto2]>5. ){IsVetoLatHit=1;}
  
      }// end veto loop------------------------------------------------------------


// loop on lat LYSO ---------------------------------------------------------------
// flag for LYSO hitting

int IsLYSOHit=0;


   for (int lys=0;lys<8;lys++){
   

// check for signal within the vetos signals

      if( lyso_snLG[lys]>5.){IsLYSOHit=1;}
  
      }// end veto loop------------------------------------------------------------
*/



}// end event loop


TFile f("histos.root","new");
h1->Write();

return 0;
}
//---------------------------------------------------------------------------

LScintillatorsL2::~LScintillatorsL2() {
  // do not care about singleton destructor
}

