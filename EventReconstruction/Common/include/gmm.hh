/*
 * gmm.hh
 *
 * Class performing a Gaussian micture model via Expectation-Maximization algorithm
 * Contact : laurent.basara@tifpa.infn.it
 *
 */


#ifndef GMM_HH
#define GMM_HH

#include "LStatTools.hh"

class GMM
{

   public:
      GMM(LStatTools stat) :  distrib_init(stat),
                              distribs(2, stat)
                               {init();}

      static float normal_pdf(float x, float mu=0, float sigma=1);
      static std::vector<float> normal_pdf(std::vector<float> x, float mu=0, float sigma=1);
      void iterate(uint16_t niter=20);



      // Getters
      float alpha1() {return alpha[0];}
      float alpha2() {return alpha[1];}
      float mu1()    {return mu[0];}
      float mu2()    {return mu[1];}
      float sigma1() {return sigma[0];}
      float sigma2() {return sigma[1];}
      float* alphas()   {return alpha;}
      float* mus()      {return mu;}
      float* sigmas()   {return sigma;}

      //Setters
      void set_alpha1(float a) {alpha[0]=a; alpha[1]=1-a;}
      void set_mus(float mu1, float mu2) {mu[0]=mu1; mu[1]=mu2;}
      void set_sigmas(float sig1, float sig2) {sigma[0]=sig1; sigma[1]=sig2;}
      void set_verbose (bool v) {verbose=v;}

   private:
      void init();

      float alpha[2]={0};
      float mu[2]={0};
      float sigma[2]={0};
	  static const uint16_t ngaus = { 2 };
      LStatTools distrib_init;
      std::vector <LStatTools> distribs;

      uint16_t totiter=0;
      void EMstep();

      bool verbose=false;



};

#endif /* GMM_HH */
