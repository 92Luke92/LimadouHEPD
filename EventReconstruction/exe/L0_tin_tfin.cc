#include "LEvRec0File.hh"
#include "LEvRec0.hh"
#include <iostream>


int main(int argc, char **argv) {
	LEvRec0Md md;
	LEvRec0File fin(argv[1]);
	fin.SetMdPointer(md);

	// initial time
	fin.GetMDEntry(0);
	unsigned int secini = md.broadcast.OBDH.sec;

	// final time
	fin.GetMDEntry(1);
	unsigned int secfin = md.broadcast.OBDH.sec;

	std::string l0name(argv[1]);
  
  	// Getting the basename
  	size_t index=l0name.rfind("/");
  	l0name.erase(l0name.begin(),l0name.begin()+index+1);


	std::cout << l0name << " " << secini << " " << secfin << std::endl;
	fin.Close();

	return 0;
}