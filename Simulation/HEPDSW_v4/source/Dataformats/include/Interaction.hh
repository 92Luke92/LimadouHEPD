//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

#ifndef Interaction_h
#define Interaction_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class Interaction : public G4VHit
{
public:

  Interaction();
   Interaction(G4ThreeVector,G4int,G4double,G4String,G4int,G4String,G4int);
  ~Interaction();
  Interaction(const Interaction &right);
  const Interaction& operator=(const Interaction &right);
  G4int operator==(const Interaction &right) const;
   
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  void Draw();
  void Print();

  void SetIntPoint(G4ThreeVector aIntPoint);
  void AddKinEnergy(G4double aKin);
   inline G4ThreeVector GetIntPoint(){return theIntPoint;}
   inline G4int GetParticleType(){return theParticleType;}
   inline G4double GetKinEnergy(){return theKinEnergy;}
   inline G4String GetDetName(){return theDetName;}
   inline G4int GetDetID(){return theDetID;}
   inline G4String GetIntName(){return theIntName;}
   inline G4int GetIntNameID(){return theIntNameID;}

private:
   G4ThreeVector theIntPoint;
   G4int theParticleType;
   G4double theKinEnergy;
   G4String theDetName;
   G4int theDetID;
   G4String theIntName;
   G4int theIntNameID;
};

typedef G4THitsCollection<Interaction> InteractionsCollection;

extern G4Allocator<Interaction> InteractionAllocator;

inline void* Interaction::operator new(size_t)
{
  void *aHit;
  aHit = (void *) InteractionAllocator.MallocSingle();
  return aHit;
}

inline void Interaction::operator delete(void *aHit)
{
  InteractionAllocator.FreeSingle((Interaction*) aHit);
}

#endif


