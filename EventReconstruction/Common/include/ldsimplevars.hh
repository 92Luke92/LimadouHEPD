/*
 * alwayslep.hh
 *
 *
 */



#ifndef LDSIMPLEVARS_HH
#define LDSIMPLEVARS_HH


#include "leptondiscriminator.hh"


class ldsimpleVars: public leptonDiscriminator

{
   public:
      ldsimpleVars() {};
   private:
      enum varnames{evol, range, maxm2, stdev};
      std::vector<float> varShaper(const LEvRec1Stream* L1ev); // null-vector through list initialization
      float discriminate(std::vector<float> shapedVars);
};

#endif /* LDSIMPLEVARS_HH */
