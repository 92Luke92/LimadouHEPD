#include "LCaloCalibrationManager.hh"
#include "LEvRec0.hh"
#include "LEvRec0File.hh"
#include "LStatTools.hh"

#include <math.h>
#include <fstream>
#include <iomanip>
#include <iostream>

LCaloCalibrationManager::LCaloCalibrationManager() {
  calRunFile = 0;
  InitialTargetRun = -1;
  FinalTargetRun = -1;
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
    InitialTargetRun = -1;
    FinalTargetRun = -1;
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

void LCaloCalibrationManager::SetTargetRuns(const int InitialRun,
                                            const int FinalRun) {
  InitialTargetRun = InitialRun;
  FinalTargetRun = FinalRun;
  return;
}

//---------------------------------------------------------------------------

int *LCaloCalibrationManager::GetPeaksHG() const {
  int *result = new int[NPMT];
  for (int iCh = 0; iCh < NPMT; ++iCh) result[iCh] = GetPeakHG(iCh);
  return result;
}

//---------------------------------------------------------------------------

int *LCaloCalibrationManager::GetPeaksLG() const {
  int *result = new int[NPMT];
  for (int iCh = 0; iCh < NPMT; ++iCh) result[iCh] = GetPeakLG(iCh);
  return result;
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
  std::cout << __LCALOCALIBRATIONMANAGER__ << "Finding peak for pmt number "
            << pmtnum << std::endl;
  int spectrum[PeakFinderWindowWidth] = {0};
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
  std::map  <int, float>   calc [NPMT];
  int outcnts[NPMT][2]={{0}};

  LEvRec0 cev;
  calRunFile->SetTheEventPointer(cev);
  //  const int nEvents=calRunFile->GetEntries();

  const double sizew = RMSFINDINGHALFWINDOW;
  double maxv[NPMT], minv[NPMT];
  for (int iCh = 0; iCh < NPMT; ++iCh) {
    maxv[iCh] = old_mean[iCh] + (sizew * old_rms[iCh]);
    minv[iCh] = old_mean[iCh] - (sizew * old_rms[iCh]);
  }


  // for (int iEv = 0; iEv < nEvents; ++iEv) { // Event loop
  for (int iEv = __skipEv; iEv < __nEv; ++iEv) {  // Event loop
    calRunFile->GetEntry(iEv);
    for (int iCh = 0; iCh < NPMT; ++iCh) {
      double content = (isHG ? static_cast<double>(cev.pmt_high[iCh])
                             : static_cast<double>(cev.pmt_low[iCh]));
      if (minv[iCh] < content && content < maxv[iCh] && cev.trigger_flag[iCh] == 0) {
            calc[iCh][content] ++;
      }
    }
  }


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
  // for (int iEv = 0; iEv < nEvents; ++iEv) { // Event loop
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

  std::map  <int, float>   histo[NPMT];

  LEvRec0 cev;
  calRunFile->SetTheEventPointer(cev);


  for (int iEv = __skipEv; iEv < __nEv; ++iEv) {  // Event loop
    calRunFile->GetEntry(iEv);

    for (int ch = 0; ch < NPMT; ++ch) {  // PMT channel loop
      unsigned short content = (isHG ? cev.pmt_high[ch] : cev.pmt_low[ch]);
      if (float(content) > pedestal[ch] + SKEWKURTFINDINGHALFWINDOW * sigmaIN[ch] ||
          float(content) < pedestal[ch] - SKEWKURTFINDINGHALFWINDOW * sigmaIN[ch] ||
          cev.trigger_flag[ch] != 0)
        continue;

      histo[ch][content]++;
    }
  }

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
  std::vector<double> mean(NPMT, 0), rms(NPMT, 0), usedEVTS(NPMT, 0);
  std::map  <int, float>   histo[NPMT];

  LEvRec0 cev;
  calRunFile->SetTheEventPointer(cev);


  for (int iEv = __skipEv; iEv < __nEv; ++iEv) {  // Event loop
    //    std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2) <<
    //    int(double(iEv) / double(nEvents - 1) * 100) << "%" << std::flush;
    calRunFile->GetEntry(iEv);
    for (int ch = 0; ch < NPMT; ++ch) {  // PMT channel loop
      int content = (isHG ? static_cast<int>(cev.pmt_high[ch])
                             : static_cast<int>(cev.pmt_low[ch]));
      if (cev.trigger_flag[ch] == 0)   histo[ch][content]++;
    }
  }

  for (int iCh = 0; iCh < NPMT; ++iCh) {
    LStatTools stat(histo[iCh]);
    meanOut[iCh] = stat.mean();
    rmsOut[iCh] = stat.mean();
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
  int nEventsU = 0;

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
  int *peaks = isHG ? GetPeaksHG() : GetPeaksLG();


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
      new LCaloCalibration(RunId, InitialTargetRun, FinalTargetRun, ped,
                           sigma);  //, outliers, skewness, kurtosis);

  return result;
}

//---------------------------------------------------------------------------

LCaloCalibrationManager::~LCaloCalibrationManager() {
  // do not care about singleton destructor
}
