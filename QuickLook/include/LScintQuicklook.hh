#ifndef LSCINTQUICKLOOK 
#define LSCINTQUICKLOOK 1

#include "detector_const.hh"

#include "TCanvas.h"
#include "TH2.h"

void PrintHelp(void);
void PMTScan(TString rootname, TString outPath);
void TriggerScan(TString rootname, TString outPath);
void EventRateL0(TString rootname, TString outPath);
//int  runIDtoStartTime(int (*matrix)[2], int size, int runID);
int  runIDtoStartTime(int **matrix, int size, int runID);

#endif
 
