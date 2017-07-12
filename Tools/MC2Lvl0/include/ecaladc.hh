/*
 * ecaladc.hh
 *
 * Ecal Edep to ADC
 *
 */


#ifndef ECALADC_HH
#define ECALADC_HH

#include <vector>
#include "Edep_Pos.h"
#include "detector_const.hh"

class EcalADC
{
   public:
     EcalADC( std::vector<Edep_Pos> edepPositions ) : edepPos(edepPositions) {};
     std::vector<short> GetStrips();
     void NormalizePMThg ( ushort* pmt_high);
     void NormalizePMTlg ( ushort* pmt_low);



   private:

     std::vector<Edep_Pos> edepPos;
     std::vector<float> CorrectPMT (std::vector<Edep_Pos>);
     std::vector<int> GetPMTHGPeds();
     std::vector<int> GetPMTLGPeds();
     float PMTAttCorr (float dist);
     int GetScintLayer (std::string PMT);
     float EcalMev2ADCfactorHG (std::string PMT);
     float EcalMev2ADCfactorLG (std::string PMT);


};

#endif /* ECALADC_HH */
