#ifndef __LTRIGGERCLUSTER__
#define __LTRIGGERCLUSTER__  "LTriggerCluster  ########## "

const int CLUSTERBARS = 2; // a cluster is made of two adjacent bars: index order in the cluster is the sn decreasing order
const int BARPMTS = 2; // two pmts per trigger bar
const double TRIGGERCLSNTHRESHOLD=5.;//

class LTriggerSignal;

class LTriggerCluster {
public:
  LTriggerCluster();
  LTriggerCluster& operator=(const LTriggerCluster& other);
  LTriggerCluster(const LTriggerSignal *sig, const bool isHG_INP, const double threshold=TRIGGERCLSNTHRESHOLD);
  void Dump() const;
  inline bool IsHG(void) const {return isHG;};
  void LoadTriggerSignal(const LTriggerSignal *sig, const bool isHG_INP, const double threshold=TRIGGERCLSNTHRESHOLD);
  
private:
  void SetDefault();
  bool isHG;
  int barIndex[CLUSTERBARS];
  double count[CLUSTERBARS][BARPMTS];
  double sigma[CLUSTERBARS][BARPMTS];
  double sn[CLUSTERBARS][BARPMTS];
  bool trigger_flag[CLUSTERBARS][BARPMTS];
};


#endif
