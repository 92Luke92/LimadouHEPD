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
  OPmethod=false;
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
            break;
        case Synthesis:
            methodHg = new SynthesisMethod("Synthesis_mean_abc_IQ_abc.csv");
            methodLg = new SynthesisMethod("Synthesis_mean_abc_IQ_abc.csv");
            break;
        case OptPhot:
	    OPmethod=true;
	    methodHg = new OptPhotMethod("OptPhot_HG_pol2.csv");
	    methodLg = new OptPhotMethod("OptPhot_LG_pol2.csv");
	      break;
        case RawEdep:
            methodHg = new RawEdepMethod("nullped.csv");
            methodLg = new RawEdepMethod("nullped.csv");

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
        float attcor = 1;// PMTAttCorr (distance);
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

void EcalADC::NormalizePMThg_OP (std::vector<RootPmtHits> pmtHits, ushort*pmt_high)
{
    NormalizePMT_OP(pmtHits, pmt_high, methodHg);
    return;
}

void EcalADC::NormalizePMTlg_OP (std::vector<RootPmtHits> pmtHits, ushort*pmt_low)
{
    NormalizePMT_OP(pmtHits, pmt_low, methodLg);
    return;
}

void EcalADC::NormalizePMT ( ushort* pmt_out, calomev2adcmethod* method) {
    for (uint ip = 0; ip < NPMT; ip++) {
        ushort adc=method->adcFromMev( correctedPMTs[ip], ip );
        pmt_out[ip] = adc;
    }
    return;
}


void EcalADC::NormalizePMT_OP (std::vector<RootPmtHits> pmtHits, ushort* pmt_out, calomev2adcmethod* method) {
  uint Di;
  for(size_t ph=0; ph<pmtHits.size(); ph++){
    Di=999;
    for (uint i = 0; i < 64; i++) {
      correctedPMTs[i] = pmtHits[ph].GetNPhot(i); //i: PMTid of MC
      float FcorrectedPMTs = correctedPMTs[i];
       if(i>=44 && i<=52) FcorrectedPMTs *= 4;
      //mapping PMTid: MC --> DATA
      if(i==0) Di=32; //T1w
      if(i==1) Di=0; //T1e
      if(i==2) Di=33; //T2w
      if(i==3) Di=1; //T2e
      if(i==4) Di=34; //T3w
      if(i==5) Di=2; //T3e
      if(i==6) Di=35; //T4w
      if(i==7) Di=3; //T4e
      if(i==8) Di=36; //T5w
      if(i==9) Di=4; //T5e
      if(i==10) Di=37; //T6w
      if(i==11) Di=5; //T6e
  
      if(i==12) Di=38; //P1nw
      if(i==13) Di=6; //P1se
      if(i==14) Di=7; //P2sw
      if(i==15) Di=39; //P2ne
      if(i==16) Di=40; //P3nw
      if(i==17) Di=8; //P3se
      if(i==18) Di=9; //P4sw
      if(i==19) Di=41; //P4ne
      if(i==20) Di=42; //P5nw
      if(i==21) Di=10; //P5se
      if(i==22) Di=11; //P6sw
      if(i==23) Di=43; //P6ne
      if(i==24) Di=44; //P7nw
      if(i==25) Di=12; //P7se
      if(i==26) Di=13; //P8sw
      if(i==27) Di=45; //P8ne
      if(i==28) Di=46; //P9nw
      if(i==29) Di=14; //P9se
      if(i==30) Di=15; //P10sw
      if(i==31) Di=47; //P10ne
      if(i==32) Di=48; //P11nw
      if(i==33) Di=16; //P11se
      if(i==34) Di=17; //P12sw
      if(i==35) Di=49; //P12ne
      if(i==36) Di=50; //P13nw
      if(i==37) Di=18; //P13se
      if(i==38) Di=19; //P14sw
      if(i==39) Di=51; //P14ne
      if(i==40) Di=52; //P15nw
      if(i==41) Di=20; //P15se
      if(i==42) Di=21; //P16sw
      if(i==43) Di=53; //P16ne
      
      if(i==44) Di=29; //L1ne
      if(i==45) Di=62; //L4n
      if(i==46) Di=28; //L7nw
      if(i==47) Di=60; //L2e
      if(i==48) Di=31; //L5c
      if(i==49) Di=30; //L8w
      if(i==50) Di=59; //L3se
      if(i==51) Di=61; //L6s
      if(i==52) Di=27; //L9sw
      if(i>52 && i<64) continue;
	 
      ushort adc=method->adcFromMev( FcorrectedPMTs, Di ); //Di: PMTid of DATA
        pmt_out[Di] = adc;
    }
  }
    return;
}



float EcalADC::PMTAttCorr (float dist)
{
    float lambda = 2764.; //mm
    return exp (- dist / lambda);
}







