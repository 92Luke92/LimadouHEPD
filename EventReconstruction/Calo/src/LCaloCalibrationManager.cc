// here to start documentation
// co-author: vincenzo vitale, vvitale@roma2.infn.it
//
// class purpose: pedestal calibration
// and management of output files


#include "LCaloCalibrationManager.hh"
#include "LEvRec0.hh"
#include "LEvRec0File.hh"


#include <math.h>
#include <fstream>
#include <iomanip>
#include <iostream>

LCaloCalibrationManager::LCaloCalibrationManager() {
  calRunFile = 0;
  verboseFLAG = true;
  __skipEv = 0;
  __nEv = 0;
}

LCaloCalibrationManager &LCaloCalibrationManager::GetInstance() {
  static LCaloCalibrationManager instance;  // Guaranteed to be destroyed.
                                            // Instantiated on first use.
  return instance;
}

//---------------------------------------------------------------------------

int LCaloCalibrationManager::LoadRun(const char *fileInp) {
  // Reset previous loaded runs
  if (calRunFile) {
    if (calRunFile->IsOpen()) calRunFile->Close();
    calRunFile = 0;
    // Sure we want also to reset the target runs? Today we reply yes... check
    __skipEv = 0;
    __nEv = 0;
  }

  calRunFile = new LEvRec0File(fileInp);
  if (!calRunFile || !(calRunFile->IsOpen())) {
    std::cerr << __LCALOCALIBRATIONMANAGER__
              << "Error! Attempt to load a calibration run, but the file "
                 "cannot be opened."
              << std::endl;
    calRunFile = 0;
    return -999;
  }
  return 0;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::GetPeaksHG(int *result) const {

  const bool isHG=true;
  auto predicate = [&](int cursor, bool trigger_flag, int iCh)
    {(void)iCh; return cursor < HGPEAKFINDERWINDOW && trigger_flag==0;};
  std::vector <std::map  <int, float>>  spectrum=MapCalibFromPredicate(predicate, isHG);

  for (int iCh=0; iCh< NPMT ; iCh++) {
    int maxval = 0;
    int peakpos = -1;
    for (int loop1 = 0; loop1 < HGPEAKFINDERWINDOW; ++loop1) {
      if (spectrum[iCh][loop1] > maxval) {
        maxval = spectrum[iCh][loop1];
        peakpos = loop1;
      }
    }
    result[iCh] = peakpos;
  }

  return;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::GetPeaksLG(int *result) const {
  const bool isHG=false;
  auto predicate = [&](int cursor, bool trigger_flag, int iCh)
    {(void)iCh; return cursor < LGPEAKFINDERWINDOW && trigger_flag==0;};
  std::vector <std::map  <int, float>>  spectrum=MapCalibFromPredicate(predicate, isHG);

  for (int iCh=0; iCh< NPMT ; iCh++) {
    int maxval = 0;
    int peakpos = -1;
    for (int loop1 = 0; loop1 < LGPEAKFINDERWINDOW; ++loop1) {
      if (spectrum[iCh][loop1] > maxval) {
        maxval = spectrum[iCh][loop1];
        peakpos = loop1;
      }
    }
    result[iCh] = peakpos;
  }

  return;
}

//---------------------------------------------------------------------------

int LCaloCalibrationManager::GetPeakHG(const int pmtnum) const {
  const bool isHG = true;
  return FindPeak(pmtnum, HGPEAKFINDERWINDOW, isHG);
}

//---------------------------------------------------------------------------

int LCaloCalibrationManager::GetPeakLG(const int pmtnum) const {
  const bool isHG = false;
  return FindPeak(pmtnum, LGPEAKFINDERWINDOW, isHG);
}

//---------------------------------------------------------------------------

int LCaloCalibrationManager::FindPeak(const int pmtnum,
                                      const int PeakFinderWindowWidth,
                                      const bool isHG) const {

//docum.
// FindPeak is a methods to search for a peak within the PMT adc spectrum. 
// Data are binned within the spectrum array. Then a maximum search is applied.
// It is used to seed the PMTsWindowedRms method.



  std::cout << __LCALOCALIBRATIONMANAGER__ << "Finding peak for pmt number "
            << pmtnum << std::endl;
  int* spectrum= new int[PeakFinderWindowWidth];
  spectrum={0};
  int peakpos = -1;
  LEvRec0 cev;
  calRunFile->SetTheEventPointer(cev);
  // int nEvents=calRunFile->GetEntries();
  // std::cout << "Num of Events on File " << nEvents << std::endl;

  // fill spectrum
  for (int loop = __skipEv; loop < __nEv; ++loop) {
    calRunFile->GetEntry(loop);
    int cursor = (isHG ? static_cast<int>(cev.pmt_high[pmtnum])
                   : static_cast<int>(cev.pmt_low[pmtnum]));
    if (cursor < PeakFinderWindowWidth && cev.trigger_flag[pmtnum] == 0) {
      ++spectrum[cursor];
    }
  }  // end loop

  // search maximum
  int maxval = 0;
  for (int loop1 = 0; loop1 < PeakFinderWindowWidth; ++loop1) {
    if (spectrum[loop1] > maxval) {
      maxval = spectrum[loop1];
      peakpos = loop1;
    }
  }

  delete[] spectrum;

  return peakpos;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsWindowedRmsHG(const double *old_mean,
                                                const double *old_rms,
                                                double *new_mean,
                                                double *new_rms,
                                                int *cntssxdx) const {
  const bool isHG = true;
  PMTsWindowedRms(old_mean, old_rms, isHG, new_mean, new_rms, cntssxdx);
  return;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsWindowedRmsLG(const double *old_mean,
                                                const double *old_rms,
                                                double *new_mean,
                                                double *new_rms,
                                                int *cntssxdx) const {
  const bool isHG = false;
  PMTsWindowedRms(old_mean, old_rms, isHG, new_mean, new_rms, cntssxdx);
  return;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsWindowedRms(const double *old_mean,
                                              const double *old_rms,
                                              const bool isHG, double *new_mean,
                                              double *new_rms,
                                              int *cntssxdx) const {

//docum.
// PMTsWindowedRms is a more general method for the pedestal (and pedestal rms) calculation in respect of PMTsWindowedRms.
// Currently (june 2017) PMTsWindowedRms provides the same results of PMTsRawMeanRms only if used with 
// "fake-calibration data". With run-data the obtained rms are not under control.
// With PMTsWindowedRms the  pedestal search is limited within an acceptable window, hence the method name.
// A correction factor is used to correct the rms calculation, because the used window cuts the
// distribution lateral tails. It needs in input an estimate of the pedestal position.


  int outcnts[NPMT][2]={{0}};


  const double sizew = RMSFINDINGHALFWINDOW;
  double maxv[NPMT], minv[NPMT];
  for (int iCh = 0; iCh < NPMT; ++iCh) {
    maxv[iCh] = old_mean[iCh] + (sizew * old_rms[iCh]);
    minv[iCh] = old_mean[iCh] - (sizew * old_rms[iCh]);
  }


  auto predicate = [&](int content, bool trigger_flag, int iCh)
    {return minv[iCh] < content && content < maxv[iCh] && trigger_flag == 0;};
  std::vector <std::map  <int, float>> calc=MapCalibFromPredicate(predicate, isHG);

  // output mean rms
  double correction_factor =
      RMSCORRECTIONFACTOR +
      (isHG ? 0.01 : 0.02);  // why this over correction???????????
  for (int iCh = 0; iCh < NPMT; ++iCh) {
      LStatTools stat(calc[iCh]);
      new_mean[iCh] = stat.mean();
      new_rms[iCh] = stat.rms() * correction_factor;
  // Outliers
    maxv[iCh] = new_mean[iCh] + (sizew * new_rms[iCh]);
    minv[iCh] = new_mean[iCh] - (sizew * new_rms[iCh]);
  }


  LEvRec0 cev;
  calRunFile->SetTheEventPointer(cev);

  // If we don't use independently outcnts[0] and  [1], reduce through MapCalibFromPredicate

  for (int iEv = __skipEv; iEv < __nEv; ++iEv) {  // Event loop
    calRunFile->GetEntry(iEv);
    for (int iCh = 0; iCh < NPMT; ++iCh) {
      double content = (isHG ? static_cast<double>(cev.pmt_high[iCh])
                             : static_cast<double>(cev.pmt_low[iCh]));
      if (content < minv[iCh] && content != 0.)    ++outcnts[iCh][0];
      if (content > maxv[iCh])            ++outcnts[iCh][1];
    }
  }
  // output outliers
  for (int iCh = 0; iCh < NPMT; ++iCh) {
    cntssxdx[iCh] = outcnts[iCh][0] + outcnts[iCh][1];
  }

  return;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsMomenta34LG(const double *pedestal,
                                              const double *sigma, double *m3,
                                              double *m4) const {
  bool isHG = true;
  PMTsMomenta34(pedestal, sigma, isHG, m3, m4);
  return;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsMomenta34HG(const double *pedestal,
                                              const double *sigma, double *m3,
                                              double *m4) const {
  bool isHG = true;
  PMTsMomenta34(pedestal, sigma, isHG, m3, m4);
  return;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsMomenta34(const double *pedestal,
                                            const double *sigmaIN,
                                            const bool isHG, double *m3Out,
                                            double *m4Out) const {



  auto predicate = [&](int content, bool trigger_flag, int iCh)
  {return (float(content) < pedestal[iCh] + SKEWKURTFINDINGHALFWINDOW * sigmaIN[iCh] &&
          float(content) > pedestal[iCh] - SKEWKURTFINDINGHALFWINDOW * sigmaIN[iCh] &&
          trigger_flag == 0);};
  std::vector <std::map  <int, float>> histo=MapCalibFromPredicate(predicate, isHG);

  // output
  for (int iCh = 0; iCh < NPMT; ++iCh) {
    LStatTools stat(histo[iCh]);
    m3Out[iCh] = stat.skewness();
    m4Out[iCh] = stat.norm_kurtosis() * KURTOSISCORRECTIONFACTOR;
  }

  return;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsRawMeanRmsHG(double *mean,
                                               double *rms) const {
  const bool isHG = true;
  PMTsRawMeanRms(isHG, mean, rms);
  return;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsRawMeanRmsLG(double *mean,
                                               double *rms) const {
  const bool isHG = false;
  PMTsRawMeanRms(isHG, mean, rms);
  return;
}

//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsRawMeanRms(const bool isHG, double *meanOut,
                                             double *rmsOut) const {
//docum.
// PMTsMeanRms is the main method for the pedestal (and pedestal rms) calculation.
// It can be used only on "fake-calibration data" as the run-data provide polluted pedestals
// and no pedestals for the PMT in trigger.
// the pedestal peak is the mean of the adc spectrum, while the pedestal rms is the 
// rms associated at the pedstal peak.
// Mean and sigma calculation are now performed with LStatTools::mean and ::sigma. Previuosly they 
// where hardcoded here.



  auto predicate = [&](int content, bool trigger_flag, int iCh)
    {(void)content; (void) iCh; return trigger_flag == 0;};
  std::vector <std::map  <int, float>> calc=MapCalibFromPredicate(predicate, isHG);

  for (int iCh = 0; iCh < NPMT; ++iCh) {
    LStatTools stat(calc[iCh]);
    meanOut[iCh] = stat.mean();
    rmsOut[iCh] = stat.rms();
  }

  return;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsMeanRmsData(const int pmt,
                                              double *res) const {


   std::map  <int, float>   calc;

  LEvRec0 cev;
  calRunFile->SetTheEventPointer(cev);
  // int nEvents=calRunFile->GetEntries();

  const double maxv = DATACALWINDOWMAX;
  const double minv = DATACALWINDOWMIN;

  for (int iEv = __skipEv; iEv < __nEv; ++iEv) {  // Event loop
    calRunFile->GetEntry(iEv);
    double signal = static_cast<double>(cev.pmt_high[pmt]);
    if (minv < signal && signal < maxv && cev.trigger_flag[pmt] == 0) calc[signal] ++;
  }

 LStatTools stat(calc);
  res[0] = stat.mean();
  res[1] = stat.rms();

  return;
}

//---------------------------------------------------------------------------

LCaloCalibration *LCaloCalibrationManager::Calibrate(const bool isHG,
                                                     const int nEvents,
                                                     const int skipEvents) {
  if (calRunFile == 0 || !(calRunFile->IsOpen())) {
    std::cerr << __LCALOCALIBRATIONMANAGER__
              << "Error! Attempt to call the \"Calibrate\" method, but no "
                 "calibration run loaded."
              << std::endl;
    return 0;
  }

  int nEntries = calRunFile->GetEntries();
  __nEv = nEvents;
  __skipEv = skipEvents;
  if (nEvents == -1) __nEv = nEntries;
  if (skipEvents == -1) __skipEv = 0;

  if (nEntries < __skipEv + __nEv) {
    std::cerr << __LCALOCALIBRATIONMANAGER__
              << "Impossible to go ahead: nEntries < __skipEv+__nEv"
              << std::endl;
    return 0;
  }

  if (verboseFLAG)
    std::cout << __LCALOCALIBRATIONMANAGER__
              << (isHG ? "High gain " : "Low gain ") << "calibration started"
              << std::endl;

  // Pedestals and sigmas
  double ped0[NPMT], sigma0[NPMT];  // zero approximation
  double ped1[NPMT], sigma1[NPMT];  // first approximation
  double ped[NPMT], sigma[NPMT];
  int outliers[NPMT];
  // double skewness[NPMT], kurtosis[NPMT];

  // Zero approximation initialization
  int peaks[NPMT];
  if(isHG==true) GetPeaksHG(peaks);
  else GetPeaksLG(peaks);
    
  for (int iCh = 0; iCh < NPMT; ++iCh) {
    ped0[iCh] = static_cast<double>(peaks[iCh]);
    sigma0[iCh] = STARTINGSIGMA;
  }
  
  if (verboseFLAG)
    std::cout << __LCALOCALIBRATIONMANAGER__ << " 0th approx. done"  << std::endl;
  
  // First approximation
  PMTsWindowedRms(ped0, sigma0, isHG, ped1, sigma1, outliers);
  if (verboseFLAG)
    std::cout << __LCALOCALIBRATIONMANAGER__ << " 1th approx. done"  << std::endl;
  
  // Second approximation
  PMTsWindowedRms(ped1, sigma1, isHG, ped, sigma, outliers);
  if (verboseFLAG)
    std::cout << __LCALOCALIBRATIONMANAGER__ << " 2th approx. done"  << std::endl;
  
  // 3-4 Momenta
  //    PMTsMomenta34HG(ped, sigma, skewness, kurtosis);
  //    if(verboseFLAG)  std::cout << __LCALOCALIBRATIONMANAGER__
  //			       << " 3-4 momenta done" << std::endl;
  

  int RunId = calRunFile->GetRunId();
  LCaloCalibration *result =
      new LCaloCalibration(RunId, ped, sigma);  //, outliers, skewness, kurtosis);

  return result;
}

//---------------------------------------------------------------------------


std::vector <std::map  <int, float>> LCaloCalibrationManager::MapCalibFromPredicate(std::function <bool(int content, bool trigger_flag, int channel)> predicate, const bool isHG) const {
  std::vector <std::map  <int, float>>  histo(NPMT);
  LEvRec0 cev;
  bool MixedFLAG = calRunFile->IsMixed(); // important to see if we have to skip half the events
  calRunFile->SetTheEventPointer(cev);
  for (int iEv = __skipEv; iEv < __nEv; iEv++) {  // Event loop
    calRunFile->GetEntry(iEv);
    if(MixedFLAG==true && cev.IsZeroSuppressed()) continue;
    for (int iCh = 0; iCh < NPMT; iCh++) {
      int content = isHG ? cev.pmt_high[iCh] : cev.pmt_low[iCh];
      if (predicate(content, cev.trigger_flag[iCh], iCh))
        histo[iCh][content]++;
    }
  }
  return histo;
}




//---------------------------------------------------------------------------


LCaloCalibrationManager::~LCaloCalibrationManager() {
  // do not care about singleton destructor
}
