#include "detectorID.h"
#include <iostream>


int main() {

   std::cout << " >>> Map of detector IDs "  << std::endl;
   for (auto iv : detectorID) std::cout << iv.first << " -> " << iv.second << std::endl;

   return 0;
}
