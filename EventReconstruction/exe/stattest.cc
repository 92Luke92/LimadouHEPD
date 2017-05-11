#include <iostream>
#include "LStatTools.hh"
#include <string>


using namespace std;

int main (int argc, char *argv[])
{

    std::vector<int> data={2, 3, 4, 5, 2, 3, 3, 3};
    LStatTools stat(data);
    stat.dump();
    LStatTools stat2({360, 362, 364, 325}, {5, 2, 3, 4});
    std::cout << stat.mean() << " -- " << stat.rms()<< std::endl;
    std::cout << " --- " << std::endl;
    stat2.dump();
    std::cout << " --- " << std::endl;
    std::cout << stat2.mean() << " -- " << stat2.rms()<< std::endl;
    return 0;
}

