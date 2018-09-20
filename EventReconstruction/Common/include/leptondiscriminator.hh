/*
 * leptondiscriminator.hh
 *
 *
 *
 */


#ifndef LEPTONDISCRIMINATOR_H
#define LEPTONDISCRIMINATOR_H

#include <vector>
#include "LEvRec1.hh"

class leptonDiscriminator
{
   public:
      leptonDiscriminator() {};
      float seemsElectron(const LEvRec1Stream* L1ev);

   private:
      virtual std::vector <float> varShaper(const LEvRec1Stream* L1ev) = 0;
      virtual float discriminate(std::vector<float> shapedVars) = 0;

};

#endif /* LEPTONDISCRIMINATOR_H */
