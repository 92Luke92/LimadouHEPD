//#include "LEvRec1File.hh"
#include <cstdlib>
#include <iostream>
#include "LScintillatorsL2.hh"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Error! Usage: ./VVtest <DataRunL1> "			
			<< std::endl;
		return EXIT_FAILURE;
	}
	std::string st = argv[1];
	LScintillatorsL2::GetInstance().LoadRun(st);
	//LScintillatorsL2::GetInstance().Calc(0);
        LScintillatorsL2::GetInstance().Test(10);
	return EXIT_SUCCESS;
}
