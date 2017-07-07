//#include "LEvRec1File.hh"
#include <cstdlib>
#include <iostream>
#include "LScintillatorsL2.hh"

int main(int argc, char *argv[]) {

        std::cout << "******************************************" << std::endl;
        std::cout << "** be sure to have equalization         **" << std::endl;
        std::cout << "** and energy reconstrcution parameters **" << std::endl;
        std::cout << "** files on the run directory           **" << std::endl;
        std::cout << "******************************************" << std::endl;

	if (argc != 2) {
		std::cerr << "Error! Usage: ./VVtest <DataRunL1> "			
			<< std::endl;
		return EXIT_FAILURE;
	}
  
	std::string st = argv[1];
	LScintillatorsL2::GetInstance().LoadRun(st);
	//LScintillatorsL2::GetInstance().Calc(0,0);
        LScintillatorsL2::GetInstance().Test(1);
	return EXIT_SUCCESS;
}
