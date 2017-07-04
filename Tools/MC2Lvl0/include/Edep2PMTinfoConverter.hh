#include "MapEvents.hh"
#include <vector>

std::vector<PMTinfo> Edep2PMTinfoConverter(std::vector<RootCaloHit> CaloHits){

   	MapEvents * Map= new MapEvents();
	Map->LoadCaloHits(CaloHits);
	Map->Mappify();
	std::vector<PMTinfo> PMTs = Map->FromEdep2PMTinfo();
	return PMTs;
} 
