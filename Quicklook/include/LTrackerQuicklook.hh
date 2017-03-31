#ifndef LTRACKERQUICKLOOK 
#define LTRACKERQUICKLOOK 1

#include "detector_const.hh"

#include "TCanvas.h"
#include "TH2.h"

const double HOTCHANNELTHRESHOLD=20.;
const double COLDCHANNELTHRESHOLD=3.;
const double GAUSCHANNELTHRESHOLD=3.5;

const int NCALIBEVENTS_QL=1000;

TCanvas *drawing6_chan_2D(TH2D *ladder[N_LADDER]);
int TrackerQuickLook(std::string namefile);
void PrintHelp(void);
void PMTScan(TString rootname);
void TriggerScan(TString rootname);
void BroadcastToXML(TString rootname, TString xslPath, TString xslPath2);
void CPUTimeTempToXML(TString rootname, TString xslPath);
void HVPSConfigToXML(TString rootname, TString xslPath);
void RunInfoToXML(TString rootname, TString xslPath);
void ScintConfigToXML(TString rootname, TString xslPath);
void SilConfigToXML(TString rootname, TString xslPath);
void TelemetryToXML(TString rootname, TString xslPath);

#endif
 
