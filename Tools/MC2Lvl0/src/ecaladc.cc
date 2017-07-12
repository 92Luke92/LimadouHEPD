/*
 * ecaladc.cc
 *
 *
 *
 */


#include "ecaladc.hh"
#include "PMTnumbers.hh"
#include "TVector3.h"
#include <iostream>




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




std::vector<float> EcalADC::CorrectPMT (std::vector<Edep_Pos> pmt_info)
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

void EcalADC::NormalizePMThg ( ushort* pmt_high)
{
    std::vector<int> ped = GetPMTHGPeds();
    std::vector<float> rawPMT=CorrectPMT(edepPos);
    for (int ip = 0; ip < NPMT; ip++) {
        float MeVToADC = EcalMev2ADCfactorHG (PMTID[ip]);
        int untrimmedPMT = static_cast<int> (rawPMT[ip] * MeVToADC) + ped[ip];
        if (untrimmedPMT > NADC) untrimmedPMT = NADC - 1;
        pmt_high[ip] = static_cast<short> (untrimmedPMT);
    }
    return;
}

void EcalADC::NormalizePMTlg ( ushort* pmt_low)
{
    std::vector<int> ped = GetPMTLGPeds();
    std::vector<float> rawPMT=CorrectPMT(edepPos);
    for (int ip = 0; ip < NPMT; ip++) {
        float MeVToADC = EcalMev2ADCfactorLG (PMTID[ip]);
        int untrimmedPMT = static_cast<int> (rawPMT[ip] * MeVToADC) + ped[ip];
        if (untrimmedPMT > NADC) untrimmedPMT = NADC - 1;
        pmt_low[ip] = static_cast<short> (untrimmedPMT);
    }
    return;
}


std::vector<int> EcalADC::GetPMTHGPeds()
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


std::vector<int> EcalADC::GetPMTLGPeds()
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

int EcalADC::GetScintLayer (std::string PMT)
{
    int layer = -1;
    auto index = std::find (CaloPMTnames.begin(), CaloPMTnames.end(), PMT);
    if (index != CaloPMTnames.end() ) {
        int PMTindex = std::distance (CaloPMTnames.begin(), index);
        layer = PMTindex % (CaloPMTnames.size() / 2);
    }
    return layer;
}

float EcalADC::EcalMev2ADCfactorHG (std::string PMT)
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

float EcalADC::EcalMev2ADCfactorLG (std::string PMT)
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


float EcalADC::PMTAttCorr (float dist)
{
    float lambda = 2764.; //mm
    return exp (- dist / lambda);
}
