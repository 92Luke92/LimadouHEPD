#ifndef LTRACKERQUICKLOOKPLOTS
#define LTRACKERQUICKLOOKPLOTS 1

#include "detector_const.hh"
#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"

TCanvas *drawing6_chan_1D(TH1D *ladder[N_LADDER],bool log);
TCanvas *drawing6_chan_2D(TH2D *ladder[N_LADDER]);
TCanvas *drawing6_va_2D(TH2D *ladder[N_LADDER]);
TCanvas *drawing12_2D(TH2D *pside[N_LADDER],TH2D *nside[N_LADDER]);

#endif
