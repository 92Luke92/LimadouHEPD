#ifndef __LEVREC2__
#define __LEVREC2__ 1

#include <cstddef>
#include "ldsimplevars.hh"

struct LEvRec2Stream;

class LEvRec2 {

public:
  LEvRec2();
  ~LEvRec2();

  void CopyFromLEvRec2Stream(const LEvRec2Stream evstr);

  double Energy;
  double Angle;
  double EnergyLoss;
  int ParticleID;

private:
  void Reset();

};

struct LEvRec2Stream {

  double Energy;
  double Angle;
  double EnergyLoss;
  int ParticleID;
  //float seemsLepton(leptonDiscriminator* discr=new ldsimpleVars);


  LEvRec2Stream();
  LEvRec2Stream(const LEvRec2 event);
  void Reset();
  void CopyFromLEvRec2(const LEvRec2 event);
  ~LEvRec2Stream();
};

#endif
