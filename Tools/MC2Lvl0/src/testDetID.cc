#include "detectorID.h"
#include "PMTnumbers.hh"
#include <iostream>


int main() {

   std::cout << " >>> Map of detector IDs "  << std::endl;
   for (auto iv : detectorID) std::cout << iv.first << " -> " << iv.second << std::endl;
   std::cout << " >>> Vector of PMT IDs "  << std::endl;
   for (uint iv=0; iv<PMTID.size() ; iv++) std::cout << iv << " -> " << PMTID[iv] << std::endl;
   return 0;
}
