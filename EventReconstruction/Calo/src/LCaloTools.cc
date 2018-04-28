#include "LCaloTools.hh"

LTriggerSignal GetTriggerSignal(const LEvRec0 lev0, const LCalibration cal) {
  LTriggerSignal result;
  const int nunits = result.GetNUnits();
  const int npmts = result.GetNPMTs();
  const int OFFSET = 0; // PMT index offset for this kind of calo  

  const double *pedestalHG = cal.GetCaloHGCalibration()->GetPedestal();
  const double *sigmaHG = cal.GetCaloHGCalibration()->GetSigma();
  const double *pedestalLG = cal.GetCaloLGCalibration()->GetPedestal();
  const double *sigmaLG = cal.GetCaloLGCalibration()->GetSigma();

  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
     int index = OFFSET+iu+32*ipmt; // even ipmt is east, odd ipmt is west
     result.cont_hg[iu][ipmt] = static_cast<double>(lev0.pmt_high[index])-pedestalHG[index];
     result.sn_hg[iu][ipmt] = result.cont_hg[iu][ipmt]/sigmaHG[index];
     result.cont_lg[iu][ipmt] = static_cast<double>(lev0.pmt_low[index])-pedestalLG[index];
     result.sn_lg[iu][ipmt] = result.cont_lg[iu][ipmt]/sigmaLG[index];
     result.trigger_flag[iu][ipmt] = lev0.trigger_flag[index];
    }
  }

  return result;
}

LScintillatorSignal GetScintillatorSignal(const LEvRec0 lev0, const LCalibration cal) {
  LScintillatorSignal result;
  const int nunits = result.GetNUnits();
  const int npmts = result.GetNPMTs();
  const int OFFSET = NTRIGSCINT; // PMT index offset for this kind of calo  

  const double *pedestalHG = cal.GetCaloHGCalibration()->GetPedestal();
  const double *sigmaHG = cal.GetCaloHGCalibration()->GetSigma();
  const double *pedestalLG = cal.GetCaloLGCalibration()->GetPedestal();
  const double *sigmaLG = cal.GetCaloLGCalibration()->GetSigma();

  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
      int index = OFFSET+iu+32*ipmt;  // (even,even)=SouthEast (even,odd)=NorthWest (odd,even)=SouthWest (odd,odd)=NorthEast
      result.cont_hg[iu][ipmt] = static_cast<double>(lev0.pmt_high[index])-pedestalHG[index];
      result.sn_hg[iu][ipmt] = result.cont_hg[iu][ipmt]/sigmaHG[index];
      result.cont_lg[iu][ipmt] = static_cast<double>(lev0.pmt_low[index])-pedestalLG[index];
      result.sn_lg[iu][ipmt] = result.cont_lg[iu][ipmt]/sigmaLG[index];
      result.trigger_flag[iu][ipmt] = lev0.trigger_flag[index];
    }
  }
  
  return result;
}

LVetoSignal GetVetoSignal(const LEvRec0 lev0, const LCalibration cal) {
  LVetoSignal result;
  const int nunits = result.GetNUnits();
  const int npmts = result.GetNPMTs();
  const int OFFSET = NTRIGSCINT+NSCINTPLANES; // PMT index offset for this kind of calo  

  const double *pedestalHG = cal.GetCaloHGCalibration()->GetPedestal();
  const double *sigmaHG = cal.GetCaloHGCalibration()->GetSigma();
  const double *pedestalLG = cal.GetCaloLGCalibration()->GetPedestal();
  const double *sigmaLG = cal.GetCaloLGCalibration()->GetSigma();

  for(int iu=0; iu<nunits; ++iu) { // iu: north, east, south, west, bottom
    for(int ipmt=0; ipmt<npmts; ++ipmt) { // pmt even: up, odd: down   ---   for bottom(iu=4) even:NorthEast, odd:SouthWest
      int index = OFFSET+iu+32*ipmt; 
      result.cont_hg[iu][ipmt] = static_cast<double>(lev0.pmt_high[index])-pedestalHG[index];
      result.sn_hg[iu][ipmt] = result.cont_hg[iu][ipmt]/sigmaHG[index];
      result.cont_lg[iu][ipmt] = static_cast<double>(lev0.pmt_low[index])-pedestalLG[index];
      result.sn_lg[iu][ipmt] = result.cont_lg[iu][ipmt]/sigmaLG[index];
      result.trigger_flag[iu][ipmt] = lev0.trigger_flag[index];
    }
  }
  
  return result;
}

LLysoSignal GetLysoSignal(const LEvRec0 lev0, const LCalibration cal) {
  LLysoSignal result;
  const int nunits = result.GetNUnits();
  const int npmts = result.GetNPMTs();
  const int OFFSET = NTRIGSCINT+NSCINTPLANES+NVETOSCINT; // PMT index offset for this kind of calo  

  const double *pedestalHG = cal.GetCaloHGCalibration()->GetPedestal();
  const double *sigmaHG = cal.GetCaloHGCalibration()->GetSigma();
  const double *pedestalLG = cal.GetCaloLGCalibration()->GetPedestal();
  const double *sigmaLG = cal.GetCaloLGCalibration()->GetSigma();

  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
      int index = (iu<5 ? OFFSET+iu+32*ipmt : OFFSET+(iu-5)+32); //9-7-1-8-5   and 3-2-6-4
      result.cont_hg[iu][ipmt] = static_cast<double>(lev0.pmt_high[index])-pedestalHG[index];
      result.sn_hg[iu][ipmt] = result.cont_hg[iu][ipmt]/sigmaHG[index];
      result.cont_lg[iu][ipmt] = static_cast<double>(lev0.pmt_low[index])-pedestalLG[index];
      result.sn_lg[iu][ipmt] = result.cont_lg[iu][ipmt]/sigmaLG[index];
      result.trigger_flag[iu][ipmt] = lev0.trigger_flag[index];
    }
  }
  
  return result;
}

