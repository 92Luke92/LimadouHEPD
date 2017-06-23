#include "LTriggerCluster.hh"
#include "LTriggerSignal.hh"

#include <iostream>
#include <stdlib.h>
#include "detector_const.hh"

LTriggerCluster& LTriggerCluster::operator=(const LTriggerCluster& other) {
  // check for self-assignment
  if(&other == this)
    return *this;

  // Copy operations
  this->isHG = other.isHG;
  for(int nbar=0; nbar<CLUSTERBARS; ++nbar) {
    this->barIndex[nbar] = other.barIndex[nbar];
    for(int ipmt=0; ipmt<BARPMTS; ++ipmt) {
      this->count[nbar][ipmt] = other.count[nbar][ipmt];
      this->sigma[nbar][ipmt] = other.sigma[nbar][ipmt];
      this->sn[nbar][ipmt] = other.sn[nbar][ipmt];
      this->trigger_flag[nbar][ipmt] = other.trigger_flag[nbar][ipmt];
    }
  }

  return *this;
}

LTriggerCluster::LTriggerCluster(){
  SetDefault();
}

void LTriggerCluster::SetDefault() {
isHG=true;  // default value
  for(int nbar=0; nbar<CLUSTERBARS; ++nbar) {
    barIndex[nbar] = -999;
    for(int ipmt=0; ipmt<BARPMTS; ++ipmt) {
      count[nbar][ipmt] = -999.;
      sigma[nbar][ipmt] = -999.;
      sn[nbar][ipmt] = -999.;
      trigger_flag[nbar][ipmt] = true;
    }
  }
  return;
}

LTriggerCluster::LTriggerCluster(const LTriggerSignal *triggerSignal, const bool isHG_INP, const double threshold) {
  LoadTriggerSignal(triggerSignal, isHG_INP, threshold);
}

void LTriggerCluster::LoadTriggerSignal(const LTriggerSignal *triggerSignal, const bool isHG_INP, const double threshold) {
  if(triggerSignal==0) {
    std::cerr << __LTRIGGERCLUSTER__ << "LTriggerCluster::LoadTriggerSignal(const LTriggerSignal *triggerSignal, const bool isHG_INP, const double threshold)   error:"
	      << " triggerSignal=0" << std::endl;
    SetDefault();
    return;
  }

  // Check how many bars are significant
  int nsigbars=triggerSignal->GetNOfSignificantBars(isHG_INP,threshold);
  if(nsigbars==0 || nsigbars>CLUSTERBARS) {
    std::cout << __LTRIGGERCLUSTER__ << "Warning! Found " << nsigbars << " trigger bars ("
	      << (isHG_INP ? "high" : "low") << " gain) having SN higher than " << threshold << std::endl;
    if(nsigbars==0) {
      std::cout << __LTRIGGERCLUSTER__ << "Impossible to continue." << std::endl;
      return;
    } else {
      std::cout << __LTRIGGERCLUSTER__ << "Only the " << CLUSTERBARS << " adjacent most significant kept. Possible signal loss" << std::endl;
    }
  }

  // isHG
  isHG = isHG_INP;
  
  // Most significant bar
  int msbar = triggerSignal->GetTheMostSignificantBar(isHG,threshold);
  barIndex[0] = msbar;
  for(int ipmt=0; ipmt<BARPMTS; ++ipmt) {
    count[0][ipmt] = (isHG ? triggerSignal->cont_hg[msbar][ipmt] : triggerSignal->cont_lg[msbar][ipmt]);
    sn[0][ipmt] = (isHG ? triggerSignal->sn_hg[msbar][ipmt] : triggerSignal->sn_lg[msbar][ipmt]);
    sigma[0][ipmt] = count[0][ipmt]/sn[0][ipmt];
    triggerSignal->trigger_flag[0][ipmt] = triggerSignal->trigger_flag[msbar][ipmt];
  }

  // Most significant ADJACENT bar (reason why we cannot use the LCaloSignal::GetThe2ndMostSignificantBar)
  double sn_fw = -99999.; 
  double sn_bw = -99999.;
  if(msbar != NTRIGSCINT-1) sn_fw = triggerSignal->GetSNOfUnit(msbar+1,isHG);
  if(msbar != 0)            sn_bw = triggerSignal->GetSNOfUnit(msbar-1,isHG);
  
  int ms2bar = (sn_fw > sn_bw ? msbar + 1 : msbar - 1);
  barIndex[1] = ms2bar;
  for(int ipmt=0; ipmt<BARPMTS; ++ipmt) {
    count[1][ipmt] = (isHG ? triggerSignal->cont_hg[ms2bar][ipmt] : triggerSignal->cont_lg[ms2bar][ipmt]);
    sn[1][ipmt] = (isHG ? triggerSignal->sn_hg[ms2bar][ipmt] : triggerSignal->sn_lg[ms2bar][ipmt]);
    sigma[1][ipmt] = count[1][ipmt]/sn[1][ipmt];
    triggerSignal->trigger_flag[1][ipmt] = triggerSignal->trigger_flag[ms2bar][ipmt];
  }

  return;
}

void LTriggerCluster::Dump() const {
  std::cout << "***************" << std::endl
	    << (isHG ? "\t High Gain" : "\t Low Gain") << std::endl 
            <<"\t bar indexes ";
  for(int nbar=0; nbar<CLUSTERBARS; ++nbar) {
    std::cout << barIndex[nbar] << " ";
    }
  
  std::cout << std::endl;
  std::cout << "\t counts ";
  for(int i = 0; i < CLUSTERBARS; ++i) {
    for(int ipmt=0; ipmt<BARPMTS; ++ipmt) std::cout << count[i][ipmt] << ",";
    std::cout << "      ";
  }
  std::cout << std::endl << "\t sigma  ";
  for(int i = 0; i < CLUSTERBARS; ++i) {
    for(int ipmt=0; ipmt<BARPMTS; ++ipmt) std::cout << sigma[i][ipmt] << ",";
    std::cout << "      ";
  }
  std::cout << std::endl << "\t sn     ";
  for(int i = 0; i < CLUSTERBARS; ++i) {
    for(int ipmt=0; ipmt<BARPMTS; ++ipmt) std::cout << sn[i][ipmt] << ",";
    std::cout << "      ";
  }
  std::cout << std::endl << "\t trigger_flag ";
  for(int i = 0; i < CLUSTERBARS; ++i) {
    for(int ipmt=0; ipmt<BARPMTS; ++ipmt) std::cout << trigger_flag[i][ipmt] << ",";
    std::cout << "      ";
  }
  std::cout << std::endl << "***************" << std::endl;
  return;
}


