#ifndef __LCALIBRATIONMONITOR__
#define __LCALIBRATIONMONITOR__ 1

#include "LCalibration.hh"

#include <string>

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TH2.h"

const int WINDOW_LEN = 100; //number of calibrations used as reference
const int PMT_MAX_FAIL_CHAN = 1;
const double PMT_SIGMA_THRESHOLD = 2.5;
const double PMT_PED_THRESHOLD = 2.5;
const int TRK_MAX_FAIL_CHAN = 20;
const double TRK_PED_THRESHOLD = 3.5;
const double TRK_SIGMA_THRESHOLD = 3.5;

//Fake calibration files generators
void SlowDrift(const LCalibration* seed,const int nFiles,const double offset,char* label);

void Jump(const LCalibration* seed,const int nFiles,const double jump,char* label);

//Monitoring Tools

LCalibration* CalculateWindow(const char* infile,double *err_sigma_LG,double *err_sigma_HG,double *err_sigma_TRK,bool graphics=false);    
bool IsGood(const LCalibration *reference,const double* err_sigma_LG,const double* err_sigma_HG,const double* err_sigma_trk,const LCalibration *latest);
void IsGoodGraphics(const LCalibration *reference,const double* err_sigma_LG,const double* err_sigma_HG,const double* err_sigma_trk,const LCalibration *latest);
void WindowGraphics(LCalibration* calibs[WINDOW_LEN],int islot=WINDOW_LEN);
void WindowTimeEvolution(LCalibration* calibs[WINDOW_LEN],int islot=WINDOW_LEN);
//Comparison Functions:
bool ComparePMT(const LCalibration *first, const double* err_sigma_LG,const double* err_sigma_HG,const LCalibration *second);
bool CompareTracker(const LCalibration *first,const double* err_sigma_trk, const  LCalibration *second);
//Plotting Tools

TCanvas *drawing6(TGraphErrors *ladder[N_LADDER],TGraphErrors *ladder2[N_LADDER],double ymin,double ymax,const char* title,bool log=false);
TCanvas* drawing6(TH2D* ladder[N_LADDER],double zmin,double zmax,bool logz=false);
TCanvas *drawing1(TGraphErrors *input,TGraphErrors *input2,double ymin,double ymax,const char* title,bool log=false);
TCanvas *drawing1(TH2D *input,double zmin,double zmax,bool logz=false);
#endif
