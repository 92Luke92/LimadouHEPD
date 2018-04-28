#ifndef __LCALOSIGNAL__
#define __LCALOSIGNAL__ 1

#include <cstddef>
const double THRESHOLD=10.; // s/n ratio

class LCaloSignal {
public:
  LCaloSignal();
  LCaloSignal(const LCaloSignal &cs);
  LCaloSignal& operator=(const LCaloSignal& other); // copy assignment
  ~LCaloSignal();
  virtual void Dump() const =0;
  void Reset();
  void FillRandom(void);
  inline int GetNUnits() const {return nunits;};
  inline int GetNPMTs() const {return npmts;};
  double **cont_hg;
  double **sn_hg;
  double **cont_lg;
  double **sn_lg;
  bool **trigger_flag;
  virtual double GetX(const int iu, const int ipmt) const =0;
  virtual double GetY(const int iu, const int ipmt) const =0;
  virtual double GetZ(const int iu, const int ipmt) const =0;
  int GetNOfSignificantUnits(const bool isHG, const double threshold = THRESHOLD) const;
  int GetTheMostSignificantUnit(const bool isHG, const double threshold = THRESHOLD) const;
  int GetThe2ndMostSignificantUnit(const bool isHG, const double threshold = THRESHOLD) const;
  double GetSNOfMSU(const bool isHG, const double threshold = THRESHOLD) const;
  double GetSNOf2ndMSU(const bool isHG, const double threshold = THRESHOLD) const;
  double GetCountsOfMSU(const bool isHG, const double threshold = THRESHOLD) const;
  double GetCountsOf2ndMSU(const bool isHG, const double threshold = THRESHOLD) const;
  double GetSNOfUnit(const int unit, const bool isHG) const;
  double GetCountsOfUnit(const int unit, const bool isHG) const;
  double GetCounts(const bool isHG, const double threshold = THRESHOLD) const;
  
protected:
  void CreateContainers(void);
  void DumpModule(double** inp, const char *string) const;
  void DumpTriggerFlag() const;
  void DumpAll() const;
  int nunits;
  int npmts;

private:
  void ClearHEAP();
};


#endif
