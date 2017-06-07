/*
 * LStatTools.cc
 *
 *
 */


#include "LStatTools.hh"




LStatTools::LStatTools(std::vector<int> data) {
   for (auto elt : data)
      datamap[elt]++;
   init();
};

LStatTools::LStatTools(std::vector<int> data, std::vector<float> weight) {
   for (int i=0; i<data.size(); i++) {
      datamap[data[i]]=weight[i];
   }
   init();
};

void LStatTools::init()  {
   float accu=0;
   sum_weights=0;
   for (const auto& elt : datamap) {
      accu += elt.first*elt.second;
      sum_weights+=elt.second;
   }
   mu=accu/sum_weights;
}



void LStatTools::info() {

   std::cout << "N bins      : " << datamap.size() << std::endl;
   std::cout << "Sum weights : " << sum_weights << std::endl;
   std::cout << "Mean        : " << mu << std::endl;
   std::cout << "Variance    : " << variance() << std::endl;
   std::cout << "Rms         : " << rms() << std::endl;
   std::cout << "Skewness    : " << skewness() << std::endl;
   std::cout << "Kurtosis    : " << kurtosis() << std::endl;

   return;
}

void LStatTools::dump() {

   info();
   std::cout << " --- " << std::endl;
   for (const auto& elt : datamap) {
      std::cout << elt.first << " : " << elt.second << std::endl;
   }
   std::cout << " --- " << std::endl;
   return;
}




float LStatTools::moment(uint16_t n) {
   float accu=0, sum=0;
   float mu=mean();
   for (const auto& elt : datamap) {
      accu += pow((elt.first-mu), n)*elt.second;
   }
   return accu/sum_weights;
}



std::map <int, float> LStatTools::getNormalizedMap() {
   std::map <int, float>  normap;
    for (const auto& elt : datamap) {
      normap[elt.first]=elt.second/sum_weights;
   }
   return normap;
}


std::vector<int>   LStatTools::getBins() {
   std::vector<int> bins;
    for (const auto& elt : datamap)  bins.push_back(elt.first);
   return bins;
}

std::vector<float> LStatTools::getBinContent() {
   std::vector<float> binscontent;
   for (const auto& elt : datamap)  binscontent.push_back(elt.second);
   return binscontent;
}
std::vector<float> LStatTools::getNormalizedBinContent() {
   std::vector<float> binsncontent;
   for (const auto& elt : datamap)  binsncontent.push_back(elt.second/sum_weights);
   return binsncontent;
}
