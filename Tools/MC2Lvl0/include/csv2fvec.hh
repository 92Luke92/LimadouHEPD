/*
 * csv2fvec.hh
 *
 * Converts a csv file into a std::vector<std::vector<float>>
 *
 */


#ifndef CSV2FVEC_HH
#define CSV2FVEC_HH

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <string>
#include <tuple>
#include <stdlib.h>



class csv2fvec
{
   public:
      csv2fvec();
      std::vector<std::vector<float>> fromDatacard(std::string datacardname);
      std::vector<std::vector<float>> fromFile(std::string filename);

   private:

};

#endif /* CSV2FVEC_HH */
