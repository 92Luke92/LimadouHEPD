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


int LCaloCalManager::PMTsWindowedRmsHG(int pmt, double old_mean, double old_rms,  double *res, int *cntssxdx)
{

        std::vector<double> calc(2,0);
        int outcnts[2]={0};

        LEvRec0 cev;        
        calRunFile->SetTheEventPointer(cev);
        int nEvents=calRunFile->GetEntries();

        double sizew=2.0;
        double maxv = old_mean + (sizew*old_rms);
        double minv = old_mean - (sizew*old_rms);
        int nEventsU=0;
 
	for (int iEv = 0; iEv < nEvents; iEv++)// Event loop
	{
		
		calRunFile->GetEntry(iEv);
		        if( minv < cev.pmt_high[pmt] && cev.pmt_high[pmt] < maxv){
			calc[0] += cev.pmt_high[pmt];
			calc[1] += cev.pmt_high[pmt] * cev.pmt_high[pmt];
 			nEventsU++;}
                         
                        if(cev.pmt_high[pmt] < minv && cev.pmt_high[pmt]!=0 ){outcnts[0]++;}		
                        if(cev.pmt_high[pmt] > maxv  ){outcnts[1]++;}	
	}

	std::cout << std::endl;
	
		calc[0] /= nEventsU;
		calc[1] /= nEventsU;
		calc[1] -= calc[0] * calc[0];
		calc[1] = sqrt(calc[1]);
		
// output
   
       res[0]=calc[0]; res[1]=calc[1]; cntssxdx[0]=outcnts[0]; 	cntssxdx[1]=outcnts[1]; 


	return 0;
        
}

//---------------------------------------------------------------------------
int LCaloCalManager::PMTsWindowedRmsLG(int pmt, double old_mean, double old_rms,  double *res, int *cntssxdx)
{

        std::vector<double> calc(2,0);
        int outcnts[2]={0};

        LEvRec0 cev;        
        calRunFile->SetTheEventPointer(cev);
        int nEvents=calRunFile->GetEntries();

        double sizew=2.0;
        double maxv = old_mean + (sizew*old_rms);
        double minv = old_mean - (sizew*old_rms);
        int nEventsU=0;
 
	for (int iEv = 0; iEv < nEvents; iEv++)// Event loop
	{
		
		calRunFile->GetEntry(iEv);
		        if( minv < cev.pmt_low[pmt] && cev.pmt_low[pmt] < maxv){
			calc[0] += cev.pmt_low[pmt];
			calc[1] += cev.pmt_low[pmt] * cev.pmt_low[pmt];
 			nEventsU++;}
                         
                        if(cev.pmt_low[pmt] < minv && cev.pmt_low[pmt]!=0 ){outcnts[0]++;}		
                        if(cev.pmt_low[pmt] > maxv  ){outcnts[1]++;}	
	}

	std::cout << std::endl;
	
		calc[0] /= nEventsU;
		calc[1] /= nEventsU;
		calc[1] -= calc[0] * calc[0];
		calc[1] = sqrt(calc[1]);
		
// output
   
       res[0]=calc[0]; res[1]=calc[1]; cntssxdx[0]=outcnts[0]; 	cntssxdx[1]=outcnts[1]; 


	return 0;
        
}


//---------------------------------------------------------------------------

