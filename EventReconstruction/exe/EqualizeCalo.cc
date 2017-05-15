//#include "LEvRec1File.hh"
#include <cstdlib>
#include <iostream>
#include "LCaloEqualizationManager.hh"

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Error! Usage:    ./EqualizeCalo <calRunFile> "
                 "<calibrationFile> <calOutFile>"
              << std::endl;
    std::cerr << "Aborted." << std::endl;
    return EXIT_FAILURE;
  }

  LCaloEqualizationManager::GetInstance().LoadRun(argv[1], argv[2]);
  LCaloEqualization *eqHG =
      LCaloEqualizationManager::GetInstance().EqualizeHG();
  if (eqHG) {
    eqHG->Write(argv[3]);
  }

  return EXIT_SUCCESS;
}
