#ifndef PMTINFO_H
#define PMTINFO_H

#include "TVector3.h"

struct PMTinfo{;
        float totEdep;
        TVector3 position;

	PMTinfo(){
		totEdep=0;
		position.SetX(0);
		position.SetY(0);
		position.SetZ(0);
	}
};

#endif
