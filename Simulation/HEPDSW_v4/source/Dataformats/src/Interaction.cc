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

#include "Interaction.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include <iostream>

G4Allocator<Interaction> InteractionAllocator;

Interaction::Interaction():theIntPoint(0){
   theParticleType = 0;
   theKinEnergy = 0;
   theDetName = 0;
   theIntName = "";
   theIntNameID = 0;
}

Interaction::Interaction(G4ThreeVector aIntPoint,G4int aPDG, G4double aKE,
			 G4String aDetName, G4int aDetID, G4String aIntName, G4int aIntNameID)
{
   theIntPoint = aIntPoint;
   theParticleType = aPDG;
   theKinEnergy = aKE;
   theDetName = aDetName;
   theDetID = aDetID;
   theIntName = aIntName;
   theIntNameID = aIntNameID;
}

Interaction::~Interaction()
{;}


Interaction::Interaction(const Interaction &right)
  : G4VHit()
{
   theParticleType = right.theParticleType;
   theKinEnergy = right.theKinEnergy;
   theDetName = right.theDetName;
   theIntName = right.theIntName;
   theIntNameID = right.theIntNameID;
   theDetID = right.theDetID;
   theIntPoint = right.theIntPoint;
}

const Interaction& Interaction::operator=(const Interaction &right)
{
   theParticleType = right.theParticleType;
   theKinEnergy = right.theKinEnergy;
   theDetName = right.theDetName;
   theDetID = right.theDetID;
   theIntName = right.theIntName;
   theIntNameID = right.theIntNameID;
   theIntPoint = right.theIntPoint;
   return *this;
}

G4int Interaction::operator==(const Interaction &right) const
{
   return (   theParticleType == right.theParticleType &&
	      theKinEnergy == right.theKinEnergy &&
	      theDetName == right.theDetName &&
	      theDetID == right.theDetID &&
	      theIntName == right.theIntName &&
	      theIntNameID == right.theIntNameID &&
	      theIntPoint == right.theIntPoint);
}


void Interaction::Draw()
{;}


void Interaction::Print()
{
   std::cout<<"Interaction = "<<std::endl;
   std::cout << "ParticleType = " << theParticleType << "; " << std::endl;
   std::cout << "KinEnergy = " << theKinEnergy << "; " << std::endl;
   std::cout << "DetName = " << theDetName << "; " << std::endl;
   std::cout << "DetID = " << theDetID << "; " << std::endl;
   std::cout << "IntName = " << theIntName << "; " << std::endl;
   std::cout << "IntNameID = " << theIntNameID << "; " << std::endl;
   std::cout << "IntPoint = " << theIntPoint << "; " << std::endl;
}

void Interaction::SetIntPoint(G4ThreeVector aIntPoint){
  theIntPoint = aIntPoint;
}

void Interaction::AddKinEnergy(G4double aKin){
  theKinEnergy += aKin;
}

