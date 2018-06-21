/*
 * synthesismethod.hh
 *
 *
 *
 */

#ifndef SYNTHESISMETHOD_HH
#define SYNTHESISMETHOD_HH

#include <calomev2adcmethod.hh>


class SynthesisMethod : public calomev2adcmethod
{
  public:
    SynthesisMethod(std::string datacardname);
    float adcFromMevNoPed(float mev, int sensor);

  private:
    void init();

    struct PMTnumbers  {
      float meanA, meanB, meanC; // meanADC = meanA Edep*Edep + meanB * Edep + meanC
      float irqA, irqB, irqC;    // irqADC = irqA Edep*Edep + irqB * Edep + irqC
    };

    std::array<PMTnumbers, NPMT>  pmtParameters;

    float iqr2std(float iqr) { return iqr/1.35;}
    // csv numbers correspond not to std but the more stable interquartile range (IQR)
    // this conversion corresponds to the hypothesis of Gaussianity (quite wrong here)

};

#endif // SYNTHESISMETHOD_HH
