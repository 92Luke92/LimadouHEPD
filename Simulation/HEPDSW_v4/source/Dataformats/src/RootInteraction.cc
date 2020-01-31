////////////////////////////////////////////////////////////////////////////////
//
#include "RootInteraction.hh"

ClassImp(RootInteraction)

////////////////////////////////////////////////////////////////////////////////
//
RootInteraction::RootInteraction ():theIntPoint(0,0,0)
{
   theParticleType = 0;
   theKinEnergy = 0;
   theDetName = "";
   theDetID = 0;
   theIntName = "";
   theIntNameID = 0;
}
////////////////////////////////////////////////////////////////////////////////
//
RootInteraction::RootInteraction(TVector3 aIntPoint,int aPDG,float aKE, TString aDetName, int aDetID, TString aIntName, int aIntNameID)
{
   theIntPoint = aIntPoint;
   theParticleType = aPDG;
   theKinEnergy = aKE;
   theDetName = aDetName;
   theDetID = aDetID;
   theIntName = aIntName;
   theIntNameID = aIntNameID;
}
////////////////////////////////////////////////////////////////////////////////
//
RootInteraction::~RootInteraction ()
{}
////////////////////////////////////////////////////////////////////////////////
//
RootInteraction::RootInteraction (const RootInteraction& right) : TObject()
{
   theParticleType = right.theParticleType;
   theKinEnergy = right.theKinEnergy;
   theDetName = right.theDetName;
   theDetID = right.theDetID;
   theIntName = right.theIntName;
   theIntNameID = right.theIntNameID;
   theIntPoint = right.theIntPoint;
}
////////////////////////////////////////////////////////////////////////////////
//
