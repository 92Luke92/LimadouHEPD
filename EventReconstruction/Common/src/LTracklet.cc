#include "LTracklet.hh"
#include <iostream>
#include <stdlib.h>

LTracklet& LTracklet::operator=(const LTracklet& cs) {
  if (this != &cs) { // self-assignment check expected
    for(int i=0; i<TRACKERCLUSTERS; ++i) tracker_cl[i] = cs.tracker_cl[i];
    trigger_cl = cs.trigger_cl;
  }
  return *this;
}

LTracklet::LTracklet(const LTracklet& other) {
  for(int i=0; i<TRACKERCLUSTERS; ++i) tracker_cl[i] = other.tracker_cl[i];
  trigger_cl = other.trigger_cl;
}
  
LTracklet::LTracklet(const LTrackerCluster *track_cl, const LTriggerCluster trig_cl){
  for(int i=0; i<TRACKERCLUSTERS; ++i) tracker_cl[i] = track_cl[i];
  trigger_cl = trig_cl;
}

void LTracklet::Dump(const int iev){
  std::cout<< "......ooooo0000000 - Dump ev. - "<<iev<<" 0000000ooooo......" <<std::endl;
	for(int i=0; i<TRACKERCLUSTERS; ++i) tracker_cl[i].Dump();
	trigger_cl.Dump();
  std::cout<< "......ooooo0000000 - End Dump - 0000000ooooo......" <<std::endl;
}
    
