//#include "LEvRec1File.hh"
#include <cstdlib>
#include <iostream>
#include "LScintillatorsL2.hh"

int main(int argc, char *argv[]) {

        std::cout << "..........................................................." << std::endl;
        std::cout << ". use not equalized data in input and place the proper    ." << std::endl;
        std::cout << ". equalization and energy recon parameters files in the   ." << std::endl;
        std::cout << ". run directory                                           ." << std::endl;
        std::cout << "..........................................................." << std::endl;

	if (argc != 2) {
		std::cerr << "Error! Usage: L1AnVV <DataRunL1> "			
			<< std::endl;
		return EXIT_FAILURE;
	}
  
	std::string st = argv[1];
	LScintillatorsL2::GetInstance().LoadRun(st);
	//LScintillatorsL2::GetInstance().Calc(0,0);
        LScintillatorsL2::GetInstance().Test(1);
        //LScintillatorsL2::GetInstance().MakeLYSOStudy(st);
	return EXIT_SUCCESS;
}
