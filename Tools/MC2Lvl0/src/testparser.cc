#include "csv2fvec.hh"




int main() {

   csv2fvec datacard;
   std::vector<std::vector<float>> entries=datacard.fromDatacard("test.txt");

  for (auto line : entries) {
     for(float elt:line) std::cout << elt << " ";
     std::cout << std::endl;
  }



  return 0;
}