void EqualizeCalo(LEvRec1 &lev1, const LCaloEqualization eqHG, const LCaloEqualization eqLG) {
  if(!eqHG.CheckStatus() || !eqLG.CheckStatus()) {
    std::cerr << __LEVREC1__ << " error. Equalizatin loaded not usable. Equalization skipped." << std::endl;
    return;
  }
  bool isHG=true;
  EqualizeCaloSingleGain(lev1, isHG, eqHG);
  bool isLG=!isHG;
  EqualizeCaloSingleGain(lev1, isLG, eqLG);
  return;
}

void EqualizeCaloSingleGain(LEvRec1 &lev1, const bool isHG, const LCaloEqualization eq) {
  
  LTriggerSignal trigMPV;
  int nunits = trigMPV.GetNUnits();
  int npmts = trigMPV.GetNPMTs();
  int OFFSET = 0; // PMT index offset for this kind of calo  
  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
     int index = OFFSET+iu+32*ipmt; // even ipmt is east, odd ipmt is west
      if(isHG )trigMPV.cont_hg[iu][ipmt] = eq.GetMPVFactor(index);
      else trigMPV.cont_lg[iu][ipmt] = eq.GetMPVFactor(index);
     }
   }

  LScintillatorSignal scintMPV;
  nunits = scintMPV.GetNUnits();
  npmts = scintMPV.GetNPMTs();
  OFFSET = NTRIGSCINT; // PMT index offset for this kind of calo  
  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
     int index = OFFSET+iu+32*ipmt; // even ipmt is east, odd ipmt is west
      if(isHG )scintMPV.cont_hg[iu][ipmt] = eq.GetMPVFactor(index);
      else scintMPV.cont_lg[iu][ipmt] = eq.GetMPVFactor(index);
     }
   }

  LVetoSignal vetoMPV;
  nunits = vetoMPV.GetNUnits();
  npmts = vetoMPV.GetNPMTs();
  OFFSET = NTRIGSCINT+NSCINTPLANES; // PMT index offset for this kind of calo  
  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
     int index = OFFSET+iu+32*ipmt; // even ipmt is east, odd ipmt is west
      if(isHG )vetoMPV.cont_hg[iu][ipmt] = eq.GetMPVFactor(index);
      else vetoMPV.cont_lg[iu][ipmt] = eq.GetMPVFactor(index);
     }
   }

  LLysoSignal lysoMPV;
  nunits = lysoMPV.GetNUnits();
  npmts = lysoMPV.GetNPMTs();
  OFFSET = NTRIGSCINT+NSCINTPLANES+NVETOSCINT; // PMT index offset for this kind of calo  
  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
     int index = (iu<5 ? OFFSET+iu+32*ipmt : OFFSET+(iu-5)+32); //9-7-1-8-5   and 3-2-6-4
      if(isHG )lysoMPV.cont_hg[iu][ipmt] = eq.GetMPVFactor(index);
      else lysoMPV.cont_lg[iu][ipmt] = eq.GetMPVFactor(index);
     }
   }

   
  // ++++++++++++++++++++ First approach
  // ++++++++++++++++++++ equalize all PMTs to Plane 1 nw
  const double refMPV = (isHG ? scintMPV.cont_hg[1][1] : scintMPV.cont_lg[1][1]);
  // ++++++++++++++++++++ Second approach
  // ++++++++++++++++++++ equalize all PMTs to MIP=100
  // const double refMPV = 100.;

  nunits = trigMPV.GetNUnits();
  npmts = trigMPV.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
      if(isHG) lev1.trig.cont_hg[iu][ipmt] /= (trigMPV.cont_hg[iu][ipmt]*refMPV);
      else lev1.trig.cont_lg[iu][ipmt] /= (trigMPV.cont_lg[iu][ipmt]*refMPV);
    }
  }

  nunits = scintMPV.GetNUnits();
  npmts = scintMPV.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
      if(isHG) lev1.scint.cont_hg[iu][ipmt] /= (scintMPV.cont_hg[iu][ipmt]*refMPV);
      else lev1.scint.cont_lg[iu][ipmt] /= (scintMPV.cont_lg[iu][ipmt]*refMPV);
    }
  }

  nunits = vetoMPV.GetNUnits();
  npmts = vetoMPV.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
      if(isHG) lev1.veto.cont_hg[iu][ipmt] /= (vetoMPV.cont_hg[iu][ipmt]*refMPV);
      else lev1.veto.cont_lg[iu][ipmt] /= (vetoMPV.cont_lg[iu][ipmt]*refMPV);
    }
  }

  nunits = lysoMPV.GetNUnits();
  npmts = lysoMPV.GetNPMTs();
  for(int iu=0; iu<nunits; ++iu) {
    for(int ipmt=0; ipmt<npmts; ++ipmt) {
      if(isHG) lev1.lyso.cont_hg[iu][ipmt] /= (lysoMPV.cont_hg[iu][ipmt]*refMPV);
      else lev1.lyso.cont_lg[iu][ipmt] /= (lysoMPV.cont_lg[iu][ipmt]*refMPV);
    }
  }

  return;
}

