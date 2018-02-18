#ifndef LSCINTQUICKLOOK 
#define LSCINTQUICKLOOK 1

#include "detector_const.hh"

#include "TCanvas.h"
#include "TH2.h"

void PrintHelp(void);
void PMTScan(TString rootname, TString outPath);
void TriggerScan(TString rootname, TString outPath);


#endif
 
