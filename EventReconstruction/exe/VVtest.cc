//#include "LEvRec1File.hh"
#include <cstdlib>
#include <iostream>
#include "LScintillatorsL2.hh"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Error! Usage: ./EqualizeCalo <DataRunL1> "			
			<< std::endl;
		return EXIT_FAILURE;
	}
	std::string st = argv[1];
	LScintillatorsL2::GetInstance().LoadRun(st);
	LScintillatorsL2::GetInstance().Test();
	return EXIT_SUCCESS;
}
