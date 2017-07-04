std::vector<PMTinfo> Edep2PMTinfoConverter(std::vector<RootCaloHit> CaloHits, float PMTgain){

   	MapEvents * Map= new MapEvents();
	Map->LoadCaloHits(CaloHits);
	Map->Mappify();
	std::vector<PMTinfo> PMTs = Map->FromEdep2PMTinfo(PMTgain);
	return PMTs;
} 
