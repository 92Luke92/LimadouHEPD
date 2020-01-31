#ifndef RootInteraction_h
#define RootInteraction_h 1
////////////////////////////////////////////////////////////////////////////////
//
#include <vector>

#include "TObject.h"
#include "TString.h"
#include "TVector3.h"

////////////////////////////////////////////////////////////////////////////////
//
class RootInteraction : public TObject
{
public:
   RootInteraction ();
   RootInteraction (TVector3 aIntPoint,int aPDG,float aKE, TString aID, int aDetID, TString aIntName, int aIntNameID);
   
  ~RootInteraction ();
  RootInteraction (const RootInteraction&);

   inline TVector3 GetIntPoint(){return theIntPoint;}
   inline int GetParticleType(){return theParticleType;}
   inline float GetKinEnergy(){return theKinEnergy;}
   inline TString GetDetName(){return theDetName;}
   inline int GetDetID(){return theDetID;}
   inline TString GetIntName(){return theIntName;}
   inline int GetIntNameID(){return theIntNameID;}
   
  ClassDef(RootInteraction,1);
  
private:
   TVector3 theIntPoint;
   int theParticleType;
   float theKinEnergy;
   TString theDetName;
   int theDetID;
   TString theIntName;
   int theIntNameID;
};

#endif
