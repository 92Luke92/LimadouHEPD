#ifndef RootCaloHit_h
#define RootCaloHit_h 1
////////////////////////////////////////////////////////////////////////////////
//
#include <vector>
#include <map>

#include "TObject.h"
#include "TString.h"
#include "TVector3.h"

////////////////////////////////////////////////////////////////////////////////
//
class RootCaloHit : public TObject
{
public:
  RootCaloHit ();
  //  RootCaloHit (TString aVolume,double aTotalEdep,std::map<int,double> aEdep);
  RootCaloHit (int aDetID,TVector3 aEntry,TVector3 aExit,double aDeltaTime,float aKE,double aTotalEdep,std::map<int,double> aEdep,std::map<int,TVector3> aStepPos);
  // RootCaloHit (int aDetID,TVector3 aEntry,TVector3 aExit,float aKE,double aTotalEdep,std::map<int,double> aEdep);
  ~RootCaloHit ();
  RootCaloHit (const RootCaloHit&);
  
  
  inline TVector3 GetEntryPoint(){return theEntryPoint;}
  inline TVector3 GetExitPoint(){return theExitPoint;}
  inline double GetGlobalTime(){return theGlobalTime;}
  inline float GetKinEnergy(){return theKinEnergy;}
  inline double      GetTotalEdep(){return theTotalEdep;}
  //  inline TString     GetVolume(){return theVolume;}
  inline int GetVolume(){return theDet; }
  inline std::vector<int> GetParticleList(){return theParticleList;}
  double      GetEdep(int aTkID);
  TVector3    GetStepPos(int aTkID);

  ClassDef(RootCaloHit,2);
  
private:
  //  TString         theVolume;  
  int    theDet;
  TVector3 theEntryPoint;
  TVector3 theExitPoint;
  double theGlobalTime;
  float theKinEnergy;
  double   theTotalEdep;	 
  std::map<int,double> theEdep;
  std::map<int,TVector3> theStepPos;
  std::vector<int>     theParticleList;
};

#endif
