#include <iostream>
#include "LStatTools.hh"
#include <string>


using namespace std;

int main (int argc, char *argv[])
{

    std::vector<int> data={1, 2, 3, 2, 1, 3, 3, 2, 2};
    LStatTools stat(data);
    stat.dump();
    LStatTools stat2({360, 362, 364, 325}, {5, 2, 3, 4});
    std::cout << stat.mean() << " -- " << stat.rms()<< std::endl;

    stat2.dump();


    return 0;
}

