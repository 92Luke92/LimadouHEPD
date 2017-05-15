/*
 * LStatTools.hh
 *
 *
 * Since all over the code we compute means and rms using copy and paste code
 * A general class to compute mean, rms, and superior order moments of an abstract "data" vector
 *
 */

#include <vector>
#include <map>
#include <iostream>
#include "math.h"


#ifndef STATTOOLS_HH
#define STATTOOLS_HH

class LStatTools
{
   public:
      LStatTools(std::vector<int> data) ;
      LStatTools(std::vector<int> data, std::vector<float> weight) ;
      LStatTools(std::map <int, float> map) : datamap(map) {};
      void info(); // Main stat info (mean, rms, N elements)
      void dump(); // info() + detail of each element

      float moment(uint n);
      float norm_moment(uint n) {return moment(n)/pow(sigma(), n);}
      float mean() {return mu;}
      float sigma() {return rms();}
      float rms() {return sqrt(variance());}
      float variance() {return moment(2);}
      float skewness() {return norm_moment(3);}
      float kurtosis() {return norm_moment(4);}
      float norm_kurtosis() {return norm_moment(4)-3;}
      float integral() {return sum_weights;}

      std::vector<int>   getBins();
      std::vector<float> getBinContent();
      std::vector<float> getNormalizedBinContent();
      std::map <int, float> getMap() {return datamap;}
      std::map <int, float> getNormalizedMap();



   private:
      std::map <int, float> datamap;
      // ^^ int keeps size smaller, histos better constructed, Normalized map correctly computed in "histogram norm" sense.
      // please don't change to float :(
      float mu;
      float sum_weights=0;
      void computemean();
      void init();

};

#endif /* STATTOOLS_HH */
