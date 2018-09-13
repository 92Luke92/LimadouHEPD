#ifndef __LCSESPOSITION__
#define __LCSESPOSITION__ "LCSESPosition    ########## "

class LCSESPosition {
public:
	LCSESPosition();
	LCSESPosition& operator=(const LCSESPosition& other);	
	unsigned int time; // OBDH sec, from January 1st 2009, 00:00 UTC
	double latitude;	// degrees
	double longitude;	// degrees E
	double altitude;	// km
};

#endif
