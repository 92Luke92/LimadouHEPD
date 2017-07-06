// Local
#include "detectorID.h"
#include "Edep_Pos.h"
#include "LEvRec0Writer.hh"
#include "MCcoorPhysicalFrame.hh"
#include "trackeradc.hh"
#include "PMTnumbers.hh"
#include "MapEvents.hh"
#include "Edep2Edep_PosConverter.hh"


// C++ std
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>

// MCEventAnalyze
#include "RootEvent.hh"
#include "RootTrack.hh"
#include "RootVertex.hh"
#include "RootCaloHit.hh"
#include "RootTrackerHit.hh"


// ROOT libs
#include "TTree.h"
#include "TVector3.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"

//Various
#include "LEvRec0.hh"




float Vector3Dist (TVector3 v1, TVector3 v2);
std::string  getMCfilename (int argc, char** argv);
std::string  getLvl0filename (const std::string mcfilename);
void LoopOnEvents (LEvRec0Writer* lvl0writer, TTree* Tmc);
std::vector<float> CaloHitsToEdep (std::vector<RootCaloHit>);
void getPMThigh (std::vector<float>, ushort * pmt_high);
void getPMTlow (std::vector<float>, ushort * pmt_low);
void getStrips (std::vector<RootTrackerHit>, short* strips);
std::vector<float> CorrectPMT (std::vector<Edep_Pos>);
std::vector<short> NormalizePMThg (std::vector<float>);
std::vector<short> NormalizePMTlg (std::vector<float>);
std::vector<int> GetPMTHGPeds();
std::vector<int> GetPMTLGPeds();
float PMTAttCorr (float dist);
float EcalMev2ADCfactorHG (std::string PMT);
float EcalMev2ADCfactorLG (std::string PMT);





int main (int argc, char** argv) {
    const std::string mcfilename = getMCfilename (argc, argv);
    const std::string lvl0filename = getLvl0filename (mcfilename);
    TFile* filemc = TFile::Open (mcfilename.c_str(), "READ");
    TTree* Tmc = (TTree*) filemc->Get ("HEPD/EventTree");
    LEvRec0Writer lvl0writer (lvl0filename);
    LoopOnEvents (&lvl0writer, Tmc);
    lvl0writer.Write();
    lvl0writer.Close(); delete Tmc;
    filemc->Close();    delete filemc;
    return 0;
} // main conclusion




void LoopOnEvents (LEvRec0Writer* lvl0writer, TTree* Tmc)
{
    int ne = Tmc->GetEntries();
    RootEvent* MCevt = new RootEvent;
    TBranch* b_Event = new TBranch;
    Tmc->SetBranchAddress ("Event", &MCevt, &b_Event);
    for (int ie = 0; ie < ne; ie++) {
        int nb = Tmc->GetEntry (ie);
        int eventid =  MCevt->EventID();
        std::vector<RootTrack> rootTracks =  MCevt->GetTracks();
        std::vector<RootCaloHit> caloHits =  MCevt->GetCaloHit();
        std::vector<float> caloEdep = CaloHitsToEdep (caloHits);
        std::vector<RootTrackerHit>  trackerHits =  MCevt->GetTrackerHit();
        LEvRec0* ev = lvl0writer->pev();
        ev->Reset();
        getPMThigh (caloEdep, ev->pmt_high);
        getPMTlow (caloEdep, ev->pmt_low);
        getStrips (trackerHits, ev->strip);
        lvl0writer->Fill();
        std::cout << ie << "\r" << std::flush;
    }
    delete b_Event;
    delete MCevt;
    std::cout << "Done     " << std::endl;
}


float Vector3Dist (TVector3 v1, TVector3 v2)
{
    TVector3 diff = v1 - v2;
    return static_cast<float> (diff.Mag() ); // get magnitude (=rho=Sqrt(x*x+y*y+z*z)))
}


float VectorXYDist (TVector3 v1, TVector3 v2)
{
    v2.SetZ (v1.Z() );
    return Vector3Dist (v1, v2);
}



std::string  getMCfilename (int argc, char** argv)
{
    std::string filename = "../../../Simulation/run/Simulations_root/hepd5000_qmd_173MeV_proton_3C0.root"; // Supposing you run from Tools/MC2Lvl0/build/ ; I know, it's ugly :(
    if (argc > 1) filename = argv[1];
    std::cout << "MC2Lvl0: MC file name set to " << filename << std::endl;
    return filename;
}

std::string  getLvl0filename (const std::string mcfilename)
{
	
    vector<string> strings;
    istringstream f(mcfilename.c_str());
    string s;
    while (std::getline(f, s, '/')) {
	    strings.push_back(s);
    }

    std::string lvl0filename = ("MC2Lvl0_"+strings[strings.size()-1]).c_str();  
    return lvl0filename;
}


std::vector<float> CaloHitsToEdep (std::vector<RootCaloHit> CaloHits)
{
    std::vector<Edep_Pos> pmt_info = Calo2Edep_PosConverter (CaloHits);
    return CorrectPMT (pmt_info); //MeV
}

void getPMThigh (std::vector<float> correctedPMThg, ushort* pmt_high)
{
    std::vector<short> normalizedPMThg = NormalizePMThg (correctedPMThg);
    for (uint ip = 0; ip < NPMT; ip++) {
        pmt_high[ip] = normalizedPMThg[ip];
    }
    return;
}


void getPMTlow (std::vector<float> correctedPMTlg, ushort* pmt_low)
{
    std::vector<short> normalizedPMTlg = NormalizePMTlg (correctedPMTlg); //ADC
    for (uint ip = 0; ip < NPMT; ip++) {
        pmt_low[ip] = normalizedPMTlg[ip];
    }
    return;
}


