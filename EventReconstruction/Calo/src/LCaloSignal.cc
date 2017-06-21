#include "LCaloSignal.hh"
#include "detector_const.hh"
#include <iostream>
#include <random>
#include <chrono>


LCaloSignal::LCaloSignal() {
  nunits = 0;
  npmts = 0;

  cont_hg=0;
  sn_hg=0;
  cont_lg=0;
  sn_lg=0;
  trigger_flag=0;
}

LCaloSignal::LCaloSignal(const LCaloSignal &cs) {
  nunits = cs.GetNUnits();
  npmts = cs.GetNPMTs();
  cont_hg = new double*[nunits];
  sn_hg = new double*[nunits];
  cont_lg = new double*[nunits];
  sn_lg = new double*[nunits];
  trigger_flag = new bool*[nunits];
  for(int iu=0; iu<nunits; ++iu) {
    cont_hg[iu] = new double[npmts];
    sn_hg[iu] = new double[npmts];
    cont_lg[iu] = new double[npmts];
    sn_lg[iu] = new double[npmts];
    trigger_flag[iu] = new bool[npmts];
    for(int ip=0; ip<npmts; ++ip) {
      cont_hg[iu][ip] = cs.cont_hg[iu][ip];
      sn_hg[iu][ip] = cs.sn_hg[iu][ip];
      cont_lg[iu][ip] = cs.cont_lg[iu][ip];
      sn_lg[iu][ip] = cs.sn_lg[iu][ip];
      trigger_flag[iu][ip] = cs.trigger_flag[iu][ip];
    }
  }
}


LCaloSignal& LCaloSignal::operator=(const LCaloSignal& cs) {  // copy assignment
  if (this != &cs) { // self-assignment check expected
    ClearHEAP();
    nunits = cs.GetNUnits();
    npmts = cs.GetNPMTs();
    cont_hg = new double*[nunits];
    sn_hg = new double*[nunits];
    cont_lg = new double*[nunits];
    sn_lg = new double*[nunits];
    trigger_flag = new bool*[nunits];
    for(int iu=0; iu<nunits; ++iu) {
      cont_hg[iu] = new double[npmts];
      sn_hg[iu] = new double[npmts];
      cont_lg[iu] = new double[npmts];
      sn_lg[iu] = new double[npmts];
      trigger_flag[iu] = new bool[npmts];
      for(int ip=0; ip<npmts; ++ip) {
	cont_hg[iu][ip] = cs.cont_hg[iu][ip];
	sn_hg[iu][ip] = cs.sn_hg[iu][ip];
	cont_lg[iu][ip] = cs.cont_lg[iu][ip];
	sn_lg[iu][ip] = cs.sn_lg[iu][ip];
	trigger_flag[iu][ip] = cs.trigger_flag[iu][ip];
      }
    }
  }
  return *this;
}

void LCaloSignal::ClearHEAP() {
  if(cont_hg) {
    for(int iu=0; iu<nunits; ++iu) if(cont_hg[iu]) delete[] cont_hg[iu];
    delete[] cont_hg;
  }
  if(sn_hg) {
    for(int iu=0; iu<nunits; ++iu) if(sn_hg[iu]) delete[] sn_hg[iu];
    delete[] sn_hg;
  }
  if(cont_lg) {
    for(int iu=0; iu<nunits; ++iu) if(cont_lg[iu]) delete[] cont_lg[iu];
    delete[] cont_lg;
  }
  if(sn_lg) {
    for(int iu=0; iu<nunits; ++iu) if(sn_lg[iu]) delete[] sn_lg[iu];
    delete[] sn_lg;
  }
  if(trigger_flag) {
    for(int iu=0; iu<nunits; ++iu) if(trigger_flag[iu]) delete[] trigger_flag[iu];
    delete[] trigger_flag;
  }

  nunits = 0;
  npmts = 0;

  cont_hg=0;
  sn_hg=0;
  cont_lg=0;
  sn_lg=0;
  trigger_flag=0;

  return;
}

LCaloSignal::~LCaloSignal() {
  ClearHEAP();
}

void LCaloSignal::Reset() {
  for(int i=0; i<nunits; ++i) {
    for(int j=0; j<npmts; ++j) {
      cont_hg[i][j]=0.;
      cont_lg[i][j]=0.;
      sn_hg[i][j]=0.;
      sn_lg[i][j]=0.;
      trigger_flag[i][j]=false;
    }
  }
  return;
}


void LCaloSignal::CreateContainers() {
  if(!(nunits>0)) return;
  
  cont_hg = new double*[nunits];
  sn_hg = new double*[nunits];
  cont_lg = new double*[nunits];
  sn_lg = new double*[nunits];
  trigger_flag = new bool*[nunits];
  for(int iu=0; iu<nunits; ++iu) {
    cont_hg[iu] = new double[npmts];
    sn_hg[iu] = new double[npmts];
    cont_lg[iu] = new double[npmts];
    sn_lg[iu] = new double[npmts];
    trigger_flag[iu] = new bool[npmts];
  }
  Reset();
  return;
}


void LCaloSignal::DumpModule(double** inp, const char *string) const {
  std::cout << string << std::endl;
  for(int iu=0; iu<nunits; ++iu) {
    std::cout << "  Unit " << iu << ": ";
    for(int ip=0; ip<npmts; ++ip) std::cout << inp[iu][ip] << "  ";
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void LCaloSignal::DumpTriggerFlag(void) const {
  std::cout << "TRIGGER FLAG" << std::endl;
  for(int iu=0; iu<nunits; ++iu) {
    std::cout << "  Unit " << iu << ": ";
    for(int ip=0; ip<npmts; ++ip) std::cout << trigger_flag[iu][ip] << "  ";
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void LCaloSignal::DumpAll() const {
  if(!(nunits>0)) return;
  DumpModule(cont_hg, "HIGH GAIN counts");
  DumpModule(sn_hg, "HIGH GAIN signal to noise");
  DumpModule(cont_lg, "LOW GAIN counts");
  DumpModule(sn_lg, "LOW GAIN signal to noise");
  DumpTriggerFlag();
  return;
}


void LCaloSignal::FillRandom(void) {
  /* initialize random seed: */
  unsigned rseed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(rseed);
  const double pedestal = 420.;
  const double sigmaIN=4.1;
  std::normal_distribution<double> distribution(pedestal,sigmaIN);


  for(int i=0; i<nunits; ++i) {
    for(int j=0; j<npmts; ++j) {
      cont_hg[i][j]=distribution(generator);
      cont_lg[i][j]=distribution(generator);
      sn_hg[i][j]=cont_hg[i][j]/sigmaIN;
      sn_lg[i][j]=cont_lg[i][j]/sigmaIN;
      trigger_flag[i][j]=false;
    }
  }
  return;
}
