/*
 * matteomethod.hh
 *
 *
 *
 */

#ifndef MATTEOMETHOD_HH
#define MATTEOMETHOD_HH

#include <MeV2ADCmethod.hh>

class MatteoMethod : public MeV2ADCMethod
{
  public:
    MatteoMethod(std::string datacardname);
    short adcFromMev(float mev, int sensor);
    short Err_adcFromMev(float mev, int sensor);

  protected:	
    struct PMTnumbersMatteo : public PMTnumbers {
      float Slope, ErrSlope;
      float Interc, ErrInterc;
      PMTnumbersMatteo(){};
      PMTnumbersMatteo(const PMTnumbers& other) : PMTnumbers(other){};
    };


   private:
    void  UpdateMyPMTs(); 
    void  convertParameterDatacard();

    std::array<PMTnumbersMatteo, NPMT>  MyPMTs;

};

#endif 