int LCaloCalManager::PMTsMomenta34(const char *fileInp, double *HGm3, double *HGm4,  double *LGm3, double *LGm4)
{

        std::vector<double> meanHG(NPMT, 0), meanHG2(NPMT, 0), meanHG3(NPMT, 0), meanHG4(NPMT, 0), 
						meanLG(NPMT, 0), meanLG2(NPMT, 0), meanLG3(NPMT, 0), meanLG4(NPMT, 0),
						sigmaHG(NPMT, 0), skewHG(NPMT, 0), kurtHG(NPMT, 0),
						sigmaLG(NPMT, 0), skewLG(NPMT, 0), kurtLG(NPMT, 0);
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
			double xHG = cev.pmt_high[ch];
			double xLG = cev.pmt_low[ch];

			meanHG[ch]  += xHG;
			meanHG2[ch] += xHG*xHG;
			meanHG3[ch] += xHG*xHG*xHG;
			meanHG4[ch] += xHG*xHG*xHG*xHG;
			
			meanLG[ch]  += xLG;
			meanLG2[ch] += xLG*xLG;
			meanLG3[ch] += xLG*xLG*xLG;
			meanLG4[ch] += xLG*xLG*xLG*xLG;




		}
	}

	std::cout << std::endl;
	for (int iCh = 0; iCh < NPMT; iCh++)
	{
		meanHG[iCh]  /= nEvents;
		meanHG2[iCh] /= nEvents;
		meanHG3[iCh] /= nEvents;
		meanHG4[iCh] /= nEvents;
		
		meanLG[iCh]  /= nEvents;
		meanLG2[iCh] /= nEvents;
		meanLG3[iCh] /= nEvents;
		meanLG4[iCh] /= nEvents;
	}
// output
	double sigmaSkew = sqrt(6. / static_cast<double>(nEvents));
	double sigmaKurt = sqrt(24. / static_cast<double>(nEvents));
	for (int iCh = 0; iCh < NPMT; iCh++)
	{
		double m2 = meanHG2[iCh] - meanHG[iCh] * meanHG[iCh];
		double m3 = meanHG3[iCh] - 3 * meanHG2[iCh] * meanHG[iCh] + 2 * meanHG[iCh] * meanHG[iCh] * meanHG[iCh];
		double m4 = meanHG4[iCh] - 4 * meanHG3[iCh] * meanHG[iCh] + 6 * meanHG2[iCh] * meanHG[iCh] * meanHG[iCh] - 3 * meanHG[iCh] * meanHG[iCh] * meanHG[iCh] * meanHG[iCh];
		sigmaHG[iCh] = sqrt(m2);
		skewHG[iCh]  = m3/(m2*sqrt(m2)); HGm3[iCh]=skewHG[iCh];
		kurtHG[iCh]  = m4 / (m2*m2) -3;  HGm4[iCh]=kurtHG[iCh];
		
		m2 = meanLG2[iCh] - meanLG[iCh] * meanLG[iCh];
		m3 = meanLG3[iCh] - 3 * meanLG2[iCh] * meanLG[iCh] + 2 * meanLG[iCh] * meanLG[iCh] * meanLG[iCh];
		m4 = meanLG4[iCh] - 4 * meanLG3[iCh] * meanLG[iCh] + 6 * meanLG2[iCh] * meanLG[iCh] * meanLG[iCh] - 3 * meanLG[iCh] * meanLG[iCh] * meanLG[iCh] * meanLG[iCh];
		sigmaLG[iCh] = sqrt(m2); 
		skewLG[iCh]  = m3 / (m2*sqrt(m2)); LGm3[iCh]=skewLG[iCh];
		kurtLG[iCh]  = m4 / (m2*m2) - 3;   LGm4[iCh]=kurtLG[iCh];
	}


// out 
	std::ofstream outmHG("MomDebugHG.txt");
	std::ofstream outmLG("MomDebugLG.txt");

	for (int i = 0; i < NPMT; i++)
	{
		outmHG << i << " " << skewHG[i] << " " << skewHG[i] / sigmaSkew << " " << kurtHG[i] << " " << kurtHG[i]/sigmaKurt << std::endl;
		outmLG << i << " " << skewLG[i] << " " << skewLG[i] / sigmaSkew << " " << kurtLG[i] << " " << kurtLG[i]/sigmaKurt << std::endl;
	}
	outmHG.close();
	outmLG.close();

	return 0;
}

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





//---------------------------------------------------------------------------


LCaloCalManager::~LCaloCalManager() {
  // do not care about singleton destructor
}


