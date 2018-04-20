#include "LCSESPosition.hh"

LCSESPosition::LCSESPosition() {
	time=0;
	latitude=longitude=altitude=-999.;
}

LCSESPosition& LCSESPosition::operator=(const LCSESPosition& other) {
	time=other.time;
	latitude = other.latitude;
	longitude = other.longitude;
	altitude = other.altitude;
	return *this;
}	
