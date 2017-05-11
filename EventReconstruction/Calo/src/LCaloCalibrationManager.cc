#include "LCaloCalibrationManager.hh"
#include "LEvRec0.hh"
#include "LEvRec0File.hh"

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
  double calc[NPMT][2]={{0}};
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

  int nEventsU[NPMT];
  for (int iCh = 0; iCh < NPMT; ++iCh) nEventsU[iCh] = 0;
  // for (int iEv = 0; iEv < nEvents; ++iEv) { // Event loop
  for (int iEv = __skipEv; iEv < __nEv; ++iEv) {  // Event loop
    calRunFile->GetEntry(iEv);
    for (int iCh = 0; iCh < NPMT; ++iCh) {
      double content = (isHG ? static_cast<double>(cev.pmt_high[iCh])
                             : static_cast<double>(cev.pmt_low[iCh]));
      if (minv[iCh] < content && content < maxv[iCh] &&
          cev.trigger_flag[iCh] == 0) {
            calc[iCh][0] += content;
            calc[iCh][1] += (content * content);
            ++nEventsU[iCh];
      }
    }
  }

  // std::cout << std::endl;
  for (int iCh = 0; iCh < NPMT; ++iCh) {
    calc[iCh][0] /= nEventsU[iCh];
    calc[iCh][1] /= nEventsU[iCh];
    calc[iCh][1] -= (calc[iCh][0] * calc[iCh][0]);
    calc[iCh][1] = sqrt(calc[iCh][1]);
  }
  // output mean rms
  double correction_factor =
      RMSCORRECTIONFACTOR +
      (isHG ? 0.01 : 0.02);  // why this over correction???????????
  for (int iCh = 0; iCh < NPMT; ++iCh) {
    new_mean[iCh] = calc[iCh][0];
    new_rms[iCh] = calc[iCh][1] * correction_factor;
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
      if (content < minv[iCh] && content != 0.) {
        ++outcnts[iCh][0];
      }
      if (content > maxv[iCh]) {
        ++outcnts[iCh][1];
      }
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
  std::vector<double> mean(NPMT, 0), mean2(NPMT, 0), mean3(NPMT, 0),
      mean4(NPMT, 0), sigma(NPMT, 0), skew(NPMT, 0), kurt(NPMT, 0);
  std::vector<int> nEventsU(NPMT, 0);

  LEvRec0 cev;
  calRunFile->SetTheEventPointer(cev);
  // const int nEvents=calRunFile->GetEntries();

  //  std::cout << "Events to be processed: " << nEvents << std::endl;
  // std::cout << "Events processed: " << std::setprecision(2) << std::setw(2)
  // << 0 << "%" << std::flush;

  //  for (int iEv = 0; iEv < nEvents; ++iEv) { // Event loop
  for (int iEv = __skipEv; iEv < __nEv; ++iEv) {  // Event loop
    // std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2) <<
    // int(double(iEv) / double(nEvents - 1) * 100) << "%" << std::flush;
    calRunFile->GetEntry(iEv);

    for (int ch = 0; ch < NPMT; ++ch) {  // PMT channel loop
      double content = (isHG ? static_cast<double>(cev.pmt_high[ch])
                             : static_cast<double>(cev.pmt_low[ch]));
      if (content > pedestal[ch] + SKEWKURTFINDINGHALFWINDOW * sigmaIN[ch] ||
          content < pedestal[ch] - SKEWKURTFINDINGHALFWINDOW * sigmaIN[ch] ||
          cev.trigger_flag[ch] != 0)
        continue;

      mean[ch] += content;
      mean2[ch] += (content * content);
      mean3[ch] += (content * content * content);
      mean4[ch] += (content * content * content * content);
      ++nEventsU[ch];
    }
  }

  // std::cout << std::endl;
  for (int iCh = 0; iCh < NPMT; ++iCh) {
    mean[iCh] /= nEventsU[iCh];
    mean2[iCh] /= nEventsU[iCh];
    mean3[iCh] /= nEventsU[iCh];
    mean4[iCh] /= nEventsU[iCh];
  }

  // output
  for (int iCh = 0; iCh < NPMT; ++iCh) {
    double m2 = mean2[iCh] - mean[iCh] * mean[iCh];
    double m3 = mean3[iCh] - 3 * mean2[iCh] * mean[iCh] +
                2 * mean[iCh] * mean[iCh] * mean[iCh];
    double m4 = mean4[iCh] - 4 * mean3[iCh] * mean[iCh] +
                6 * mean2[iCh] * mean[iCh] * mean[iCh] -
                3 * mean[iCh] * mean[iCh] * mean[iCh] * mean[iCh];
    sigma[iCh] = sqrt(m2);
    skew[iCh] = m3 / (m2 * sqrt(m2));
    m3Out[iCh] = skew[iCh];
    kurt[iCh] = m4 / (m2 * m2) - 3.;
    m4Out[iCh] = kurt[iCh] * KURTOSISCORRECTIONFACTOR;
    ;
  }

  /*
  // out
  double sigmaSkew = sqrt(6. / static_cast<double>(nEvents or __nEv??));
  double sigmaKurt = sqrt(24. / static_cast<double>(nEvents or __nEv??));
  std::ofstream outm("MomDebug.txt");
  for (int i = 0; i < NPMT; ++i){
    outm << i << " " << skew[i] << " " << skew[i] / sigmaSkew << " " << kurt[i]
  << " " << kurt[i]/sigmaKurt << std::endl;
  }
  outm.close();
  */
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

  LEvRec0 cev;
  calRunFile->SetTheEventPointer(cev);
  // const int nEvents=calRunFile->GetEntries();

  // std::cout << "Events to be processed: " << nEvents << std::endl;
  // std::cout << "Events processed: " << std::setprecision(2) << std::setw(2)
  // << 0 << "%" << std::flush;

  //  for (int iEv = 0; iEv < nEvents; ++iEv) { // Event loop
  for (int iEv = __skipEv; iEv < __nEv; ++iEv) {  // Event loop
    //    std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2) <<
    //    int(double(iEv) / double(nEvents - 1) * 100) << "%" << std::flush;
    calRunFile->GetEntry(iEv);
    for (int ch = 0; ch < NPMT; ++ch) {  // PMT channel loop
      double content = (isHG ? static_cast<int>(cev.pmt_high[ch])
                             : static_cast<int>(cev.pmt_low[ch]));
      if (cev.trigger_flag[ch] == 0) {
        ++usedEVTS[ch];
        mean[ch] += content;
        rms[ch] += (content * content);
      }
    }
  }

  for (int iCh = 0; iCh < NPMT; ++iCh) {
    mean[iCh] /= usedEVTS[iCh];
    rms[iCh] /= usedEVTS[iCh];
    rms[iCh] -= (mean[iCh] * mean[iCh]);
    rms[iCh] = sqrt(rms[iCh]);
  }

  for (int iCh = 0; iCh < NPMT; ++iCh) {
    meanOut[iCh] = mean[iCh];
    rmsOut[iCh] = rms[iCh];
  }

  /*  // output
  std::ofstream out("debug.txt");
  for (int i = 0; i < NPMT; ++i) {
    out << i << " " << meanOut[i] << " " << rmsOut[i] << std::endl;
  }

  out.close();
  // debug
  */

  return;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void LCaloCalibrationManager::PMTsMeanRmsData(const int pmt,
                                              double *res) const {
  std::vector<double> calc(2, 0);

  LEvRec0 cev;
  calRunFile->SetTheEventPointer(cev);
  // int nEvents=calRunFile->GetEntries();

  const double maxv = DATACALWINDOWMAX;
  const double minv = DATACALWINDOWMIN;
  int nEventsU = 0;

  //  for (int iEv = 0; iEv < nEvents; ++iEv) {// Event loop
  for (int iEv = __skipEv; iEv < __nEv; ++iEv) {  // Event loop
    calRunFile->GetEntry(iEv);
    double signal = static_cast<double>(cev.pmt_high[pmt]);
    if (minv < signal && signal < maxv && cev.trigger_flag[pmt] == 0) {
      calc[0] += signal;
      calc[1] += signal * signal;
      ++nEventsU;
    }
  }

  // std::cout << std::endl;

  calc[0] /= nEventsU;
  calc[1] /= nEventsU;
  calc[1] -= (calc[0] * calc[0]);
  calc[1] = sqrt(calc[1]);

  // output
  res[0] = calc[0];
  res[1] = calc[1];

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
  if (isHG) {
    int *peaks = GetPeaksHG();
    for (int iCh = 0; iCh < NPMT; ++iCh)
      ped0[iCh] = static_cast<double>(peaks[iCh]);
    for (int iCh = 0; iCh < NPMT; ++iCh) sigma0[iCh] = STARTINGSIGMA;
    if (verboseFLAG)
      std::cout << __LCALOCALIBRATIONMANAGER__ << " 0th approx. done"
                << std::endl;

    // First approximation
    PMTsWindowedRmsHG(ped0, sigma0, ped1, sigma1, outliers);
    if (verboseFLAG)
      std::cout << __LCALOCALIBRATIONMANAGER__ << " 1th approx. done"
                << std::endl;

    // Second approximation
    PMTsWindowedRmsHG(ped1, sigma1, ped, sigma, outliers);
    if (verboseFLAG)
      std::cout << __LCALOCALIBRATIONMANAGER__ << " 2th approx. done"
                << std::endl;

    // 3-4 Momenta
    //    PMTsMomenta34HG(ped, sigma, skewness, kurtosis);
    //    if(verboseFLAG)  std::cout << __LCALOCALIBRATIONMANAGER__
    //			       << " 3-4 momenta done" << std::endl;
  } else {
    int *peaks = GetPeaksLG();
    for (int iCh = 0; iCh < NPMT; ++iCh)
      ped0[iCh] = static_cast<double>(peaks[iCh]);
    for (int iCh = 0; iCh < NPMT; ++iCh) sigma0[iCh] = STARTINGSIGMA;
    if (verboseFLAG)
      std::cout << __LCALOCALIBRATIONMANAGER__ << " 0th approx. done"
                << std::endl;

    // First approximation
    PMTsWindowedRmsLG(ped0, sigma0, ped1, sigma1, outliers);
    if (verboseFLAG)
      std::cout << __LCALOCALIBRATIONMANAGER__ << " 1th approx. done"
                << std::endl;

    // Second approximation
    PMTsWindowedRmsLG(ped1, sigma1, ped, sigma, outliers);
    if (verboseFLAG)
      std::cout << __LCALOCALIBRATIONMANAGER__ << " 2th approx. done"
                << std::endl;

    // 3-4 Momenta
    //    PMTsMomenta34LG(ped, sigma, skewness, kurtosis);
    //    if(verboseFLAG)  std::cout << __LCALOCALIBRATIONMANAGER__
    //			       << " 3-4 momenta done" << std::endl;
  }

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
