/*
 * ecaladc.cc
 *
 *
 *
 */


#include "ecaladc.hh"
#include <iostream>





float EcalADC::VectorXYDist (TVector2 v1, TVector2 v2)
{
    TVector2 diff = v1 - v2;
    return static_cast<float> (diff.Mod() ); // return TMath::Sqrt(fX*fX+fY*fY);
}



EcalADC::EcalADC(method ecalmethod)
{
    switch(ecalmethod) {
        case Laurent:
            methodHg = new LaurentMethod("laurentHGpeakshift.csv");
            methodLg = new LaurentMethod("laurentLGpeakshift.csv");
            break;
        case Francesco:
            methodHg = new FrancescoMethod("laurentHGpeakshift.csv","francesco_parameterHG.csv");
            methodLg = new FrancescoMethod("laurentLGpeakshift.csv","francesco_parameterLG.csv");
            break;
        case Matteo:
            methodHg = new MatteoMethod("PmtSlopeIntercErr_MeanMatteo.csv");
            methodLg = new MatteoMethod("PmtSlopeIntercErr_MeanMatteo.csv");
        case RawEdep:
            methodHg = new RawEdepMethod("laurentHGpeakshift.csv");
            methodLg = new RawEdepMethod("laurentLGpeakshift.csv");
    }
    initMCpos();
}




void EcalADC::setMCEnergy (int mcE) {
   mcEnergy = mcE;
   methodHg->setBeamEnergy(mcE);
   methodLg->setBeamEnergy(mcE);
}


void EcalADC::initMCpos() {
    const std::array<float, NSCINTPLANES> PMTzMC={312.2,297.42,282.64,267.86,253.08,238.3,223.52,208.74,193.96,179.18,
        164.814,150.51,136.241,121.944,107.659,93.3685};
    for (uint iPMT=0; iPMT<scintPMT.size(); iPMT++) {
      float x=(iPMT%2 == 0)?82.5:-82.5; //mm
      float y=(iPMT < NPMT/2)?82.5:-82.5; //mm
      MCtoPhysicalDetectorFrame mcVector(TVector3(x, y, PMTzMC[iPMT%scintPMT.size()]));
      PMTpos[scintPMT[iPMT]]=mcVector.GetPhysicalVector().XYvector();
   }
   return;
}


void EcalADC::SetPositions(std::vector<Edep_Pos> pmt_info) {
    for (int ip = 0; ip < NPMT; ip++)
        correctedPMTs[ip] = pmt_info[ip].totEdep;

    for (PMTenum ip : scintPMT) {
        TVector2 PmtPos = PMTpos[ip];
        TVector2 ParticlePos = pmt_info[ip].position.XYvector();
        float distance = VectorXYDist (PmtPos, ParticlePos);
        float attcor = PMTAttCorr (distance);
        correctedPMTs[ip] *= attcor;
    }
}


void EcalADC::NormalizePMThg ( ushort* pmt_high)
{
    NormalizePMT(pmt_high, methodHg);
    return;
}

void EcalADC::NormalizePMTlg ( ushort* pmt_low)
{
    NormalizePMT(pmt_low, methodLg);
    return;
}


void EcalADC::NormalizePMT ( ushort* pmt_out, calomev2adcmethod* method) {
    for (uint ip = 0; ip < NPMT; ip++) {
	pmt_out[ip] = method->adcFromMev( correctedPMTs[ip], ip );
    }
    return;
}




float EcalADC::PMTAttCorr (float dist)
{
    float lambda = 2764.; //mm
    return exp (- dist / lambda);
}







