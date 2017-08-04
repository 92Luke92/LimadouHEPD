#ifndef __LLYSOMSCRYSTAL__
#define __LLYSOMSCRYSTAL__  "LLysoMSCrystal  ########## "

const double MSCRYSTALTHRESHOLD=5.;//

class LLysoSignal;

class LLysoMSCrystal {
public:
  LLysoMSCrystal();
  LLysoMSCrystal& operator=(const LLysoMSCrystal& other);
  LLysoMSCrystal(const LLysoSignal *sig, const bool isHG_INP, const double threshold=MSCRYSTALTHRESHOLD);
  void Dump() const;
  inline bool IsHG(void) const {return isHG;};
  void LoadLysoSignal(const LLysoSignal *sig, const bool isHG_INP, const double threshold=MSCRYSTALTHRESHOLD);
  int mscindex;
  double posX;
  double posY;
  double posZ;
  
private:
  void SetDefault();
  bool isHG;
  double count;
  double sigma;
  double sn;
};

#endif