#include "MapEvents.hh"
#include <vector>

std::vector<PMTinfo> Calo2PMTinfoConverter(std::vector<RootCaloHit> CaloHits){

   	MapEvents * Map= new MapEvents();
	Map->LoadCaloHits(CaloHits);
	Map->Mappify();
	std::vector<PMTinfo> PMTs = Map->FromEdep2PMTinfo();
	return PMTs;
}

std::vector<std::vector<PMTinfo>> Tracker2PMTinfoConverter(std::vector<RootTrackerHit> TrackerHits){

   	MapEvents * Map= new MapEvents();
	Map->LoadTrackerHits(TrackerHits);
	Map->Mappify();
	std::vector<std::vector<PMTinfo>> TrackerEdepPos = Map->TrackerHitInfo();
	return TrackerEdepPos;
}
