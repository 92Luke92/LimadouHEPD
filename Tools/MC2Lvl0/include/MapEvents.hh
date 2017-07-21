#ifndef MAPEVENTS_H
#define MAPEVENTS_H

// C++ std
#include <iostream>
#include <string>
#include <vector>

// MCEventAnalyze
#include "RootEvent.hh"
#include "RootTrack.hh"
#include "RootVertex.hh"
#include "RootCaloHit.hh"
#include "RootTrackerHit.hh"

// ROOT libs
#include "TVector3.h"


//Local
#include "Edep_Pos.h"
#include "detector_const.hh"



class MapEvents{

private:

	//Memory
	int eventid;
	std::vector<RootTrack> myTracks;
	std::vector<RootCaloHit> myCaloHit;
	std::vector<RootCaloHit> myVetoHit;
	std::vector<RootTrackerHit> myTrackerHit;

	//"Maps"
	std::vector<std::vector<RootCaloHit>>    Trigger;
	std::vector<std::vector<RootCaloHit>>    Calo;
	std::vector<std::vector<RootCaloHit>>    Lyso;
	std::vector<std::vector<RootCaloHit>>    Veto;
	std::vector<std::vector<RootTrackerHit>> Tracker;

	std::vector<RootCaloHit> PMTMap;

        std::vector<std::vector<RootCaloHit>> GetCaloId(const std::string &detectorId);

public:
	MapEvents(){
		for(int i=0;i<NTRIGSCINT ;i++)  Trigger.push_back(std::vector<RootCaloHit>());
		for(int i=0;i<NSCINTPLANES;i++)  Calo   .push_back(std::vector<RootCaloHit>());
		for(int i=0;i<NVETOSCINT ;i++)  Veto   .push_back(std::vector<RootCaloHit>());
		for(int i=0;i<NLYSOCRYSTALS ;i++)  Lyso   .push_back(std::vector<RootCaloHit>());
		for(int i=0;i<12;i++)  Tracker.push_back(std::vector<RootTrackerHit>());

		for(int i=0;i<NPMT;i++) { RootCaloHit dummy; PMTMap.push_back(dummy);}
	};

	void LoadEvent(RootEvent   *Event);
	void LoadCaloHits(std::vector<RootCaloHit> calohit)          { myCaloHit    = calohit;}
	void LoadTrackerHits(std::vector<RootTrackerHit> trackerhit) { myTrackerHit = trackerhit;}


	void  Mappify();
	float GetCaloLayerEdep(const std::string &detectorId,int layer);
	float GetCaloLayerEdepPrim(const std::string &detectorId,int layer);
	TVector3  GetCaloHitImpactPoint(const std::string &detectorId,int layer);
	void MapPMTs();
	bool TestHitLayer(const std::string &detectorId,int layer);
	std::vector<Edep_Pos> FromEdep2Edep_Pos();
	std::vector<std::vector<Edep_Pos>> TrackerHitInfo();
};




#endif
