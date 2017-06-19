#ifndef LTRACKERQUICKLOOK 
#define LTRACKERQUICKLOOK 1

#include "detector_const.hh"

#include "TCanvas.h"
#include "TH2.h"

const double HOTCHANNELTHRESHOLD=20.;
const double COLDCHANNELTHRESHOLD=3.;
const double GAUSCHANNELTHRESHOLD=3.5;

const int NCALIBEVENTS_QL=1024;

int TrackerQuickLook(std::string namefile);
void LTrackerCalibrationQL(std::string namefile);
void PrintHelp(void);
void PMTScan(TString rootname);
void TriggerScan(TString rootname);


#endif
 
