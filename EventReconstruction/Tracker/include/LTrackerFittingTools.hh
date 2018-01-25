#ifndef __LTRACKERFITTINGTOLLS__
#define __LTRACKERFITTINGTOLLS__ 1

#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"
#include "TProfile.h"
#include "TF1.h"
#include <string>
#include "string.h"

double Correction_1(double eta, double par0, double par1, double par2);
double Correction_2(TProfile * histo, double x);
void Fitting_func(TProfile *histo[6], double par0[6], double par1[6], double par2[6]);
void Fitting_func_sym(TProfile *histo[6], double par0[6], double par1[6], double par2[6]/*, double errpar[6]*/);
Double_t langaufun(Double_t *x, Double_t *par);
void Langau(TH1F *histo[6], double langau_MPV[6], double langau_sigma[6], double chi2red[6], double inf, double sup, Double_t values[6][4], std::string check, std::string typeofpart);
double GetMeanUncertanty(double mean, double MPV, double err_MPV, double sig_l, double err_sig_l, double sig_g, double err_sig_g, double area, double err_area, double inf, double sup);


#endif

