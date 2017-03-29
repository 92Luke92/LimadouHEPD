#ifndef __LCALOSIGNAL__
#define __LCALOSIGNAL__ 1

#include <cstddef>

class LCaloSignal {
public:
  LCaloSignal();
  LCaloSignal(const LCaloSignal &cs);
  ~LCaloSignal();
  virtual void Dump()=0;
  void Reset();
  void FillRandom(void);
  inline int GetNUnits() const {return nunits;};
  inline int GetNPMTs() const {return npmts;};
  
  double **cont_hg;
  double **sn_hg;
  double **cont_lg;
  double **sn_lg;

protected:
  void CreateContainers(void);
  void DumpModule(double** inp, const char *string);
  void DumpAll();
  int nunits;
  int npmts;

};


#endif
