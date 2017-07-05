#include "MapEvents.hh"
#include <vector>

std::vector<Edep_Pos> Calo2Edep_PosConverter(std::vector<RootCaloHit> CaloHits){

   	MapEvents * Map= new MapEvents();
	Map->LoadCaloHits(CaloHits);
	Map->Mappify();
	std::vector<Edep_Pos> PMTs = Map->FromEdep2Edep_Pos();
	return PMTs;
}

std::vector<std::vector<Edep_Pos>> Tracker2Edep_PosConverter(std::vector<RootTrackerHit> TrackerHits){

   	MapEvents * Map= new MapEvents();
	Map->LoadTrackerHits(TrackerHits);
	Map->Mappify();
	std::vector<std::vector<Edep_Pos>> TrackerEdepPos = Map->TrackerHitInfo();
	return TrackerEdepPos;
}
