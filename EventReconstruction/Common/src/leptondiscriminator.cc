/*
 * leptondiscriminator.cc
 *
 *
 *
 */


#include "leptondiscriminator.hh"






float leptonDiscriminator::seemsElectron(const LEvRec1Stream* L1ev) {
   std::vector<float> varsShaped=varShaper(L1ev);
   return discriminate(varsShaped);
}
