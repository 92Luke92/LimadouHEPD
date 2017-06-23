/*
 * gmm.cc
 *
 * Class performing a Gaussian micture model via Expectation-Maximization algorithm
 * Contact : laurent.basara@tifpa.infn.it
 *
 */


#include "gmm.hh"
#include <array>

void GMM::init() {
   std::vector<int> bins=distrib_init.getBins();
   int xmin=bins.front();
   int xmax=bins.back();

   alpha[0]=0.8;
   alpha[1]=0.2;

   float third=(xmax-xmin)/3.;

   mu[0]=float(xmin+third);
   mu[1]=float(xmin+2*third);

   sigma[0]=third;
   sigma[1]=third;

}


float GMM::normal_pdf(float x, float mu, float sigma) {
   static const float inv_sqrt_2pi = 0.3989422804014327;
   float a = (x - mu) / sigma;
   return inv_sqrt_2pi / sigma * exp(-0.5f * a * a);
}


std::vector<float> GMM::normal_pdf(std::vector<float> x, float mu, float sigma) {
   std::vector<float>  pdfs;
   for (auto elt : x) pdfs.push_back(normal_pdf(elt, mu, sigma));
   return pdfs;
}



void GMM::EMstep () {
   std::vector<int> bins=distrib_init.getBins();
   std::vector<float> vals=distrib_init.getNormalizedBinContent();

   // Expectation
   std::array<std::vector<float>,ngaus> w_vals;

   for (uint16_t ib=0; ib<bins.size(); ib++) {
      float tot_prob=0;
      std::vector<float> prob;
      for (int ig=0; ig<ngaus; ig++) {
         float iprob=normal_pdf(float(bins[ib]), mu[ig], sigma[ig]);
         prob.push_back(iprob);
         tot_prob+=iprob;
      }
      for (int ig=0; ig<ngaus; ig++) {
         w_vals[ig].push_back(prob[ig]/tot_prob * vals[ib]);
      }
   }

   // Maximization
   for (int ig=0; ig<ngaus; ig++) {
      distribs[ig]=LStatTools(bins, w_vals[ig]);
      //if (verbose) distribs[ig].dump();
      alpha[ig]=distribs[ig].integral();
      mu[ig] =  distribs[ig].mean();
      sigma[ig]=distribs[ig].sigma();
      if (sigma[ig]<0.1) sigma[ig]=0.1;
   }

}



void GMM::iterate (uint16_t niter) {

   for (uint16_t iEM=0; iEM<niter; iEM++) {
      EMstep();

      if (verbose) {
         std::cout << "iter " << ++totiter << std::endl;
         for (int ig=0; ig<ngaus; ig++)
            std::cout << "  Gaus " << ig+1 << " : alpha " << alpha[ig]
            << ", mu " << mu[ig] << ", sigma " << sigma[ig] << std::endl;
      }
   }
   return;
}
