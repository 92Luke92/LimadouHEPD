#ifndef LTRACKERQUICKLOOK 
#define LTRACKERQUICKLOOK 1

#include "detector_const.hh"
#include "LEvRec0File.hh"

#include "TCanvas.h"
#include "TH2.h"
#include "TString.h"

const double HOTCHANNELTHRESHOLD=20.;
const double COLDCHANNELTHRESHOLD=3.;
const double GAUSCHANNELTHRESHOLD=3.5;

const int NCALIBEVENTS_QL=1024;

int TrackerQuickLook(TString namefile, TString outPath);
void LTrackerCalibrationQL(std::string namefile);
void PrintHelp(void);

int CompressedTrackerQL(LEvRec0File input,TString outname);

#endif
 
