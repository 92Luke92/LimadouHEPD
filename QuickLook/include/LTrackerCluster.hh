#ifndef __LTRACKERCLUSTER__
#define __LTRACKERCLUSTER__ 1

const int CLUSTERCHANNELS=5; // it must be odd!
const int NEV_WINDOW = CLUSTERCHANNELS+2;
const double MAX_SIGMA2=300.;

const double CLFINDTHRESHOLD=3.;
const double CLSNTHRESHOLD=4.2;//
const int CLSNTHRESHOLD1CHAN=3;

class LTrackerCluster {
public:
  LTrackerCluster();
   
  LTrackerCluster& operator=(const LTrackerCluster& other);
  LTrackerCluster(const int inpSeed, const double *inpCont, const double *inpSigma);
  void Dump();
  bool operator < (const LTrackerCluster& cl) const;
  bool operator > (const LTrackerCluster& cl) const;

  int seed;
  double count[CLUSTERCHANNELS];
  double sigma[CLUSTERCHANNELS];
  double sn[CLUSTERCHANNELS];

  
  

  double GetSides(const double SideThreshold);
  double GetCounts(const double SideThreshold);
  double ChargeCenter(const double SideThreshold);
  int ClusterSize(const double SideThreshold);
  bool IsAtBorder();
  bool IsAtStartBorder();
  bool IsAtStopBorder();
  inline double GetEta(){return eta;};
  inline double GetEtaCounts(){return etaCounts;};
  inline double GetEtaSigma(){return etaSigma;};
  
private:
  double ComputeEta();
  double ComputeEta3();
  double eta;
  double etaCounts;
  double etaSigma;
  double etaSN;
};


#endif
