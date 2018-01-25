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

#include "MCTruthSD.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "Track.hh"
#include "Vertex.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

MCTruthSD::MCTruthSD(G4String name)
  :G4VSensitiveDetector(name)
{
  collectionName.insert("trackCollection");
  collectionName.insert("vertexCollection");
  sprintf(KaptonBlanket,"ThermalBlanketKapton");
}

MCTruthSD::~MCTruthSD()
{;}

void MCTruthSD::Initialize(G4HCofThisEvent*)
{
  TrackCollection = new TracksCollection(SensitiveDetectorName,collectionName[0]);
  VertexCollection = new VertexsCollection(SensitiveDetectorName,collectionName[1]);
  verboseLevel = 0;

}

G4bool MCTruthSD::ProcessHits(G4Step* aStep,G4TouchableHistory*){
  G4cout << "MCTruthSD process hits" << G4endl;
  G4Track * theTrack    = aStep->GetTrack();
  int theTrackID    = theTrack->GetTrackID();
  G4ThreeVector theExitPoint = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector theEntryPoint = aStep->GetPreStepPoint()->GetPosition();
  G4double theKE      = aStep->GetPreStepPoint()->GetKineticEnergy()/MeV;
  G4int theParticleType = theTrack->GetParticleDefinition()->GetPDGEncoding();
  G4String pname = theTrack->GetDefinition()->GetParticleName();
  G4int motherTrackId = theTrack->GetParentID();
  G4ThreeVector gmd  = aStep->GetPreStepPoint()->GetMomentumDirection();
  G4String cvolume = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  G4String cProcessName = "unknown";
  const char* ptvolume = ((const char*) cvolume);
  if (theTrackID == 1 && strcmp(ptvolume,KaptonBlanket) == 0 ) {
  G4cout << " Track "<< theTrackID << " pdg "<< theParticleType <<" in the volume "<< cvolume << G4endl;
  G4cout << " entree X " << theEntryPoint.getX() << " Y " <<  theEntryPoint.getY() << " Z " <<  theEntryPoint.getZ() << G4endl;
  G4cout << " exit X " << theExitPoint.getX() << " Y " <<  theExitPoint.getY() << " Z " <<  theExitPoint.getZ() << G4endl;
  G4cout << " ke (MeV) " << theKE << G4endl;
  Track* track = new Track(theTrackID,theParticleType,pname,theEntryPoint,gmd,theKE,
  						motherTrackId,cvolume,cProcessName);
  TrackCollection->insert(track);
  }
  return true;
}


void MCTruthSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  G4cout << "MCTruth end-of-event" << G4endl;
  static G4int HCID0 = -1;
  if(HCID0<0)
    HCID0 = GetCollectionID(0);
  G4cout << "HCID0 " << HCID0 << G4endl;
  HCE->AddHitsCollection(HCID0, TrackCollection );
  PrintAll();

  static G4int HCID1 = -1;
  if(HCID1<0)
    HCID1 = GetCollectionID(1);
  HCE->AddHitsCollection(HCID1, VertexCollection );
}

void MCTruthSD::clear()
{
}

void MCTruthSD::DrawAll()
{
}

void MCTruthSD::PrintAll()
{
  G4cout << " trk collection entrees " << TrackCollection->entries() << G4endl;
  for (int i=0; i<TrackCollection->entries(); i++) {
    G4cout << " ---> entree " << i << G4endl;
    G4cout << " Track "<< (*TrackCollection)[i]->GetTrackId() << " pdg "<< (*TrackCollection)[i]->GetPDGCode() << G4endl;
    G4cout << " entree X " << (*TrackCollection)[i]->GetPosition().getX() << " Y " << (*TrackCollection)[i]->GetPosition().getY() << " Z " << (*TrackCollection)[i]->GetPosition().getZ() << G4endl;
    G4cout << " volume " << (*TrackCollection)[i]->GetVertexVolumeName() << G4endl;
  }

}

