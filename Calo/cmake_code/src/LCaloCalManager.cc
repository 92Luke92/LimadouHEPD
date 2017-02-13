#include "LCaloCalManager.hh"
#include "LEvRec0File.hh"
#include "LEvRec0.hh"

#include <iostream>
#include <math.h>
#include <iomanip> 
#include <fstream>



LCaloCalManager::LCaloCalManager() {
  calRunFile=0;
  InitialTargetRun=-1;
  FinalTargetRun=-1;
  verboseFLAG=true;
}


LCaloCalManager& LCaloCalManager::GetInstance() {
  static LCaloCalManager instance; // Guaranteed to be destroyed.
                                              // Instantiated on first use.
  return instance;
}


//---------------------------------------------------------------------------


int LCaloCalManager::LoadRun(const char *fileInp) {
  // Reset previous loaded runs
  if(calRunFile) {
    if(calRunFile->IsOpen()) calRunFile->Close();
    calRunFile = 0;
    // Sure we want also to reset the target runs? Today we reply yes... check
    InitialTargetRun=-1;
    FinalTargetRun=-1;
  }
  
  calRunFile = new LEvRec0File(fileInp);
  if(!calRunFile || !(calRunFile->IsOpen())) {
    std::cerr << "Error! Attempt to load a calibration run, but the file cannot be opened."
	      << std::endl;
    calRunFile = 0;
    return -999;
  }
  return 0;
}


//---------------------------------------------------------------------------

void LCaloCalManager::SetTargetRuns(const int InitialRun, const int FinalRun) {
  InitialTargetRun=InitialRun;
  FinalTargetRun=FinalRun;
  return;
}


//---------------------------------------------------------------------------

int LCaloCalManager::Devel(int pmtnum, int ntoloop)
{

// for debug ----------
//TH1D *e1 = new TH1D("","ADC",2000,0,2000);// counts 
//---------------------

  LEvRec0 cev;
  int sum=0;
  calRunFile->SetTheEventPointer(cev);
  int nEvents=calRunFile->GetEntries();
  std::cout << "Num of Events on File " << nEvents << std::endl;
  
  // get maximum possible events
  if (ntoloop>nEvents){ ntoloop=nEvents;}
   
  for(int loop=0; loop< ntoloop; loop++){ 
  calRunFile->GetEntry(loop);
  sum = sum + cev.pmt_high[pmtnum];

  };// end loop

 std::cout << " mean on " << ntoloop << " events =  " << (double)sum/ntoloop << std::endl;
return 0;}



//---------------------------------------------------------------------------

int LCaloCalManager::PMTsMeanRms(const char *fileInp , double *HGmean, double *HGrms,  double *LGmean, double *LGrms)
{

        std::vector<double> meanHG(NPMT,0), rmsHG(NPMT, 0), meanLG(NPMT, 0),rmsLG(NPMT, 0);
        
        LEvRec0 cev;        
        calRunFile->SetTheEventPointer(cev);
        int nEvents=calRunFile->GetEntries();

	std::cout << "Events to be processed: " << nEvents << std::endl;
	std::cout << "Events processed: " << std::setprecision(2) << std::setw(2) << 0 << "%" << std::flush;

	for (int iEv = 0; iEv < nEvents; iEv++)// Event loop
	{
		std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2) << int(double(iEv) / double(nEvents - 1) * 100) << "%" << std::flush;
		calRunFile->GetEntry(iEv);
		for (int ch = 0; ch < NPMT; ch++)// PMT channel loop
		{
			meanHG[ch] += cev.pmt_high[ch];
			rmsHG[ch] += cev.pmt_high[ch] * cev.pmt_high[ch];
			meanLG[ch] += cev.pmt_low[ch];
			rmsLG[ch] += cev.pmt_low[ch] * cev.pmt_low[ch];
		}
	}

	std::cout << std::endl;
	for (int iCh = 0; iCh < NPMT; iCh++)
	{
		meanHG[iCh] /= nEvents;
		rmsHG[iCh] /= nEvents;
		rmsHG[iCh] -= meanHG[iCh] * meanHG[iCh];
		rmsHG[iCh] = sqrt(rmsHG[iCh]);
		meanLG[iCh] /= nEvents;
		rmsLG[iCh] /= nEvents;
		rmsLG[iCh] -= meanLG[iCh] * meanLG[iCh];
		rmsLG[iCh] = sqrt(rmsLG[iCh]);
	}
// output
	std::ofstream outHG("debugHG.txt");
	std::ofstream outLG("debugLG.txt");

	for (int i = 0; i < NPMT; i++)
	{
		outHG << i << " " << meanHG[i] << " " << rmsHG[i] << std::endl;
		outLG << i << " " << meanLG[i] << " " << rmsLG[i] << std::endl;
                HGmean[i] =  meanHG[i];
                HGrms[i]  =  rmsHG[i];
                LGmean[i] =  meanLG[i];
                LGrms[i]  =  rmsLG[i];
	}

	outHG.close();
	outLG.close();
	return 0;
        

}

//---------------------------------------------------------------------------




LCaloCalManager::~LCaloCalManager() {
  // do not care about singleton destructor
}


