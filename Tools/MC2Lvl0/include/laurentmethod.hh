/*
 * laurentmethod.hh
 *
 *
 *
 */


#ifndef LAURENTMETHOD_HH
#define LAURENTMETHOD_HH

#include <prescaledmev2adcmethod.hh>

class LaurentMethod: public prescaledmev2adcmethod {
  public:
    LaurentMethod (std::string datacardname);
    float adcFromMevNoPed (float mev, int sensor);
    void setBeamEnergy(int energy);

  protected:
   struct PMTnumbersLaurent: public PMTnumbers {
        float a=0;
        float b=0;
	PMTnumbersLaurent(){};
  	PMTnumbersLaurent(const PMTnumbers& other):PMTnumbers(other){};
   };



  private:
     void UpdateMyPMTs();
     std::array<PMTnumbersLaurent, NPMT>  MyPMTs;

};

#endif
