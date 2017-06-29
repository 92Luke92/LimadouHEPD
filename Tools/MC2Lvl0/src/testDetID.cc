#include "detectorID.h"
#include <iostream>


int main() {

   for (auto v : detectorID) std::cout << v.first << " -> " << v.second << std::endl;
   return 0;
}
