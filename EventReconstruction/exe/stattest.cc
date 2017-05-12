#include <iostream>
#include "LStatTools.hh"
#include "gmm.hh"
#include <string>


using namespace std;

int main (int argc, char *argv[])
{

    std::vector<int> data={1, 2, 2, 3, 3, 3, 4, 4, 5, 7, 8, 8, 8, 9};
    LStatTools stat(data);
    stat.dump();
    LStatTools stat2({360, 362, 364, 325}, {5, 2, 3, 4});
    stat2.dump();

    LStatTools stat3(stat2.getBins(), stat2.getNormalizedBinContent());
    stat3.dump();



    for (auto elt : GMM::normal_pdf({-2, 1, 0, 1, 2, 1.5}))
        cout << elt << endl;

    GMM model(stat);
    model.set_verbose(true);
    model.iterate(30);


    return 0;
}

