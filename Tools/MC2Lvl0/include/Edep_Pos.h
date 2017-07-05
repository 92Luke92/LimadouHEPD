#ifndef PMTINFO_H
#define PMTINFO_H

#include "TVector3.h"

struct Edep_Pos{;
        float totEdep;
        TVector3 position;

	Edep_Pos(){
		totEdep=0;
		position.SetX(0);
		position.SetY(0);
		position.SetZ(0);
	}
};

#endif
