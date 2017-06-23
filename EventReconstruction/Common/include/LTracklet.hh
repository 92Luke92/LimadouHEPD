#ifndef __LTRACKLET__
#define __LTRACKLET__ "LTracklet  ################ "

#include "LTrackerCluster.hh"
#include "LTriggerCluster.hh"

const int TRACKERCLUSTERS = 4;  // 2 sides, 2 planes


class LTracklet {
public:
  LTracklet(){;};
  LTracklet(const LTracklet& other); 
  LTracklet(const LTrackerCluster*, const LTriggerCluster); 
  LTracklet& operator=(const LTracklet& other);
  LTrackerCluster tracker_cl[TRACKERCLUSTERS];
  LTriggerCluster trigger_cl;
};

#endif
