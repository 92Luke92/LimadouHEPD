#include "laurentmethod.hh"




int main() {

   LaurentMethod datacard("laurentHGpeakshift.csv");
  datacard.dumpDatacard();

   LaurentMethod datacard2("test.txt");
  datacard2.dumpDatacard();

  return 0;
}
