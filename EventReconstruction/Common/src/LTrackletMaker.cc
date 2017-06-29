#include "LTrackletMaker.hh"
#include "LTrackerSignal.hh"
#include "LTriggerSignal.hh"
#include "LEvRec1.hh"
#include "LTracklet.hh"
#include "LTrackerTools.hh"

LTrackletMaker::LTrackletMaker() {
}


LTracklet LTrackletMaker::GetTracklet(const LEvRec1 *signal, const bool isHG) const {

  // ################# TRACKER ######################
  const LTrackerSignal *trackerSignal = &(signal->tracker);

  LTrackerCluster MostSignificantTrackerClusters[4]; // p-outer, n-outer, p-inner, n-inner 

  bool pOuterDONE=false;
  bool pInnerDONE=false;
  bool nOuterDONE=false;
  bool nInnerDONE=false;
  
  for(auto cl: trackerSignal->cls) { // clusters are SN descending ordered
    if(pOuterDONE&&pInnerDONE&&nOuterDONE&&nInnerDONE) break;
    int seed = cl.seed;
    if(ChanToSide(seed)==0) { // side p
      if(pOuterDONE && pInnerDONE) continue;
      else {
	if(ChanToPlane(seed)==0) { // side p - outer plane
	  if(pOuterDONE) continue;
	  else {
	    MostSignificantTrackerClusters[0]=cl;
	    pOuterDONE=true;
	  }
	} else { // side p - inner plane
	  if(pInnerDONE) continue;
	  else  {
	    MostSignificantTrackerClusters[2]=cl;
	    pInnerDONE=true;
	  }
	}
      }
    } else  { // side n
      if(nOuterDONE && nInnerDONE) continue;
      else {
	if(ChanToPlane(seed)==0) { // side n - outer plane
	  if(nOuterDONE) continue;
	  else  {
	    MostSignificantTrackerClusters[1]=cl;
	    nOuterDONE=true;
	  } 
	} else { // side n - inner plane
	  if(nInnerDONE) continue;
	  else  {
	    MostSignificantTrackerClusters[3]=cl;
	    nInnerDONE=true;
	  }
	}
      }
    }
  }

  // ################# TRIGGER ######################
  const LTriggerSignal *triggerSignal = &(signal->trig);
  LTriggerCluster triggerCluster = LTriggerCluster(triggerSignal, isHG);  // trigger bar default threhosld at 5.

  LTracklet result(MostSignificantTrackerClusters, triggerCluster);
  
  return result;
}