void getStrips (std::vector<RootTrackerHit> TrackerHits, short* strips)
{
    std::vector<std::vector<Edep_Pos>> TrackerEdepPos = Tracker2Edep_PosConverter (TrackerHits);
    TrackerADC trkadc (TrackerEdepPos);
    std::vector<short> trkstrips = trkadc.GetStrips();
    for (uint ic = 0; ic < NCHAN; ic++) {
        strips[ic] = trkstrips[ic];
    }
    return;
}


std::vector<float> CorrectPMT (std::vector<Edep_Pos> pmt_info)
{
    std::vector<float>   correctedPMTs (NPMT);
    posmap PMTposmap = getPosMap();
    for (int ip = 0; ip < NPMT; ip++) {
        correctedPMTs[ip] = pmt_info[ip].totEdep;
        auto index = std::find (CaloPMTnames.begin(), CaloPMTnames.end(), PMTID[ip]);
        if (index != CaloPMTnames.end() ) {
            int PMTindex = std::distance (CaloPMTnames.begin(), index);
            TVector3 PMTpos = PMTposmap[PMTID[ip]];
            TVector3 ParticlePos = pmt_info[ip].position;
            float distance = VectorXYDist (PMTpos, ParticlePos);
            float attcor = PMTAttCorr (distance);
            correctedPMTs[ip] *= attcor;
        }
    }
    return correctedPMTs;
}

std::vector<short> NormalizePMThg (std::vector<float> rawPMT)
{
    std::vector<short>  nPMTHG (NPMT);
    std::vector<int> ped = GetPMTHGPeds();
    for (int ip = 0; ip < NPMT; ip++) {
        float MeVToADC = EcalMev2ADCfactorHG (PMTID[ip]);
        int untrimmedPMT = static_cast<int> (rawPMT[ip] * MeVToADC) + ped[ip];
        if (untrimmedPMT > NADC) untrimmedPMT = NADC - 1;
        nPMTHG[ip] = static_cast<short> (untrimmedPMT);
    }
    return nPMTHG;
}

std::vector<short> NormalizePMTlg (std::vector<float> rawPMT)
{
    std::vector<short>  nPMTLG (NPMT);
    std::vector<int> ped = GetPMTLGPeds();
    for (int ip = 0; ip < NPMT; ip++) {
        float MeVToADC = EcalMev2ADCfactorLG (PMTID[ip]);
        int untrimmedPMT = static_cast<int> (rawPMT[ip] * MeVToADC) + ped[ip];
        if (untrimmedPMT > NADC) untrimmedPMT = NADC - 1;
        nPMTLG[ip] = static_cast<short> (untrimmedPMT);
    }
    return nPMTLG;
}


std::vector<int> GetPMTHGPeds()
{
    std::vector<int>  PMTHGPeds (NPMT);
    pedmap Pmap = CaloPMTpedsHG;
    for (int ip = 0; ip < NPMT; ip++) {
        int currentped = 0;
        std::string current_name = PMTID[ip];
        pedmap::iterator iter = Pmap.find (current_name);
        if (iter != Pmap.end() ) currentped = static_cast<int> (iter->second.mean);
        PMTHGPeds[ip] = currentped;
    }
    return PMTHGPeds;
}


std::vector<int> GetPMTLGPeds()
{
    std::vector<int>  PMTLGPeds (NPMT);
    pedmap Pmap = CaloPMTpedsLG;
    for (int ip = 0; ip < NPMT; ip++) {
        int currentped = 0;
        std::string current_name = PMTID[ip];
        pedmap::iterator iter = Pmap.find (current_name);
        if (iter != Pmap.end() ) currentped = static_cast<int> (iter->second.mean);
        PMTLGPeds[ip] = currentped;
    }
    return PMTLGPeds;
}

int GetScintLayer (std::string PMT)
{
    int layer = -1;
    auto index = std::find (CaloPMTnames.begin(), CaloPMTnames.end(), PMT);
    if (index != CaloPMTnames.end() ) {
        int PMTindex = std::distance (CaloPMTnames.begin(), index);
        layer = PMTindex % (CaloPMTnames.size() / 2);
    }
    return layer;
}

float EcalMev2ADCfactorHG (std::string PMT)
{
    float MaxMeVlayer = 15;
    int layer = GetScintLayer (PMT);
    if (layer >= 0)   MaxMeVlayer = MeVPeakLayer[layer];
    float absMaxADCLayer = PMTMaxPeakHG[PMT];
    float pedLayer = CaloPMTpedsHG[PMT].mean;
    float relMaxADClayer = absMaxADCLayer - pedLayer;
    float MeV2ADC = relMaxADClayer / MaxMeVlayer;
    return MeV2ADC;
}

float EcalMev2ADCfactorLG (std::string PMT)
{
    float MaxMeVlayer = 15;
    int layer = GetScintLayer (PMT);
    if (layer >= 0)   MaxMeVlayer = MeVPeakLayer[layer];
    float absMaxADCLayer = PMTMaxPeakLG[PMT];
    float pedLayer = CaloPMTpedsLG[PMT].mean;
    float relMaxADClayer = absMaxADCLayer - pedLayer;
    float MeV2ADC = relMaxADClayer / MaxMeVlayer;
    return MeV2ADC;
}


float PMTAttCorr (float dist)
{
    float lambda = 2764.; //mm
    return exp (- dist / lambda);
}
