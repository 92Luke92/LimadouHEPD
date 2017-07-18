#ifndef MAPEVENTS_H
#define MAPEVENTS_H

// C++ std
#include <iostream>
#include <string>

// MCEventAnalyze
#include "RootEvent.hh"
#include "RootTrack.hh"
#include "RootVertex.hh"
#include "RootCaloHit.hh"
#include "RootTrackerHit.hh"

// ROOT libs
#include "TVector3.h"
#include <vector>

//Local
#include "Edep_Pos.h"

using namespace std;

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

public:
	MapEvents(){
		for(int i=0;i<6 ;i++)  Trigger.push_back(std::vector<RootCaloHit>());
		for(int i=0;i<16;i++)  Calo   .push_back(std::vector<RootCaloHit>());
		for(int i=0;i<5 ;i++)  Veto   .push_back(std::vector<RootCaloHit>());
		for(int i=0;i<9 ;i++)  Lyso   .push_back(std::vector<RootCaloHit>());
		for(int i=0;i<12;i++)  Tracker.push_back(std::vector<RootTrackerHit>());

		for(int i=0;i<64;i++) { RootCaloHit dummy; PMTMap.push_back(dummy);}
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

void MapEvents::LoadEvent(RootEvent   *Event){

	eventid =  Event->EventID();
        myTracks =  Event->GetTracks();
        myCaloHit =  Event->GetCaloHit();
        myVetoHit =  Event->GetVetoHit();
        myTrackerHit =  Event->GetTrackerHit();
}

void MapEvents::MapPMTs(){
	for(int PMTnr=0;PMTnr<6;PMTnr++)
		if(TestHitLayer("Trigger",PMTnr)){ PMTMap[PMTnr+0] =Trigger[PMTnr][0];
                PMTMap[PMTnr+32]=Trigger[PMTnr][0];
        }
	for(int PMTnr=0;PMTnr<16;PMTnr++)
		if(TestHitLayer("Calo",PMTnr)){ PMTMap[PMTnr+6] =Calo[PMTnr][0];
                PMTMap[PMTnr+39]=Calo[PMTnr][0];
        }
	for(int PMTnr=0;PMTnr<5;PMTnr++)
		if(TestHitLayer("Veto",PMTnr)){ PMTMap[PMTnr+22] =Veto[PMTnr][0];
                PMTMap[PMTnr+54]=Veto[PMTnr][0];
        }
	for(int PMTnr=0;PMTnr<5;PMTnr++)
		if(TestHitLayer("LYSO",PMTnr)) PMTMap[PMTnr+26] =Lyso[PMTnr][0];
       	for(int PMTnr=5;PMTnr<9;PMTnr++)
		if(TestHitLayer("LYSO",PMTnr)) PMTMap[(PMTnr-5)+59] =Lyso[PMTnr][0];


	return;
}


void MapEvents::Mappify(){

	if(myCaloHit.size()>0){
		for(size_t ch=0;ch<myCaloHit.size();ch++){
			int layer = myCaloHit[ch].GetVolume();

			if(layer==1311) Trigger[0].push_back(myCaloHit[ch]);
			if(layer==1312) Trigger[1].push_back(myCaloHit[ch]);
			if(layer==1313) Trigger[2].push_back(myCaloHit[ch]);
			if(layer==1321) Trigger[3].push_back(myCaloHit[ch]);
			if(layer==1322) Trigger[4].push_back(myCaloHit[ch]);
			if(layer==1323) Trigger[5].push_back(myCaloHit[ch]);

			if(layer==1216) Calo[0] .push_back(myCaloHit[ch]);
			if(layer==1215) Calo[1] .push_back(myCaloHit[ch]);
			if(layer==1214) Calo[2] .push_back(myCaloHit[ch]);
			if(layer==1213) Calo[3] .push_back(myCaloHit[ch]);
				if(layer==1212) Calo[4] .push_back(myCaloHit[ch]);
			if(layer==1211) Calo[5] .push_back(myCaloHit[ch]);
			if(layer==1210) Calo[6] .push_back(myCaloHit[ch]);
			if(layer==1209) Calo[7] .push_back(myCaloHit[ch]);
			if(layer==1208) Calo[8] .push_back(myCaloHit[ch]);
			if(layer==1207) Calo[9] .push_back(myCaloHit[ch]);
			if(layer==1206) Calo[10].push_back(myCaloHit[ch]);
			if(layer==1205) Calo[11].push_back(myCaloHit[ch]);
			if(layer==1204) Calo[12].push_back(myCaloHit[ch]);
			if(layer==1203) Calo[13].push_back(myCaloHit[ch]);
			if(layer==1202) Calo[14].push_back(myCaloHit[ch]);
			if(layer==1201) Calo[15].push_back(myCaloHit[ch]);

			if(layer==1111) Lyso[0] .push_back(myCaloHit[ch]);
			if(layer==1121) Lyso[1] .push_back(myCaloHit[ch]);
			if(layer==1131) Lyso[2] .push_back(myCaloHit[ch]);
			if(layer==1112) Lyso[3] .push_back(myCaloHit[ch]);
			if(layer==1122) Lyso[4] .push_back(myCaloHit[ch]);
			if(layer==1132) Lyso[5] .push_back(myCaloHit[ch]);
			if(layer==1113) Lyso[6] .push_back(myCaloHit[ch]);
			if(layer==1123) Lyso[7] .push_back(myCaloHit[ch]);
			if(layer==1133) Lyso[8] .push_back(myCaloHit[ch]);

			if(layer==1411) Veto[0] .push_back(myCaloHit[ch]);
			if(layer==1412) Veto[1] .push_back(myCaloHit[ch]);
			if(layer==1421) Veto[2] .push_back(myCaloHit[ch]);
			if(layer==1422) Veto[3] .push_back(myCaloHit[ch]);
			if(layer==1430) Veto[4] .push_back(myCaloHit[ch]);
		}

		MapPMTs();
	}

	if(myTrackerHit.size()>0){
		for(size_t th=0;th<myTrackerHit.size();th++){ // Orders follows strip -> chan one (0p, 0s, ... 5s)
			int layerTrack=myTrackerHit[th].GetDetectorId();
			if(layerTrack==2212) Tracker[0] .push_back(myTrackerHit[th]);
			if(layerTrack==2211) Tracker[1] .push_back(myTrackerHit[th]);
			if(layerTrack==2112) Tracker[2] .push_back(myTrackerHit[th]);
			if(layerTrack==2111) Tracker[3] .push_back(myTrackerHit[th]);
			if(layerTrack==2222) Tracker[4] .push_back(myTrackerHit[th]);
			if(layerTrack==2221) Tracker[5] .push_back(myTrackerHit[th]);
			if(layerTrack==2122) Tracker[6] .push_back(myTrackerHit[th]);
			if(layerTrack==2121) Tracker[7] .push_back(myTrackerHit[th]);
			if(layerTrack==2232) Tracker[8] .push_back(myTrackerHit[th]);
			if(layerTrack==2231) Tracker[9] .push_back(myTrackerHit[th]);
			if(layerTrack==2132) Tracker[10].push_back(myTrackerHit[th]);
			if(layerTrack==2131) Tracker[11].push_back(myTrackerHit[th]);

		}
	}
	return;
}

float GetCaloHitTotalEdep(RootCaloHit Hit){
	float edep=0;
	std::vector<int> plist = Hit.GetParticleList();
        int nplist = (int) plist.size();

	for(int p = 0; p < nplist; p++)
		edep+=Hit.GetEdep(plist[p]);

	return edep;
}


float GetCaloHitEdepPrimary(RootCaloHit Hit){
	float edep=0;
	std::vector<int> plist = Hit.GetParticleList();
        int nplist = (int) plist.size();

	for(int p = 0; p < nplist; p++)
		if(plist[p]==1) edep+=Hit.GetEdep(plist[p]);

	return edep;
}


float GetTrackerHitTotalEdep(RootTrackerHit Hit){
        float edep=0;
        edep=Hit.GetELoss();
        return edep;
}

TVector3 GetImpactPoint(RootCaloHit Hit){
	TVector3 ImpactPoint(0,0,0);
	ImpactPoint.SetZ((Hit.GetExitPoint().z()+Hit.GetEntryPoint().z())/2);
        ImpactPoint.SetX((Hit.GetExitPoint().x()+Hit.GetEntryPoint().x())/2);
        ImpactPoint.SetY((Hit.GetExitPoint().y()+Hit.GetEntryPoint().y())/2);
	return ImpactPoint;
}

TVector3 GetImpactPoint(RootTrackerHit Hit){
	TVector3 ImpactPoint(0,0,0);
	ImpactPoint.SetZ((Hit.GetExitPoint().z()+Hit.GetEntryPoint().z())/2);
        ImpactPoint.SetX((Hit.GetExitPoint().x()+Hit.GetEntryPoint().x())/2);
        ImpactPoint.SetY((Hit.GetExitPoint().y()+Hit.GetEntryPoint().y())/2);
	return ImpactPoint;
}

float MapEvents::GetCaloLayerEdep(const std::string &detectorId,int layer){

	std::vector<std::vector<RootCaloHit>> detector;

	if((detectorId.compare("Trigger"))==0) detector=Trigger;
	else if((detectorId.compare("Calo"))==0)    detector=Calo;
	else if((detectorId.compare("LYSO"))==0)    detector=Lyso;
	else if((detectorId.compare("Veto"))==0)    detector=Veto;
	else { cout<<detectorId.compare("Calo")<<": "<<"Wrong detector ID"<<endl; return 0;}

	if(layer>=detector.size()) { cout<<"Wrong layer ID"<<endl; return 0;}

	if(detector[layer].size()>0) return GetCaloHitTotalEdep(detector[layer][0]);
	else return 0;
}

float MapEvents::GetCaloLayerEdepPrim(const std::string &detectorId,int layer){

	std::vector<std::vector<RootCaloHit>> detector;

	if((detectorId.compare("Trigger"))==0) detector=Trigger;
	else if((detectorId.compare("Calo"))==0)    detector=Calo;
	else if((detectorId.compare("LYSO"))==0)    detector=Lyso;
	else if((detectorId.compare("Veto"))==0)    detector=Veto;
	else { cout<<detectorId.compare("Calo")<<": "<<"Wrong detector ID"<<endl; return 0;}

	if(layer>=detector.size()) { cout<<"Wrong layer ID"<<endl; return 0;}

	if(detector[layer].size()>0) return GetCaloHitEdepPrimary(detector[layer][0]);
	else return 0;
}


TVector3 MapEvents::GetCaloHitImpactPoint(const std::string &detectorId,int layer){

	std::vector<std::vector<RootCaloHit>> detector;
	TVector3 ImpactPoint(0,0,0);


        if((detectorId.compare("Trigger"))==0) detector=Trigger;
        else if((detectorId.compare("Calo"))==0)    detector=Calo;
        else if((detectorId.compare("LYSO"))==0)    detector=Lyso;
        else if((detectorId.compare("Veto"))==0)    detector=Veto;
        else { cout<<detectorId.compare("Calo")<<": "<<"Wrong detector ID"<<endl; return ImpactPoint;}

        if(layer>=detector.size()) { cout<<"Wrong layer ID"<<endl; return ImpactPoint;}

        if(detector[layer].size()>0) ImpactPoint=GetImpactPoint(detector[layer][0]);

	return ImpactPoint;
}


bool MapEvents::TestHitLayer(const std::string &detectorId,int layer){
	if((detectorId.compare("Tracker"))!=0){
		std::vector<std::vector<RootCaloHit>> detector;

		if((detectorId.compare("Trigger"))==0)      detector=Trigger;
		else if((detectorId.compare("Calo"))==0)    detector=Calo;
		else if((detectorId.compare("LYSO"))==0)    detector=Lyso;
		else if((detectorId.compare("Veto"))==0)    detector=Veto;

		else { cout<<detectorId.compare("Calo")<<": "<<"Wrong detector ID"<<endl; return false;}

		if(layer>=detector.size()) { cout<<"Wrong layer ID"<<endl; return false;}

		return (detector[layer].size()>0);
	}
	else{
		std::vector<std::vector<RootTrackerHit>> detector=Tracker;
		if(layer>=detector.size()) { cout<<"Wrong layer ID"<<endl; return false;}
		return (detector[layer].size()>0);
	}

}

std::vector<Edep_Pos> MapEvents::FromEdep2Edep_Pos(){

	std::vector<Edep_Pos> LVL0;

	for(int i=0;i<PMTMap.size();i++){
		Edep_Pos PMT;
		LVL0.push_back(PMT);
	}

	for(int i=0;i<PMTMap.size();i++){
		LVL0[i].totEdep=GetCaloHitTotalEdep(PMTMap[i]);
		LVL0[i].position=GetImpactPoint(PMTMap[i]);
	}
	return LVL0;

}


std::vector<std::vector<Edep_Pos>> MapEvents::TrackerHitInfo(){

        std::vector<std::vector<Edep_Pos>> trackerhitinfo;
        for(int i=0;i<12;i++)  trackerhitinfo.push_back(std::vector<Edep_Pos>());

        for(int i=0;i<12;i++){
                Edep_Pos TrackerEdepPos;
                trackerhitinfo[i].push_back(TrackerEdepPos);
                if(TestHitLayer("Tracker",i)){
                        trackerhitinfo[i][0].totEdep=GetTrackerHitTotalEdep(Tracker[i][0]);
                        trackerhitinfo[i][0].position=GetImpactPoint(Tracker[i][0]);
		}
        }
        return trackerhitinfo;
}


#endif
