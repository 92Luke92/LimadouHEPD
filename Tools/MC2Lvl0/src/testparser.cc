#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <string>
#include <tuple>
#include <stdlib.h>




int main() {
  std::vector<std::vector<float>> entries;


  {
    std::string datacarddir=std::getenv("DATACARDS");
    std::cout << datacarddir << std::endl;
    std::ifstream fp(datacarddir+"test.txt");
    std::string line;

    while(std::getline(fp, line)) {
       float cell;
       std::istringstream row(line);
       std::vector<float> vline;
       while (row>>cell) {
         vline.push_back(cell);
         if (row.peek() == ',') row.ignore();
       }

       entries.push_back(vline);
    }

  }

  for (auto line : entries) {
     for(float elt:line) std::cout << elt << " ";
     std::cout << std::endl;
  }



  return 0;
}
