#include "LCSESPositionManager.hh"
#include "LCSESPosition.hh"
#include <fstream>
#include <iostream>

const unsigned int __expectedtimediff = 1; //sec



LCSESPositionManager::LCSESPositionManager() {
	TableFileName = "";
}

void LCSESPositionManager::LoadTable(const char *finname) {
	std::ifstream fin(finname, std::ifstream::in);
	if(!fin.is_open()) {
		std::cerr << __LCSESPOSITIONMANAGER__ << "Impossible to load file " << finname << std::endl;
		return;
	} else {
		TableFileName = finname;
	}
}

bool LCSESPositionManager::TableLoaded() {
	if(TableFileName.compare("")!=0) return true;
	else return false;
}

LCSESPosition LCSESPositionManager::GetCSESPosition(unsigned int inptime) {
	LCSESPosition result;
	if(!TableLoaded()) {
		std::cerr << __LCSESPOSITIONMANAGER__ << "Warning! Using GetCSESPosition with no SGP4 table loaded!" << std::endl;
		return result;
	}

	// Reading loop
	unsigned int ltime=-999;
	LCSESPosition pos_before;
	double latitude, longitude, altitude;
	std::ifstream fin(TableFileName.c_str(), std::ifstream::in);
	while(1) {
		fin >> ltime >> altitude >> latitude >> longitude;
		if(ltime<inptime) {
			pos_before.time = ltime;
			pos_before.altitude = altitude;
			pos_before.latitude = latitude;
			pos_before.longitude = longitude;
			continue;
		}
		if(fin.eof()) break;
	}
	if(pos_before.time != 0) result = pos_before;

	return result;
}