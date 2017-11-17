/*
 * francescomethod.cc
 *

 *
 *
 */


#include "francescomethod.hh"


FrancescoMethod::FrancescoMethod(std::string datacardname,std::string pardatacardname)
   : MeV2ADCMethod(datacardname)
{
	UpdateMyPMTs(); 
	initRandom(pardatacardname);
}

void FrancescoMethod::UpdateMyPMTs(){
	int i=0;
	for (auto pmt : PMTs) {
		PMTnumbersFrancesco tmp(PMTs[i]);	
		MyPMTs[i]=tmp;
		i++;
	}
}

void FrancescoMethod::initRandom(std::string pardatacardname) {
  csv2fvec parfile;
  pardatacard=parfile.fromDatacard(pardatacardname);
  Rand= new TRandom3(time(0));
  convertParameterDatacard();

}

void FrancescoMethod::convertParameterDatacard(){
	for (auto line : pardatacard) {
		int idx = static_cast<int> ( line[0]);
		MyPMTs[idx].a1=line[1];
		MyPMTs[idx].b1=line[2];
		MyPMTs[idx].a2=line[3];
		MyPMTs[idx].b2=line[4];
		MyPMTs[idx].a3=line[5];
		MyPMTs[idx].b3=line[6];
		MyPMTs[idx].c3=line[7];
	}
}

short FrancescoMethod::SmearADC(short ADC,int sensor){
	PMTnumbersFrancesco thisPMT=MyPMTs[sensor];
	float smearMean = thisPMT.a3 + thisPMT.b3*ADC + thisPMT.c3*ADC*ADC;
	float smearSigma1= thisPMT.a1 + thisPMT.b1*ADC;
	float smearSigma2= thisPMT.a2 + thisPMT.b2*ADC;
	float smearedADC = Rand->Gaus(smearMean,smearSigma1);
	if(smearedADC>smearMean){
		smearedADC=smearMean;
		while(smearedADC<=smearMean) smearedADC = Rand->Gaus(smearMean,smearSigma2);
	}
	smearedADC+=ADC;
	if(ADC<=thisPMT.pedMean + thisPMT.pedSigma) return ADC;
	else return static_cast<short> (smearedADC);
}

short FrancescoMethod::adcFromMev(float mev, int sensor) {
   PMTnumbersFrancesco thisPMT=MyPMTs[sensor];
   float adcShift = mev * thisPMT.mev2adc;
   int untrimmedPMT = static_cast<int> (adcShift + thisPMT.pedMean);
   short trimmed = trimADC(untrimmedPMT); 	 
   return SmearADC(trimmed,sensor);	 	
}


