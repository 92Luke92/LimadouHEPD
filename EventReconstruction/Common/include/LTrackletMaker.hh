#ifndef __LTRACKLETMAKER__
#define __LTRACKLETMAKER__ "LTrackletMaker  ########### "

class LTrackerSignal;
class LTriggerSIgnal;
class LEvRec1;
class LTracklet;

class LTrackletMaker {

public:
  LTrackletMaker();
  LTracklet GetTracklet(const LEvRec1 *sig, const bool isHG) const;
  
private:
};

#endif
