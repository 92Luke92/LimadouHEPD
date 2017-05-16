//#include "LEvRec1File.hh"
#include <cstdlib>
#include <iostream>
#include "LCaloEqualizationManager.hh"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Error! Usage: ./EqualizeCalo <steeringFile> "			
			<< std::endl;
		return EXIT_FAILURE;
	}
	std::string st = argv[1];
	LCaloEqualizationManager::GetInstance().LoadSteering(st);
	LCaloEqualizationManager::GetInstance().Run();
	return EXIT_SUCCESS;
}
