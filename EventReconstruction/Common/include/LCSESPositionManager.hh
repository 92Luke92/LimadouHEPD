#ifndef __LCSESPOSITIONMANAGER__
#define __LCSESPOSITIONMANAGER__ "LCSESPositionManager ###### "

#include "LCSESPosition.hh"
#include <string>

class LCSESPositionManager {
	public:
		LCSESPositionManager();
		LCSESPosition GetCSESPosition(unsigned int time);
		void LoadTable(const char *fin);
	private:
		bool TableLoaded();
		std::string TableFileName;
};

#endif
