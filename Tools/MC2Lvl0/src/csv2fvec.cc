/*
 * csv2fvec.cc
 *
 *
 *
 */


#include "csv2fvec.hh"


csv2fvec::csv2fvec()
{

}

std::vector<std::vector<float>> csv2fvec::fromDatacard(std::string datacardname)
{
    std::string datacarddir=std::getenv("DATACARDS");

    return fromFile(datacarddir+"/"+datacardname);
}

std::vector<std::vector<float>> csv2fvec::fromFile(std::string filename)
{

  std::vector<std::vector<float>> entries;
  std::ifstream fp(filename);
  if (!fp.good()) {
       std::cout << "CSV2VEC: failed loading " << filename << std::endl;
       return entries;
  }
  std::string line;
    while(std::getline(fp, line, '\n')) {
       float cell;
       std::istringstream row(line);
       std::vector<float> vline;
       while (row>>cell) {
         vline.push_back(cell);
         if (row.peek() == ',') row.ignore();
       }

       entries.push_back(vline);
    }
   std::cout << "CSV2VEC: success loading " << filename << " ("  << entries.size() << " entries)"<< std::endl;
   return entries;

}


